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
    ob_pipeline *pipeline = nullptr;  // pipeline, used to open the Infrared stream after connecting the device

    // Create a pipeline to open the Infrared stream after connecting the device
    pipeline = ob_create_pipeline(&error);
    check_error(error);

    // Create config to configure the resolution, frame rate, and format of the Infrared stream
    ob_config *config = ob_create_config(&error);
    check_error(error);

    // Configure the infrared stream(IR_LEFT)
    ob_stream_profile      *ir_left_profile  = NULL;
    ob_stream_profile_list *ir_left_profiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_IR_LEFT, &error);
    check_error(error);

    if(ir_left_profiles == nullptr) {
        printf("The obtained IR_Left resolution list is NULL. For monocular structured light devices, try opening the IR data stream using the IR example. ");
        return 0;
    }

    // Find the corresponding profile according to the specified format, first look for the y16 format
    ir_left_profile = ob_stream_profile_list_get_profile(ir_left_profiles, OB_PROFILE_DEFAULT, &error);
    check_error(error);

    // enable stream
    ob_config_enable_stream(config, ir_left_profile, &error);
    check_error(error);

    // Configure the infrared stream(IR_RIGHT)
    ob_stream_profile      *ir_right_profile  = NULL;
    ob_stream_profile_list *ir_right_profiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_IR_RIGHT, &error);
    check_error(error);

    // Find the corresponding profile according to the specified format, first look for the y16 format
    ir_right_profile = ob_stream_profile_list_get_profile(ir_right_profiles, OB_PROFILE_DEFAULT, &error);
    check_error(error);

    // enable stream
    ob_config_enable_stream(config, ir_right_profile, &error);
    check_error(error);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error);

    // Create a rendering display window
    win = new Window("DoubleInfraredViewer", 1280, 800, RENDER_ONE_ROW);
    check_error(error);

    // Wait in a loop, exit after the window receives the "esc" key
    while(*win) {
        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
        check_error(error);

        if(frameset == nullptr) {
            continue;
        }

        ob_frame *ir_left_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_LEFT, &error);
        check_error(error);

        ob_frame *ir_right_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_RIGHT, &error);
        check_error(error);

        if(ir_left_frame == nullptr || ir_right_frame == nullptr) {
            std::cout << "left ir frame or right ir frame is null. leftFrame: " << ir_left_frame << ", rightFrame: " << ir_right_frame << std::endl;
            continue;
        }

        // Render a set of frame in the window, only the infrared frame is rendered here.
        win->addToRender({ ir_left_frame, ir_right_frame });

        ob_delete_frame(frameset, &error);
        check_error(error);
    };

    // stop the pipeline
    ob_pipeline_stop(pipeline, &error);
    check_error(error);

    // destroy the window
    delete win;

    // destroy profile
    ob_delete_stream_profile(ir_left_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(ir_left_profiles, &error);
    check_error(error);

    // destroy profile
    ob_delete_stream_profile(ir_right_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(ir_right_profiles, &error);
    check_error(error);

    // destroy the pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);

    return 0;
}
