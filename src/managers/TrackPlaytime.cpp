#include "common.hpp"
#include "managers/TrackPlaytime.hpp"

#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"

DEFINE_TYPE(BeatmapPlayCount::Managers, TrackPlaytime);

namespace BeatmapPlayCount::Managers {
    void TrackPlaytime::ctor(
        GlobalNamespace::GameplayCoreSceneSetupData* _sceneSetupData,
        GlobalNamespace::AudioTimeSyncController* _audioTimeSyncController
    ) {
        audioTimeSyncController = _audioTimeSyncController;
        beatmapId = _sceneSetupData->previewBeatmapLevel->get_levelID();

        isGameplayInPracticeMode = _sceneSetupData->practiceSettings != nullptr;

        getLogger().debug("isGameplayInPracticeMode %d", isGameplayInPracticeMode);
    }

    void TrackPlaytime::IncrementPlayCount() {
        if (!incremented) {
            getStorage().incrementPlayCount(static_cast<std::string>(beatmapId));
            incremented = true;
        }
    }

    bool TrackPlaytime::CanIncrementByPercentageBecauseOfPracticeMode() {
        return isGameplayInPracticeMode &&
            getConfig().IncrementCountInPracticeMode.GetValue();
    }

    bool TrackPlaytime::CanIncrementByPercentage() {
        return CanIncrementByPercentageBecauseOfPracticeMode();
    }

    void TrackPlaytime::Tick() {
        if (!CanIncrementByPercentage() || incremented) {
            return;
        }

        auto MinimumSongProgressToIncrementPlayCount = getConfig().MinimumSongProgressToIncrementPlayCount.GetValue();

        auto currentTime = audioTimeSyncController->get_songTime();
        auto endTime = audioTimeSyncController->get_songEndTime();
        auto progress = currentTime / endTime;
        getLogger().debug("Progress %f", progress);
        if (progress >= MinimumSongProgressToIncrementPlayCount) {
            IncrementPlayCount();

        }
    }
}
