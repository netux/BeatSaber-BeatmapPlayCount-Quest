#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

#include "GlobalNamespace/GameplayCoreSceneSetupData.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"
#include "GlobalNamespace/AudioTimeSyncController_InitData.hpp"
#include "GlobalNamespace/ILevelEndActions.hpp"

#include "Zenject/IInitializable.hpp"
#include "Zenject/ITickable.hpp"
#include "System/IDisposable.hpp"

#include "System/Action.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(BeatmapPlayCount::Managers, TrackPlaytime, Il2CppObject, std::vector<Il2CppClass*>({ classof(::Zenject::IInitializable*), classof(::Zenject::ITickable*), classof(::System::IDisposable*) }),
    DECLARE_PRIVATE_FIELD(::GlobalNamespace::AudioTimeSyncController*, audioTimeSyncController);
    DECLARE_PRIVATE_FIELD(::GlobalNamespace::ILevelEndActions*, levelEndActionImpl);
    DECLARE_PRIVATE_FIELD(::System::Action*, handleLevelFinishedEventAction);
    DECLARE_PRIVATE_FIELD(StringW, beatmapId);
    DECLARE_PRIVATE_FIELD(StringW, beatmapCharacteristic);
    DECLARE_PRIVATE_FIELD_DEFAULT(bool, incremented, false);
    DECLARE_PRIVATE_FIELD(bool, isGameplayInPracticeMode);
    DECLARE_PRIVATE_FIELD(bool, isGameplayAnExternalModReplay);
    DECLARE_PRIVATE_FIELD(bool, doesBeatmapHaveBannedCharacteristic);
    DECLARE_PRIVATE_FIELD(float, songStartTime);

    DECLARE_CTOR(ctor,
        ::GlobalNamespace::GameplayCoreSceneSetupData* _sceneSetupData,
        ::GlobalNamespace::AudioTimeSyncController* _audioTimeSyncController,
        ::GlobalNamespace::AudioTimeSyncController::InitData* _audioTimeSyncControllerInitData,
        ::GlobalNamespace::ILevelEndActions* _levelEndActionImpl
    );

    DECLARE_INSTANCE_METHOD(void, handleLevelFinishedEvent);

    DECLARE_OVERRIDE_METHOD(void, Initialize, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::IInitializable::Initialize>::get());
    DECLARE_OVERRIDE_METHOD(void, Tick, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::ITickable::Tick>::get());
    DECLARE_OVERRIDE_METHOD(void, Dispose, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::System::IDisposable::Dispose>::get());

private:
    void IncrementPlayCount();

    bool CanIncrementByPercentageBecauseOfPracticeMode();
    bool CanIncrementByPercentage();
    bool CanIncrement();
)
