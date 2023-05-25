#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "../window.hpp"

extern "C" {
#include <libobsensor/h/Context.h>
#include <libobsensor/h/Device.h>
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/Sensor.h>
#include <libobsensor/h/StreamProfile.h>
#include <stdlib.h>
}

// Create global variables
Window *win = nullptr;        // render window, based on opencv
ob_error *error = NULL;       // Used to return SDK interface error information
ob_context *ctx = nullptr;    // context, used to create device
ob_device *device = nullptr;  // device, used to create pipeline
ob_pipeline *pipe = nullptr;  // pipeline, used to open the data stream after
                              // connecting the device

ob_frame *current_frameset = nullptr;  // The frame used to render
std::mutex frameset_mutex;             // frameSet mutex
// exception handler
void check_error(ob_error *error) {
  if (error) {
    printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error),
           ob_error_args(error));
    printf("\tmessage: %s\n", ob_error_message(error));
    printf("\terror type: %d\n", ob_error_exception_type(error));
    ob_delete_error(error);
    exit(EXIT_FAILURE);
  }
}

// DataFrame collection callback function
void frameset_callback(ob_frame *frameset, void *user_data) {
  std::unique_lock<std::mutex> lock(frameset_mutex);
  if (current_frameset != nullptr) {
    // Destroy frameSet to reclaim memory
    ob_delete_frame(current_frameset, &error);
    check_error(error);
  }
  current_frameset = frameset;
}

int main(int argc, char **args) {
  // create context
  ctx = ob_create_context(&error);
  check_error(error);

  // Currently only FemtoMega devices support network mode, and its default ip
  // is: 192.168.1.10
  char ip[16];
  printf("Input your device ip(default: 192.168.1.10):");
  if (fgets(ip, sizeof ip, stdin) != NULL) {
    char *newline = strchr(ip, '\n');
    if (newline) *newline = 0;
    if (strlen(ip) == 0) {
      strcpy(ip, "192.168.1.10");
    }
  }

  // Create a network device by ip address and port number (current FemtoMega
  // port number is: 8090, and cannot be modified)
  device = ob_create_net_device(ctx, ip, 8090, &error);
  check_error(error);

  // Create a pipeline to open the color stream after connecting the device
  pipe = ob_create_pipeline_with_device(device, &error);
  check_error(error);

  // Create config to configure the resolution, frame rate, and format of the
  // color stream
  ob_config *config = ob_create_config(&error);
  check_error(error);

  ob_stream_profile *color_profile = nullptr;
  // Get the list of Color stream profiles
  ob_stream_profile_list *color_profiles =
      ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_COLOR, &error);
  check_error(error);
  // Select the default stream configuration
  color_profile = ob_stream_profile_list_get_profile(color_profiles, 0, &error);
  check_error(error);
  // enable stream
  ob_config_enable_stream(config, color_profile, &error);
  check_error(error);

  ob_stream_profile *depth_profile = nullptr;
  // Get the depth stream profile list
  ob_stream_profile_list *depth_profiles =
      ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_DEPTH, &error);
  check_error(error);
  // Select the default stream configuration
  depth_profile = ob_stream_profile_list_get_profile(depth_profiles, 0, &error);
  check_error(error);
  // enable stream
  ob_config_enable_stream(config, depth_profile, &error);
  check_error(error);

  // Start the pipeline with config
  ob_pipeline_start_with_callback(pipe, config, frameset_callback, nullptr,
                                  &error);
  check_error(error);

  // Create a rendering display window
  win = new Window("NetDevice",
                   ob_video_stream_profile_width(color_profile, &error),
                   ob_video_stream_profile_height(color_profile, &error));
  check_error(error);

  // Wait in a loop, exit after the window receives the "ESC" key
  while (*win) {
    ob_frame *frameset_for_render = nullptr;

    {  // Define the scope through curly braces, and the lock will be
       // automatically unlocked after exiting the scope. Unlocking in time can
       // prevent the frameset output thread of the pipeline from being blocked
       // for too long, resulting in an increase in internal cache and an
       // increase in data frame delay
      std::unique_lock<std::mutex> lock(frameset_mutex);
      frameset_for_render = current_frameset;
      current_frameset = nullptr;
    }

    if (frameset_for_render != nullptr) {
      ob_frame *color_frame =
          ob_frameset_color_frame(frameset_for_render, &error);
      check_error(error);

      ob_frame *depth_frame =
          ob_frameset_depth_frame(frameset_for_render, &error);
      check_error(error);

      if (color_frame != nullptr && depth_frame != nullptr) {
        ob_format color_format = ob_frame_format(color_frame, &error);
        check_error(error);
        if (color_format == OB_FORMAT_H264 || color_format == OB_FORMAT_H265) {
          // For H264 and H265 format image frames, users can refer to FFmpeg or
          // other decoding libraries and their routines to complete decoding
          // and rendering display. This example is to keep the project and code
          // concise, so it will not be shown.
          uint32_t index = ob_frame_index(color_frame, &error);
          check_error(error);
          if (index % 30 == 0) {
            // Print the color data frame information every 30 frames
            uint64_t tsp = ob_frame_time_stamp(color_frame, &error);
            check_error(error);
            printf("Color frame: index=%d, timestamp=%lld", index, tsp);
          }
          win->render({depth_frame}, RENDER_ONE_ROW);
        } else {
          win->render({color_frame, depth_frame}, RENDER_ONE_ROW);
        }
      }

      // destroy the color frame
      if (color_frame != nullptr) {
        ob_delete_frame(color_frame, &error);
        check_error(error);
      }

      // Destroy the depth frame
      if (depth_frame != nullptr) {
        ob_delete_frame(depth_frame, &error);
        check_error(error);
      }

      // destroy frameset
      ob_delete_frame(frameset_for_render, &error);
      check_error(error);
    }

    // Sleep for 30ms, control the rendering display refresh rate
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  };

  // stop the pipeline
  ob_pipeline_stop(pipe, &error);
  check_error(error);

  // destroy window
  delete win;

  // Destroy frameSet to reclaim memory
  if (current_frameset != nullptr) {
    ob_delete_frame(current_frameset, &error);
    check_error(error);
    current_frameset = nullptr;
  }

  // destroy the color profile
  ob_delete_stream_profile(color_profile, &error);
  check_error(error);

  // destroy the color profile list
  ob_delete_stream_profile_list(color_profiles, &error);
  check_error(error);

  // destroy the depth profile
  ob_delete_stream_profile(depth_profile, &error);
  check_error(error);

  // Destroy the depth profile list
  ob_delete_stream_profile_list(depth_profiles, &error);
  check_error(error);

  // destroy device
  ob_delete_device(device, &error);
  check_error(error);

  // destroy the pipeline
  ob_delete_pipeline(pipe, &error);
  check_error(error);

  // destroy context
  ob_delete_context(ctx, &error);
  check_error(error);

  return 0;
}
