// #include "./RecorderPlaybackWindow.hpp"
#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include "libobsensor/hpp/RecordPlayback.hpp"
#include <thread>
#include <mutex>

/**
 * Low API vs Pipeline:
 *   Low API: It is a basic playback api, user can use it to playback a file.
 *   Pipeline: It is a high level api with more features (such as frames synchronization, depth align to color, etc. It is same to use pipeline to capture
 * frames form device and enable those features).
 */
#define LOW_API 0  // 1: use low api (basic playback api), 0: use pipeline

uint64_t lastFrameTimestamp = 0;

int main(int argc, char **argv) try {
#if LOW_API
    // Use the playback file to create a playback object
    ob::Playback playback("./Orbbec.bag");

    // Set playback status callback
    playback.setPlaybackStateCallback([&](OBMediaState state) {
        if(state == OB_MEDIA_BEGIN) {
            std::cout << "Playback file begin." << std::endl;
        }
        else if(state == OB_MEDIA_END) {
            std::cout << "Playback file end." << std::endl;
        }
    });

    // Read device information from the playback file
    auto deviceInfo = playback.getDeviceInfo();
    std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
              << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;

    // Read the internal reference information from the playback file
    auto cameraParam = playback.getCameraParam();
    std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
              << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;

    Window app("Playback", 1280, 480);

    // frame data callback
    ob::PlaybackCallback callback = ([&](std::shared_ptr<ob::Frame> frame, void *pCookies) {
        auto currentTimestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        lastFrameTimestamp    = currentTimestamp;
        app.render({ frame }, RENDER_ONE_ROW);
    });

    // start playback
    playback.start(callback);

    while(app) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // stop playback
    playback.stop();
#else
    // Using pipeline to playback, you can get more features (such as frames synchronization, depth align to color, etc.)
    // Create a pipeline object for playback
    ob::Pipeline pipe("./OrbbecPipeline.bag");

    // Get the playback object and set the playback status callback
    auto playback = pipe.getPlayback();
    playback->setPlaybackStateCallback([&](OBMediaState state) {
        if(state == OB_MEDIA_BEGIN) {
            std::cout << "Playback file begin." << std::endl;
        }
        else if(state == OB_MEDIA_END) {
            std::cout << "Playback file end." << std::endl;
        }
    });

    // Read device information from the playback file
    auto deviceInfo = playback->getDeviceInfo();
    std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
              << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;

    // Read the camera parameters from the playback file
    auto cameraParam = pipe.getCameraParam();
    std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
              << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;

    // Get the depth stream profile, if the count of stream profile list is 0, it means that the playback file does not contain this type of stream.
    auto depthStreamProfileList = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    if(depthStreamProfileList->count() > 0) {
        auto depthStreamProfile = depthStreamProfileList->getProfile(OB_PROFILE_DEFAULT)->as<ob::VideoStreamProfile>();
        std::cout << "======================Depth stream : width:" << depthStreamProfile->width() << " height: " << depthStreamProfile->height()
                  << " fps: " << depthStreamProfile->fps() << " format: " << depthStreamProfile->format() << std::endl;
    }

    // Get the color stream profile
    auto colorStreamProfileList = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    if(colorStreamProfileList->count() > 0) {
        auto colorStreamProfile = colorStreamProfileList->getProfile(OB_PROFILE_DEFAULT)->as<ob::VideoStreamProfile>();
        std::cout << "======================Color stream : width:" << colorStreamProfile->width() << " height: " << colorStreamProfile->height()
                  << " fps: " << colorStreamProfile->fps() << " format: " << colorStreamProfile->format() << std::endl;
    }

    // Get the ir stream profile
    auto irStreamProfileList = pipe.getStreamProfileList(OB_SENSOR_IR);
    if(irStreamProfileList->count() > 0) {
        auto irStreamProfile = irStreamProfileList->getProfile(OB_PROFILE_DEFAULT)->as<ob::VideoStreamProfile>();
        std::cout << "======================IR stream : width:" << irStreamProfile->width() << " height: " << irStreamProfile->height()
                  << " fps: " << irStreamProfile->fps() << " format: " << irStreamProfile->format() << std::endl;
    }

    // Start playback by pass NULL to playback all contained streams
    pipe.start(NULL);
    Window app("Playback", 640, 480, RENDER_ONE_ROW);

    std::vector<std::shared_ptr<ob::Frame>> framesForRender;

    while(app) {
        framesForRender.clear();
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameset = pipe.waitForFrames(100);
        if(frameset == nullptr) {
            continue;
        }

        // try to get depth frame from frameset. If this type of frame is alway null, it means that the playback file does not contain this type of stream.
        std::shared_ptr<ob::Frame> frame = frameset->depthFrame();
        if(frame != nullptr) {
            // if frame is not null, add it to render
            framesForRender.push_back(frame);
        }

        // same as depthFrame
        frame = frameset->colorFrame();
        if(frame != nullptr) {
            framesForRender.push_back(frame);
        }

        // same as depthFrame
        frame = frameset->irFrame();
        if(frame != nullptr) {
            framesForRender.push_back(frame);
        }

        // add frames to render
        if(!framesForRender.empty()) {
            app.resize(640 * framesForRender.size(), 480);  // resize window to fit frames
            app.addToRender(framesForRender);
        }
    }

    // stop playback
    pipe.stop();
#endif

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
