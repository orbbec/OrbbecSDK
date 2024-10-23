// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"

#include <iomanip>
#include <iostream>

void printDeviceList(const std::string &prompt, std::shared_ptr<ob::DeviceList> deviceList) {
    auto count = deviceList->getCount();
    if(count == 0) {
        return;
    }
    std::cout << count << " device(s) " << prompt << ": " << std::endl;
    for(uint32_t i = 0; i < count; i++) {
        auto uid          = deviceList->getUid(i);
        auto vid          = deviceList->getVid(i);
        auto pid          = deviceList->getPid(i);
        auto serialNumber = deviceList->getSerialNumber(i);
        auto connection   = deviceList->getConnectionType(i);
        std::cout << " - uid: " << uid << ", vid: 0x" << std::hex << std::setfill('0') << std::setw(4) << vid << ", pid: 0x" << pid
                  << ", serial number: " << serialNumber << ", connection: " << connection << std::endl;
    }
    std::cout << std::endl;
}

void rebootDevices(std::shared_ptr<ob::DeviceList> deviceList) {
    for(uint32_t i = 0; i < deviceList->getCount(); i++) {
        // get device from device list
        auto device = deviceList->getDevice(i);

        // reboot device
        device->reboot();
    }
}

int main(void) try {
    // create context
    ob::Context ctx;

    // register device callback
    ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> deviceList) {
        printDeviceList("added", deviceList);
        printDeviceList("removed", removedList);
    });

    // query current device list
    auto currentList = ctx.queryDeviceList();
    printDeviceList("connected", currentList);

    std::cout << "Press 'r' to reboot the connected devices to trigger the device disconnect and reconnect event, or manually unplug and plugin the device."
              << std::endl;
    std::cout << "Press 'Esc' to exit." << std::endl << std::endl;

    // main loop, wait for key press
    while(true) {
        auto key = ob_smpl::waitForKeyPressed(100);
        // Press the esc key to exit
        if(key == 27) {
            break;
        }
        else if(key == 'r' || key == 'R') {
            // update device list
            currentList = ctx.queryDeviceList();

            std::cout << "Rebooting devices..." << std::endl;
            rebootDevices(currentList);
        }
    }

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}
