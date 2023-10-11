/*
Notes:
on the Arm platform/Linux ,this sample requires users to compile with Opencv4.2 or above,otherwise, it cannot be rendered.
*/
#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <mutex>
#include <thread>

int main(int argc, char **argv) try {

    // create frame resource
    std::mutex                 videoFrameMutex;
    std::shared_ptr<ob::Frame> colorFrame;
    std::shared_ptr<ob::Frame> depthFrame;
    std::shared_ptr<ob::Frame> irFrame;

    std::mutex                 accelFrameMutex;
    std::shared_ptr<ob::Frame> accelFrame;

    std::mutex                 gyroFrameMutex;
    std::shared_ptr<ob::Frame> gyroFrame;

    std::vector<std::shared_ptr<ob::Frame>> frames;

    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    try {
        auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
        auto colorProfile  = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(colorProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "color stream not found!" << std::endl;
    }

    try {
        auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
        auto depthProfile  = depthProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(depthProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "depth stream not found!" << std::endl;
    }

    try {
        auto irProfiles = pipe.getStreamProfileList(OB_SENSOR_IR);
        auto irProfile  = irProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "ir stream not found!" << std::endl;
    }

    // Configure the alignment mode as hardware D2C alignment
    config->setAlignMode(ALIGN_D2C_HW_MODE);

    // Start the pipeline with config

    pipe.start(config, [&](std::shared_ptr<ob::FrameSet> frameset) {
        std::unique_lock<std::mutex> lk(videoFrameMutex);
        colorFrame = frameset->colorFrame();
        depthFrame = frameset->depthFrame();
        irFrame    = frameset->irFrame();
    });

    auto                        dev = pipe.getDevice();
    std::shared_ptr<ob::Sensor> accelSensor;
    std::shared_ptr<ob::Sensor> gyroSensor;
    try {
        accelSensor = dev->getSensor(OB_SENSOR_ACCEL);
        gyroSensor  = dev->getSensor(OB_SENSOR_GYRO);
    }
    catch(...) {
        std::cout << "IMU sensor not found!" << std::endl;
    }
    if(accelSensor && gyroSensor) {
        auto accelProfiles = accelSensor->getStreamProfileList();
        auto accelProfile  = accelProfiles->getProfile(OB_PROFILE_DEFAULT);
        accelSensor->start(accelProfile, [&](std::shared_ptr<ob::Frame> frame) {
            std::unique_lock<std::mutex> lk(accelFrameMutex);
            accelFrame = frame;
        });

        auto gyroProfiles = gyroSensor->getStreamProfileList();
        auto gyroProfile  = gyroProfiles->getProfile(OB_PROFILE_DEFAULT);
        gyroSensor->start(gyroProfile, [&](std::shared_ptr<ob::Frame> frame) {
            std::unique_lock<std::mutex> lk(gyroFrameMutex);
            gyroFrame = frame;
        });
    }

    // Create a window for rendering and set the resolution of the window
    Window app("MultiStream", 1280, 720, RENDER_GRID);
    frames.resize(5);
    while(app) {
        {
            std::unique_lock<std::mutex> lock(videoFrameMutex);
            if(colorFrame) {
                frames.at(0) = colorFrame;
            }
            if(depthFrame) {
                frames.at(1) = depthFrame;
            }
            if(irFrame) {
                frames.at(2) = irFrame;
            }
        }
        {
            std::unique_lock<std::mutex> lk(accelFrameMutex);
            if(accelFrame) {
                frames.at(3) = accelFrame;
            }
        }
        {
            std::unique_lock<std::mutex> lk(gyroFrameMutex);
            if(gyroFrame) {
                frames.at(4) = gyroFrame;
            }
        }
        std::vector<std::shared_ptr<ob::Frame>> framesForRender;
        for(auto &frame: frames) {
            if(frame) {
                framesForRender.push_back(frame);
            }
        }
        app.addToRender(framesForRender);
    }

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();
    if(accelSensor && gyroSensor) {
        accelSensor->stop();
        gyroSensor->stop();
    }
    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
