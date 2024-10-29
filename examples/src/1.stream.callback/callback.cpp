// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

int main(void) try {

    // Create a pipeline.
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config.
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Get device from pipeline.
    auto device = pipe.getDevice();

    // Get sensorList from device.
    auto sensorList = device->getSensorList();

    for(uint32_t index = 0; index < sensorList->getCount(); index++) {
        // Query all supported infrared sensor type and enable the infrared stream.
        // For dual infrared device, enable the left and right infrared streams.
        // For single infrared device, enable the infrared stream.
        OBSensorType sensorType = sensorList->getSensorType(index);

        // exclude non-video sensor type
        if(!ob::TypeHelper::isVideoSensorType(sensorType)) {
            continue;
        }

        // Enable the stream for the sensor type.
        config->enableStream(sensorType);
    }

    std::mutex                    framesetMutex;
    std::shared_ptr<ob::FrameSet> frameset = nullptr;

    // Start the pipeline with callback.
    pipe.start(config, [&](std::shared_ptr<ob::FrameSet> output) {
        std::lock_guard<std::mutex> lock(framesetMutex);
        frameset = output;
    });

    // Create a window for rendering, and set the size of the window.
    ob_smpl::CVWindow win("Callback", 1280, 720, ob_smpl::ARRANGE_GRID);

    while(win.run()) {
        std::lock_guard<std::mutex> lock(framesetMutex);

        if(frameset == nullptr) {
            continue;
        }

        // Rendering display
        win.pushFramesToView(frameset);
    }

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

