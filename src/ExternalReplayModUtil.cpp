#include "ExternalReplayModUtil.hpp"

#include "conditional-dependencies/shared/main.hpp"

namespace BeatmapPlayCount::ExternalReplayModUtil {
    auto _returnFalse = []() { return false; };

    bool IsGameplayReplayModReplay() {
        static auto Replay_isInReplay = CondDeps::Find<bool>("replay", "IsInReplay");
        return Replay_isInReplay.value_or(_returnFalse)();
    }

    bool CheckIfGameplayIsAReplay() {
        return IsGameplayReplayModReplay();
    }
}
