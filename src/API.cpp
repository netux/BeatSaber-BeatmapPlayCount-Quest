#include "common.hpp"
#include "API.hpp"
#include "managers/TrackPlaytime.hpp"

#define ID MOD_ID
#include "conditional-dependencies/shared/main.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"

namespace BeatmapPlayCount::API {
    EXPOSE_API(GetPlayCount, int, std::string beatmapId) {
        return getStorage().getPlayCount(beatmapId);
    }

    EXPOSE_API(SetAllowCurrentPlayToIncrementCount, bool, bool flag) {
        auto trackPlaytime = BeatmapPlayCount::Managers::TrackPlaytime::getInstance();
        if (!trackPlaytime) return false;

        trackPlaytime->allowIncrementingPlayCount = flag;
        return true;
    }
}

#pragma GCC diagnostic pop

#undef ID
