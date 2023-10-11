#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <map>
#include <mutex>
#include <string>
#include <memory>
#include <string>
#include <thread>

#include "utils.hpp"

extern "C" {
#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/StreamProfile.h>
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Device.h>
#include <libobsensor/h/Context.h>
#include <libobsensor/h/Filter.h>
}

#define ESC_KEY 27

void check_error(ob_error *error) {
    if(error) {
        printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error), ob_error_args(error));
        printf("\tmessage: %s\n", ob_error_message(error));
        printf("\terror type: %d\n", ob_error_exception_type(error));
        ob_delete_error(error);
        exit(EXIT_FAILURE);
    }
}

typedef struct pipeline_holder_t {
    ob_device      *device;
    ob_device_info *device_info;
    ob_pipeline    *pipeline;
    bool            isStarted;

    uint32_t colorCount;
    uint32_t irCount;
    uint32_t irLeftCount;
    uint32_t irRightCount;
    uint32_t depthCount;
} pipeline_holder;

std::recursive_mutex                                    pipeline_holder_mutex;
std::map<std::string, std::shared_ptr<pipeline_holder>> pipeline_holder_map;

void handle_device_connected(ob_device_list *connect_list);
void handle_device_disconnected(ob_device_list *disconnect_list);
// Device state change callback
void on_device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data);

void start_stream(std::shared_ptr<pipeline_holder> holder);
void stop_stream(std::shared_ptr<pipeline_holder> holder);
void reboot_devices();
void delete_holder(std::shared_ptr<pipeline_holder> holder);
void sample_frame_set_callback(ob_frame *frameset, void *user_data);

std::string frameTypeToString(const OBFrameType frameType);
void        print_device_info(ob_device_info *device_info);  // print without new line
void        print_depth_frame(ob_frame *frame);              // print without new line
void        print_video_frame(ob_frame *frame);              // print without new line

int main(int argc, char **argv) {
    ob_error *error = NULL;

    // create context
    ob_context *ctx = ob_create_context(&error);
    check_error(error);

    // Register device callback
    ob_set_device_changed_callback(ctx, on_device_changed_callback, NULL, &error);
    check_error(error);

    // Query device list
    ob_device_list *dev_list = ob_query_device_list(ctx, &error);
    check_error(error);
    handle_device_connected(dev_list);
    ob_delete_device_list(dev_list, &error);
    check_error(error);

    while(true) {
        if(kbhit()) {
            int key = getch();

            // press esc key to exit
            if(key == ESC_KEY) {
                break;
            }
            // Press the r key to reboot the device to trigger the device disconnect and reconnect event, or manually unplug and plugin the device.
            if(key == 'r' || key == 'R') {
                reboot_devices();
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    // Free resource
    {
        std::lock_guard<std::recursive_mutex> lk(pipeline_holder_mutex);
        for(auto itr = pipeline_holder_map.begin(); itr != pipeline_holder_map.end(); itr++) {
            stop_stream(itr->second);
            delete_holder(itr->second);
        }
        pipeline_holder_map.clear();
    }

    // destroy context
    if(ctx) {
        ob_delete_context(ctx, &error);
        check_error(error);
    }

    return 0;
}

// Device state change callback
void on_device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data) {
    handle_device_disconnected(removed);
    handle_device_connected(added);

    ob_error *error = NULL;

    // Need to manually destroy the device list
    // destroy device list
    ob_delete_device_list(removed, &error);
    check_error(error);

    // destroy device list
    ob_delete_device_list(added, &error);
    check_error(error);
}

// Device connect callback
void handle_device_connected(ob_device_list *connect_list) {
    ob_error *error = NULL;

    std::lock_guard<std::recursive_mutex> lk(pipeline_holder_mutex);
    uint32_t                              count = ob_device_list_device_count(connect_list, &error);
    check_error(error);
    printf("Device connect: %d\n", count);
    for(uint32_t i = 0; i < count; i++) {
        const char *uid = ob_device_list_get_device_uid(connect_list, i, &error);
        check_error(error);
        std::string str_uid = std::string(uid);
        auto        itr     = pipeline_holder_map.find(str_uid);
        if(itr != pipeline_holder_map.end()) {
            printf("Deveice connect, device already connection.");
            print_device_info(itr->second->device_info);
            printf("\n");
        }
        else {
            ob_device *device = ob_device_list_get_device(connect_list, i, &error);
            check_error(error);

            ob_pipeline *pipeline = ob_create_pipeline_with_device(device, &error);
            check_error(error);

            ob_device_info *device_info = ob_device_get_device_info(device, &error);
            check_error(error);

            std::shared_ptr<pipeline_holder> holder(new pipeline_holder{ device, device_info, pipeline, false, 0, 0, 0, 0, 0 });
            start_stream(holder);
            pipeline_holder_map.insert({ str_uid, holder });

            printf("Deveice connect, ");
            print_device_info(device_info);
            printf("\n");
        }
    }
}

// Device disconnect callback
void handle_device_disconnected(ob_device_list *disconnect_list) {
    ob_error *error = NULL;

    std::lock_guard<std::recursive_mutex> lk(pipeline_holder_mutex);
    uint32_t                              count = ob_device_list_device_count(disconnect_list, &error);
    check_error(error);
    printf("Device disconnect: %d\n", count);

    for(uint32_t i = 0; i < count; i++) {
        const char *uid = ob_device_list_get_device_uid(disconnect_list, i, &error);
        check_error(error);
        std::string str_uid = std::string(uid);
        auto        itr     = pipeline_holder_map.find(str_uid);
        if(itr != pipeline_holder_map.end()) {
            auto holder = itr->second;
            stop_stream(holder);
            pipeline_holder_map.erase(str_uid);

            printf("Deveice discconnect, ");
            print_device_info(holder->device_info);
            printf("\n");

            delete_holder(holder);
        }
        else {
            const char *uid = ob_device_list_get_device_uid(disconnect_list, i, &error);
            check_error(error);
            printf("Device disconnect, Unsolve device, uid: %s\n", uid);
        }
    }
}

void start_stream(std::shared_ptr<pipeline_holder> holder) {
    ob_error *error = NULL;

    // Start the pipeline with default config (`NULL` passed)
    ob_pipeline_start_with_callback(holder->pipeline, NULL, sample_frame_set_callback, holder.get(), &error);
    check_error(error);
}

void stop_stream(std::shared_ptr<pipeline_holder> holder) {
    ob_error *error = NULL;

    holder->isStarted = false;
    ob_pipeline_stop(holder->pipeline, &error);

    if(error) {
        printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error), ob_error_args(error));
        printf("\tmessage: %s\n", ob_error_message(error));
        printf("\terror type: %d\n", ob_error_exception_type(error));
        ob_delete_error(error);
    }
    error = NULL;
}

