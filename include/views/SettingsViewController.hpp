#pragma once

#include "HMUI/ViewController.hpp"

#include "custom-types/shared/macros.hpp"

#include "lapiz/shared/macros.hpp"

#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/UI/Toggle.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"
#include "UnityEngine/UI/VerticalLayoutGroup.hpp"
#include "questui/shared/BeatSaberUI.hpp"

#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/BeatmapCharacteristicCollectionSO.hpp"

#include "System/Collections/Generic/List_1.hpp"

DECLARE_CLASS_CODEGEN(BeatmapPlayCount::Views, SettingsViewController, HMUI::ViewController,
    DECLARE_INSTANCE_FIELD(UnityEngine::GameObject*, container);
    DECLARE_INSTANCE_FIELD(QuestUI::SliderSetting*, sliderFor_MinimumSongProgressToIncrementPlayCount);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, toggleFor_IncrementCountInPracticeMode);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::HorizontalLayoutGroup*, layoutFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::Toggle*, toggleFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes);
    DECLARE_INSTANCE_FIELD(TMPro::TextMeshProUGUI*, textFor_BannedBeatmapCharacteristics);
    DECLARE_INSTANCE_FIELD(UnityEngine::UI::VerticalLayoutGroup*, layoutFor_BannedBeatmapCharacteristics);

    DECLARE_OVERRIDE_METHOD(void, DidActivate, il2cpp_utils::il2cpp_type_check::MetadataGetter<&HMUI::ViewController::DidActivate>::get(),
        bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling
    );

    DECLARE_INSTANCE_METHOD(void, AddBannedBeatmapCharacteristicsToggle,
        UnityEngine::Transform* parent, GlobalNamespace::BeatmapCharacteristicSO* beatmapCharacteristicName
    );
    DECLARE_INSTANCE_METHOD(void, CreateBannedBeatmapCharacteristicsToggles,
        UnityEngine::Transform* parent
    );
)
