#include <switch.h>
#include <algorithm>
#include <array>
#include <cstdio>
#include "ldr_launch_queue.hpp"
#include "meta_tools.hpp"

static std::array<LaunchQueue::LaunchItem, LAUNCH_QUEUE_SIZE> g_launch_queue = {0};

Result LaunchQueue::Add(u64 tid, const char *args, u64 arg_size) {
    if(arg_size > LAUNCH_QUEUE_ARG_SIZE_MAX) {
        return 0x209;
    }
    
    int idx = GetFreeIndex(tid);
    if(idx == LAUNCH_QUEUE_FULL)
        return 0x409;
    
    g_launch_queue[idx].tid = tid;
    g_launch_queue[idx].arg_size = arg_size;
    
    std::copy(args, args + arg_size, g_launch_queue[idx].args);
    return 0x0;
}

Result LaunchQueue::AddCopy(u64 tid_base, u64 tid) {
    int idx = GetIndex(tid_base);
    if (idx == LAUNCH_QUEUE_FULL) {
        return 0x0;
    }
    
    return Add(tid, g_launch_queue[idx].args, g_launch_queue[idx].arg_size);
}


Result LaunchQueue::AddItem(const LaunchItem *item) {
    if(item->arg_size > LAUNCH_QUEUE_ARG_SIZE_MAX) {
        return 0x209;
    }
    
    int idx = GetFreeIndex(item->tid);
    if(idx == LAUNCH_QUEUE_FULL)
        return 0x409;
    
    g_launch_queue[idx] = *item;
    return 0x0;
}

int LaunchQueue::GetIndex(u64 tid) {
    auto it = std::find_if(g_launch_queue.begin(), g_launch_queue.end(), member_equals_fn(&LaunchQueue::LaunchItem::tid, tid));
    if (it == g_launch_queue.end()) {
        return LAUNCH_QUEUE_FULL;
    }
    return std::distance(g_launch_queue.begin(), it);
}

int LaunchQueue::GetFreeIndex(u64 tid) {
    for(unsigned int i = 0; i < LAUNCH_QUEUE_SIZE; i++) {
        if(g_launch_queue[i].tid == tid || g_launch_queue[i].tid == 0x0) {
            return i;
        }
    }
    return LAUNCH_QUEUE_FULL;
}

bool LaunchQueue::Contains(u64 tid) {
    return GetIndex(tid) != LAUNCH_QUEUE_FULL;
}

void LaunchQueue::Clear() {
    for (unsigned int i = 0; i < LAUNCH_QUEUE_SIZE; i++) {
        g_launch_queue[i].tid = 0;
    }
}


LaunchQueue::LaunchItem *LaunchQueue::GetItem(u64 tid) {
    int idx = GetIndex(tid);
    if (idx == LAUNCH_QUEUE_FULL) {
        return NULL;
    }
    return &g_launch_queue[idx];
}
