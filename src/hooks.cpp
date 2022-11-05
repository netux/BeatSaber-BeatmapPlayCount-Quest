#include "common.hpp"
#include "hooks.hpp"
#include "BundledResources.hpp"

#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/LevelParamsPanel.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/LocalizedHoverHint.hpp"
#include "HMUI/HoverHint.hpp"
#include "HMUI/ImageView.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/UI/HorizontalLayoutGroup.hpp"

namespace BeatmapPlayCount::Hooks {
    SafePtrUnity<UnityEngine::GameObject> playCountContainerGameObject;
    SafePtrUnity<TMPro::TextMeshProUGUI> playCountText;

    MAKE_HOOK_MATCH(PatchCountTextAndTrackLastBeatmapId, &GlobalNamespace::StandardLevelDetailView::RefreshContent, void,
        GlobalNamespace::StandardLevelDetailView* instance
    ) {
        PatchCountTextAndTrackLastBeatmapId(instance);

        if (!playCountContainerGameObject)
        {
            // Get reference GameObject
            auto levelParamsPanel = RET_V_UNLESS(getLogger(), il2cpp_utils::GetFieldValue<GlobalNamespace::LevelParamsPanel*>(instance, "_levelParamsPanel"));
            auto notesPerSecondText = RET_V_UNLESS(getLogger(), il2cpp_utils::GetFieldValue<TMPro::TextMeshProUGUI*>(levelParamsPanel, "_notesPerSecondText"));
            auto notesPerSecondContainerGameObject = notesPerSecondText->get_transform()->get_parent()->get_gameObject();

            // Create play count text from reference
            playCountContainerGameObject = UnityEngine::GameObject::Instantiate(
                notesPerSecondContainerGameObject,
                notesPerSecondContainerGameObject->get_transform()->get_parent()->get_parent() /* LevelDetail game object */
            );
            playCountContainerGameObject->set_name("Play Count");
            playCountContainerGameObject->get_transform()->set_localPosition(UnityEngine::Vector3(4.0, -3.0, 0.0));


            auto playCountContainerHLG = playCountContainerGameObject->AddComponent<UnityEngine::UI::HorizontalLayoutGroup*>();
            playCountContainerHLG->set_spacing(0.5f);
            playCountContainerHLG->set_childForceExpandWidth(false);
            playCountContainerHLG->set_childForceExpandHeight(false);
            playCountContainerHLG->set_childAlignment(UnityEngine::TextAnchor::MiddleRight);

            UnityEngine::Object::Destroy(playCountContainerGameObject->GetComponent<HMUI::HoverHint*>());                     // remove as this is causing exception spam
            UnityEngine::Object::Destroy(playCountContainerGameObject->GetComponent<GlobalNamespace::LocalizedHoverHint*>()); // when hovering over the label

            auto playCountIconGameObject = playCountContainerGameObject->get_transform()->Find("Icon")->get_gameObject();
            playCountIconGameObject->GetComponent<HMUI::ImageView*>()->set_sprite(BeatmapPlayCount::BundledResources::GetPlayCountSprite());

            auto playCountTextGameObject = playCountContainerGameObject->get_transform()->Find("ValueText")->get_gameObject();

            playCountText = playCountTextGameObject->GetComponent<TMPro::TextMeshProUGUI*>();
            playCountText->set_fontStyle(TMPro::FontStyles::Normal);
            playCountText->set_alignment(TMPro::TextAlignmentOptions::Right);
            playCountText->set_enableWordWrapping(false);
        }

        auto beatmapId = std::string(instance->get_selectedDifficultyBeatmap()->get_level()->i_IPreviewBeatmapLevel()->get_levelID());
        getLogger().debug("beatmap id: %s", beatmapId.c_str());

        auto count = getStorage().getPlayCount(beatmapId);

        playCountText->set_text(std::to_string(count));
    }

    void install() {
        INSTALL_HOOK(getLogger(), PatchCountTextAndTrackLastBeatmapId);
    }
}
