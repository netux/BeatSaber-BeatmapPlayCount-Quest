#include "common.hpp"
#include "hooks.hpp"

#include "managers/TrackPlaytime.hpp"

#include "custom-types/shared/register.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "lapiz/shared/zenject/Zenjector.hpp"
#include "lapiz/shared/zenject/Location.hpp"

#include "Zenject/DiContainer.hpp"
#include "Zenject/FromBinderNonGeneric.hpp"
#include "Zenject/ConcreteIdBinderGeneric_1.hpp"

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
    auto zenjector = Lapiz::Zenject::Zenjector::Get();

    logger.debug("Installing hooks...");
    BeatmapPlayCount::Hooks::install();
    logger.debug("Installed all hooks!");

    logger.debug("Setting up Zenject installers...");
    zenjector->Install(Lapiz::Zenject::Location::StandardPlayer, [](Zenject::DiContainer* container){
        // container-> is used to access the DiContainer address,
        // where you can install anything you need.
        container->BindInterfacesAndSelfTo<BeatmapPlayCount::Managers::TrackPlaytime*>()->AsSingle()->NonLazy();
    });
    logger.debug("Setup all installers!");
}
