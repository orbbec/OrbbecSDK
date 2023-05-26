#if defined(_WIN32)
#include <conio.h>
#else
#include "../conio.h"
#endif
#include <libobsensor/h/Context.h>
#include <libobsensor/h/Device.h>
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/StreamProfile.h>
#include <stdio.h>
#include <stdlib.h>

#define ESC 27
ob_pipeline *pipeline = NULL;
ob_error *error = NULL;
ob_config *config = NULL;
bool isExit = false;

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

void create_and_start_with_config() {
  // Start the pipeline with default config (`NULL` passed)
  ob_pipeline_start_with_config(pipeline, NULL, &error);
  check_error(error);
}

// Device connect callback
void device_connect_callback(ob_device_list *connectList) {
  uint32_t count = ob_device_list_device_count(connectList, &error);
  check_error(error);
  printf("Device connect: %d\n", count);
  if (count > 0) {
    if (pipeline == NULL) {
      pipeline = ob_create_pipeline(&error);
      check_error(error);
      create_and_start_with_config();
    }
  }
}

// Device disconnect callback
void device_disconnect_callback(ob_device_list *disconnectList) {
  uint32_t count = ob_device_list_device_count(disconnectList, &error);
  check_error(error);
  printf("Device disconnect: %d\n", count);
  if (count > 0) {
    isExit = true;
  }
}

// Device state change callback
void on_device_changed_callback(ob_device_list *removed, ob_device_list *added,
                                void *pCallback) {
  device_disconnect_callback(removed);
  device_connect_callback(added);

  // Need to manually destroy the device list
  // destroy device list
  ob_delete_device_list(removed, &error);
  check_error(error);

  // destroy device list
  ob_delete_device_list(added, &error);
  check_error(error);
}

int main(int argc, char **argv) {
  // create context
  ob_context *ctx = ob_create_context(&error);
  check_error(error);

  // Register device callback
  ob_set_device_changed_callback(ctx, on_device_changed_callback, NULL, &error);
  check_error(error);

  // Query device list
  ob_device_list *dev_list = ob_query_device_list(ctx, &error);
  check_error(error);

  // Get the number of devices
  uint32_t dev_count = ob_device_list_device_count(dev_list, &error);
  check_error(error);

  if (dev_count > 0) {
    if (pipeline == NULL) {
      // create pipeline
      pipeline = ob_create_pipeline(&error);
      check_error(error);
      create_and_start_with_config();
    }
  }

  while (true) {
    if (kbhit()) {
      int key = getch();

      // press esc key to exit
      if (key == ESC) {
        break;
      }
      // Press the r key to reboot the device to trigger the device disconnect
      // and reconnect event, or manually unplug and plugin the device.
      if (key == 'r' || key == 'R') {
        if (pipeline) {
          ob_device *device = ob_pipeline_get_device(pipeline, &error);
          check_error(error);
          ob_device_reboot(device, &error);
          check_error(error);
          ob_delete_device(device, &error);
          check_error(error);
        }
      }
    }
    if (pipeline) {
      // Waiting for one frame, the timeout is 100ms
      ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
      check_error(error);

      if (frameset) {
        // Get the depth data frame
        ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
        check_error(error);

        if (depth_frame) {
          ob_frame_type type = ob_frame_get_type(depth_frame, &error);
          check_error(error);
          uint32_t index = ob_frame_index(depth_frame, &error);
          check_error(error);
          uint32_t width = ob_video_frame_width(depth_frame, &error);
          check_error(error);
          uint32_t height = ob_video_frame_height(depth_frame, &error);
          check_error(error);
          ob_format format = ob_frame_format(depth_frame, &error);
          check_error(error);
          uint64_t timestampUs = ob_frame_time_stamp_us(depth_frame, &error);
          check_error(error);
          float scale = ob_depth_frame_get_value_scale(depth_frame, &error);
          check_error(error);
          uint16_t *data = (uint16_t *)ob_frame_data(depth_frame, &error);
          check_error(error);
          uint16_t pixelValue = *(data + width * height / 2 + width / 2);

          printf(
              "=====Depth Frame Info====== FrameType:%d, index:%d, width:%d, "
              "height:%d, format:%d, timeStampUs:%lldus, centerDepth:%dmm\n",
              type, index, width, height, format, timestampUs,
              (uint32_t)(pixelValue * scale));

          // Destroy the depth data frame
          ob_delete_frame(depth_frame, &error);
          check_error(error);
        }

        // Get the color data frame
        ob_frame *color_frame = ob_frameset_color_frame(frameset, &error);
        check_error(error);

        if (color_frame) {
          ob_frame_type type = ob_frame_get_type(color_frame, &error);
          check_error(error);
          uint32_t index = ob_frame_index(color_frame, &error);
          check_error(error);
          uint32_t width = ob_video_frame_width(color_frame, &error);
          check_error(error);
          uint32_t height = ob_video_frame_height(color_frame, &error);
          check_error(error);
          ob_format format = ob_frame_format(color_frame, &error);
          check_error(error);
          uint64_t timestampUs = ob_frame_time_stamp_us(color_frame, &error);
          check_error(error);

          printf(
              "=====Color Frame Info====== FrameType:%d, index:%d, width:%d, "
              "height:%d, format:%d, timeStampUs:%lldus\n",
              type, index, width, height, format, timestampUs);

          // Destroy the color data frame
          ob_delete_frame(color_frame, &error);
          check_error(error);
        }

        // destroy frameset
        ob_delete_frame(frameset, &error);
        check_error(error);
      }

      if (isExit) {
        // destroy pipeline
        ob_pipeline_stop(pipeline, &error);
        check_error(error);
        ob_delete_pipeline(pipeline, &error);
        check_error(error);
        pipeline = NULL;
        isExit = false;
      }
    }
  }

  if (pipeline) {
    // stop pipeline
    ob_pipeline_stop(pipeline, &error);
    check_error(error);

    // destroy pipeline
    ob_delete_pipeline(pipeline, &error);
    check_error(error);
  }

  // destroy device list
  if (dev_list) {
    ob_delete_device_list(dev_list, &error);
    check_error(error);
  }
  // destroy context
  if (ctx) {
    ob_delete_context(ctx, &error);
    check_error(error);
  }
}
