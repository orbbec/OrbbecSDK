#include "window.hpp"

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

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

// Create global variables
Window      *win    = nullptr;  // render window, based on opencv
ob_error    *error  = NULL;     // Used to return SDK interface error information
ob_pipeline *pipe   = nullptr;  // pipeline, used to open the Infrared stream after connecting the device
ob_device   *device = nullptr;  // device, obtained through the pipeline, and the Infrared image can be set through the device

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
    // Create a pipeline to open the Infrared stream after connecting the device
    pipe = ob_create_pipeline(&error);
    check_error(error);

    // Create config to configure the resolution, frame rate, and format of the Infrared stream
    ob_config *config = ob_create_config(&error);
    check_error(error);

    // Configure the infrared stream
    ob_stream_profile      *ir_profile = NULL;
    ob_stream_profile_list *profiles   = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_IR, &error);
    check_error(error);

    // Find the corresponding profile according to the specified format, first look for the y16 format
    ir_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 0, OB_FORMAT_Y16, 30, &error);
    // If the specified format is not found, search for the default profile to open the stream
    if(error) {
        ir_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
        ob_delete_error(error);
        error = nullptr;
    }

    // enable stream
    ob_config_enable_stream(config, ir_profile, &error);
    check_error(error);

    // Get the device through the pipeline
    device = ob_pipeline_get_device(pipe, &error);
    check_error(error);

    // Determine whether to support switching left and right ir channels
    if(ob_device_is_property_supported(device, OB_PROP_IR_CHANNEL_DATA_SOURCE_INT, OB_PERMISSION_READ_WRITE, &error)) {
        // Gemini2 products support SENSOR_IR to select sensor output, 0 is left IR, 1 is right IR.
        int32_t dataSource = 0;
        ob_device_set_int_property(device, OB_PROP_IR_CHANNEL_DATA_SOURCE_INT, dataSource, &error);
        check_error(error);
    }
    check_error(error);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipe, config, &error);
    check_error(error);

    // Create a rendering display window
    uint32_t width = ob_video_stream_profile_width(ir_profile, &error);
    check_error(error);
    uint32_t height = ob_video_stream_profile_height(ir_profile, &error);
    check_error(error);
    win = new Window("InfraredViewer", width, height);
    check_error(error);

    // Wait in a loop, exit after the window receives the "esc" key
    while(*win) {
        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipe, 100, &error);
        check_error(error);

        if(frameset == nullptr) {
            continue;
        }

        ob_frame *ir_frame = ob_frameset_ir_frame(frameset, &error);
        check_error(error);
        if(ir_frame != nullptr) {
            // add frame to render, will auto delete frame when render finished
            win->addToRender(ir_frame);
        }

        ob_delete_frame(frameset, &error);
        check_error(error);
    };

    // stop the pipeline
    ob_pipeline_stop(pipe, &error);
    check_error(error);

    // destroy the window
    delete win;

    // destroy profile
    ob_delete_stream_profile(ir_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(profiles, &error);
    check_error(error);

    // destroy device
    ob_delete_device(device, &error);
    check_error(error);

    // destroy the pipeline
    ob_delete_pipeline(pipe, &error);
    check_error(error);

    return 0;
}
