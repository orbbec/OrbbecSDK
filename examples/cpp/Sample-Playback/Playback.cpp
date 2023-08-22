// #include "./RecorderPlaybackWindow.hpp"
#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include "libobsensor/hpp/RecordPlayback.hpp"
#include <thread>
#include <mutex>

#define LOW_API 0

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

    // Read the internal reference information from the playback file
    auto cameraParam = pipe.getCameraParam();
    std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
              << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;

    // Start playback with default config(`NULL passed`)
    pipe.start(NULL);
    Window app("Playback", 640, 480);

    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }
        app.addToRender(frameSet->depthFrame());
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
