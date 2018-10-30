#pragma once
#include <switch.h>
#include <stratosphere.hpp>

#include "ldr_registration.hpp"
#include "ldr_process_creation.hpp"

enum ProcessManagerServiceCmd {
    Pm_Cmd_CreateProcess = 0,
    Pm_Cmd_GetProgramInfo = 1,
    Pm_Cmd_RegisterTitle = 2,
    Pm_Cmd_UnregisterTitle = 3
};

class ProcessManagerService final : public IServiceObject {
    struct ProgramInfo {
        u8 main_thread_priority;
        u8 default_cpu_id;
        u16 application_type;
        u32 main_thread_stack_size;
        u64 title_id;
        u32 acid_sac_size;
        u32 aci0_sac_size;
        u32 acid_fac_size;
        u32 aci0_fah_size;
        u8 ac_buffer[0x3E0];
    };
    
    static_assert(sizeof(ProcessManagerService::ProgramInfo) == 0x400, "Incorrect ProgramInfo definition.");      
    private:
        /* Actual commands. */
        Result CreateProcess(Out<MovedHandle> proc_h, u64 flags, u64 index, CopiedHandle reslimit_h);
        Result GetProgramInfo(Registration::TidSid tid_sid, OutPointerWithServerSize<ProcessManagerService::ProgramInfo, 0x1> out_program_info);
        Result RegisterTitle(Out<u64> index, Registration::TidSid tid_sid);
        Result UnregisterTitle(u64 index);
        
        /* Utilities */
        Result PopulateProgramInfoBuffer(ProcessManagerService::ProgramInfo *out, Registration::TidSid *tid_sid);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<Pm_Cmd_CreateProcess, &ProcessManagerService::CreateProcess>(),
            MakeServiceCommandMeta<Pm_Cmd_GetProgramInfo, &ProcessManagerService::GetProgramInfo>(),
            MakeServiceCommandMeta<Pm_Cmd_RegisterTitle, &ProcessManagerService::RegisterTitle>(),
            MakeServiceCommandMeta<Pm_Cmd_UnregisterTitle, &ProcessManagerService::UnregisterTitle>(),
        };
};
