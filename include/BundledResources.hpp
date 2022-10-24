#pragma once

#include "UnityEngine/Sprite.hpp"

namespace BeatmapPlayCount::BundledResources {
    void Load();

    UnityEngine::Sprite* GetPlayCountSprite();
}
