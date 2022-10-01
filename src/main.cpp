#include "common.hpp"
#include "hooks.hpp"
#include "managers.hpp"

#include "custom-types/shared/register.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

BeatmapPlayCount::Storage& getStorage() {
    static BeatmapPlayCount::Storage* storage = new BeatmapPlayCount::Storage(modInfo);
    return *storage;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    modInfo = info;

    // getStorage().load();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();

    auto& logger = getLogger();

    logger.debug("Installing hooks...");
    BeatmapPlayCount::Hooks::install();
    logger.debug("Installed all hooks!");
}
