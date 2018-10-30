#pragma once
#include <switch.h>

#include <stratosphere.hpp>
#include "ldr_registration.hpp"

enum RoServiceCmd {
    Ro_Cmd_LoadNro = 0,
    Ro_Cmd_UnloadNro = 1,
    Ro_Cmd_LoadNrr = 2,
    Ro_Cmd_UnloadNrr = 3,
    Ro_Cmd_Initialize = 4,
};

class RelocatableObjectsService final : public IServiceObject {
    Handle process_handle = 0;
    u64 process_id = U64_MAX;
    bool has_initialized = false;
    public:
        virtual ~RelocatableObjectsService() override {
            Registration::CloseRoService(this, this->process_handle);
            if (this->has_initialized) {
                svcCloseHandle(this->process_handle);
            }
        }
        
    private:
        /* Actual commands. */
        Result LoadNro(Out<u64> load_address, PidDescriptor pid_desc, u64 nro_address, u64 nro_size, u64 bss_address, u64 bss_size);
        Result UnloadNro(PidDescriptor pid_desc, u64 nro_address);
        Result LoadNrr(PidDescriptor pid_desc, u64 nrr_address, u64 nrr_size);
        Result UnloadNrr(PidDescriptor pid_desc, u64 nrr_address);
        Result Initialize(PidDescriptor pid_desc, CopiedHandle process_h);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<Ro_Cmd_LoadNro, &RelocatableObjectsService::LoadNro>(),
            MakeServiceCommandMeta<Ro_Cmd_UnloadNro, &RelocatableObjectsService::UnloadNro>(),
            MakeServiceCommandMeta<Ro_Cmd_LoadNrr, &RelocatableObjectsService::LoadNrr>(),
            MakeServiceCommandMeta<Ro_Cmd_UnloadNrr, &RelocatableObjectsService::UnloadNrr>(),
            MakeServiceCommandMeta<Ro_Cmd_Initialize, &RelocatableObjectsService::Initialize>(),
        };
};
