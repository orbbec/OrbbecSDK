// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.h>

#include "utils_c.h"

#include <stdio.h>
#include <stdlib.h>

// helper function to check for errors and exit if there is one
void check_ob_error(ob_error **err) {
    if(*err) {
        const char *error_message = ob_error_get_message(*err);
        fprintf(stderr, "Error: %s\n", error_message);
        ob_delete_error(*err);
        exit(-1);
    }
    *err = NULL;
}

int select_index(const char *prompt, int min_value, int max_value) {  // todo: to support select index > 9
    int value = 0;
    printf("\n%s (Input a number between %d and %d or \'q\' to exit program): ", prompt, min_value, max_value);
    while(true) {
        char input;
        int  ret = scanf("%c", &input);
        (void)ret;
        getchar();

        if(input == 'q' || input == 'Q') {
            value = -1;
            break;
        }
        if(input >= '0' && input <= '9' && input - '0' >= min_value && input - '0' <= max_value) {
            value = input - '0';
            break;
        }
        printf("Invalid input, please input a number between %d and %d or \'q\' to exit program: ", min_value, max_value);
    }
    return value;
}

// Enumerates stream information.
void enumerate_stream_info(ob_sensor *sensor) {
    ob_error *error = NULL;

    // Get sensor type.
    ob_sensor_type sensor_type = ob_sensor_get_type(sensor, &error);
    check_ob_error(&error);

    // Get stream profile list.
    ob_stream_profile_list *stream_profile_list = ob_sensor_get_stream_profile_list(sensor, &error);
    check_ob_error(&error);

    // Get stream profile count.
    uint32_t stream_profile_count = ob_stream_profile_list_get_count(stream_profile_list, &error);
    check_ob_error(&error);

    printf("Available stream profiles: \n");
    for(uint32_t index = 0; index < stream_profile_count; index++) {
        // Get stream profile.
        const ob_stream_profile *stream_profile = ob_stream_profile_list_get_profile(stream_profile_list, index, &error);
        check_ob_error(&error);

        // Print video stream profile information.
        if(sensor_type == OB_SENSOR_IR || sensor_type == OB_SENSOR_COLOR || sensor_type == OB_SENSOR_DEPTH || sensor_type == OB_SENSOR_IR_LEFT
           || sensor_type == OB_SENSOR_IR_RIGHT) {
            ob_stream_type stream_type = ob_stream_profile_get_type(stream_profile, &error);
            check_ob_error(&error);
            const char *stream_type_str = ob_stream_type_to_string(stream_type);

            ob_format stream_format = ob_stream_profile_get_format(stream_profile, &error);
            check_ob_error(&error);
            const char *stream_format_str = ob_format_to_string(stream_format);

            uint32_t stream_width = ob_video_stream_profile_get_width(stream_profile, &error);
            check_ob_error(&error);

            uint32_t stream_height = ob_video_stream_profile_get_height(stream_profile, &error);
            check_ob_error(&error);

            uint32_t stream_fps = ob_video_stream_profile_get_fps(stream_profile, &error);
            check_ob_error(&error);

            printf("  %d - type: %4s, format: %4s, width: %4d, height: %4d, fps: %4d\n", index, stream_type_str, stream_format_str, stream_width, stream_height,
                   stream_fps);
        }
        else if(sensor_type == OB_SENSOR_ACCEL) {
            // Print acc stream profile information.
            ob_format stream_format = ob_stream_profile_get_format(stream_profile, &error);
            check_ob_error(&error);

            ob_accel_sample_rate acc_fps = ob_accel_stream_profile_get_sample_rate(stream_profile, &error);
            check_ob_error(&error);

            printf("  %d - type: %s, fps: %s\n", index, ob_format_to_string(stream_format), ob_imu_rate_type_to_string(acc_fps));
        }
        else if(sensor_type == OB_SENSOR_GYRO) {
            // Print gyro stream profile information.
            ob_format stream_format = ob_stream_profile_get_format(stream_profile, &error);
            check_ob_error(&error);

            ob_gyro_sample_rate gyro_fps = ob_gyro_stream_profile_get_sample_rate(stream_profile, &error);
            check_ob_error(&error);

            printf("  %d - type: %s, fps: %s\n", index, ob_format_to_string(stream_format), ob_imu_rate_type_to_string(gyro_fps));
        }

        // destroy stream profile
        ob_delete_stream_profile(stream_profile, &error);
        check_ob_error(&error);
    }

    // destroy stream profile list
    ob_delete_stream_profile_list(stream_profile_list, &error);
    check_ob_error(&error);
}

