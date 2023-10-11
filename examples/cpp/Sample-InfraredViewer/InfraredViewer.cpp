#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"

int main(int argc, char **argv) try {
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Get all stream profiles of the infrared camera, including stream resolution, frame rate, and frame format
    // Please adjust the sensor according to the actual product, some device types only have OB_SENSOR_IR_LEFT and OB_SENSOR_IR_RIGHT.
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_IR);

    if(profiles == nullptr) {
        std::cerr
            << "The obtained IR resolution list is NULL. For binocular structured light devices, try using the doubleIr example to turn on the ir data stream. "
            << std::endl;
        return 0;
    }

    std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
    try {
        // Find the corresponding profile according to the specified format, first look for the y16 format
        irProfile = profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
    }
    catch(ob::Error &e) {
        // If the specified format is not found, search for the default profile to open the stream
        irProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }

    // By creating config to configure which streams to enable or disable for the pipeline, here the infrared stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(irProfile);

    // Start the pipeline with config
    pipe.start(config);

    // Create a window for rendering and set the resolution of the window
    Window app("InfraredViewer", irProfile->width(), irProfile->height());
    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        // Render a set of frame in the window, only the infrared frame is rendered here.
        // If the open stream type is not OB_SENSOR_IR, use the getFrame interface to get the frame.
        app.addToRender(frameSet->irFrame());
    }

    // Stop the pipeline, no frame data will be generated
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
