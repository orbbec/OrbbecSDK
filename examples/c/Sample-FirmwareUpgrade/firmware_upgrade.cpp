#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono>
#include <cstdio>
#include <exception>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "utils.hpp"

extern "C" {
#include <libobsensor/ObSensor.h>
}

#define ESC_KEY 27

bool                    is_wait_reboot_complete_ = false;
bool                    is_device_removed_       = false;
bool                    is_upgrade_success_      = false;
std::condition_variable wait_reboot_condition_;
std::mutex              wait_reboot_mutex_;
char                    device_uid_[128]    = { 0 };
char                    device_sn_[32]      = { 0 };
ob_device              *rebooted_device_    = nullptr;
int                     callback_user_data_ = 0;

static const char *check_firmware_file_path(int argc, char **argv);
static void        dump_device_info(ob_device *device);
static bool        upgrade_firmware(ob_device *device, const char *firmware_path);
static uint64_t    get_current_time_ms();
static void        device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data);
static void        device_upgrade_callback(ob_upgrade_state state, const char *message, uint8_t percent, void *user_data);

void check_error(ob_error *error) {
    if(error) {
        printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error), ob_error_args(error));
        printf("\tmessage: %s\n", ob_error_message(error));
        printf("\terror type: %d\n", ob_error_exception_type(error));
        ob_delete_error(error);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv) {
    const char *firmware_file_path = check_firmware_file_path(argc, argv);
    if(!firmware_file_path || 0 == strlen(firmware_file_path)) {
        printf("command: \n$ ./frameware_upgrade[.exe] firmwareFile.bin\n");
        return 0;
    }

    // print sdk version number
    printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());
    // print sdk stage version
    printf("SDK stage version: %s\n", ob_get_stage_version());

    // Create a Context.
    ob_error   *error = NULL;
    ob_context *ctx   = ob_create_context(&error);
    check_error(error);

    // set device changed callback
    ob_set_device_changed_callback(ctx, device_changed_callback, &callback_user_data_, &error);
    check_error(error);

    // Query the list of connected devices
    ob_device_list *dev_list = ob_query_device_list(ctx, &error);
    check_error(error);

    // Get the number of connected devices
    int dev_count = ob_device_list_device_count(dev_list, &error);
    check_error(error);
    if(dev_count == 0) {
        printf("Device not found!\n");

        ob_delete_device_list(dev_list, &error);
        check_error(error);

        ob_delete_context(ctx, &error);
        check_error(error);
        return -1;
    }

    // Create a device, 0 means the index of the first device
    ob_device *dev = ob_device_list_get_device(dev_list, 0, &error);
    check_error(error);

    // Print device brief information
    dump_device_info(dev);

    // Store uid to wait device reboot
    {
        ob_device_info *dev_info = ob_device_get_device_info(dev, &error);
        check_error(error);

        const char *uid = ob_device_info_uid(dev_info, &error);
        check_error(error);
        strcpy(device_uid_, uid);

        const char *sn = ob_device_info_serial_number(dev_info, &error);
        check_error(error);
        strcpy(device_sn_, sn);

        // Release resource of device info
        ob_delete_device_info(dev_info, &error);
        check_error(error);
    }

    // Confirm to upgrade firmware
    printf("Do you to upgrade firmware: %s\n", firmware_file_path);
    printf("Please input Y/N: ");
    while(true) {
        if(kbhit()) {
            int confirm_char = getch();
            if(ESC_KEY == confirm_char) {
                printf("\nCancel upgrade firmware\n");

                ob_delete_device(dev, &error);
                check_error(error);

                ob_delete_device_list(dev_list, &error);
                check_error(error);

                ob_delete_context(ctx, &error);
                check_error(error);
                return 0;
            }
            else if('N' == confirm_char || 'n' == confirm_char) {
                printf("\nAbort upgrade firmware\n");

                ob_delete_device(dev, &error);
                check_error(error);

                ob_delete_device_list(dev_list, &error);
                check_error(error);

                ob_delete_context(ctx, &error);
                check_error(error);
                return 0;
            }
            else if('Y' == confirm_char || 'y' == confirm_char) {
                // Goto upgrade firmware
                break;
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
    printf("\n");

    // Upgrade firmware file Path
    if(!upgrade_firmware(dev, firmware_file_path)) {
        printf("Upgrade firmware failed\n");

        ob_delete_device(dev, &error);
        check_error(error);

        ob_delete_device_list(dev_list, &error);
        check_error(error);

        ob_delete_context(ctx, &error);
        check_error(error);
        return -1;
    }
    printf("Upgrade firmware complete\n");

    // Reboot device
    printf("Reboot device\n");
    is_device_removed_       = false;
    is_wait_reboot_complete_ = true;
    ob_device_reboot(dev, &error);
    check_error(error);

    // Release resource of device
    ob_delete_device(dev, &error);
    check_error(error);

    // Release resource of device list
    ob_delete_device_list(dev_list, &error);
    check_error(error);

    // wait reboot complete
    {
        printf("Wait device reboot completed\n");
        std::unique_lock<std::mutex> lk(wait_reboot_mutex_);
        wait_reboot_condition_.wait_for(lk, std::chrono::milliseconds(60000), []() { return !is_wait_reboot_complete_; });
    }

    printf("is_wait_reboot_complete_: %d\n", is_wait_reboot_complete_);

    // Check is reboot complete
    if(rebooted_device_) {
        printf("Device reboot complete.\n");
        dump_device_info(rebooted_device_);
    }
    else {
        printf("Wait device reboot complete failed.\n");
    }

    printf("Press ESC_KEY to exit! \n");
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

    ob_delete_context(ctx, &error);
    check_error(error);

    return 0;
}

// Print device name, serial number, vid, pid and firmware version.
void dump_device_info(ob_device *device) {
    ob_error *error = NULL;

    // Get device information
    ob_device_info *dev_info = ob_device_get_device_info(device, &error);
    check_error(error);

    // Get the name of the device
    const char *name = ob_device_info_name(dev_info, &error);
    check_error(error);
    printf("Device name: %s\n", name);

    // Get the pid, vid, uid of the device
    int pid = ob_device_info_pid(dev_info, &error);
    check_error(error);
    int vid = ob_device_info_vid(dev_info, &error);
    check_error(error);
    const char *uid = ob_device_info_uid(dev_info, &error);
    check_error(error);
    printf("Device pid: %d vid: %d uid: %s\n", pid, vid, uid);

    // Get the firmware version number of the device by
    const char *fw_ver = ob_device_info_firmware_version(dev_info, &error);
    check_error(error);
    printf("Firmware version: %s\n", fw_ver);

    // Get the serial number of the device
    const char *sn = ob_device_info_serial_number(dev_info, &error);
    check_error(error);
    printf("Serial number: %s\n", sn);

    // Release resource.
    ob_delete_device_info(dev_info, &error);
    check_error(error);
}

// Check file exists.
const char *check_firmware_file_path(int argc, char **argv) {
    if(argc < 2) {
        printf("Please input firmware path.\n");
        return "";
    }

    const char *filePath = *(argv + 1);
    FILE       *file     = fopen(filePath, "r");
    if(!file) {
        printf("Open Firmware file failed. filePath: %s\n", filePath);
        return "";
    }

    fclose(file);

    return filePath;
}

uint64_t get_current_time_ms() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

bool upgrade_firmware(ob_device *device, const char *firmwarePath) {
    const char *index     = strstr(firmwarePath, ".img");
    bool        isImgFile = (bool)index;
    index                 = strstr(firmwarePath, ".bin");
    bool isBinFile        = (bool)index;
    if(!(isImgFile || isBinFile)) {
        printf("Upgrade Fimware failed. invalid firmware file: %s\n", firmwarePath);
        return false;
    }

    // Upgrade firmware with firmware file.
    is_upgrade_success_ = false;
    ob_error *error     = NULL;
    ob_device_upgrade(device, firmwarePath, device_upgrade_callback, false, &callback_user_data_, &error);
    check_error(error);
    return is_upgrade_success_;
}

// Process firmware callback event
void device_upgrade_callback(ob_upgrade_state state, const char *message, uint8_t percent, void *user_data) {
    printf("%s (state: %d, percent: %u)\n", message ? message : "", (int)state, (uint32_t)percent);
    if(state == STAT_DONE) {
        is_upgrade_success_ = true;
    }
}

// Process device change callback event
void device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data) {
    // Wait device attache and detach event after device upgrade firmware completed and call reboot.
    ob_error *error = NULL;
    if(is_wait_reboot_complete_) {
        if(added) {
            uint32_t device_count = ob_device_list_device_count(added, &error);
            check_error(error);
            if(device_count > 0) {
                ob_device *device = ob_device_list_get_device(added, 0, &error);
                check_error(error);

                // Get device info to get sn
                ob_device_info *dev_info = ob_device_get_device_info(device, &error);
                check_error(error);

                // Compare device sn.
                bool        is_assigned_device = false;
                const char *sn                 = ob_device_info_serial_number(dev_info, &error);
                check_error(error);
                if(0 == strcmp(sn, device_sn_)) {
                    rebooted_device_         = device;
                    is_assigned_device       = true;
                    is_wait_reboot_complete_ = false;

                    std::unique_lock<std::mutex> lk(wait_reboot_mutex_);
                    wait_reboot_condition_.notify_all();
                }

                // Release resource of dev_info
                ob_delete_device_info(dev_info, &error);
                check_error(error);

                if(!is_assigned_device) {
                    // Release resource of device
                    ob_delete_device(device, &error);
                    check_error(error);
                }
            }
        }

        if(removed) {
            uint32_t device_count = ob_device_list_device_count(removed, &error);
            check_error(error);
            if(device_count > 0) {
                // Get device sn
                const char *uid = ob_device_list_get_device_uid(removed, 0, &error);
                check_error(error);

                if(0 == strcmp(device_uid_, uid)) {
                    is_device_removed_ = true;
                }
            }
        }
    }  // if is_wait_reboot_complete_

    // Need to manually destroy the device list
    // destroy device list
    ob_delete_device_list(removed, &error);
    check_error(error);

    // destroy device list
    ob_delete_device_list(added, &error);
    check_error(error);
}