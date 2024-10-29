// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"

#include <mutex>
#include <thread>

bool    sync       = false;
uint8_t align_mode = 0;

int main(void) try {
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    auto config = std::make_shared<ob::Config>();

    // enable depth and color streams with specified format
    config->enableVideoStream(OB_STREAM_DEPTH, OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FPS_ANY, OB_FORMAT_Y16);
    config->enableVideoStream(OB_STREAM_COLOR, OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FPS_ANY, OB_FORMAT_RGB);

    // set the frame aggregate output mode to ensure all types of frames are included in the output frameset
    config->setFrameAggregateOutputMode(OB_FRAME_AGGREGATE_OUTPUT_ALL_TYPE_FRAME_REQUIRE);

    // Create a pipeline with default device to manage stream
    auto pipe = std::make_shared<ob::Pipeline>();

    // Start the pipeline with config
    pipe->start(config);

    // Create a filter to align depth frame to color frame
    auto depth2colorAlign = std::make_shared<ob::Align>(OB_STREAM_COLOR);

    // create a filter to align color frame to depth frame
    auto color2depthAlign = std::make_shared<ob::Align>(OB_STREAM_DEPTH);

    // Set the callback function for the Align Filter to display the aligned frames in the window
    depth2colorAlign->setCallBack([](std::shared_ptr<ob::Frame> frame) {
        auto frameset = frame->as<ob::FrameSet>();
        auto colorFrame = frameset->getFrame(OB_FRAME_COLOR);
        auto depthFrame = frameset->getFrame(OB_FRAME_DEPTH);
        auto index = colorFrame->getIndex();
        if(index%30 == 0){
            std::cout << "Depth to Color Aligned Frame: " << index << std::endl;
        }

    });
    color2depthAlign->setCallBack([](std::shared_ptr<ob::Frame> frame) {
        auto frameset = frame->as<ob::FrameSet>();
        auto colorFrame = frameset->getFrame(OB_FRAME_COLOR);
        auto depthFrame = frameset->getFrame(OB_FRAME_DEPTH);
        auto index = colorFrame->getIndex();
        if(index%30 == 0){
            std::cout << "Color to Depth Aligned Frame: " << index << std::endl;
        }
    });

    while(true) {
        auto key = ob_smpl::waitForKeyPressed(1);
        if(key == ESC_KEY){
            break;
        }
        else if(key == 'T' || key == 't'){
            align_mode = (align_mode + 1) % 2;
            std::cout << "Align mode: " << (align_mode? "Depth to Color" : "Color to Depth") << std::endl;
        }
        // Wait for a frameset from the pipeline
        auto frameSet = pipe->waitForFrameset(100);
        if(frameSet == nullptr) {
            continue;
        }

        // Get filter according to the align mode
        std::shared_ptr<ob::Filter> alignFilter = depth2colorAlign;
        if(align_mode % 2 == 1) {
            alignFilter = color2depthAlign;
        }

        // push the frameset to the Align Filter to align the frames.
        // The frameset will be processed in an internal thread, and the resulting frames will be asynchronously output via the callback function.
        alignFilter->pushFrame(frameSet);
    }

    // Stop the Pipeline, no frame data will be generated
    pipe->stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

