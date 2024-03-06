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
    Window      *win      = nullptr;  // render window, based on opencv
    ob_error    *error    = NULL;     // Used to return SDK interface error information
    ob_pipeline *pipeline = nullptr;  // pipeline, used to open the depth stream after connecting the device

    // Create a pipeline to open the depth stream after connecting the device
    pipeline = ob_create_pipeline(&error);
    check_error(error);

    // Create config to configure the resolution, frame rate, and format of the depth stream
    ob_config *config = ob_create_config(&error);
    check_error(error);

    // Configure the depth stream
    ob_stream_profile      *depth_profile = NULL;
    ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
    check_error(error);

    // Find the corresponding profile according to the specified format, first look for the y16 format
    depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
    // If the specified format is not found, search for the default profile to open the stream
    if(error) {
        depth_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
        ob_delete_error(error);
        error = nullptr;
    }

    // enable stream
    ob_config_enable_stream(config, depth_profile, &error);
    check_error(error);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error);

    // Create a window for rendering, and set the resolution of the window
    uint32_t width = ob_video_stream_profile_width(depth_profile, &error);
    check_error(error);
    uint32_t height = ob_video_stream_profile_height(depth_profile, &error);
    check_error(error);
    win = new Window("DepthViewer", width, height);
    check_error(error);

    // Wait in a loop, exit after the window receives the "esc" key
    while(*win) {
        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
        check_error(error);

        if(frameset == nullptr) {
            continue;
        }

        ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
        check_error(error);
        if(depth_frame != nullptr) {
            // for Y16 format depth frame, print the distance of the center pixel every 30 frames
            uint32_t index = ob_frame_index(depth_frame, &error);
            check_error(error);
            ob_format format = ob_frame_format(depth_frame, &error);
            check_error(error);
            if(index % 30 == 0 && format == OB_FORMAT_Y16) {
                uint32_t width = ob_video_frame_width(depth_frame, &error);
                check_error(error);
                uint32_t height = ob_video_frame_height(depth_frame, &error);
                check_error(error);
                float scale = ob_depth_frame_get_value_scale(depth_frame, &error);
                check_error(error);
                uint16_t *data = (uint16_t *)ob_frame_data(depth_frame, &error);
                check_error(error);

                // pixel value multiplied by scale is the actual distance value in millimeters
                float center_distance = data[width * height / 2 + width / 2] * scale;

                // attention: if the distance is 0, it means that the depth camera cannot detect the object（may be out of detection range）
                printf("Facing an object %.2f mm away.\n", center_distance);
            }

            // add frame to render
            // attention: the frame will be released inside the window，for user's code should release it by call ob_delete_frame()
            win->addToRender(depth_frame);
        }
        ob_delete_frame(frameset, &error);
        check_error(error);
    };

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

    // destroy the pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);

    return 0;
}
