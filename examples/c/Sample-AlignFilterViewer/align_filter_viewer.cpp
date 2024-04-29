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

static bool  sync  = false;
static float alpha = 0.5;

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

// key press event processing
void keyEventProcess(Window *app, ob_pipeline *pipe, ob_error *error) {
    ////Get the key value
    int key = app->waitKey(10);
    if(key == '+' || key == '=') {
        // Press the + key to increase alpha
        alpha += 0.1f;
        if(alpha >= 1.0f) {
            alpha = 1.0f;
        }
        app->setAlpha(alpha);
    }
    else if(key == '-' || key == '_') {
        // press - key to decrease alpha
        alpha -= 0.1f;
        if(alpha <= 0.0f) {
            alpha = 0.0f;
        }
        app->setAlpha(alpha);
    }
    else if(key == 'F' || key == 'f') {
        // Press the F key to switch synchronization
        sync = !sync;
        if(sync) {
            try {
                // enable synchronization
                ob_pipeline_enable_frame_sync(pipe, &error);
            }
            catch(...) {
                std::cerr << "Sync not support" << std::endl;
            }
        }
        else {
            try {
                // turn off sync
                ob_pipeline_disable_frame_sync(pipe, &error);
            }
            catch(...) {
                std::cerr << "Sync not support" << std::endl;
            }
        }
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

    // Configure the color stream
    ob_stream_profile      *color_profile  = nullptr;
    ob_stream_profile_list *color_profiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
    if(error) {
        printf("Current device is not support color sensor!\n");
        exit(EXIT_FAILURE);
    }

    // Find the corresponding Profile according to the specified format, and choose the RGB888 format first
    color_profile = ob_stream_profile_list_get_video_stream_profile(color_profiles, 1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30, &error);
    // If the specified format is not found, search for the default Profile to open the stream
    if(error) {
        color_profile = ob_stream_profile_list_get_profile(color_profiles, OB_PROFILE_DEFAULT, &error);
        ob_delete_error(error);
        error = nullptr;
    }

    // enable stream
    ob_config_enable_stream(config, color_profile, &error);
    check_error(error);

    /* Config depth align to color or color align to depth.
    ob_stream_type align_to_stream = OB_STREAM_DEPTH; */
    ob_stream_type align_to_stream = OB_STREAM_COLOR;
    ob_filter     *align_filter    = ob_create_align(&error, align_to_stream);

    // Start the pipeline with config
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error);

    // Create a window for rendering, and set the resolution of the window
    uint32_t width = ob_video_stream_profile_width(color_profile, &error);
    check_error(error);
    uint32_t height = ob_video_stream_profile_height(color_profile, &error);
    check_error(error);
    win = new Window("AlignFilterViewer", width, height, RENDER_OVERLAY);
    check_error(error);

    bool resizeWindows = true;

    // Wait in a loop, exit after the window receives the "esc" key
    while(*win) {
        keyEventProcess(win, pipeline, error);

        // Wait for up to 100ms for a frameset in blocking mode.
        ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
        check_error(error);

        if(frameset == nullptr) {
            continue;
        }

        ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
        check_error(error);

        ob_frame *color_frame = ob_frameset_color_frame(frameset, &error);
        check_error(error);

        if(depth_frame != nullptr && color_frame != nullptr) {
            ob_frame *new_depth_frame = nullptr;
            ob_frame *new_color_frame = nullptr;

            ob_frame *new_frame_set = ob_filter_process(align_filter, frameset, &error);
            check_error(error);

            new_depth_frame = ob_frameset_depth_frame(new_frame_set, &error);
            check_error(error);

            new_color_frame = ob_frameset_color_frame(new_frame_set, &error);
            check_error(error);

            ob_delete_frame(depth_frame, &error);
            check_error(error);

            ob_delete_frame(color_frame, &error);
            check_error(error);

            depth_frame = new_depth_frame;
            color_frame = new_color_frame;
            if(resizeWindows) {
                int w, h = 0;
                if(align_to_stream == OB_STREAM_COLOR) {
                    w = ob_video_frame_width(color_frame, &error);
                    check_error(error);

                    h = ob_video_frame_height(color_frame, &error);
                    check_error(error);
                }
                else {
                    w = ob_video_frame_width(depth_frame, &error);
                    check_error(error);

                    h = ob_video_frame_height(depth_frame, &error);
                    check_error(error);
                }
                win->resize(w, h);
                resizeWindows = false;
            }

            win->addToRender({ color_frame, depth_frame });

            ob_delete_frame(new_frame_set, &error);
            check_error(error);
        }
        ob_delete_frame(frameset, &error);
        check_error(error);
    };

    ob_delete_filter(align_filter, &error);
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
    // destroy profile
    ob_delete_stream_profile(color_profile, &error);
    check_error(error);

    // destroy profile list
    ob_delete_stream_profile_list(color_profiles, &error);
    check_error(error);

    // destroy the pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);

    return 0;
}
