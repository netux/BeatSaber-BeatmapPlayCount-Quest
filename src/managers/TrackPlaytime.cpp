#include "common.hpp"
#include "managers/TrackPlaytime.hpp"

#include "GlobalNamespace/StandardGameplaySceneSetupData.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/IPreviewBeatmapLevel.hpp"

DEFINE_TYPE(BeatmapPlayCount::Managers, TrackPlaytime);

namespace BeatmapPlayCount::Managers {
    void TrackPlaytime::ctor(
        GlobalNamespace::StandardGameplaySceneSetupData* _standardSceneSetupData,
        GlobalNamespace::AudioTimeSyncController* _audioTimeSyncController
    ) {
        audioTimeSyncController = _audioTimeSyncController;
        beatmapId = _standardSceneSetupData->previewBeatmapLevel->get_levelID();
    }

    void TrackPlaytime::IncrementPlayCount() {
        if (!incremented) {
            getStorage().incrementPlayCount(static_cast<std::string>(beatmapId));
            incremented = true;
        }
    }

    void TrackPlaytime::Tick() {
        if (incremented) {
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