// enumerate sensor list.
void enumerate_sensor_info(ob_device *device) {
    ob_error *error = NULL;

    // Get sensor list.
    ob_sensor_list *sensor_list = ob_device_get_sensor_list(device, &error);
    check_ob_error(&error);

    // Get sensor count.
    uint32_t sensor_count = ob_sensor_list_get_count(sensor_list, &error);
    check_ob_error(&error);

    while(true){
        // Print sensor information.
        printf("Available sensors: \n");
        for(uint32_t index = 0; index < sensor_count; index++) {
            // Get device sensor.
            ob_sensor *sensor = ob_sensor_list_get_sensor(sensor_list, index, &error);
            check_ob_error(&error);

            // Get sensor type.
            ob_sensor_type sensor_type = ob_sensor_get_type(sensor, &error);
            check_ob_error(&error);
            const char *sensor_name = ob_sensor_type_to_string(sensor_type);

            // Print sensor information.
            printf("  %d - sensor name: %s\n", index, sensor_name);

            // destroy sensor
            ob_delete_sensor(sensor, &error);
            check_ob_error(&error);
        }

        int index = select_index("Select a sensor to enumerate its stream profiles", 0, sensor_count - 1);
        if(index >= 0) {
            // Get the selected sensor.
            ob_sensor *sensor = ob_sensor_list_get_sensor(sensor_list, index, &error);
            check_ob_error(&error);

            // Enumerate stream information of selected sensor.
            enumerate_stream_info(sensor);

            // destroy sensor
            ob_delete_sensor(sensor, &error);
            check_ob_error(&error);
        }else{
            break;
        }

    }
    // destroy sensor list
    ob_delete_sensor_list(sensor_list, &error);
    check_ob_error(&error);
}

// Enumerates device information.
void print_device_info(ob_device *device, int index) {
    ob_error *error = NULL;

    // Get device information.
    ob_device_info *dev_inf = ob_device_get_device_info(device, &error);
    check_ob_error(&error);

    // Get device name.
    const char *dev_name = ob_device_info_get_name(dev_inf, &error);
    check_ob_error(&error);

    // Get device pid.
    int dev_pid = ob_device_info_get_pid(dev_inf, &error);
    check_ob_error(&error);

    // Get device serial number.
    const char *dev_sn = ob_device_info_get_serial_number(dev_inf, &error);
    check_ob_error(&error);

    // Get connection type.
    const char *conn_type = ob_device_info_get_connection_type(dev_inf, &error);
    check_ob_error(&error);

    printf("  %d - device name: %s, device pid: %d, device sn: %s, connection type: %s\n", index, dev_name, dev_pid, dev_sn, conn_type);
}

int main(void) {

    // Used to return SDK interface error information.
    ob_error *error = NULL;

    // Get OrbbecSDK version.
    int major_version = ob_get_major_version();
    int minor_version = ob_get_minor_version();
    int patch_version = ob_get_patch_version();
    printf("Orbbec SDK version: %d.%d.%d\n", major_version, minor_version, patch_version);

    // Create context.
    ob_context *ctx = ob_create_context(&error);
    check_ob_error(&error);

    // Get device list from context.
    ob_device_list *dev_list = ob_query_device_list(ctx, &error);
    check_ob_error(&error);

    // Get device count from device list.
    uint32_t dev_count = ob_device_list_get_count(dev_list, &error);
    check_ob_error(&error);

    // Check if any device is connected.
    if(dev_count == 0) {
        printf("No device found! Please connect a supported device and retry this program.\n");

        ob_delete_device_list(dev_list, &error);
        check_ob_error(&error);

        ob_delete_context(ctx, &error);
        check_ob_error(&error);

        printf("\nPress any key to exit.");
        ob_smpl_wait_for_key_press(0);

        return -1;
    }

    while(true) {
        printf("Connected devices: \n");
        for(uint32_t index = 0; index < dev_count; index++) {
            // Get device from device list.
            ob_device *dev = ob_device_list_get_device(dev_list, index, &error);
            check_ob_error(&error);

            // print device information
            print_device_info(dev, index);

            // destroy device
            ob_delete_device(dev, &error);
            check_ob_error(&error);
        }

        // Select a device.
        int device_index = select_index("Select a device to enumerate its sensors", 0, dev_count - 1);
        if(device_index >= 0) {
            // get device from device list
            ob_device *device = ob_device_list_get_device(dev_list, device_index, &error);
            check_ob_error(&error);

            // enumerate sensors of device
            enumerate_sensor_info(device);

            // destroy device
            ob_delete_device(device, &error);
            check_ob_error(&error);
        }else{
            break;
        }
    }

    // destroy sensor list
    ob_delete_device_list(dev_list, &error);
    check_ob_error(&error);

    // destroy context
    ob_delete_context(ctx, &error);
    check_ob_error(&error);

    printf("\nProgram ended successfully. Press any key to exit.");
    ob_smpl_wait_for_key_press(0);

    return 0;
}

