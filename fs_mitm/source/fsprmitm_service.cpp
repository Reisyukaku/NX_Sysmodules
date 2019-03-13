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
 
#include <map>
#include <memory>
#include <mutex>

#include <switch.h>
#include <stratosphere.hpp>
#include "fsprmitm_service.hpp"
#include "fs_shim.h"

void FsprMitmService::PostProcess(IMitmServiceObject *obj, IpcResponseContext *ctx) {
    //Stub
}

Result FsprMitmService::SetEnabledProgramVerification(bool enable) {
    // Disregard args. We dont need verification ;^)
    return fsSetEnabledProgramVerificationFwd(this->forward_service.get(), 0);
}