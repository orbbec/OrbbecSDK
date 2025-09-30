#include "window.hpp"
#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include "utils.hpp"
#include <chrono>
#define ESC 27

int main(int argc, char **argv) try {

    // Create a pipeline with default device
    ob::Pipeline pipe;
    // Start the pipeline with default config, more info please refer to the `misc/config/OrbbecSDKConfig_v1.0.xml`
    pipe.start();

    auto lastTime = std::chrono::high_resolution_clock::now();

    while(true) {
        // Check for key press and break if it's ESC
        if(kbhit() && getch() == ESC) {
            break;
        }
        auto frameSet = pipe.waitForFrames();
        if(!frameSet) {
            continue;
        }
        auto colorFrame = frameSet->colorFrame();
        auto depthFrame = frameSet->depthFrame();
        auto now        = std::chrono::high_resolution_clock::now();
        // Print the frame data every second
        if(std::chrono::duration_cast<std::chrono::seconds>(now - lastTime).count() >= 1) {
            if(colorFrame) {
                std::cout << "Color Frame: " << colorFrame->width() << "x" << colorFrame->height() << " " << colorFrame->timeStampUs() << " us" << std::endl;
            }
            if(depthFrame) {
                std::cout << "Depth Frame: " << depthFrame->width() << "x" << depthFrame->height() << " " << depthFrame->timeStampUs() << " us" << std::endl;
            }
            lastTime = now;  // Reset the timer
        }
    }

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
