#include "common.hpp"
#include "assets.hpp"

#include "UnityEngine/Sprite.hpp"
#include "UnityEngine/AssetBundle.hpp"

namespace BeatmapPlayCount::BundledResources {
    // TODO(netux): Right now loading the bundle happens everytime StandardLevelDetailView is instantiated (see hooks.cpp),
    // because when loading from main.cpp, PlayCountSprite is a nullptr in hooks.cpp
    // * Figure out how to make the value of PlayCountSprite persist in other files when calling Load() in main.cpp
    // * inline and extern don't seem to work here :/ (or I'm using them wrong)
    SafePtrUnity<UnityEngine::Sprite> PlayCountSprite;

    void Load() {
        // Thanks https://discord.com/channels/441805394323439646/583108561396170837/959778056111800350

        static auto AssetBundle_LoadFromMemory = (function_ptr_t<UnityEngine::AssetBundle*, ArrayW<uint8_t>, uint8_t>)
            CRASH_UNLESS(il2cpp_functions::resolve_icall("UnityEngine.AssetBundle::LoadFromMemory_Internal"));

        auto assetBundle = AssetBundle_LoadFromMemory(
            static_cast<ArrayW<uint8_t>>(BeatmapPlayCount::IncludedAssets::Bundle_bundle), 0
        );

        PlayCountSprite.emplace(CRASH_UNLESS(assetBundle->LoadAsset<UnityEngine::Sprite*>("PlayCount.png")));

        assetBundle->Unload(/* unloadAllLoadedObjects: */ false);

        getLogger().debug("Loaded bundled resources");
    }

    UnityEngine::Sprite* GetPlayCountSprite() {
        if (!PlayCountSprite) {
            BeatmapPlayCount::BundledResources::Load();
        }
        return PlayCountSprite.isAlive() ? PlayCountSprite.ptr() : nullptr;
    }
}
