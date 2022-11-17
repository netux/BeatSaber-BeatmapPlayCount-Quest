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

    // NOTE(netux): can't use EXPOSE_API_STATIC as we need a method defined as per in the header.
    //
    // Why even define these methods on the header if we are using ConditionalDependencies?
    // I don't know. To allow for direct access if this mod is required by another mod I guess.
    EXPOSE_API(RegisterCanIncrementPlayCountCheck, void, std::string name, bool (*checkFn)()) {
        BeatmapPlayCount::Managers::TrackPlaytime::RegisterCanIncrementPlayCountCheck(name, checkFn);
    }

    EXPOSE_API(UnregisterCanIncrementPlayCountCheck, void, std::string name) {
        BeatmapPlayCount::Managers::TrackPlaytime::UnregisterCanIncrementPlayCountCheck(name);
    }
}

#pragma GCC diagnostic pop

#undef ID
