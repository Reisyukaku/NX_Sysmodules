#pragma once
#include <switch.h>
#include <stratosphere.hpp>
#include "ldr_content_management.hpp"

enum RNXServiceCmd {
    RNX_CMD_GetReiNXVersion = 0,
    RNX_CMD_SetHbTidForDelta = 1,
};

class RNXService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result GetReiNXVersion();
        Result SetHbTidForDelta(u64 tid);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<RNX_CMD_GetReiNXVersion, &RNXService::GetReiNXVersion>(),
            MakeServiceCommandMeta<RNX_CMD_SetHbTidForDelta, &RNXService::SetHbTidForDelta>(),
        };
};