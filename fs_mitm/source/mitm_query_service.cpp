#include <switch.h>
#include <stratosphere.hpp>
#include "mitm_query_service.hpp"

static std::vector<u64> g_known_pids;
static std::vector<u64> g_known_tids;
static HosMutex g_pid_tid_mutex;

Result MitMQueryUtils::get_associated_tid_for_pid(u64 pid, u64 *tid) {
    Result rc = 0xCAFE;
    g_pid_tid_mutex.Lock();
    for (unsigned int i = 0; i < g_known_pids.size(); i++) {
        if (g_known_pids[i] == pid) {
            *tid = g_known_tids[i];
            rc = 0x0;
            break;
        }
    }
    g_pid_tid_mutex.Unlock();
    return rc;
}

void MitMQueryUtils::associate_pid_to_tid(u64 pid, u64 tid) {
    g_pid_tid_mutex.Lock();
    g_known_pids.push_back(pid);
    g_known_tids.push_back(tid);
    g_pid_tid_mutex.Unlock();
}