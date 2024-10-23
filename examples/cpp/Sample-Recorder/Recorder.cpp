// #include "./RecorderPlaybackWindow.hpp"
#if 0
#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include "libobsensor/hpp/RecordPlayback.hpp"
#include "libobsensor/hpp/Filter.hpp"

#define LOW_API 0

int main(int argc, char **argv) try {

#if LOW_API
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Get the corresponding stream configuration
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    // Find the corresponding Profile according to the specified format, and look for the Y16 format first
    auto depthProfile = profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
    // After the Y16 format is not found, the format does not match and the corresponding Profile is searched for open stream
    if(!depthProfile) {
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }

    // Create a Config to configure which streams to enable or disable for the Pipeline, here the depth stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(depthProfile);

    // Start the pipeline with config
    pipe.start(config);

    // create recorder object
    ob::Recorder recorder(pipe.getDevice());
    recorder.start("./Orbbec.bag");
    // Create a window for rendering and set the resolution of the window
    Window app("Recorder", 1280, 480);

    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode. .
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        // Write frame data to the recorder object
        recorder.write(frameSet);
        std::vector<std::shared_ptr<ob::Frame>> frames;
        if(frameSet->depthFrame()) {
            frames.push_back(frameSet->depthFrame());
        }
        app.render(frames, RENDER_ONE_ROW);
    }

    // stop the recorder
    recorder.stop();
    // Stop the Pipeline, no frame data will be generated
    pipe.stop();
#else
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Get depth stream configuration
    auto                                    profiles     = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
    if(profiles) {
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }

    // Create a Config to configure which streams to enable for the Pipeline, here the depth stream will be enabled
    // User can enable other type of stream, all streams that has been enabled will be recorded
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(depthProfile);

    // Start the pipeline with config
    pipe.start(config);

    // Start record
    pipe.startRecord("./OrbbecPipeline.bag");

    // Create a window for rendering and set the resolution of the window
    Window app("Recorder", 640, 480);
    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }
        auto depthFrame = frameSet->depthFrame();
        app.addToRender(depthFrame);
    }

    pipe.stopRecord();
    // Stop the Pipeline, no frame data will be generated
    pipe.stop();
#endif

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
#endif
int main()
{
    return 0;
}