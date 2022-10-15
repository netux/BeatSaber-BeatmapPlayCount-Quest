#include "common.hpp"
#include "hooks.hpp"

#include "GlobalNamespace/StandardLevelDetailView.hpp"
#include "GlobalNamespace/LevelParamsPanel.hpp"
#include "GlobalNamespace/IBeatmapLevel.hpp"
#include "GlobalNamespace/IDifficultyBeatmap.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "TMPro/TextMeshProUGUI.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"

namespace BeatmapPlayCount::Hooks {
    static SafePtrUnity<TMPro::TextMeshProUGUI> playCountTextGameObject;

    MAKE_HOOK_MATCH(PatchCountTextAndTrackLastBeatmapId, &GlobalNamespace::StandardLevelDetailView::RefreshContent, void,
        GlobalNamespace::StandardLevelDetailView* instance
    ) {
        PatchCountTextAndTrackLastBeatmapId(instance);

        if (!playCountTextGameObject)
        {
            // Get reference text GameObject

            auto levelParamsPanel = RET_V_UNLESS(getLogger(), il2cpp_utils::GetFieldValue<GlobalNamespace::LevelParamsPanel*>(instance, "_levelParamsPanel"));
            auto notesPerSecondText = RET_V_UNLESS(getLogger(), il2cpp_utils::GetFieldValue<TMPro::TextMeshProUGUI*>(levelParamsPanel, "_notesPerSecondText"));

            // Create text GameObject from reference

            playCountTextGameObject = UnityEngine::GameObject::Instantiate(notesPerSecondText, instance->get_transform());
            playCountTextGameObject->set_name("PlayCountText");
            playCountTextGameObject->get_transform()->set_localPosition(UnityEngine::Vector3(20.0, -15.5, 0.0));

            auto textComp = playCountTextGameObject->GetComponent<TMPro::TextMeshProUGUI*>();
            textComp->set_fontStyle(TMPro::FontStyles::Normal);
            textComp->set_alignment(TMPro::TextAlignmentOptions::Right);
            textComp->set_enableWordWrapping(false);
        }

        auto beatmapId = std::string(instance->get_selectedDifficultyBeatmap()->get_level()->i_IPreviewBeatmapLevel()->get_levelID());
        getLogger().debug("beatmap id: %s", beatmapId.c_str());

        auto count = getStorage().getPlayCount(beatmapId);

        // TODO(netux): replace "plyd #:" with play button icon
        playCountTextGameObject->set_text("plyd #: " + std::to_string(count));
    }

    void install() {
        INSTALL_HOOK(getLogger(), PatchCountTextAndTrackLastBeatmapId);
    }
}
