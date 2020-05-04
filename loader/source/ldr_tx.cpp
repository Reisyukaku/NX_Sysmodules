#include <switch.h>
#include <stratosphere.hpp>
#include <stdio.h>
#include "ldr_loader_service.hpp"

namespace ams::ldr {
    namespace TX {
        Result TXService::IsSelectedGamecard(const sf::InPointerBuffer data) {
            //Returns either 0xA08 or 0; deals with sending rom data
            return 0;
        }

        Result TXService::TXGetGamecardStatus(sf::Out<u32> ret) {
            //checks a flag set by ipc123
            ret.SetValue(0x100);
            return 0;
        }

        Result TXService::TX_125() {
            //This isnt even called in rommenu afaik
            return 0;
        }

        Result TXService::IsLicenseValid(sf::Out<u32> ret) {
            //The answer to unlocking piracy tools is to summon satan
            ret.SetValue(0x666);
            return 0;
        }

        Result TXService::TX_127() {
            //More license related garbo
            return 0x666;
        }

        Result TXService::TX_128() {
            return 0;
        }

        Result TXService::IsPro() {
            return 1;
        }

        Result TXService::SetFTP() {
            return 0;
        }

        Result TXService::GetFTP() {
            //0=enabled; 1=disabled
            //If ftpmode disabled(?) tx130(1);
            return 1;
        }

        Result TXService::GetIPAddr() {
            return 0;
        }

        Result TXService::TX_133() {
            return 0;
        }

        Result TXService::SetStealthMode() {
            //0=enabled; 1=disabled
            //If stealthmode disabled(?) tx134(1);
            return 0;
        }

        Result TXService::GetStealthMode() {
            //0=enabled; 1=disabled
            return 1;
        }

        Result TXService::TXIsEmuNand() {
            //0=enabled; 1=disabled
            return 1;
        }

        Result TXService::TXIsFat32() {
            //0=enabled; 1=disabled
            return 0;
        }

        Result TXService::TXSetLdnmitm() {
            return 0;
        }
        Result TXService::TXGetLdnmitm() {
            //0=enabled; 1=disabled
            return 1;
        }
    }
} 