void sample_frame_set_callback(ob_frame *frameset, void *user_data) {
    ob_error *error = NULL;

    auto pholder = reinterpret_cast<pipeline_holder *>(user_data);

    // Get the depth data frame
    ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
    check_error(error);
    if(depth_frame) {
        pholder->depthCount++;
        if(pholder->depthCount == 15) {
            printf("=====Depth Frame Info====== ");
            print_device_info(pholder->device_info);
            printf(", ");
            print_depth_frame(depth_frame);
            printf("\n");
            pholder->depthCount = 0;
        }

        // Destroy the depth data frame
        ob_delete_frame(depth_frame, &error);
        check_error(error);
    }

    // Get the ir data frame
    ob_frame *ir_frame = ob_frameset_ir_frame(frameset, &error);
    check_error(error);
    if(ir_frame) {
        pholder->irCount++;
        if(pholder->irCount == 15) {
            printf("=====IR Frame Info====== ");
            print_device_info(pholder->device_info);
            printf(", ");
            print_video_frame(ir_frame);
            printf("\n");
            pholder->irCount = 0;
        }

        // Destroy the ir data frame
        ob_delete_frame(ir_frame, &error);
        check_error(error);
    }

    // Get the ir left data frame
    ob_frame *ir_left_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_LEFT, &error);
    check_error(error);
    if(ir_left_frame) {
        pholder->irLeftCount++;
        if(pholder->irLeftCount == 15) {
            printf("=====IR Left Frame Info====== ");
            print_device_info(pholder->device_info);
            printf(", ");
            print_video_frame(ir_left_frame);
            printf("\n");
            pholder->irLeftCount = 0;
        }

        // Destroy the ir data frame
        ob_delete_frame(ir_left_frame, &error);
        check_error(error);
    }

    // Get the right ir data frame
    ob_frame *ir_right_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_RIGHT, &error);
    check_error(error);
    if(ir_right_frame) {
        pholder->irRightCount++;
        if(pholder->irRightCount == 15) {
            printf("=====IR Right Frame Info====== ");
            print_device_info(pholder->device_info);
            printf(", ");
            print_video_frame(ir_right_frame);
            printf("\n");
            pholder->irRightCount = 0;
        }

        // Destroy the ir data frame
        ob_delete_frame(ir_right_frame, &error);
        check_error(error);
    }

    // Get the color data frame
    ob_frame *color_frame = ob_frameset_color_frame(frameset, &error);
    check_error(error);
    if(color_frame) {
        pholder->colorCount++;
        if(pholder->colorCount == 15) {
            printf("=====Color Frame Info====== ");
            print_device_info(pholder->device_info);
            printf(", ");
            print_video_frame(color_frame);
            printf("\n");

            pholder->colorCount = 0;
        }

        // Destroy the color data frame
        ob_delete_frame(color_frame, &error);
        check_error(error);
    }

    // destroy frameset
    ob_delete_frame(frameset, &error);
    check_error(error);
}

