// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

#include <mutex>

std::map<int, std::shared_ptr<const ob::Frame>> framesets;
std::mutex                                      framesetMutex;

void startStream(std::map<int, std::shared_ptr<ob::Pipeline>> &pipes) {

    for(auto &item: pipes) {
        int   deviceIndex = item.first;
        auto &pipe        = item.second;

        // config to enable depth and color streams
        std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
        config->enableVideoStream(OB_STREAM_COLOR);
        config->enableVideoStream(OB_STREAM_DEPTH);

        // start pipeline and pass the callback function to receive the frames
        pipe->start(config, [deviceIndex](std::shared_ptr<ob::FrameSet> frameSet) {
            std::lock_guard<std::mutex> lock(framesetMutex);
            framesets[deviceIndex] = frameSet;
        });
    }
}

void stopStream(std::map<int, std::shared_ptr<ob::Pipeline>> &pipes) {
    for(auto &item: pipes) {
        auto &pipe = item.second;
        // stop the pipeline
        pipe->stop();
    }

    std::lock_guard<std::mutex> lock(framesetMutex);
    framesets.clear();
}

int main() try {
    // Create a Context
    ob::Context ctx;

    // Query the list of connected devices
    auto devList = ctx.queryDeviceList();

    // Get the number of connected devices
    int devCount = devList->getCount();

    // Create a pipeline for each device
    std::map<int, std::shared_ptr<ob::Pipeline>> pipes;
    for(int i = 0; i < devCount; i++) {
        // Get the device from device list
        auto dev  = devList->getDevice(i);

        // Create a pipeline for the device
        auto pipe = std::make_shared<ob::Pipeline>(dev);

        // Add the pipeline to the map of pipelines
        pipes.insert({ i, pipe });
    }

    // Start the depth and color streams for all devices
    startStream(pipes);

    // Create a window for rendering and set the resolution of the window
    ob_smpl::CVWindow win("MultiDevice", 1280, 720, ob_smpl::ARRANGE_GRID);

    // Main loop to show the frames, press `ESC` to exit
    while(win.run()) {
        // Get the latest frames from all devices
        for(auto &item: framesets) {
            std::lock_guard<std::mutex> lock(framesetMutex);
            auto                        deviceIndex = item.first;
            auto                       &frameset    = item.second;

            // push the frames to the window for show
            win.pushFramesToView(frameset, deviceIndex);
        }
    }

    // Stop all streams and clear the framesets
    stopStream(pipes);

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

