#include <switch.h>
#include <cstdio>
#include <algorithm>
#include <stratosphere.hpp>
#include "ldr_debug_monitor.hpp"
#include "ldr_launch_queue.hpp"
#include "ldr_registration.hpp"

Result DebugMonitorService::AddTitleToLaunchQueue(u64 args_size, u64 tid, InPointer<char> args) {
    fprintf(stderr, "Add to launch queue: %p, %zX\n", args.pointer, std::min(args_size, args.num_elements));
    return LaunchQueue::Add(tid, args.pointer, std::min(args_size, args.num_elements));
}

void DebugMonitorService::ClearLaunchQueue() {
    LaunchQueue::Clear();
}

Result DebugMonitorService::GetNsoInfo(Out<u32> count, OutPointerWithClientSize<Registration::NsoInfo> out, u64 pid) {
    /* Zero out the output memory. */
    std::fill(out.pointer, out.pointer + out.num_elements, (const Registration::NsoInfo){0});
    /* Actually return the nso infos. */
    return Registration::GetNsoInfosForProcessId(out.pointer, out.num_elements, pid, count.GetPointer());
}
