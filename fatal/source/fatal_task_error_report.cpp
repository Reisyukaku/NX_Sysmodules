/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>
#include <switch.h>

#include "fatal_task_error_report.hpp"
#include "fatal_config.hpp"

void ErrorReportTask::EnsureReportDirectories() {
    char path[FS_MAX_PATH];  
    strcpy(path, "sdmc:/ReiNX");
    mkdir(path, S_IRWXU);
    strcat(path, "/fatal_reports");
    mkdir(path, S_IRWXU);
    strcat(path, "/dumps");
    mkdir(path, S_IRWXU);
}

bool ErrorReportTask::GetCurrentTime(u64 *out) {
    *out = 0;
    
    /* Verify that pcv isn't dead. */
    {
        Handle dummy;
        if (R_SUCCEEDED(smRegisterService(&dummy, "time:s", false, 0x20))) {
            svcCloseHandle(dummy);
            return false;
        }
    }
    
    /* Try to get the current time. */
    bool success = false;
    if (R_SUCCEEDED(timeInitialize())) {
        if (R_SUCCEEDED(timeGetCurrentTime(TimeType_LocalSystemClock, out))) {
            success = true;
        }
        timeExit();
    }
    return success;
}

Result ErrorReportTask::Run() {
    /* Signal we're done with our job. */
    eventFire(this->erpt_event);

    return 0;
}