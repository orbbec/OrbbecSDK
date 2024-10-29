// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

#include <mutex>
#include <thread>

bool enable_align_mode = 1;

// key press event processing
void handleKeyPress(ob_smpl::CVWindow &win, std::shared_ptr<ob::Pipeline> pipe, int key, std::shared_ptr<ob::Config> config) {
    if(key == 't' || key == 'T') {
        // Press the T key to switch align mode
        enable_align_mode = !enable_align_mode;

        // update the align mode in the config
        if(enable_align_mode) {
            config->setAlignMode(ALIGN_D2C_HW_MODE);
            win.addLog("Haeware Depth to Color Align: Enabled");
        }
        else {
            config->setAlignMode(ALIGN_DISABLE);
            win.addLog("Haeware Depth to Color Align: Disabled");
        }

        // restart the pipeline with the new config
        pipe->stop();
        pipe->start(config);
    }
}

// check if the given stream profiles support hardware depth-to-color alignment
bool checkIfSupportHWD2CAlign(std::shared_ptr<ob::Pipeline> pipe, std::shared_ptr<ob::StreamProfile> colorStreamProfile,
                              std::shared_ptr<ob::StreamProfile> depthStreamProfile) {
    auto hwD2CSupportedDepthStreamProfiles = pipe->getD2CDepthProfileList(colorStreamProfile, ALIGN_D2C_HW_MODE);
    if(hwD2CSupportedDepthStreamProfiles->count() == 0) {
        return false;
    }

    // Iterate through the supported depth stream profiles and check if there is a match with the given depth stream profile
    auto depthVsp = depthStreamProfile->as<ob::VideoStreamProfile>();
    auto count    = hwD2CSupportedDepthStreamProfiles->getCount();
    for(uint32_t i = 0; i < count; i++) {
        auto sp  = hwD2CSupportedDepthStreamProfiles->getProfile(i);
        auto vsp = sp->as<ob::VideoStreamProfile>();
        if(vsp->getWidth() == depthVsp->getWidth() && vsp->getHeight() == depthVsp->getHeight() && vsp->getFormat() == depthVsp->getFormat()
           && vsp->getFps() == depthVsp->getFps()) {
            // Found a matching depth stream profile, it is means the given stream profiles support hardware depth-to-color alignment
            return true;
        }
    }
    return false;
}

// create a config for hardware depth-to-color alignment
std::shared_ptr<ob::Config> createHwD2CAlignConfig(std::shared_ptr<ob::Pipeline> pipe) {
    auto coloStreamProfiles  = pipe->getStreamProfileList(OB_SENSOR_COLOR);
    auto depthStreamProfiles = pipe->getStreamProfileList(OB_SENSOR_DEPTH);

    // Iterate through all color and depth stream profiles to find a match for hardware depth-to-color alignment
    auto colorSpCount = coloStreamProfiles->getCount();
    auto depthSpCount = depthStreamProfiles->getCount();
    for(uint32_t i = 0; i < colorSpCount; i++) {
        auto colorProfile = coloStreamProfiles->getProfile(i);
        auto colorVsp     = colorProfile->as<ob::VideoStreamProfile>();
        for(uint32_t j = 0; j < depthSpCount; j++) {
            auto depthProfile = depthStreamProfiles->getProfile(j);
            auto depthVsp     = depthProfile->as<ob::VideoStreamProfile>();

            // make sure the color and depth stream have the same fps, due to some models may not support different fps
            if(colorVsp->getFps() != depthVsp->getFps()) {
                continue;
            }

            // Check if the given stream profiles support hardware depth-to-color alignment
            if(checkIfSupportHWD2CAlign(pipe, colorProfile, depthProfile)) {
                // If support, create a config for hardware depth-to-color alignment
                auto hwD2CAlignConfig = std::make_shared<ob::Config>();
                hwD2CAlignConfig->enableStream(colorProfile);                                                     // enable color stream
                hwD2CAlignConfig->enableStream(depthProfile);                                                     // enable depth stream
                hwD2CAlignConfig->setAlignMode(ALIGN_D2C_HW_MODE);                                                // enable hardware depth-to-color alignment
                hwD2CAlignConfig->setFrameAggregateOutputMode(OB_FRAME_AGGREGATE_OUTPUT_ALL_TYPE_FRAME_REQUIRE);  // output frameset with all types of frames
                return hwD2CAlignConfig;
            }
        }
    }
    return nullptr;
}

int main(void) try {
    // Create a pipeline with default device to manage stream
    auto pipe = std::make_shared<ob::Pipeline>();

    // enable frame sync inside the pipeline, which is synchronized by frame timestamp
    pipe->enableFrameSync();

    // Create a config for hardware depth-to-color alignment
    auto config = createHwD2CAlignConfig(pipe);
    if(config == nullptr) {
        std::cerr << "Current device does not support hardware depth-to-color alignment." << std::endl;
        return 1;
    }

    // Start the pipeline with config
    pipe->start(config);

    // Create a window for rendering and set the resolution of the window
    ob_smpl::CVWindow win("Hardware Depth to Color Align", 1280, 720, ob_smpl::ARRANGE_OVERLAY);
    // set key prompt
    win.setKeyPrompt("'T': Enable/Disable HwD2C, '+/-': Adjust Transparency");
    // set the callback function for the window to handle key press events
    win.setKeyPressedCallback([&](int key) { handleKeyPress(win, pipe, key, config); });

    while(win.run()) {
        // Wait for a frameset from the pipeline
        auto frameSet = pipe->waitForFrameset(100);
        if(frameSet == nullptr) {
            continue;
        }
        win.pushFramesToView(frameSet);
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
