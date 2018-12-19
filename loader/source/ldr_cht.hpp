#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum CheatServiceCmd {
    Cht_Cmd_0 = 0,
    Cht_Cmd_1 = 1,
    Cht_Cmd_2 = 2,
    Cht_Cmd_3 = 3,
    Cht_GetHeapMem = 4,
    Cht_SetHeapMem = 5,
    Cht_Cmd_6 = 6,
    Cht_Cmd_7 = 7,
    Cht_Cmd_8 = 8,
    Cht_Cmd_9 = 9,
};

typedef struct{
    char build_id[0x20];
} BuildID;

class CheatService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result Cmd_0(OutPointerWithClientSize<BuildID> out);
        Result Cmd_1();
        Result Cmd_2();
        Result Cmd_3();
        Result GetHeapMem();
        Result SetHeapMem();
        Result Cmd_6();
        Result Cmd_7();
        Result Cmd_8();
        Result Cmd_9();
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<Cht_Cmd_0, &CheatService::Cmd_0>(),
            MakeServiceCommandMeta<Cht_Cmd_1, &CheatService::Cmd_1>(),
            MakeServiceCommandMeta<Cht_Cmd_2, &CheatService::Cmd_2>(),
            MakeServiceCommandMeta<Cht_Cmd_3, &CheatService::Cmd_3>(),
            MakeServiceCommandMeta<Cht_GetHeapMem, &CheatService::GetHeapMem>(),
            MakeServiceCommandMeta<Cht_SetHeapMem, &CheatService::SetHeapMem>(),
            MakeServiceCommandMeta<Cht_Cmd_6, &CheatService::Cmd_6>(),
            MakeServiceCommandMeta<Cht_Cmd_7, &CheatService::Cmd_7>(),
            MakeServiceCommandMeta<Cht_Cmd_8, &CheatService::Cmd_8>(),
            MakeServiceCommandMeta<Cht_Cmd_9, &CheatService::Cmd_9>(),
        };
};