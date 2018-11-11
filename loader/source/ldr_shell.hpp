#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum ShellServiceCmd {
    Shell_Cmd_AddTitleToLaunchQueue = 0,
    Shell_Cmd_ClearLaunchQueue = 1,
};

class ShellService final : public IServiceObject {
    private:
        /* Actual commands. */
        Result AddTitleToLaunchQueue(u64 tid, InPointer<char> args, u32 args_size);
        void ClearLaunchQueue();
        
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<Shell_Cmd_AddTitleToLaunchQueue, &ShellService::AddTitleToLaunchQueue>(),
            MakeServiceCommandMeta<Shell_Cmd_ClearLaunchQueue, &ShellService::ClearLaunchQueue>(),
        };
};