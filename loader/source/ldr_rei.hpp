#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum RNXServiceCmd {
    RNX_Cmd_0 = 0,
};

class RNXService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result RNX_0();
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<RNX_Cmd_0, &RNXService::RNX_0>(),
        };
};