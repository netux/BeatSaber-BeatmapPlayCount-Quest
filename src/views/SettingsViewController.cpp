#include "common.hpp"
#include "views/SettingsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
using namespace QuestUI;

#include "UnityEngine/Resources.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/TextAnchor.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"
#include "UnityEngine/UI/ContentSizeFitter.hpp"

#include "GlobalNamespace/BeatmapCharacteristicSO.hpp"
#include "GlobalNamespace/BeatmapCharacteristicCollectionSO.hpp"

#include "System/Collections/Generic/List_1.hpp"

#include "Polyglot/Localization.hpp"

DEFINE_TYPE(BeatmapPlayCount::Views, SettingsViewController);

namespace BeatmapPlayCount::Views {
    void SettingsViewController::AddBannedBeatmapCharacteristicsToggle(
        UnityEngine::Transform* parent,
        GlobalNamespace::BeatmapCharacteristicSO* beatmapCharacteristic
    ) {
        auto characteristicName = beatmapCharacteristic->get_serializedName();

        auto bannedBeatmapCharacteristicsNames = getConfig().BannedBeatmapCharacteristics.GetValue();

        auto isInitiallyBanned = std::find(
            bannedBeatmapCharacteristicsNames.begin(),
            bannedBeatmapCharacteristicsNames.end(),
            beatmapCharacteristic->get_serializedName()
        ) != bannedBeatmapCharacteristicsNames.end();

        BeatSaberUI::CreateToggle(
            parent->get_transform(),
            /* text: */ Polyglot::Localization::Get(beatmapCharacteristic->get_characteristicNameLocalizationKey()),
            /* value: */ isInitiallyBanned,
            [&, characteristicName](bool isNowBanned) {
                auto newBannedCharacteristics = getConfig().BannedBeatmapCharacteristics.GetValue();

                if (isNowBanned) {
                    newBannedCharacteristics.push_back(characteristicName);
                } else {
                    auto pos = std::find(
                        newBannedCharacteristics.begin(),
                        newBannedCharacteristics.end(),
                        characteristicName
                    );
                    if (pos != newBannedCharacteristics.end()) {
                        newBannedCharacteristics.erase(pos);
                    }
                }
                getConfig().BannedBeatmapCharacteristics.SetValue(newBannedCharacteristics);
            }
        );
    }

    void SettingsViewController::CreateBannedBeatmapCharacteristicsToggles(UnityEngine::Transform* parent) {
        auto beatmapCharacteristicCollections = UnityEngine::Resources::FindObjectsOfTypeAll<
            GlobalNamespace::BeatmapCharacteristicCollectionSO*
        >();
        if (beatmapCharacteristicCollections.size() <= 0) {
            auto errorText = BeatSaberUI::CreateText(
                container->get_transform(),
                "Uh oh! Could not find retrieve of beatmap characteristics, sorry!"
            );
            errorText->set_color(UnityEngine::Color::get_red());

            getLogger().error("Could not find base game beatmap characteristic collection");
            return;
        }

        for (auto &beatmapCharacteristic : beatmapCharacteristicCollections[0]->beatmapCharacteristics) {
            if (beatmapCharacteristic->get_serializedName() == "MissingCharacteristic") {
                continue;
            }

            AddBannedBeatmapCharacteristicsToggle(parent, beatmapCharacteristic);
        }
    }

    void SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if(!(firstActivation && addedToHierarchy)) {
            return;
        }

        container = BeatSaberUI::CreateScrollableSettingsContainer(this->get_transform());

        #pragma region Minimum song progress
        sliderFor_MinimumSongProgressToIncrementPlayCount = BeatSaberUI::CreateSliderSetting(
            container->get_transform(),
            /* text: */ "Minimum song progress %% to incrent play count",
            /* increment: */ 0.01,
            /* value: */ getConfig().MinimumSongProgressToIncrementPlayCount.GetValue() * 100,
            /* minValue: */ 0, /* maxValue: */ 100,
            [](float newValue) {
                getConfig().MinimumSongProgressToIncrementPlayCount.SetValue(newValue / 100);
            }
        );
        #pragma endregion Minimum song progress

        #pragma region In practice mode
        toggleFor_IncrementCountInPracticeMode = BeatSaberUI::CreateToggle(
            container->get_transform(),
            /* text: */ "Increment count during practice mode",
            /* value: */ getConfig().IncrementCountInPracticeMode.GetValue(),
            [&](bool newValue) {
                getConfig().IncrementCountInPracticeMode.SetValue(newValue);
                if (toggleFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes != nullptr) {
                    toggleFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes->set_interactable(newValue);
                }
            }
        );

        layoutFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes = BeatSaberUI::CreateHorizontalLayoutGroup(container->get_transform());
        layoutFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(90.0f);
        layoutFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes->set_padding(UnityEngine::RectOffset::New_ctor(/* left: */ 5, 0, 0, 0));

        toggleFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes = BeatSaberUI::CreateToggle(
            layoutFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes->get_transform(),
            /* text: */ "but only when finishing the song",
            /* value: */ getConfig().OnlyIncrementInPracticeModeWhenThePlayerFinishes.GetValue(),
            [](bool newValue) {
                getConfig().OnlyIncrementInPracticeModeWhenThePlayerFinishes.SetValue(newValue);
            }
        );
        toggleFor_OnlyIncrementInPracticeModeWhenThePlayerFinishes->set_interactable(getConfig().IncrementCountInPracticeMode.GetValue());
        #pragma endregion In practice mode

        #pragma region Banned beatmap characteristics
        textFor_BannedBeatmapCharacteristics = BeatSaberUI::CreateText(container->get_transform(), "Banned gamemodes / beatmap characteristics");

        layoutFor_BannedBeatmapCharacteristics = BeatSaberUI::CreateVerticalLayoutGroup(container->get_transform());
        layoutFor_BannedBeatmapCharacteristics->GetComponent<UnityEngine::UI::LayoutElement*>()->set_preferredWidth(90.0f);
        layoutFor_BannedBeatmapCharacteristics->GetComponent<UnityEngine::UI::ContentSizeFitter*>()->set_verticalFit(UnityEngine::UI::ContentSizeFitter::FitMode::PreferredSize);
        layoutFor_BannedBeatmapCharacteristics->set_padding(UnityEngine::RectOffset::New_ctor(/* left: */ 5, 0, 0, 0));

        CreateBannedBeatmapCharacteristicsToggles(layoutFor_BannedBeatmapCharacteristics->get_transform());
        #pragma endregion Banned beatmap characteristics
    }
}
