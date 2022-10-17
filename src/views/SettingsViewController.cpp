#include "common.hpp"
#include "views/SettingsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
using namespace QuestUI;

#include "UnityEngine/RectOffset.hpp"
#include "UnityEngine/UI/LayoutElement.hpp"

DEFINE_TYPE(BeatmapPlayCount::Views, SettingsViewController);

namespace BeatmapPlayCount::Views {
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
    }
}
