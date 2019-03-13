/*
 * Copyright (c) 2018 Reisyukaku
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
 
#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum FspprSrvCmd : u32 {
    FspPrCmd_SetEnabledProgramVerification = 256,
};

class FsprMitmService : public IMitmServiceObject {
    public:
        FsprMitmService(std::shared_ptr<Service> s, u64 pid) : IMitmServiceObject(s, pid) {

        }
        
        static bool ShouldMitm(u64 pid, u64 tid) {
            return true;
        }
        
        static void PostProcess(IMitmServiceObject *obj, IpcResponseContext *ctx);
            
    protected:
        /* Overridden commands. */
        Result SetEnabledProgramVerification(bool enable);
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<FspPrCmd_SetEnabledProgramVerification, &FsprMitmService::SetEnabledProgramVerification>(),
        };
};
