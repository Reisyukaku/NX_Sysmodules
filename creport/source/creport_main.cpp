/*
* Copyright (c) 2018 EliseZeroTwo
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

/* just so people stfu about creport memes */

#include <switch.h>
#include <stratosphere.hpp>
#include <stdlib.h>

extern "C" {
    extern u32 __start__;

    u32 __nx_applet_type = AppletType_None;

    #define INNER_HEAP_SIZE 0x100000
    size_t nx_inner_heap_size = INNER_HEAP_SIZE;
    char   nx_inner_heap[INNER_HEAP_SIZE];

    void __libnx_initheap(void);
    void __appInit(void);
    void __appExit(void);
}


void __libnx_initheap(void) {
	void*  addr = nx_inner_heap;
	size_t size = nx_inner_heap_size;

	/* Newlib */
	extern char* fake_heap_start;
	extern char* fake_heap_end;

	fake_heap_start = (char*)addr;
	fake_heap_end   = (char*)addr + size;
}

void __appInit(void) {
    Result rc;
    
    SetFirmwareVersionForLibnx();

    rc = smInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_SM));
    }

    rc = fsInitialize();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    }

    rc = fsdevMountSdmc();
    if (R_FAILED(rc)) {
        fatalSimple(MAKERESULT(Module_Libnx, LibnxError_InitFail_FS));
    }
}

void __appExit(void) {
    /* Cleanup services. */
    fsdevUnmountAll();
    fsExit();
    smExit();
}

int main(int argc, char ** argv) {
	if (argc < 2)
		return 0;

	for (int x = 0; x < argc; x++)
		if (argv[x] == NULL)
			return 0;

	/* i seriously couldn't care less about crash reports so this is a REALLY stripped creport */
	u64 pid = strtoull(argv[0], NULL, 10);
	if (R_SUCCEEDED(nsdevInitialize())) {
          nsdevTerminateProcess(pid);
          nsdevExit();
	}
	if(kernelAbove500() || argv[1][0] == '1')
		return 0;

	/* if we get here, just fatal with an incomplete report as the reason */
	fatalSimple(0xC6A8);
}
