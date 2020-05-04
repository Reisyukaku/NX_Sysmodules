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
#pragma once
#include <stratosphere.hpp>

namespace ams::ldr {

    class LoaderService : public sf::IServiceObject {
        protected:
            /* Official commands. */
            virtual Result CreateProcess(sf::OutMoveHandle proc_h, PinId id, u32 flags, sf::CopyHandle reslimit_h);
            virtual Result GetProgramInfo(sf::Out<ProgramInfo> out_program_info, const ncm::ProgramLocation &loc);
            virtual Result PinProgram(sf::Out<PinId> out_id, const ncm::ProgramLocation &loc);
            virtual Result UnpinProgram(PinId id);
            virtual Result SetProgramArguments(ncm::ProgramId program_id, const sf::InPointerBuffer &args, u32 args_size);
            virtual Result FlushArguments();
            virtual Result GetProcessModuleInfo(sf::Out<u32> count, const sf::OutPointerArray<ModuleInfo> &out, os::ProcessId process_id);
            virtual Result SetEnabledProgramVerification(bool enabled);

            /* Atmosphere commands. */
            virtual Result AtmosphereRegisterExternalCode(sf::OutMoveHandle out, ncm::ProgramId program_id);
            virtual void   AtmosphereUnregisterExternalCode(ncm::ProgramId program_id);
            virtual void   AtmosphereHasLaunchedProgram(sf::Out<bool> out, ncm::ProgramId program_id);
            virtual Result AtmosphereGetProgramInfo(sf::Out<ProgramInfo> out_program_info, sf::Out<cfg::OverrideStatus> out_status, const ncm::ProgramLocation &loc);
            virtual Result AtmospherePinProgram(sf::Out<PinId> out_id, const ncm::ProgramLocation &loc, const cfg::OverrideStatus &override_status);
    };

    namespace rnx {
        
        class RNXService final : public LoaderService {
            protected:
                enum class CommandId {
                GetReiNXVersion = 0,
                SetHbTidForDelta = 1,
            };
            Result GetReiNXVersion(sf::Out<u32> maj, sf::Out<u32> min);
            Result SetHbTidForDelta(u64 tid);
            public:
                DEFINE_SERVICE_DISPATCH_TABLE {
                    MAKE_SERVICE_COMMAND_META(GetReiNXVersion),
                    MAKE_SERVICE_COMMAND_META(SetHbTidForDelta),
                };
        };
    }

    namespace TX {

        class TXService final : public LoaderService {
            protected:
                enum class CommandId {
                    IsSelectedGamecard = 123,
                    TXGetGamecardStatus = 124,
                    TX_125 = 125,
                    IsLicenseValid = 126,
                    TX_127 = 127,
                    TX_128 = 128,
                    IsPro = 129,
                    SetFTP = 130,
                    GetFTP = 131,
                    GetIPAddr = 132,
                    TX_133 = 133,
                    SetStealthMode = 134,
                    GetStealthMode = 135,
                    TXIsEmuNand = 136,
                    TXIsFat32 = 137,
                    TXSetLdnmitm = 138,
                    TXGetLdnmitm = 139
                };
                Result IsSelectedGamecard(const sf::InPointerBuffer data);
                Result TXGetGamecardStatus(sf::Out<u32> ret);
                Result TX_125();
                Result IsLicenseValid(sf::Out<u32> ret);
                Result TX_127();
                Result TX_128();
                Result IsPro();
                Result SetFTP();
                Result GetFTP();
                Result GetIPAddr();
                Result TX_133();
                Result SetStealthMode();
                Result GetStealthMode();
                Result TXIsEmuNand();
                Result TXIsFat32();
                Result TXSetLdnmitm();
                Result TXGetLdnmitm();
            public:
                DEFINE_SERVICE_DISPATCH_TABLE {
                    MAKE_SERVICE_COMMAND_META(IsSelectedGamecard),
                    MAKE_SERVICE_COMMAND_META(TXGetGamecardStatus),
                    MAKE_SERVICE_COMMAND_META(TX_125),
                    MAKE_SERVICE_COMMAND_META(IsLicenseValid),
                    MAKE_SERVICE_COMMAND_META(TX_127),
                    MAKE_SERVICE_COMMAND_META(TX_128),
                    MAKE_SERVICE_COMMAND_META(IsPro),
                    MAKE_SERVICE_COMMAND_META(SetFTP),
                    MAKE_SERVICE_COMMAND_META(GetFTP),
                    MAKE_SERVICE_COMMAND_META(GetIPAddr),
                    MAKE_SERVICE_COMMAND_META(TX_133),
                    MAKE_SERVICE_COMMAND_META(SetStealthMode),
                    MAKE_SERVICE_COMMAND_META(GetStealthMode),
                    MAKE_SERVICE_COMMAND_META(TXIsEmuNand),
                    MAKE_SERVICE_COMMAND_META(TXIsFat32),
                    MAKE_SERVICE_COMMAND_META(TXSetLdnmitm),
                    MAKE_SERVICE_COMMAND_META(TXGetLdnmitm)
                };
        };
    }
    namespace pm {

