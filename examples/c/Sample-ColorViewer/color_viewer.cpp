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
#include <libobsensor/h/Sensor.h>
}

/*
 * Notes: Currently, only Gemini 330 series devices support Metadata.
 */
const char *metaDataTypes[] = { "TIMESTAMP",
                                "SENSOR_TIMESTAMP",
                                "FRAME_NUMBER",
                                "AUTO_EXPOSURE",
                                "EXPOSURE",
                                "GAIN",
                                "AUTO_WHITE_BALANCE",
                                "WHITE_BALANCE",
                                "BRIGHTNESS",
                                "CONTRAST",
                                "SATURATION",
                                "SHARPNESS",
                                "BACKLIGHT_COMPENSATION",
                                "HUE",
                                "GAMMA",
                                "POWER_LINE_FREQUENCY",
                                "LOW_LIGHT_COMPENSATION",
                                "MANUAL_WHITE_BALANCE",
                                "ACTUAL_FRAME_RATE",
                                "FRAME_RATE",
                                "AE_ROI_LEFT",
                                "AE_ROI_TOP",
                                "AE_ROI_RIGHT",
                                "AE_ROI_BOTTOM",
                                "EXPOSURE_PRIORITY",
                                "HDR_SEQUENCE_NAME",
                                "HDR_SEQUENCE_SIZE",
                                "HDR_SEQUENCE_INDEX",
                                "LASER_POWER",
                                "LASER_POWER_LEVEL",
                                "LASER_STATUS",
                                "GPIO_INPUT_DATA" };

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
    Window      *win      = nullptr;  // rendering display window, based on opencv
    ob_error    *error    = NULL;     // Used to return SDK interface error information
    ob_pipeline *pipeline = nullptr;  // pipeline, used to open the color stream after connecting the device
    ob_device   *device   = nullptr;  // device, obtained through the pipeline, and the corresponding sensor can be obtained through the device

    // Create a pipeline to open the color stream after connecting the device
    pipeline = ob_create_pipeline(&error);
    check_error(error);

    // Create config to configure the resolution, frame rate, and format of the color stream
    ob_config *config = ob_create_config(&error);
    check_error(error);

    // Configure the color stream
    ob_stream_profile      *color_profile = nullptr;
    ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
    if(error) {
        printf("Current device is not support color sensor!\n");
        exit(EXIT_FAILURE);
    }

    // Find the corresponding Profile according to the specified format, and choose the RGB888 format first
    color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30, &error);
    // If the specified format is not found, search for the default Profile to open the stream
    if(error) {
        color_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
        ob_delete_error(error);
        error = nullptr;
    }

    // enable stream
    ob_config_enable_stream(config, color_profile, &error);
    check_error(error);

    // Get Device through Pipeline
    device = ob_pipeline_get_device(pipeline, &error);
    check_error(error);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error);

    // Create a rendering display window
    uint32_t width = ob_video_stream_profile_width(color_profile, &error);
    check_error(error);
    uint32_t height = ob_video_stream_profile_height(color_profile, &error);
    check_error(error);
    win = new Window("ColorViewer", width, height);
    check_error(error);

    // Wait in a loop, and exit after the window receives the "ESC_KEY" key
    while(*win) {
        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
        check_error(error);

        if(frameset == nullptr) {
            continue;
        }

        // Get the color frame from the frameset
        ob_frame *color_frame = ob_frameset_color_frame(frameset, &error);
        check_error(error);
        if(color_frame == nullptr) {
            ob_delete_frame(frameset, &error);
            check_error(error);
            continue;
        }

        // Get the index of the frame
        auto index = ob_frame_index(color_frame, &error);
        check_error(error);

        // print meta data every 30 frames
        if(index % 30 == 0) {
            std::cout << "*************************** Color Frame #" << index << " Metadata List ********************************" << std::endl;
            for(int metaDataType = 0; metaDataType < OB_FRAME_METADATA_TYPE_COUNT; metaDataType++) {
                // Check if it is supported metaDataType
                if(ob_frame_has_metadata(color_frame, (OBFrameMetadataType)metaDataType, &error)) {
                    // get metaData value
                    std::cout << metaDataTypes[metaDataType] << ": " << ob_frame_get_metadata_value(color_frame, (OBFrameMetadataType)metaDataType, &error)
                              << std::endl;
                }
                else {
                    std::cout << metaDataTypes[metaDataType] << ": "
                              << "unsupported" << std::endl;
                }
            }
            std::cout << "********************************************************************************" << std::endl << std::endl;
        }

        // add frame to render, will auto delete frame when render finished
        win->addToRender(color_frame);

        ob_delete_frame(frameset, &error);
        check_error(error);
    };

    // stop the pipeline
    ob_pipeline_stop(pipeline, &error);
    check_error(error);

    // destroy window
    delete win;

    // destroy profile
    ob_delete_stream_profile(color_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(profiles, &error);
    check_error(error);

    // destroy the device
    ob_delete_device(device, &error);
    check_error(error);

    // destroy the config
    ob_delete_config(config, &error);
    check_error(error);

    // destroy the pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);

    return 0;
}
