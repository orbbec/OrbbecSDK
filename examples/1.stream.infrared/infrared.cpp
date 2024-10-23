// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

std::map<OBSensorType, ob_stream_type> sensorStreamMap = {
    {OB_SENSOR_IR, OB_STREAM_IR},
    {OB_SENSOR_IR_LEFT, OB_STREAM_IR_LEFT},
    {OB_SENSOR_IR_RIGHT, OB_STREAM_IR_RIGHT}
};

int main() try {
    // Create a pipeline with default device.
    ob::Pipeline pipe;

    // Get the device from pipeline.
    std::shared_ptr<ob::Device> device = pipe.getDevice();

    // Get the sensor list from device.
    std::shared_ptr<ob::SensorList> sensorList = device->getSensorList();

    // Create a config for pipeline.
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    for(uint32_t index = 0; index < sensorList->getCount(); index++) {
        // Query all supported infrared sensor type and enable the infrared stream.
        // For dual infrared device, enable the left and right infrared streams.
        // For single infrared device, enable the infrared stream.
        OBSensorType sensorType = sensorList->getSensorType(index);
        if(sensorType == OB_SENSOR_IR || sensorType == OB_SENSOR_IR_LEFT || sensorType == OB_SENSOR_IR_RIGHT) {
            // Enable the stream with specified profile;
            config->enableVideoStream(sensorType, OB_WIDTH_ANY, OB_HEIGHT_ANY, 30, OB_FORMAT_ANY);
        }
    }

    pipe.start(config);

    // Create a window for rendering and set the resolution of the window
    ob_smpl::CVWindow win("Infrared", 1280, 720, ob_smpl::ARRANGE_ONE_ROW);
    while(win.run()) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrameset(100);
        if(frameSet == nullptr) {
            continue;
        }

        // Render a set of frame in the window.
        win.pushFramesToView(frameSet);
    }

    // Stop the pipeline, no frame data will be generated
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

