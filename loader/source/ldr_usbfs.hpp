#pragma once
#include <switch.h>
#include <stratosphere.hpp>

enum UsbfsServiceCmd {
    Usbfs_Cmd_0 = 0,
    Usbfs_Cmd_1 = 1,
    Usbfs_Cmd_2 = 2,
    Usbfs_Cmd_3 = 3,
    Usbfs_Cmd_4 = 4,
    Usbfs_Cmd_5 = 5,
    Usbfs_Cmd_6 = 6,
    Usbfs_Cmd_7 = 7,
};

class UsbfsService final : public IServiceObject {        
    private:
        /* Actual commands. */
        Result Cmd_0();
        Result Cmd_1();
        Result Cmd_2();
        Result Cmd_3();
        Result Cmd_4();
        Result Cmd_5();
        Result Cmd_6();
        Result Cmd_7();
    public:
        DEFINE_SERVICE_DISPATCH_TABLE {
            MakeServiceCommandMeta<Usbfs_Cmd_0, &UsbfsService::Cmd_0>(),
            MakeServiceCommandMeta<Usbfs_Cmd_1, &UsbfsService::Cmd_1>(),
            MakeServiceCommandMeta<Usbfs_Cmd_2, &UsbfsService::Cmd_2>(),
            MakeServiceCommandMeta<Usbfs_Cmd_3, &UsbfsService::Cmd_3>(),
            MakeServiceCommandMeta<Usbfs_Cmd_4, &UsbfsService::Cmd_4>(),
            MakeServiceCommandMeta<Usbfs_Cmd_5, &UsbfsService::Cmd_5>(),
            MakeServiceCommandMeta<Usbfs_Cmd_6, &UsbfsService::Cmd_6>(),
            MakeServiceCommandMeta<Usbfs_Cmd_7, &UsbfsService::Cmd_7>(),
        };
};