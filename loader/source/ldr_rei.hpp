#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum RNXServiceCmd {
    RNX_GetVersion = 0,
};

class RNXService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result RNX_GetVersion();
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<RNX_GetVersion, &RNXService::RNX_GetVersion>(),
        };
};