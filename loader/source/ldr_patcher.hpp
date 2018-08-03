#pragma once
#include <switch.h>
#include <cstdio>

#include "ldr_nso.hpp"

class Patcher {  
    public:
        static void ApplyPatches(u64 title_id, u8 *mapped_nso, size_t size);
};