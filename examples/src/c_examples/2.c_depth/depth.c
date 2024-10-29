// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.h>

#include "utils_c.h"
#include "utils_types.h"

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

int main(void) {
    // Used to return SDK interface error information.
    ob_error *error = NULL;

    // Create a pipeline object to manage the streams.
    ob_pipeline *pipeline = ob_create_pipeline(&error);
    check_ob_error(&error);

    // Crete a config object to configure the pipeline streams.
    ob_config *config = ob_create_config(&error);
    check_ob_error(&error);

    // enable depth stream with default stream profile.
    ob_config_enable_stream(config, OB_STREAM_DEPTH, &error);
    check_ob_error(&error);

    // Start Pipeline with the configured streams.
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_ob_error(&error);

    printf("Depth Stream Started. Press 'ESC' key to exit the program.\n");

    // Wait frameset in a loop, exit when ESC is pressed.
    while(true) {
        // Wait for a key press
        char key = ob_smpl_wait_for_key_press(10);
        if(key == ESC_KEY) {  // ESC key
            printf("Exiting...\n");
            break;
        }

        // Wait for a frameset, timeout after 1000 milliseconds.
        const ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 1000, &error);
        check_ob_error(&error);

        // If no frameset is available with in the timeout, continue waiting.
        if(frameset == NULL) {
            continue;
        }

        // Get the depth frame from frameset。
        const ob_frame *depth_frame = ob_frameset_get_depth_frame(frameset, &error);
        check_ob_error(&error);

        // Get index from depth frame.
        uint64_t index = ob_frame_get_index(depth_frame, &error);
        check_ob_error(&error);

        // print the distance of the center pixel every 30 frames to reduce output
        if(index % 30 == 0) {
            // Get the width and height of the depth frame.
            uint32_t width = ob_video_frame_get_width(depth_frame, &error);
            check_ob_error(&error);

            // Get the height of the depth frame.
            uint32_t height = ob_video_frame_get_height(depth_frame, &error);
            check_ob_error(&error);

            // Get the scale of the depth frame.
            float scale = ob_depth_frame_get_value_scale(depth_frame, &error);
            check_ob_error(&error);

            // Get the data of the depth frame, cast to uint16_t* to access the pixels directly for Y16 format.
            uint16_t *data = (uint16_t *)ob_frame_get_data(depth_frame, &error);
            check_ob_error(&error);

            // pixel value multiplied by scale is the actual distance value in millimeters
            float center_distance = data[width * height / 2 + width / 2] * scale;

            // attention: if the distance is 0, it means that the depth camera cannot detect the object（may be out of detection range）
            printf("Facing an object at a distance of %.3f mm away.\n", center_distance);
        }

        // delete the depth frame
        ob_delete_frame(depth_frame, &error);
        check_ob_error(&error);

        // delete the frameset
        ob_delete_frame(frameset, &error);
        check_ob_error(&error);
    };

    // stop the pipeline
    ob_pipeline_stop(pipeline, &error);
    check_ob_error(&error);

    return 0;
}

