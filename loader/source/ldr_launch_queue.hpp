#pragma once
#include <switch.h>

#define LAUNCH_QUEUE_SIZE (10)
#define LAUNCH_QUEUE_FULL (-1)

#define LAUNCH_QUEUE_ARG_SIZE_MAX (0x8000)

class LaunchQueue { 
    public:
        struct LaunchItem {
            u64 tid;
            u64 arg_size;
            char args[LAUNCH_QUEUE_ARG_SIZE_MAX];
        };
        
        static LaunchQueue::LaunchItem *GetItem(u64 tid);
        
        static Result Add(u64 tid, const char *args, u64 arg_size);
        static Result AddItem(const LaunchItem *item);
        static Result AddCopy(u64 tid_base, u64 new_tid);
        static int GetIndex(u64 tid);
        static int GetFreeIndex(u64 tid);
        static bool Contains(u64 tid);
        static void Clear();
};