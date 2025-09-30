#include "window.hpp"
#include <iostream>

extern "C" {
#include <stdlib.h>
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/StreamProfile.h>
#include <libobsensor/h/Device.h>
}

/*
 *This sample is written in C++ code, based on the C language version API of OrbbecSDK.
 */
void check_error(ob_error *error) {
    if(error) {
        printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error), ob_error_args(error));
        printf("\tmessage: %s\n", ob_error_message(error));
        printf("\terror type: %d\n", ob_error_exception_type(error));
        ob_delete_error(error);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **args) {
    ob_error *error = NULL;  // Used to return SDK interface error information

    // Create a pipeline to open the depth stream after connecting the device
    ob_pipeline *pipeline = ob_create_pipeline(&error);
    check_error(error);

    // Get the device through the pipeline
    ob_device *dev = ob_pipeline_get_device(pipeline, &error);
    check_error(error);

    // check if the device supports Hdr feature
    bool is_supported = ob_device_is_property_supported(dev, OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE, &error);
    check_error(error);
    if(!is_supported) {
        printf("The device does not support Hdr feature.\n");
        return 0;
    }

    // configure Hdr
    ob_hdr_config hdr_config;
    hdr_config.enable     = true;  // enable Hdr
    hdr_config.exposure_1 = 7500;
    hdr_config.gain_1     = 16;
    hdr_config.exposure_2 = 1;
    hdr_config.gain_2     = 16;
    ob_device_set_structured_data(dev, OB_STRUCT_DEPTH_HDR_CONFIG, &hdr_config, sizeof(ob_hdr_config), &error);
    check_error(error);

    // Create HdrMerge post processor
    ob_filter *hdr_merge_filter = ob_create_hdr_merge(&error);
    check_error(error);

    // Create config to configure the resolution, frame rate, and format of the depth stream
    ob_config *config = ob_create_config(&error);
    check_error(error);

    // Configure the depth stream
    ob_stream_profile      *depth_profile = NULL;
    ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
    check_error(error);
    depth_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    check_error(error);

    // enable stream
    ob_config_enable_stream(config, depth_profile, &error);
    check_error(error);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error);

    // Create a window for rendering, and set the resolution of the window
    Window *win = new Window("HDR-Merge", 1280, 720, RENDER_GRID);
    check_error(error);

    bool merge_required = true;  // flag to check if Hdr merge is required
    printf("Press 'M' to toggle Hdr merge.\n");

    // Wait in a loop, exit after the window receives the "esc" key
    while(*win) {
        int key = win->waitKey();
        if(key == 'M' || key == 'm') {
            merge_required = !merge_required;  // toggle the flag
            if(merge_required) {
                printf("Hdr merge is enabled.\n");
            }
            else {
                printf("Hdr merge is disabled.\n");
            }
        }

        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
        check_error(error);
        if(frameset == nullptr) {
            continue;
        }

        ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
        check_error(error);
        if(depth_frame == nullptr) {
            ob_delete_frame(frameset, &error);
            check_error(error);
            continue;
        }

        if(merge_required) {
            // merge the depth frame with Hdr merge filter
            auto merged_depth_frame = ob_filter_process(hdr_merge_filter, depth_frame, &error);
            check_error(error);
            if(merged_depth_frame == nullptr) {
                ob_delete_frame(frameset, &error);
                check_error(error);
                ob_delete_frame(depth_frame, &error);
                check_error(error);
                continue;
            }

            // add the merged depth frame to the render queue
            win->addToRender(merged_depth_frame);  // will auto release merged_depth_frame inside the function

            // release depth frame
            ob_delete_frame(depth_frame, &error);
            check_error(error);
        }
        else {
            // add the original depth frame to the render queue
            win->addToRender(depth_frame);  // will auto release merged_depth_frame inside the function
        }

        // release the frameset
        ob_delete_frame(frameset, &error);
        check_error(error);
    }

    // close hdr
    hdr_config.enable = false;
    ob_device_set_structured_data(dev, OB_STRUCT_DEPTH_HDR_CONFIG, &hdr_config, sizeof(ob_hdr_config), &error);
    check_error(error);

    ob_delete_filter(hdr_merge_filter, &error);
    check_error(error);

    // stop the pipeline
    ob_pipeline_stop(pipeline, &error);
    check_error(error);

    // destroy the window
    delete win;

    // destroy profile
    ob_delete_stream_profile(depth_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(profiles, &error);
    check_error(error);

    // destroy the config
    ob_delete_config(config, &error);
    check_error(error);

    ob_delete_device(dev, &error);
    check_error(error);

    // destroy the pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);

    return 0;
}
