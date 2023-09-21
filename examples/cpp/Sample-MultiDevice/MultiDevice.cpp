/*
Notes:
on the Arm/Linux platform ,this sample requires users to compile with Opencv4.2 or above,otherwise, it cannot be rendered.
*/
#include "window.hpp"

#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <mutex>

const int maxDeviceCount = 9;

std::vector<std::shared_ptr<ob::Frame>> frames;
std::shared_ptr<ob::Frame>              colorFrames[maxDeviceCount];
std::shared_ptr<ob::Frame>              depthFrames[maxDeviceCount];
std::shared_ptr<ob::Frame>              irFrames[maxDeviceCount];
std::mutex                              frameMutex;

void StartStream(std::vector<std::shared_ptr<ob::Pipeline>> pipes);
void StopStream(std::vector<std::shared_ptr<ob::Pipeline>> pipes);

int main(int argc, char **argv) try {
    // Create a Context
    ob::Context ctx;

    // Query the list of connected devices
    auto devList = ctx.queryDeviceList();
    // Get the number of connected devices
    int devCount = devList->deviceCount();

    // traverse the device list and create a pipe
    std::vector<std::shared_ptr<ob::Pipeline>> pipes;
    for(int i = 0; i < devCount; i++) {
        // Get the device and create the pipeline
        auto dev  = devList->getDevice(i);
        auto pipe = std::make_shared<ob::Pipeline>(dev);
        pipes.push_back(pipe);
    }

    // Open depth and color streams for all devices
    StartStream(pipes);

    // Create a window for rendering and set the resolution of the window
    Window app("MultiDeviceViewer", 1280, 720, RENDER_GRID);

    while(app) {
        // Render a set of frame in the window, here will render the depth, color or infrared frames of all devices, RENDER_GRID
        // means that all frames will be rendered in a grid arrangement
        frames.clear();
        {
            std::lock_guard<std::mutex> lock(frameMutex);
            int                         i = 0;
            for(auto pipe: pipes) {
                if(colorFrames[i] != nullptr) {
                    frames.emplace_back(colorFrames[i]);
                }

                if(depthFrames[i] != nullptr) {
                    frames.emplace_back(depthFrames[i]);
                }

                if(irFrames[i] != nullptr) {
                    frames.emplace_back(irFrames[i]);
                }
                i++;
            }
        }

        app.addToRender(frames);
    }

    frames.clear();
    StopStream(pipes);

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

void StartStream(std::vector<std::shared_ptr<ob::Pipeline>> pipes) {
    int i = 0;
    for(auto &&pipe: pipes) {
        std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
        // Get the depth camera configuration list
        auto                                    depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
        std::shared_ptr<ob::VideoStreamProfile> depthProfile     = nullptr;
        if(depthProfileList) {
            // Open the default profile of Depth Sensor, which can be configured through the configuration file
            depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfileList->getProfile(0))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(depthProfile);

        // Get the color camera configuration list
        try {
            auto                                    colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
            std::shared_ptr<ob::VideoStreamProfile> colorProfile     = nullptr;
            if(colorProfileList) {
                // Open the default profile of Color Sensor, which can be configured through the configuration file
                colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfileList->getProfile(0))->as<ob::VideoStreamProfile>();
            }
            config->enableStream(colorProfile);
        }
        catch(ob::Error &e) {
            std::cerr << "Current device is not support color sensor!" << std::endl;
        }

        // Start the pipeline and pass in the configuration
        pipe->start(config, [i](std::shared_ptr<ob::FrameSet> frameSet) {
            std::lock_guard<std::mutex> lock(frameMutex);
            if(frameSet->colorFrame()) {
                colorFrames[i] = frameSet->colorFrame();
            }
            if(frameSet->depthFrame()) {
                depthFrames[i] = frameSet->depthFrame();
            }
        });
        i++;
    }
}

void StopStream(std::vector<std::shared_ptr<ob::Pipeline>> pipes) {
    int i = 0;
    for(auto &&pipe: pipes) {
        if(colorFrames[i])
            colorFrames->reset();
        if(depthFrames[i])
            depthFrames->reset();
        if(irFrames[i])
            irFrames->reset();
        // stop the pipeline
        pipe->stop();
        i++;
    }
}
