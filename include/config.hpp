#pragma once

#include "config-utils/shared/config-utils.hpp"

// Declare the mod config as "Config" and declare all its values and functions.
DECLARE_CONFIG(Config,
    // Declare variables
    CONFIG_VALUE(MinimumSongProgressToIncrementPlayCount, float, "Minimum song progress %% to incrent play count", 0.7);
    CONFIG_VALUE(IncrementCountInPracticeMode, bool, "Increment count during practice mode", true);

    CONFIG_INIT_FUNCTION(
        // Initialize variables
        CONFIG_INIT_VALUE(MinimumSongProgressToIncrementPlayCount);
        CONFIG_INIT_VALUE(IncrementCountInPracticeMode);
    )
)
