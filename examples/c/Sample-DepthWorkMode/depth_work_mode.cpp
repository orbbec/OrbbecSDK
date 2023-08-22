#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>

#include "utils.hpp"

extern "C" {
#include <libobsensor/ObSensor.h>
}

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

static char *toHexText(const uint8_t *data, const uint32_t dataLen) {
    static char alpha[] = "0123456789abcdef";
    char       *hexStr  = new char[dataLen * 2 + 1]{ 0 };
    for(uint32_t i = 0; i < dataLen; i++) {
        uint8_t val       = *(data + i);
        *(hexStr + i)     = alpha[(val >> 4) & 0xf];
        *(hexStr + i + 1) = alpha[val & 0xf];
    }

    return hexStr;
}

int main(int argc, char **argv) {

    // Print the sdk version number
    printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());

    ob_error *error = NULL;

    // Create a pipeline to open the depth stream after connecting the device
    ob_pipeline *pipe = ob_create_pipeline(&error);
    check_error(error);

    // Get the device through the pipeline
    ob_device *dev = ob_pipeline_get_device(pipe, &error);
    check_error(error);

    // Check whether the camera depth working mode is supported, currently (December 5, 2022) only the Gemini2 binocular camera supports the depth working mode
    if(!ob_device_is_property_supported(dev, OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE, &error)) {
        printf("FAILED!!, Device not support depth work mode");
        check_error(error);
        return -1;
    }
    check_error(error);

    // Query the current depth working mode
    ob_depth_work_mode cur_work_mode = ob_device_get_current_depth_work_mode(dev, &error);
    check_error(error);
    printf("Current depth work mode: %s\n", cur_work_mode.name);

    // Query the list of depth working modes supported by the camera
    ob_depth_work_mode_list *mode_list = ob_device_get_depth_work_mode_list(dev, &error);
    check_error(error);

    // get the length of the list
    uint32_t mode_count = ob_depth_work_mode_list_count(mode_list, &error);
    printf("Support depth work mode list count: %u\n", mode_count);

    int cur_mode_index = -1;
    for(uint32_t i = 0; i < mode_count; i++) {
        ob_depth_work_mode mode = ob_depth_work_mode_list_get_item(mode_list, i, &error);
        check_error(error);

        char *hex = toHexText(mode.checksum, sizeof(mode.checksum));
        printf("depth work mode[%u], name: %s, checksum: %s", i, mode.name, hex);
        delete[] hex;
        if(0 == strcmp(cur_work_mode.name, mode.name)) {
            printf(" (Current Work Mode)");
        }
        printf("\n");
    }

    int mode_index;
    printf("Please select a work mode index:");
    scanf("%d", &mode_index);
    if(mode_index >= 0 && mode_index < mode_count) {
        ob_depth_work_mode mode = ob_depth_work_mode_list_get_item(mode_list, mode_index, &error);
        check_error(error);

        // switch to new camera depth mode
        ob_device_switch_depth_work_mode_by_name(dev, mode.name, &error);
        check_error(error);

        ob_depth_work_mode tmp_mode = ob_device_get_current_depth_work_mode(dev, &error);
        check_error(error);
        if(0 == strcmp(mode.name, tmp_mode.name)) {
            printf("Switch depth work mode success! mode name: %s\n", mode.name);
        }
        else {
            printf("Switch depth work mode failed!");
        }
    }
    else {
        printf("switchDepthMode faild. invalid index: %d\n", mode_index);
    }

    // This is the end of switching the camera depth mode, you can use the pipeline to open the camera to get the stream
    // Notice:
    // 1. If you need to switch the camera depth mode, you must do it berfore start camera stream; the effective resolution supported
    // by each camera depth mode is different
    // 2. If the data stream has been opened using the pipeline, the original pipeline must be released before switching camera depth mode.  otherwise, wild
    // pointer or memory leak will be caused.
    printf("Press ESC_KEY to exit! \n");

    while(true) {
        // Get the value of the pressed key, if it is the esc key, exit the program
        int key = getch();
        if(key == ESC_KEY)
            break;
    }

    // destroy mode_list
    ob_delete_depth_work_mode_list(mode_list, &error);
    check_error(error);

    // destroy device
    ob_delete_device(dev, &error);
    check_error(error);

    // destroy the pipeline
    ob_delete_pipeline(pipe, &error);
    check_error(error);

    return 0;
}
