/*
 * Copyright (c) 2018-2020 Atmosphère-NX, Reisyukaku, D3fau4
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
#include <stratosphere.hpp>
#include "rnxmitm_initialization.hpp"
#include "rnxmitm_fs_utils.hpp"
#include "bpc_mitm/bpc_ams_power_utils.hpp"
#include "set_mitm/settings_sd_kvs.hpp"

namespace ams::mitm {

    namespace {

        void InitializeThreadFunc(void *arg);

        constexpr size_t InitializeThreadStackSize = 0x4000;
        constexpr int    InitializeThreadPriority  = -7;

        /* Globals. */
        os::Event g_init_event(os::EventClearMode_ManualClear);

        os::ThreadType g_initialize_thread;
        alignas(os::ThreadStackAlignment) u8 g_initialize_thread_stack[InitializeThreadStackSize];

        /* Initialization implementation */
        void InitializeThreadFunc(void *arg) {
            /* Wait for the SD card to be ready. */
            cfg::WaitSdCardInitialized();

            /* Open global SD card file system, so that other threads can begin using the SD. */
            mitm::fs::OpenGlobalSdCardFileSystem();

            /* Initialize the reboot manager (load a payload off the SD). */
            /* Discard result, since it doesn't need to succeed. */
            mitm::bpc::LoadRebootPayload();

            /* Connect to set:sys. */
            sm::DoWithSession([]() {
                R_ABORT_UNLESS(setsysInitialize());
            });

            /* Load settings off the SD card. */
            settings::fwdbg::InitializeSdCardKeyValueStore();

            /* Ensure that we reboot using the user's preferred method. */
            R_ABORT_UNLESS(mitm::bpc::DetectPreferredRebootFunctionality());

            /* Signal to waiters that we are ready. */
            g_init_event.Signal();
        }

    }

    void StartInitialize() {
        R_ABORT_UNLESS(os::CreateThread(std::addressof(g_initialize_thread), InitializeThreadFunc, nullptr, g_initialize_thread_stack, sizeof(g_initialize_thread_stack), InitializeThreadPriority));
        os::StartThread(std::addressof(g_initialize_thread));
    }

    bool IsInitialized() {
        return g_init_event.TryWait();
    }

    void WaitInitialized() {
        g_init_event.Wait();
    }

}
