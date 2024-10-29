// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_types.h"

#include <mutex>
#include <iostream>

void printImuValue(OBFloat3D obFloat3d, uint64_t index, uint64_t timeStampUs, float temperature, OBFrameType type, const std::string &unitStr) {
    std::cout << "frame index: " <<index << std::endl;
    auto typeStr = ob::TypeHelper::convertOBFrameTypeToString(type);
    std::cout << type << " Frame: \n\r{\n\r"
              << "  tsp = " << timeStampUs << "\n\r"
              << "  temperature = " << temperature << "\n\r"
              << "  " << type << ".x = " << obFloat3d.x << unitStr << "\n\r"
              << "  " << type << ".y = " << obFloat3d.y << unitStr << "\n\r"
              << "  " << type << ".z = " << obFloat3d.z << unitStr << "\n\r"
              << "}\n\r" << std::endl;
}

int main() try {

    // Create a pipeline with default device.
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config.
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Enable Accel stream.
    config->enableAccelStream();

    // Enable Gyro stream.
    config->enableGyroStream();

    // Only FrameSet that contains all types of data frames will be output.
    config->setFrameAggregateOutputMode(OB_FRAME_AGGREGATE_OUTPUT_ALL_TYPE_FRAME_REQUIRE);

    // Start the pipeline with config.
    pipe.start(config);

    while(true) {
        auto key = ob_smpl::waitForKeyPressed(1);
        if(key == ESC_KEY) {  // Esc key to exit.
            break;
        }
        auto frameSet = pipe.waitForFrameset();
        if(frameSet == nullptr) {
            continue;
        }

        auto accelFrameRaw    = frameSet->getFrame(OB_FRAME_ACCEL);
        auto accelFrame       = accelFrameRaw->as<ob::AccelFrame>();
        auto accelIndex       = accelFrame->getIndex();
        auto accelTimeStampUs = accelFrame->getTimeStampUs();
        auto accelTemperature = accelFrame->getTemperature();
        auto accelType        = accelFrame->getType();
        if(accelIndex % 50 == 0) {  // print information every 50 frames.
            auto accelValue = accelFrame->getValue();
            printImuValue(accelValue, accelIndex, accelTimeStampUs, accelTemperature, accelType, "m/s^2");
        }

        auto gyroFrameRaw    = frameSet->getFrame(OB_FRAME_GYRO);
        auto gyroFrame       = gyroFrameRaw->as<ob::GyroFrame>();
        auto gyroIndex       = gyroFrame->getIndex();
        auto gyroTimeStampUs = gyroFrame->getTimeStampUs();
        auto gyroTemperature = gyroFrame->getTemperature();
        auto gyroType        = gyroFrame->getType();
        if(gyroIndex % 50 == 0) {  // print information every 50 frames.
            auto gyroValue = gyroFrame->getValue();
            printImuValue(gyroValue, gyroIndex, gyroTimeStampUs, gyroTemperature, gyroType, "rad/s");
        }
    }

    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}
