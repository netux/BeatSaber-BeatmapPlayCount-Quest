#include "hooks/install.hpp"
#include "hooks/PatchPlayCountLabel.hpp"

namespace BeatmapPlayCount::Hooks {
    void installAll() {
        BeatmapPlayCount::Hooks::PatchPlayCountLabel::install();
    }
}
