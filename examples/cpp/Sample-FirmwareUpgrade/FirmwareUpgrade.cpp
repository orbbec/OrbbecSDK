#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <exception>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "utils.hpp"

#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"

#define ESC_KEY 27

bool                        isWaitRebootComplete_ = false;
bool                        isDeviceRemoved_      = false;
std::condition_variable     waitRebootCondition_;
std::mutex                  waitRebootMutex_;
std::string                 deviceUid_;
std::string                 deviceSN_;
std::shared_ptr<ob::Device> rebootedDevice_;

static std::string checkFirmwareFilePath(int argc, char **argv);
static void        dumpDeviceInfo(std::shared_ptr<ob::Device> device);
static bool        upgradeFirmware(std::shared_ptr<ob::Device> device, std::string firmwarePath);
static uint64_t    getCurrentTimeMs();

int main(int argc, char **argv) try {
    std::string firmwareFilePath = checkFirmwareFilePath(argc, argv);
    if(firmwareFilePath.empty()) {
        std::cout << "command: " << std::endl << "$ ./FirmwareUpgrade[.exe] firmwareFile.bin" << std::endl;
        return 0;
    }

    // Print the sdk version number, the sdk version number is divided into major version number, minor version number and revision number
    std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
    // Print sdk stage version
    std::cout << "SDK stage version: " << ob::Version::getStageVersion() << std::endl;

    // Create a Context.
    ob::Context ctx;
    ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> addedList) {
        if(isWaitRebootComplete_) {
            if(addedList && addedList->deviceCount() > 0) {
                auto device = addedList->getDevice(0);
                if(isDeviceRemoved_ && deviceSN_ == std::string(device->getDeviceInfo()->serialNumber())) {
                    rebootedDevice_       = device;
                    isWaitRebootComplete_ = false;

                    std::unique_lock<std::mutex> lk(waitRebootMutex_);
                    waitRebootCondition_.notify_all();
                }
            }

            if(removedList && removedList->deviceCount() > 0) {
                if(deviceUid_ == std::string(removedList->uid(0))) {
                    isDeviceRemoved_ = true;
                }
            }
        }  // if isWaitRebootComplete_
    });

    // Query the list of connected devices
    auto devList = ctx.queryDeviceList();

    // Get the number of connected devices
    if(devList->deviceCount() == 0) {
        std::cerr << "Device not found!" << std::endl;
        return -1;
    }

    // Create a device, 0 means the index of the first device
    auto dev = devList->getDevice(0);
    dumpDeviceInfo(dev);

    // Store uid to wait device reboot
    {
        auto devInfo = dev->getDeviceInfo();
        deviceUid_   = std::string(devInfo->uid());
        deviceSN_    = std::string(devInfo->serialNumber());
    }

    // Confirm to upgrade firmware
    std::cout << "Do you to upgrade firmware: " << firmwareFilePath << std::endl;
    std::cout << "Please input Y/N: ";
    while(true) {
        if(kbhit()) {
            int confirmChar = getch();
            if(ESC_KEY == confirmChar) {
                std::cout << std::endl << "Cancel upgrade firmware" << std::endl;
                return 0;
            }
            else if('N' == confirmChar || 'n' == confirmChar) {
                std::cout << std::endl << "Abort upgrade firmware" << std::endl;
                return 0;
            }
            else if('Y' == confirmChar || 'y' == confirmChar) {
                // Goto upgrade firmware
                break;
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    std::cout << std::endl;

    // Upgrade firmware file Path
    if(!upgradeFirmware(dev, firmwareFilePath)) {
        std::cout << "Upgrade firmware failed" << std::endl;
        return -1;
    }
    std::cout << "Upgrade firmware complete" << std::endl;

    // Reboot device
    std::cout << "Reboot device" << std::endl;
    isDeviceRemoved_      = false;
    isWaitRebootComplete_ = true;
    dev->reboot();
    dev     = nullptr;
    devList = nullptr;

    // wait reboot complete
    {
        std::cout << "Wait device reboot completed" << std::endl;
        std::unique_lock<std::mutex> lk(waitRebootMutex_);
        waitRebootCondition_.wait_for(lk, std::chrono::milliseconds(60000), []() { return !isWaitRebootComplete_; });
    }

    std::cout << "isWaitRebootComplete_: " << isWaitRebootComplete_ << std::endl;

    // Check is reboot complete
    if(rebootedDevice_) {
        std::cout << "Device reboot complete." << std::endl;
        dumpDeviceInfo(rebootedDevice_);
    }
    else {
        std::cout << "Wait device reboot complete failed." << std::endl;
    }

    std::cout << "Press ESC to exit! " << std::endl;
    while(true) {
        if(kbhit()) {
            // Get the value of the pressed key, if it is the esc key, exit the program
            int key = getch();
            if(key == ESC_KEY) {
                break;
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

// Print device name, serial number, vid, pid and firmware version.
void dumpDeviceInfo(std::shared_ptr<ob::Device> device) {
    // Get device information
    auto devInfo = device->getDeviceInfo();

    // Get the name of the device
    std::cout << "Device name: " << devInfo->name() << std::endl;

    // Get the pid, vid, uid of the device
    std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

    // By getting the firmware version number of the device
    auto fwVer = devInfo->firmwareVersion();
    std::cout << "Firmware version: " << fwVer << std::endl;

    // By getting the serial number of the device
    auto sn = devInfo->serialNumber();
    std::cout << "Serial number: " << sn << std::endl;
}

// Check file exists.
std::string checkFirmwareFilePath(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "Please input firmware path." << std::endl;
        return "";
    }

    std::string   filePath = std::string(*(argv + 1));
    std::ifstream fs(filePath);
    if(!fs.is_open()) {
        std::cout << "Open Firmware file failed. filePath: " << filePath << std::endl;
        return "";
    }

    fs.close();
    return filePath;
}

uint64_t getCurrentTimeMs() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

bool upgradeFirmware(std::shared_ptr<ob::Device> device, std::string firmwarePath) {
    auto index     = firmwarePath.find_last_of(".img");
    bool isImgFile = index != std::string::npos;
    index          = firmwarePath.find_last_of(".bin");
    bool isBinFile = index != std::string::npos;
    if(!(isImgFile || isBinFile)) {
        std::cout << "Upgrade Fimware failed. invalid firmware file: " << firmwarePath << std::endl;
        return false;
    }

    bool isUpgradeSuccess = false;
    try {
        // Upgrade firmware and process firmware upgrade callback.
        device->deviceUpgrade(
            firmwarePath.c_str(),
            [=, &isUpgradeSuccess](OBUpgradeState state, const char *message, uint8_t percent) {
                if(state == STAT_START) {
                    std::cout << "Upgrade Firmware start" << std::endl;
                }
                else if(state == STAT_FILE_TRANSFER) {
                    std::cout << "Upgrade Firmware file transfer, percent: " << (uint32_t)percent << std::endl;
                }
                else if(state == STAT_IN_PROGRESS) {
                    std::cout << "Upgrade Firmware in progress, percent: " << (uint32_t)percent << std::endl;
                }
                else if(state == STAT_DONE) {
                    std::cout << "Upgrade Firmware done, percent: " << (uint32_t)percent << std::endl;
                    isUpgradeSuccess = true;
                }
                else if(state == STAT_VERIFY_IMAGE) {
                    std::cout << "Upgrade Firmware verify image" << std::endl;
                }
                else {
                    std::string errMsg = (nullptr != message ? std::string(message) : "");
                    std::cout << "Upgrade Firmware failed. state: " << std::to_string(state) << ", errMsg: " << errMsg << ", percent: " << (uint32_t)percent
                              << std::endl;
                }
            },
            false);
    }
    catch(ob::Error &e) {
        std::cerr << "Upgrade Firmware ob error. function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage()
                  << "\ntype:" << e.getExceptionType() << std::endl;
    }
    catch(std::exception &e) {
        if(e.what()) {
            std::cout << "Upgrade Firmware Exception. what: " << std::string(e.what()) << std::endl;
        }
    }

    return isUpgradeSuccess;
}