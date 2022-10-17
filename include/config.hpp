#pragma once

#include "config-utils/shared/config-utils.hpp"

// Declare the mod config as "Config" and declare all its values and functions.
DECLARE_CONFIG(Config,
    // Declare variables
    CONFIG_VALUE(MinimumSongProgressToIncrementPlayCount, float, "MinimumSongProgressToIncrementPlayCount", 0.7);
    CONFIG_VALUE(IncrementCountInPracticeMode, bool, "IncrementCountInPracticeMode", true);
    CONFIG_VALUE(OnlyIncrementInPracticeModeWhenThePlayerFinishes, bool, "OnlyIncrementInPracticeModeWhenThePlayerFinishes", true);

    CONFIG_INIT_FUNCTION(
        // Initialize variables
        CONFIG_INIT_VALUE(MinimumSongProgressToIncrementPlayCount);
        CONFIG_INIT_VALUE(IncrementCountInPracticeMode);
        CONFIG_INIT_VALUE(OnlyIncrementInPracticeModeWhenThePlayerFinishes);
    )
)
