#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "../window.hpp"

extern "C" {
#include <libobsensor/h/Device.h>
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/Sensor.h>
#include <libobsensor/h/StreamProfile.h>
#include <stdlib.h>
}

/*
 * This example is written in C++, based on the C language version API of
 * OrbbecSDK, to demonstrate how to open color stream and get frames.
 */

// create global variable
Window *win = nullptr;        // rendering display window, based on opencv
ob_error *error = NULL;       // Used to return SDK interface error information
ob_pipeline *pipe = nullptr;  // pipeline, used to open the color stream after
                              // connecting the device
ob_device *device =
    nullptr;  // device, obtained through the pipeline, and the corresponding
              // sensor can be obtained through the device

ob_frame *frameset_for_render = nullptr;  // The frame used to render
bool frameset_ready =
    false;                 // Whether the frame used to render has been captured
bool thread_exit = false;  // Does the thread need to exit
std::mutex frameset_mutex;  // frameSet mutex
std::condition_variable
    frameset_proc_cv;  // frameSet handles condition variables

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

// Processing and rendering display frameset
void frameset_proc_func() {
  while (!thread_exit) {
    std::unique_lock<std::mutex> lk(frameset_mutex);
    // Wait for frameSet to be ready or thread exit signal
    frameset_proc_cv.wait(lk, [&]() { return frameset_ready || thread_exit; });
    if (!thread_exit && frameset_ready) {
      ob_frame *color_frame =
          ob_frameset_color_frame(frameset_for_render, &error);
      check_error(error);

      if (color_frame != nullptr) {
        std::vector<ob_frame *> frames = {color_frame};
        win->render(frames, RENDER_ONE_ROW);
      }

      // Destroy frameSet to reclaim memory
      ob_delete_frame(frameset_for_render, &error);
      check_error(error);
      frameset_for_render = nullptr;

      if (color_frame != nullptr) {
        // Destroy frame, reclaim memory
        ob_delete_frame(color_frame, &error);
        check_error(error);
      }

      // reset flag
      frameset_ready = false;
    }
  }
}

// grab frameset
void frameset_grab_func() {
  while (!thread_exit) {
    // Wait for up to 100ms for a frameset in blocking mode.
    ob_frame *frameset = nullptr;
    frameset = ob_pipeline_wait_for_frameset(pipe, 100, &error);
    check_error(error);

    if (frameset != nullptr) {
      if (!frameset_ready) {
        std::unique_lock<std::mutex> lk(frameset_mutex);
        frameset_for_render = frameset;
        frameset_ready = true;
        frameset_proc_cv.notify_all();
      } else {
        // frameset_proc_func has not finished processing the previously
        // acquired frameSet, discarding the acquired frameSet. If you continue
        // to wait for frameset_proc_func to finish processing the previously
        // acquired frameSet, it will cause the SDK to cache too many frameSets,
        // resulting in large memory usage and high latency. Destroy frameSet to
        // reclaim memory
        ob_delete_frame(frameset, &error);
        check_error(error);
      }
    }
  }
}

int main(int argc, char **args) {
  // Create a pipeline to open the color stream after connecting the device
  pipe = ob_create_pipeline(&error);
  check_error(error);

  // Create config to configure the resolution, frame rate, and format of the
  // color stream
  ob_config *config = ob_create_config(&error);
  check_error(error);

  // Configure the color stream
  ob_stream_profile *color_profile = nullptr;
  ob_stream_profile_list *profiles =
      ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_COLOR, &error);
  if (error) {
    printf("Current device is not support color sensor!\n");
    exit(EXIT_FAILURE);
  }

  // Find the corresponding Profile according to the specified format, and
  // choose the RGB888 format first
  color_profile = ob_stream_profile_list_get_video_stream_profile(
      profiles, 1280, 0, OB_FORMAT_RGB, 30, &error);
  // If the specified format is not found, search for the default Profile to
  // open the stream
  if (error) {
    color_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
    ob_delete_error(error);
    error = nullptr;
  }

  // enable stream
  ob_config_enable_stream(config, color_profile, &error);
  check_error(error);

  // Get Device through Pipeline
  device = ob_pipeline_get_device(pipe, &error);
  check_error(error);

  // Get whether the mirror property has writable permission
  ob_device_is_property_supported(device, OB_PROP_COLOR_MIRROR_BOOL,
                                  OB_PERMISSION_WRITE, &error);
  if (error && error->status == OB_STATUS_OK) {
    // set mirroring
    ob_device_set_bool_property(device, OB_PROP_COLOR_MIRROR_BOOL, true,
                                &error);
    check_error(error);
  }
  check_error(error);

  // Start the pipeline with config
  ob_pipeline_start_with_config(pipe, config, &error);
  check_error(error);

  // Create a rendering display window
  win = new Window("ColorViewer",
                   ob_video_stream_profile_width(color_profile, &error),
                   ob_video_stream_profile_height(color_profile, &error));
  check_error(error);

  // Start the frameSet grabbing and processing thread
  thread_exit = false;
  frameset_ready = false;
  std::thread frameset_grab_thread(frameset_grab_func);
  std::thread frameset_proc_thread(frameset_proc_func);

  // Wait in a loop, and exit after the window receives the "ESC" key
  while (*win) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  };

  // End of frameSet fetching and processing thread
  thread_exit = true;
  frameset_proc_cv.notify_all();
  if (frameset_grab_thread.joinable()) {
    frameset_grab_thread.join();
  }
  if (frameset_proc_thread.joinable()) {
    frameset_proc_thread.join();
  }

  // stop the pipeline
  ob_pipeline_stop(pipe, &error);
  check_error(error);

  // destroy window
  delete win;
  if (frameset_for_render != nullptr) {
    // Destroy frameSet to reclaim memory
    ob_delete_frame(frameset_for_render, &error);
    check_error(error);
    frameset_for_render = nullptr;
  }

  // destroy profile
  ob_delete_stream_profile(color_profile, &error);
  check_error(error);

  // destroy profile list
  ob_delete_stream_profile_list(profiles, &error);
  check_error(error);

  // destroy the device
  ob_delete_device(device, &error);
  check_error(error);

  // destroy the pipeline
  ob_delete_pipeline(pipe, &error);
  check_error(error);

  return 0;
}
