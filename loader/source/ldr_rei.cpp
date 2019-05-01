#include <switch.h>
#include <stratosphere.hpp>
#include <stdio.h>
#include "ldr_rei.hpp"

Result RNXService::GetReiNXVersion() {
    u32 ret = 0;
    //STUB
    return 0;
}

Result RNXService::SetHbTidForDelta(u64 tid) {
    ContentManagement::HbOverrideTid = tid;
    return 0;
}