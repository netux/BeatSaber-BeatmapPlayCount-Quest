#include "common.hpp"
#include "ExternalReplayModUtil.hpp"
#include "managers/TrackPlaytime.hpp"

#include "custom-types/shared/delegate.hpp"

#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/AudioTimeSyncController_InitData.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/IDifficultyBeatmapSet.hpp"
#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/ILevelEndActions.hpp"

#include "System/Action.hpp"

DEFINE_TYPE(BeatmapPlayCount::Managers, TrackPlaytime);

namespace BeatmapPlayCount::Managers {
    void TrackPlaytime::ctor(
        GlobalNamespace::GameplayCoreSceneSetupData* _sceneSetupData,
        GlobalNamespace::AudioTimeSyncController* _audioTimeSyncController,
        GlobalNamespace::AudioTimeSyncController::InitData* _audioTimeSyncControllerInitData,
        GlobalNamespace::ILevelEndActions* _levelEndActionImpl
    ) {
        audioTimeSyncController = _audioTimeSyncController;
        beatmapId = _sceneSetupData->previewBeatmapLevel->get_levelID();
        beatmapCharacteristic = _sceneSetupData->difficultyBeatmap->get_parentDifficultyBeatmapSet()->get_beatmapCharacteristic()->get_serializedName();
        levelEndActionImpl = _levelEndActionImpl;

        handleLevelFinishedEventAction = custom_types::MakeDelegate<System::Action*>(classof(System::Action*), (std::function<void()>) [this]() {
            handleLevelFinishedEvent();
        });

        isGameplayInPracticeMode = _sceneSetupData->practiceSettings != nullptr;
        songStartTime = _audioTimeSyncControllerInitData->startSongTime;
        isGameplayAnExternalModReplay = BeatmapPlayCount::ExternalReplayModUtil::CheckIfGameplayIsAReplay();

        getLogger().debug("isGameplayInPracticeMode %d", isGameplayInPracticeMode);
        getLogger().debug("isGameplayAnExternalModReplay %d", isGameplayAnExternalModReplay);
        getLogger().debug("songStartTime %f", songStartTime);
    }

    void TrackPlaytime::Initialize() {
        auto bannedBeatmapCharacteristics = getConfig().BannedBeatmapCharacteristics.GetValue();
        doesBeatmapHaveBannedCharacteristic = std::find(
            bannedBeatmapCharacteristics.begin(),
            bannedBeatmapCharacteristics.end(),
            beatmapCharacteristic
        ) != bannedBeatmapCharacteristics.end();
        getLogger().debug("doesBeatmapHaveBannedCharacteristic %d", doesBeatmapHaveBannedCharacteristic);

        levelEndActionImpl->add_levelFinishedEvent(handleLevelFinishedEventAction);
    }

    void TrackPlaytime::Dispose() {
        levelEndActionImpl->remove_levelFinishedEvent(handleLevelFinishedEventAction);
    }

    void TrackPlaytime::handleLevelFinishedEvent() {
        if (!incremented) {
            IncrementPlayCount();
        }
    }

    void TrackPlaytime::IncrementPlayCount() {
        if (!CanIncrement() || incremented) {
            return;
        }

        if (isGameplayInPracticeMode && !getConfig().IncrementCountInPracticeMode.GetValue()) {
            return;
        }

        getStorage().incrementPlayCount(static_cast<std::string>(beatmapId));
        incremented = true;
    }

    bool TrackPlaytime::CanIncrementByPercentageBecauseOfPracticeMode() {
        return isGameplayInPracticeMode &&
            getConfig().IncrementCountInPracticeMode.GetValue() &&
            !getConfig().OnlyIncrementInPracticeModeWhenThePlayerFinishes.GetValue();
    }

    bool TrackPlaytime::CanIncrementByPercentage() {
        return CanIncrement() && CanIncrementByPercentageBecauseOfPracticeMode();
    }

    bool TrackPlaytime::CanIncrement() {
        return !isGameplayAnExternalModReplay && !doesBeatmapHaveBannedCharacteristic;
    }

    void TrackPlaytime::Tick() {
        if (!CanIncrementByPercentage() || incremented) {
            return;
        }

        auto MinimumSongProgressToIncrementPlayCount = getConfig().MinimumSongProgressToIncrementPlayCount.GetValue();

        auto currentTime = audioTimeSyncController->get_songTime() - songStartTime;
        if (currentTime < 0) {
            currentTime = 0;
        }
        auto endTime = audioTimeSyncController->get_songEndTime() - songStartTime;
        auto progress = currentTime / endTime;
        if (progress >= MinimumSongProgressToIncrementPlayCount) {
            IncrementPlayCount();
        }
    }
}