void reboot_devices() {
    ob_error *error = NULL;
    for(auto itr = pipeline_holder_map.begin(); itr != pipeline_holder_map.end(); itr++) {
        ob_device_reboot(itr->second->device, &error);

        if(error) {
            printf("ob_error was raised: \n\tcall: %s(%s)\n", ob_error_function(error), ob_error_args(error));
            printf("\tmessage: %s\n", ob_error_message(error));
            printf("\terror type: %d\n", ob_error_exception_type(error));
            ob_delete_error(error);
        }
        error = NULL;
    }
}

void delete_holder(std::shared_ptr<pipeline_holder> holder) {
    ob_error *error = NULL;
    ob_delete_device_info(holder->device_info, &error);
    check_error(error);

    ob_delete_pipeline(holder->pipeline, &error);
    check_error(error);

    ob_delete_device(holder->device, &error);
    check_error(error);
}

std::string frameTypeToString(const OBFrameType frameType) {
    if(frameType == OB_FRAME_COLOR) {
        return "OB_FRAME_COLOR";
    }
    else if(frameType == OB_FRAME_IR) {
        return "OB_FRAME_IR";
    }
    else if(frameType == OB_FRAME_IR_LEFT) {
        return "OB_FRAME_IR_LEFT";
    }
    else if(frameType == OB_FRAME_IR_RIGHT) {
        return "OB_FRAME_IR_RIGHT";
    }
    else if(frameType == OB_FRAME_DEPTH) {
        return "OB_FRAME_DEPTH";
    }
    else if(frameType == OB_FRAME_ACCEL) {
        return "OB_FRAME_ACCEL";
    }
    else if(frameType == OB_FRAME_GYRO) {
        return "OB_FRAME_GYRO";
    }
    else {
        return std::to_string((int)frameType);
    }
}

void print_device_info(ob_device_info *device_info) {
    ob_error *error = NULL;
    int       vid   = ob_device_info_vid(device_info, &error);
    check_error(error);

    int pid = ob_device_info_pid(device_info, &error);
    check_error(error);

    const char *sn = ob_device_info_serial_number(device_info, &error);
    check_error(error);

    const char *uid = ob_device_info_uid(device_info, &error);
    check_error(error);

    printf("vid: %04x, pid: %04x, sn: %s, uid: %s", vid, pid, sn, uid);
}

void print_depth_frame(ob_frame *frame) {
    ob_error     *error = NULL;
    ob_frame_type type  = ob_frame_get_type(frame, &error);
    check_error(error);
    uint32_t index = ob_frame_index(frame, &error);
    check_error(error);
    uint32_t width = ob_video_frame_width(frame, &error);
    check_error(error);
    uint32_t height = ob_video_frame_height(frame, &error);
    check_error(error);
    ob_format format = ob_frame_format(frame, &error);
    check_error(error);
    uint64_t timestampUs = ob_frame_time_stamp_us(frame, &error);
    check_error(error);
    float scale = ob_depth_frame_get_value_scale(frame, &error);
    check_error(error);
    uint16_t *data = (uint16_t *)ob_frame_data(frame, &error);
    check_error(error);
    uint16_t pixelValue = *(data + width * height / 2 + width / 2);

    printf("FrameType:%s, index:%d, width:%d, height:%d, format:%d, timeStampUs:%" PRIu64 "us, centerDepth:%02fmm", frameTypeToString(type).c_str(), index,
           width, height, format, timestampUs, (float)pixelValue * scale);
}

void print_video_frame(ob_frame *frame) {
    ob_error     *error = NULL;
    ob_frame_type type  = ob_frame_get_type(frame, &error);
    check_error(error);
    uint32_t index = ob_frame_index(frame, &error);
    check_error(error);
    uint32_t width = ob_video_frame_width(frame, &error);
    check_error(error);
    uint32_t height = ob_video_frame_height(frame, &error);
    check_error(error);
    ob_format format = ob_frame_format(frame, &error);
    check_error(error);
    uint64_t timestampUs = ob_frame_time_stamp_us(frame, &error);
    check_error(error);

    printf("FrameType:%s, index:%d, width:%d, height:%d, format:%d, timeStampUs:%" PRIu64 "us", frameTypeToString(type).c_str(), index, width, height, format,
           timestampUs);
}
