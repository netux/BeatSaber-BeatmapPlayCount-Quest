#pragma once

#include "custom-types/shared/macros.hpp"
#include "lapiz/shared/macros.hpp"

#include "GlobalNamespace/StandardGameplaySceneSetupData.hpp"
#include "GlobalNamespace/AudioTimeSyncController.hpp"

#include "Zenject/ITickable.hpp"

DECLARE_CLASS_CODEGEN_INTERFACES(BeatmapPlayCount::Managers, TrackPlaytime, Il2CppObject, classof(::Zenject::ITickable*),
    DECLARE_PRIVATE_FIELD(::GlobalNamespace::AudioTimeSyncController*, audioTimeSyncController);
    DECLARE_PRIVATE_FIELD(StringW, beatmapId);
    DECLARE_PRIVATE_FIELD_DEFAULT(bool, incremented, false);

    DECLARE_CTOR(ctor,
        ::GlobalNamespace::StandardGameplaySceneSetupData* _standardSceneSetupData,
        ::GlobalNamespace::AudioTimeSyncController* _audioTimeSyncController
    );
    DECLARE_OVERRIDE_METHOD(void, Tick, il2cpp_utils::il2cpp_type_check::MetadataGetter<&::Zenject::ITickable::Tick>::get());

    void IncrementPlayCount();
)
