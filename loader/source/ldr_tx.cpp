#include <switch.h>
#include <stratosphere.hpp>
#include <stdio.h>
#include "ldr_tx.hpp"

Result TXService::TX_123(InPointer<char> data) {
    u32 ret = 0;
    //Returns either 0xA08 or 0; deals with sending rom data
    char buf[0x100] = {0};
    memcpy(buf, armGetTls(), 0x100);
    char g_nso_path[FS_MAX_PATH] = {0};
    std::fill(g_nso_path, g_nso_path + FS_MAX_PATH, 0);
    snprintf(g_nso_path, FS_MAX_PATH, "sdmc:/ipc123.bin");
    FILE *fp = fopen(g_nso_path, "wb");
    fwrite(buf, 0x100, 1, fp);
    fclose(fp);
    return 0;
}

Result TXService::TX_124(Out<u32> ret) {
    //checks a flag set by ipc123
    ret.SetValue(0x100);
    return 0;
}

Result TXService::TX_125() {
    Result ret = 0;
    //This isnt even called in rommenu afaik
    return ret;
}

Result TXService::IsLicenseValid(Out<u32> ret) {
    //The answer to unlocking piracy tools is to summon satan
    ret.SetValue(0x666);
    return 0;
}

Result TXService::TX_127() {
    u32 ret = 0x666;
    //More license related garbo
    return ret;
}

Result TXService::TX_128() {
    Result ret = 0;
    return ret;
}

Result TXService::IsPro() {
    Result ret = 1;
    return ret;
}

Result TXService::SetFTP() {
    Result ret = 0;
    return ret;
}

Result TXService::GetFTP() {
    Result ret = 1;
    //0=enabled; 1=disabled
    //If ftpmode disabled(?) tx130(1);
    return ret;
}

Result TXService::GetIPAddr() {
    Result ret = 0;
    return ret;
}

Result TXService::TX_133() {
    Result ret = 0;
    return ret;
}

Result TXService::SetStealthMode() {
    Result ret = 0;
    //0=enabled; 1=disabled
    //If stealthmode disabled(?) tx134(1);
    return ret;
}

Result TXService::GetStealthMode() {
    Result ret = 1;
    //0=enabled; 1=disabled
    return ret;
}

Result TXService::TX_136() {
    Result ret = 0;
    return ret;
}

Result TXService::TX_137() {
    Result ret = 0;
    return ret;
}
