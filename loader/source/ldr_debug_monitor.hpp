#pragma once
#include <switch.h>

#include <stratosphere.hpp>
#include "ldr_registration.hpp"

enum DebugMonitorServiceCmd {
    Dmnt_Cmd_AddTitleToLaunchQueue = 0,
    Dmnt_Cmd_ClearLaunchQueue = 1,
    Dmnt_Cmd_GetNsoInfo = 2
};

class DebugMonitorService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result AddTitleToLaunchQueue(u64 args_size, u64 tid, InPointer<char> args);
        void ClearLaunchQueue();
        Result GetNsoInfo(Out<u32> count, OutPointerWithClientSize<Registration::NsoInfo> out, u64 pid);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<Dmnt_Cmd_AddTitleToLaunchQueue, &DebugMonitorService::AddTitleToLaunchQueue>(),
            MakeServiceCommandMeta<Dmnt_Cmd_ClearLaunchQueue, &DebugMonitorService::ClearLaunchQueue>(),
            MakeServiceCommandMeta<Dmnt_Cmd_GetNsoInfo, &DebugMonitorService::GetNsoInfo>(),
        };
};
