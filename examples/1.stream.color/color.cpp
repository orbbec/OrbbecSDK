// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

int main(void) try {

    // Create a pipeline with default device.
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config.
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Enable color video stream.
    config->enableVideoStream(OB_STREAM_COLOR);

    // Start the pipeline with config.
    pipe.start(config);

    // Create a window for rendering and set the resolution of the window.
    ob_smpl::CVWindow win("Color");

    while(win.run()) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrameset();
        if(frameSet == nullptr) {
            continue;
        }

        // get color frame from frameset.
        auto colorFrame = frameSet->getFrame(OB_FRAME_COLOR);
        // Render colorFrame.
        win.pushFramesToView(colorFrame);
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

