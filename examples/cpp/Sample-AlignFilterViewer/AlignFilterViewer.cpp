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

static bool  sync  = false;
static float alpha = 0.5;

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

    // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
    auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    // Get the default color profile
    auto colorProfile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
    // Enable the color stream in the config
    config->enableStream(colorProfile);

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    // Get the default depth profile
    auto depthProfile = depthProfiles->getProfile(OB_PROFILE_DEFAULT);
    // Enable the depth stream in the config
    config->enableStream(depthProfile);

    /* Config depth align to color or color align to depth.
    OBStreamType align_to_stream = OB_STREAM_DEPTH; */
    OBStreamType align_to_stream = OB_STREAM_COLOR;
    ob::Align    align(align_to_stream);

    // Start the pipeline with config
    try {
        pipe.start(config);
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    }

    // Create a window for rendering and set the resolution of the window
    auto colorVideoProfile = colorProfile->as<ob::VideoStreamProfile>();
    Window app("AlignViewer", colorVideoProfile->width(), colorVideoProfile->height(), RENDER_OVERLAY);

    while(app) {
        keyEventProcess(app, pipe, config);

        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        auto colorFrame = frameSet->colorFrame();
        auto depthFrame = frameSet->depthFrame();

        if(colorFrame != nullptr && depthFrame != nullptr) {
            auto newFrame    = align.process(frameSet);
            auto newFrameSet = newFrame->as<ob::FrameSet>();
            colorFrame       = newFrameSet->colorFrame();
            depthFrame       = newFrameSet->depthFrame();

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
