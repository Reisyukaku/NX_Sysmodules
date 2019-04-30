#include <switch.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <functional>
#include <stratosphere.hpp>
#include "ldr_cht.hpp"
#include "ldr_registration.hpp"

bool isAppActive(){
    bool active = false;
    u64 pid;
    if (svcGetProcessId(&pid, 0xFFFFFFFF)) 
        return active;
    
    Registration::Process *proc;
    for(int i = 0; i < REGISTRATION_LIST_MAX; i++){
        proc = Registration::GetProcess(i);
        if(proc->procFlags & 0x40){
            active = true;
            break;
        }
    }
    if(!active) svcCloseHandle(0xFFFFFFFF);
    
    return active;    
}

unsigned debugActiveProcess(){
    unsigned ret = 1;
    u64 pid = 0, tid = 0;
    u8 *eventOut;
    Handle debug = 0xFFFFFFFF;
    if(!isAppActive()){
        Registration::Process *proc;
        for(int i = 0; i < REGISTRATION_LIST_MAX; i++){
            proc = Registration::GetProcess(i);
            if(proc->procFlags & 0x40){
                pid = proc->process_id;
                break;
            }
        }
        svcDebugActiveProcess(&debug, pid);
        svcGetDebugEvent(eventOut, 0xFFFFFFFF);
        svcGetInfo(&tid, 18, 0, 0);
        svcContinueDebugEvent(0xFFFFFFFF, 5, &tid, 1);
        
        ret = 0;
    }
    
    return ret;
}

Result CheatService::Cmd_0(OutPointerWithClientSize<BuildID> out) {
    u32 ret = 0;
    Registration::Process *proc;
    for(int i = 0; i < REGISTRATION_LIST_MAX; i++){
        proc = Registration::GetProcess(i);
        if(proc->procFlags & 0x40){
            auto nso_info_it = std::find_if_not(proc->nso_infos.begin(), proc->nso_infos.end(), std::mem_fn(&Registration::NsoInfoHolder::in_use));
            memcpy(out.pointer, nso_info_it->info.build_id, out.num_elements);
        }
    }
    
    return ret;
}

Result CheatService::Cmd_1() {
    Result ret = 0;
    return ret;
}

Result CheatService::Cmd_2() {
    Result ret = 0;
    MemoryInfo meminfo;
    u32 pageinfo;
    
    if(!debugActiveProcess()){
        u64 i = 0;
        do {
            svcQueryDebugProcessMemory(&meminfo, &pageinfo, 0xFFFFFFFF, i);
            //i+=0x100;
        }while(i);
    }
    
    return ret;
}

Result CheatService::Cmd_3() {
    Result ret = 0;
    return ret;;
}

Result CheatService::GetHeapMem() {
    Result ret = 0;
    return ret;;
}

Result CheatService::SetHeapMem() {
    Result ret = 0;
    return ret;;
}

Result CheatService::Cmd_6() {
    Result ret = 0;
    return ret;;
}

Result CheatService::Cmd_7() {
    Result ret = 0;
    return ret;;
}

Result CheatService::Cmd_8() {
    Result ret = 0;
    return ret;;
}

Result CheatService::Cmd_9() {
    Result ret = 0;
    return ret;;
}