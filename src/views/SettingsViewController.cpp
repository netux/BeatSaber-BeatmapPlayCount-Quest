#include "common.hpp"
#include "views/SettingsViewController.hpp"

#include "questui/shared/BeatSaberUI.hpp"
using namespace QuestUI;

DEFINE_TYPE(BeatmapPlayCount::Views, SettingsViewController);

namespace BeatmapPlayCount::Views {
    void SettingsViewController::DidActivate(bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
        if(!(firstActivation && addedToHierarchy)) {
            return;
        }

        auto container = BeatSaberUI::CreateScrollableSettingsContainer(this->get_transform());

        BeatSaberUI::CreateSliderSetting(
            container->get_transform(),
            /* text: */ getConfig().MinimumSongProgressToIncrementPlayCount.GetName(),
            /* increment: */ 0.01,
            /* value: */ getConfig().MinimumSongProgressToIncrementPlayCount.GetValue() * 100,
            /* minValue: */ 0, /* maxValue: */ 100,
            [](float newValue) {
                getConfig().MinimumSongProgressToIncrementPlayCount.SetValue(newValue / 100);
            }
        );

        BeatSaberUI::CreateToggle(
            container->get_transform(),
            /* text: */ getConfig().IncrementCountInPracticeMode.GetName(),
            /* value: */ getConfig().IncrementCountInPracticeMode.GetValue(),
            [](bool newValue) {
                getConfig().IncrementCountInPracticeMode.SetValue(newValue);
            }
        );
    }
}
