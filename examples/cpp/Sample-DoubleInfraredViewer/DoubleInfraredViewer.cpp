#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"

int main(int argc, char **argv) try {
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Get the ir_left camera configuration list
    auto irLeftProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);

    if(irLeftProfiles == nullptr) {
        std::cerr
            << "The obtained IR_Left resolution list is NULL. For monocular structured light devices, try opening the IR data stream using the IR example. "
            << std::endl;
        return 0;
    }

    // Open the default profile of IR_LEFT Sensor, which can be configured through the configuration file
    try {
        auto irLeftProfile = irLeftProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irLeftProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "IR_Left stream not found!" << std::endl;
    }

    // Get the ir_right camera configuration list
    auto irRightProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_RIGHT);

    // Open the default profile of IR_RIGHT Sensor, which can be configured through the configuration file
    try {
        auto irRightProfile = irRightProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irRightProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "IR_Right stream not found!" << std::endl;
    }

    // Start the pipeline with config
    pipe.start(config);

    // Create a window for rendering and set the resolution of the window
    Window app("DoubleInfraredViewer", 1280, 800, RENDER_ONE_ROW);

    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        // Get the data of left and right IR
        auto leftFrame  = frameSet->getFrame(OB_FRAME_IR_LEFT);
        auto rightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
        if(leftFrame == nullptr || rightFrame == nullptr) {
            std::cout << "left ir frame or right ir frame is null. leftFrame: " << leftFrame << ", rightFrame: " << rightFrame << std::endl;
            continue;
        }

        // Render a set of frame in the window, only the infrared frame is rendered here, but it must also be passed in as an array.
        app.addToRender({ leftFrame, rightFrame });
    }

    // Stop the pipeline, no frame data will be generated
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
