#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.hpp"

#include <libobsensor/ObSensor.h>

#define ESC_KEY 27

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

    // print sdk version number
    printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());
    // print sdk stage version
    printf("SDK stage version: %s\n", ob_get_stage_version());

    // Create a Context.
    ob_error   *error = NULL;
    ob_context *ctx   = ob_create_context(&error);
    check_error(error);

    // Query the list of connected devices
    ob_device_list *dev_list = ob_query_device_list(ctx, &error);
    check_error(error);

    // Get the number of connected devices
    int dev_count = ob_device_list_device_count(dev_list, &error);
    check_error(error);
    if(dev_count == 0) {
        printf("Device not found!\n");
        return -1;
    }

    // Create a device, 0 means the index of the first device
    ob_device *dev = ob_device_list_get_device(dev_list, 0, &error);
    check_error(error);

    // Get device information
    ob_device_info *dev_info = ob_device_get_device_info(dev, &error);
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

    // Get the connection type of the device
    const char *connectType = ob_device_info_connection_type(dev_info, &error);
    check_error(error);
    printf("ConnectionType: %s\n", connectType);

    printf("Sensor types: \n");
    // Get a list of supported sensors
    ob_sensor_list *sensor_list = ob_device_get_sensor_list(dev, &error);
    check_error(error);

    // Get the number of sensors
    int sensor_count = ob_sensor_list_get_sensor_count(sensor_list, &error);
    check_error(error);
    for(int i = 0; i < sensor_count; i++) {
        // Get sensor type
        ob_sensor_type sensor_type = ob_sensor_list_get_sensor_type(sensor_list, i, &error);
        check_error(error);
        switch(sensor_type) {
        case OB_SENSOR_COLOR:
            printf("\tColor sensor\n");
            break;
        case OB_SENSOR_DEPTH:
            printf("\tDepth sensor\n");
            break;
        case OB_SENSOR_IR:
            printf("\tIR sensor\n");
            break;
        case OB_SENSOR_IR_LEFT:
            printf("\tIR Left sensor\n");
            break;
        case OB_SENSOR_IR_RIGHT:
            printf("\tIR Right sensor\n");
            break;
        case OB_SENSOR_ACCEL:
            printf("\tAccel sensor\n");
            break;
        case OB_SENSOR_GYRO:
            printf("\tGyro sensor\n");
            break;
        default:
            break;
        }
    }

    printf("Press ESC_KEY to exit! \n");

    while(true) {
        // Get the value of the pressed key, if it is the esc key, exit the program
        int key = getch();
        if(key == ESC_KEY)
            break;
    }

    // destroy sensor list
    ob_delete_sensor_list(sensor_list, &error);
    check_error(error);

    // destroy device info
    ob_delete_device_info(dev_info, &error);
    check_error(error);

    // destroy device
    ob_delete_device(dev, &error);
    check_error(error);

    // destroy device list
    ob_delete_device_list(dev_list, &error);
    check_error(error);

    // destroy context
    ob_delete_context(ctx, &error);
    check_error(error);

    return 0;
}