        class ProcessManagerInterface final : public LoaderService {
            protected:
                enum class CommandId {
                    CreateProcess                 = 0,
                    GetProgramInfo                = 1,
                    PinProgram                    = 2,
                    UnpinProgram                  = 3,
                    SetEnabledProgramVerification = 4,

                    AtmosphereHasLaunchedProgram = 65000,
                    AtmosphereGetProgramInfo     = 65001,
                    AtmospherePinProgram         = 65002,
                };
            public:
                DEFINE_SERVICE_DISPATCH_TABLE {
                    MAKE_SERVICE_COMMAND_META(CreateProcess),
                    MAKE_SERVICE_COMMAND_META(GetProgramInfo),
                    MAKE_SERVICE_COMMAND_META(PinProgram),
                    MAKE_SERVICE_COMMAND_META(UnpinProgram),
                    MAKE_SERVICE_COMMAND_META(SetEnabledProgramVerification, hos::Version_10_0_0),

                    MAKE_SERVICE_COMMAND_META(AtmosphereHasLaunchedProgram),
                    MAKE_SERVICE_COMMAND_META(AtmosphereGetProgramInfo),
                    MAKE_SERVICE_COMMAND_META(AtmospherePinProgram),
                };
        };

    }

    namespace dmnt {

        class DebugMonitorInterface final : public LoaderService {
            protected:
                enum class CommandId {
                    SetProgramArguments  = 0,
                    FlushArguments       = 1,
                    GetProcessModuleInfo = 2,

                    AtmosphereHasLaunchedProgram = 65000,
                };
            public:
                DEFINE_SERVICE_DISPATCH_TABLE {
                    MAKE_SERVICE_COMMAND_META(SetProgramArguments),
                    MAKE_SERVICE_COMMAND_META(FlushArguments),
                    MAKE_SERVICE_COMMAND_META(GetProcessModuleInfo),

                    MAKE_SERVICE_COMMAND_META(AtmosphereHasLaunchedProgram),
                };
        };

    }

    namespace shell {

        class ShellInterface final : public LoaderService {
            protected:
                enum class CommandId {
                    SetProgramArguments  = 0,
                    FlushArguments       = 1,

                    AtmosphereRegisterExternalCode   = 65000,
                    AtmosphereUnregisterExternalCode = 65001,
                };
            public:
                DEFINE_SERVICE_DISPATCH_TABLE {
                    MAKE_SERVICE_COMMAND_META(SetProgramArguments),
                    MAKE_SERVICE_COMMAND_META(FlushArguments),

                    MAKE_SERVICE_COMMAND_META(AtmosphereRegisterExternalCode),
                    MAKE_SERVICE_COMMAND_META(AtmosphereUnregisterExternalCode),
                };
        };

    }

}
