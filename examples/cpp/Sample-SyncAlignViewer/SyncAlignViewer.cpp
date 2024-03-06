/**
 *Synchronous alignment example
 *
 * In this example, the mirror status of the depth and the color may be inconsistent because the depth or color sensor does not support mirroring.
 * As a result, the images displayed by the depth  and the color are reversed. If this happens, just set the mirror interface to keep the two mirror states
 * consistent.
 * In addition, the resolution obtained by some devices may not support the D2C function, so the resolutions that support D2C, please refer to the
 * product manual.
 * For example: the D2C resolution supported by DaBai DCW is 640x360, but the actual resolution obtained in this example may be 640x480, at this time the user
 * must refer to the product manual to select 640x360 resolution.
 */

#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <mutex>
#include <thread>

static bool  sync         = false;
static bool  started      = true;
static bool  hd2c         = false;
static bool  sd2c         = true;
static float alpha        = 0.5;
static int   windowWidth  = 0;
static int   windowHeight = 0;

// key press event processing
void keyEventProcess(Window &app, ob::Pipeline &pipe, std::shared_ptr<ob::Config> config) {
    ////Get the key value
    int key = app.waitKey(10);
    if(key == '+' || key == '=') {
        // Press the + key to increase alpha
        alpha += 0.1f;
        if(alpha >= 1.0f) {
            alpha = 1.0f;
        }
        app.setAlpha(alpha);
    }
    else if(key == '-' || key == '_') {
        // press - key to decrease alpha
        alpha -= 0.1f;
        if(alpha <= 0.0f) {
            alpha = 0.0f;
        }
        app.setAlpha(alpha);
    }
    else if(key == 'D' || key == 'd') {
        // Press the D key to switch the hardware D2C
        try {
            if(!hd2c) {
                started = false;
                pipe.stop();
                hd2c = true;
                sd2c = false;
                config->setAlignMode(ALIGN_D2C_HW_MODE);
                pipe.start(config);
                started = true;
            }
            else {
                started = false;
                pipe.stop();
                hd2c = false;
                sd2c = false;
                config->setAlignMode(ALIGN_DISABLE);
                pipe.start(config);
                started = true;
            }
        }
        catch(std::exception &e) {
            std::cerr << "Property not support" << std::endl;
        }
    }
    else if(key == 'S' || key == 's') {
        // Press the S key to switch the software D2C
        try {
            if(!sd2c) {
                started = false;
                pipe.stop();
                sd2c = true;
                hd2c = false;
                config->setAlignMode(ALIGN_D2C_SW_MODE);
                pipe.start(config);
                started = true;
            }
            else {
                started = false;
                pipe.stop();
                hd2c = false;
                sd2c = false;
                config->setAlignMode(ALIGN_DISABLE);
                pipe.start(config);
                started = true;
            }
        }
        catch(std::exception &e) {
            std::cerr << "Property not support" << std::endl;
        }
    }
    else if(key == 'F' || key == 'f') {
        // Press the F key to switch synchronization
        sync = !sync;
        if(sync) {
            try {
                // enable synchronization
                pipe.enableFrameSync();
            }
            catch(...) {
                std::cerr << "Sync not support" << std::endl;
            }
        }
        else {
            try {
                // turn off sync
                pipe.disableFrameSync();
            }
            catch(...) {
                std::cerr << "Sync not support" << std::endl;
            }
        }
    }
}

int main(int argc, char **argv) try {
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(colorProfile);
    }
    catch(...) {
        std::cerr << "Current device is not support color sensor!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);

    // Configure the alignment mode as hardware D2C alignment
    config->setAlignMode(ALIGN_D2C_HW_MODE);

    // Start the pipeline with config
    try {
        pipe.start(config);
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    }

    // Create a window for rendering and set the resolution of the window
    Window app("SyncAlignViewer", colorProfile->width(), colorProfile->height(), RENDER_OVERLAY);
    while(app) {
        keyEventProcess(app, pipe, config);

        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        auto colorFrame = frameSet->colorFrame();
        auto depthFrame = frameSet->depthFrame();
        if(colorFrame != nullptr && depthFrame != nullptr) {
            app.addToRender({ colorFrame, depthFrame });
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
