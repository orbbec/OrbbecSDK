// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

int main(void) try {
    // Create a pipeline.
    ob::Pipeline pipe;

    // Start the pipeline with default config.
    // Modify the default configuration by the configuration file: "OrbbecSDKConfig.xml"
    pipe.start();

    // Create a window for showing the frames, and set the size of the window.
    ob_smpl::CVWindow win("QuickStart", 1280, 720, ob_smpl::ARRANGE_ONE_ROW);

    while(win.run()) {
        // Wait for frameSet from the pipeline, the default timeout is 1000ms.
        auto frameSet = pipe.waitForFrameset();

        // Push the frames to the window for showing.
        win.pushFramesToView(frameSet);
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

