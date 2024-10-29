// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

#include <mutex>
#include <thread>

bool    sync       = false;
uint8_t align_mode = 0;

// key press event processing
void handleKeyPress(ob_smpl::CVWindow &win, std::shared_ptr<ob::Pipeline> pipe, int key) {
    ////Get the key value
    if(key == 'F' || key == 'f') {
        // Press the F key to switch synchronization
        sync = !sync;
        if(sync) {
            // enable frame sync inside the pipeline, which is synchronized by frame timestamp
            pipe->enableFrameSync();
        }
        else {
            // turn off sync
            pipe->disableFrameSync();
        }
        win.addLog("Sync: " + std::string(sync ? "On" : "Off"));
    }
    else if(key == 't' || key == 'T') {
        // Press the T key to switch align mode
        align_mode = (align_mode + 1) % 2;
        win.addLog("Align Mode: " + std::string(align_mode == 0 ? "Depth to Color" : "Color to Depth"));
    }
}

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

    // Create a window for rendering and set the resolution of the window
    ob_smpl::CVWindow win("Sync&Align", 1280, 720, ob_smpl::ARRANGE_OVERLAY);
    // set key prompt
    win.setKeyPrompt("'T': Switch Align Mode, 'F': Toggle Synchronization, '+/-': Adjust Transparency");
    // set the callback function for the window to handle key press events
    win.setKeyPressedCallback([&win, pipe](int key) { handleKeyPress(win, pipe, key); });

    // Create a filter to align depth frame to color frame
    auto depth2colorAlign = std::make_shared<ob::Align>(OB_STREAM_COLOR);

    // create a filter to align color frame to depth frame
    auto color2depthAlign = std::make_shared<ob::Align>(OB_STREAM_DEPTH);

    // Set the callback function for the Align Filter to display the aligned frames in the window
    depth2colorAlign->setCallBack([&win](std::shared_ptr<ob::Frame> frame) { win.pushFramesToView(frame); });
    color2depthAlign->setCallBack([&win](std::shared_ptr<ob::Frame> frame) { win.pushFramesToView(frame); });

    while(win.run()) {
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

