#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum TXServiceCmd {
    TX_Cmd_123 = 123,
    TX_Cmd_124 = 124,
    TX_Cmd_125 = 125,
    TX_Cmd_IsLicenseValid = 126,
    TX_Cmd_127 = 127,
    TX_Cmd_128 = 128,
    TX_Cmd_IsPro = 129,
    TX_Cmd_SetFTP = 130,
    TX_Cmd_GetFTP = 131,
    TX_Cmd_GetIPAddr = 132,
    TX_Cmd_133 = 133,
    TX_Cmd_SetStealthMode = 134,
    TX_Cmd_GetStealthMode = 135,
    TX_Cmd_136 = 136,
    TX_Cmd_137 = 137
};

class TXService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result TX_123(InPointer<char> data);
        Result TX_124(Out<u32> ret);
        Result TX_125();
        Result IsLicenseValid(Out<u32> ret);
        Result TX_127();
        Result TX_128();
        Result IsPro();
        Result SetFTP();
        Result GetFTP();
        Result GetIPAddr();
        Result TX_133();
        Result SetStealthMode();
        Result GetStealthMode();
        Result TX_136();
        Result TX_137();
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<TX_Cmd_123, &TXService::TX_123>(),
            MakeServiceCommandMeta<TX_Cmd_124, &TXService::TX_124>(),
            MakeServiceCommandMeta<TX_Cmd_125, &TXService::TX_125>(),
            MakeServiceCommandMeta<TX_Cmd_IsLicenseValid, &TXService::IsLicenseValid>(),
            MakeServiceCommandMeta<TX_Cmd_127, &TXService::TX_127>(),
            MakeServiceCommandMeta<TX_Cmd_128, &TXService::TX_128>(),
            MakeServiceCommandMeta<TX_Cmd_IsPro, &TXService::IsPro>(),
            MakeServiceCommandMeta<TX_Cmd_SetFTP, &TXService::SetFTP>(),
            MakeServiceCommandMeta<TX_Cmd_GetFTP, &TXService::GetFTP>(),
            MakeServiceCommandMeta<TX_Cmd_GetIPAddr, &TXService::GetIPAddr>(),
            MakeServiceCommandMeta<TX_Cmd_133, &TXService::TX_133>(),
            MakeServiceCommandMeta<TX_Cmd_SetStealthMode, &TXService::SetStealthMode>(),
            MakeServiceCommandMeta<TX_Cmd_GetStealthMode, &TXService::GetStealthMode>(),
            MakeServiceCommandMeta<TX_Cmd_136, &TXService::TX_136>(),
            MakeServiceCommandMeta<TX_Cmd_137, &TXService::TX_137>(),
        };
};