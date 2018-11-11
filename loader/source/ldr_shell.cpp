#include <switch.h>
#include <stratosphere.hpp>
#include "ldr_shell.hpp"
#include "ldr_launch_queue.hpp"

Result ShellService::AddTitleToLaunchQueue(u64 tid, InPointer<char> args, u32 args_size) {
    if (args.num_elements < args_size) args_size = args.num_elements;
    return LaunchQueue::Add(tid, args.pointer, args_size);
}

void ShellService::ClearLaunchQueue() {
    LaunchQueue::Clear();
}