#pragma once

#include "beatsaber-hook/shared/rapidjson/include/rapidjson/rapidjson.h"
#include "config-utils/shared/config-utils.hpp"

template<>
inline void ConfigUtils::ConfigValue<std::vector<std::string>>::SaveValue() {
    rapidjson::Value& jsonValue = config->config[name].SetArray();
    auto allocator = config->config.GetAllocator();
    for (auto& element : value) {
        jsonValue.PushBack(rapidjson::Value(element, allocator).Move(), allocator);
    }
    config->Write();
}

template<>
inline void ConfigUtils::ConfigValue<std::vector<std::string>>::LoadValue() {
    rapidjson::Value& jsonValue = config->config[name];
    if(jsonValue.IsArray()){
        value = std::vector<std::string>();
        for (auto& element : jsonValue.GetArray()) {
            value.push_back(element.GetString());
        }
    } else {
        SetValue(defaultValue);
    }
}


// Declare the mod config as "Config" and declare all its values and functions.
DECLARE_CONFIG(Config,
    // Declare variables
    CONFIG_VALUE(MinimumSongProgressToIncrementPlayCount, float, "MinimumSongProgressToIncrementPlayCount", 0.7);
    CONFIG_VALUE(IncrementCountInPracticeMode, bool, "IncrementCountInPracticeMode", true);
    CONFIG_VALUE(OnlyIncrementInPracticeModeWhenThePlayerFinishes, bool, "OnlyIncrementInPracticeModeWhenThePlayerFinishes", true);
    CONFIG_VALUE(BannedBeatmapCharacteristics, std::vector<std::string>, "BannedBeatmapCharacteristics", { "Lightshow" });

    CONFIG_INIT_FUNCTION(
        // Initialize variables
        CONFIG_INIT_VALUE(MinimumSongProgressToIncrementPlayCount);
        CONFIG_INIT_VALUE(IncrementCountInPracticeMode);
        CONFIG_INIT_VALUE(OnlyIncrementInPracticeModeWhenThePlayerFinishes);
        CONFIG_INIT_VALUE(BannedBeatmapCharacteristics);
    )
)
