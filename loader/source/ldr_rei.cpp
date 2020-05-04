#include <switch.h>
#include <stratosphere.hpp>
#include <stdio.h>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream> 
#include "ldr_loader_service.hpp"
using namespace std;
namespace ams::ldr {
	namespace rnx {
		ams::Result RNXService::GetReiNXVersion(ams::sf::Out<u32> maj, ams::sf::Out<u32> min) {
		const auto api_info = ams::exosphere::GetApiInfo();
			*maj.GetPointer() = api_info.GetMajorVersion();
			*min.GetPointer() = api_info.GetMinorVersion();
			return 0;
		}
		
		ams::Result RNXService::SetHbTidForDelta(u64 tid) {
			
			const char *hexstring = "0x010000000000100D";
			int title_id = (int)strtol(hexstring, NULL, 0);
			tid = title_id;
		}
	}
}