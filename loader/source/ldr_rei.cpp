#include <switch.h>
#include <stratosphere.hpp>
#include <stdio.h>
#include "ldr_rei.hpp"

Result RNXService::GetReiNXVersion(Out<u32> maj, Out<u32> min) {
    u32 ret = 0;
    *maj.GetPointer() = VERSION_MAJOR;
    *min.GetPointer() = VERSION_MINOR;
    return 0;
}

Result RNXService::SetHbTidForDelta(u64 tid) {
    ContentManagement::HbOverrideTid = tid;
    return 0;
}