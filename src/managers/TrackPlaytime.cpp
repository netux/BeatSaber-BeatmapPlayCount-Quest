#include "common.hpp"
#include "managers/TrackPlaytime.hpp"

#include "custom-types/shared/delegate.hpp"

#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/AudioTimeSyncController_InitData.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"
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
        levelEndActionImpl = _levelEndActionImpl;

        handleLevelFinishedEventAction = custom_types::MakeDelegate<System::Action*>(classof(System::Action*), (std::function<void()>) [this]() {
            handleLevelFinishedEvent();
        });

        isGameplayInPracticeMode = _sceneSetupData->practiceSettings != nullptr;
        songStartTime = _audioTimeSyncControllerInitData->startSongTime;

        getLogger().debug("isGameplayInPracticeMode %d", isGameplayInPracticeMode);
        getLogger().debug("songStartTime %f", songStartTime);
    }

    void TrackPlaytime::Initialize() {
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
        if (incremented) {
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
        return CanIncrementByPercentageBecauseOfPracticeMode();
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
        getLogger().debug("Progress %f", progress);
        if (progress >= MinimumSongProgressToIncrementPlayCount) {
            IncrementPlayCount();
        }
    }
}