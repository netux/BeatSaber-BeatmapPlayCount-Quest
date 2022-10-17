#pragma once

#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"

#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/Toggle.hpp"

#include "questui/shared/BeatSaberUI.hpp"

// Declare a ViewController to let us create UI in the mod settings menu
DECLARE_CLASS_CODEGEN(BeatmapPlayCount::Views, SettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, container);
    DECLARE_INSTANCE_FIELD(QuestUI::SliderSetting*, sliderFor_MinimumSongProgressToIncrementPlayCount);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, toggleFor_IncrementCountInPracticeMode);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, layoutFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, toggleFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes);

    // Override DidActivate, which is called whenever you enter the menu
    DECLARE_OVERRIDE_METHOD(
        void, DidActivate,
        il2cpp_utils::FindMethodUnsafe("HMUI", "ViewController", "DidActivate", 3),
        bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling
    );
)
