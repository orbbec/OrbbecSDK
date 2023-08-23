#include "libobsensor/ObSensor.hpp"
#include "utils.hpp"
#include <iostream>
#include <thread>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <iomanip>
#include <mutex>

#define ESC 27

typedef struct PipelineHolder_t {
    std::shared_ptr<ob::Pipeline>   pipeline;
    std::shared_ptr<ob::DeviceInfo> deviceInfo;
    bool                            isStarted;
} PipelineHolder;

typedef struct FramePrintInfo_t {
    uint32_t colorCount;
    uint32_t irCount;
    uint32_t irLeftCount;
    uint32_t irRightCount;
    uint32_t depthCount;
} FramePrintInfo;

std::recursive_mutex                                   pipelineHolderMutex;
std::map<std::string, std::shared_ptr<PipelineHolder>> pipelineHolderMap;

std::ostream &operator<<(std::ostream &os, const OBFrameType frameType);
std::ostream &operator<<(std::ostream &os, std::shared_ptr<ob::DeviceInfo> deviceInfo);
std::ostream &operator<<(std::ostream &os, std::shared_ptr<ob::DepthFrame> frame);
std::ostream &operator<<(std::ostream &os, std::shared_ptr<ob::VideoFrame> frame);

void handleDeviceConnected(std::shared_ptr<ob::DeviceList> connectList);
void handleDeviceDisconnected(std::shared_ptr<ob::DeviceList> disconnectList);
void rebootDevices();
void startStream(std::shared_ptr<PipelineHolder> holder);
void stopStream(std::shared_ptr<PipelineHolder> holder);

int main(int argc, char **argv) try {

    // create context
    ob::Context ctx;

    // register device callback
    ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> addedList) {
        handleDeviceDisconnected(removedList);
        handleDeviceConnected(addedList);
    });

    // handle current connected devices.
    handleDeviceConnected(ctx.queryDeviceList());

    while(true) {
        if(kbhit()) {
            int key = getch();

            // Press the esc key to exit
            if(key == ESC) {
                break;
            }

            // Press the r key to reboot the device to trigger the device disconnect and reconnect event, or manually unplug and plugin the device.
            if(key == 'r' || key == 'R') {
                rebootDevices();
            }
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    // Free resource
    {
        std::lock_guard<std::recursive_mutex> lk(pipelineHolderMutex);
        for(auto itr = pipelineHolderMap.begin(); itr != pipelineHolderMap.end(); itr++) {
            stopStream(itr->second);
        }
        pipelineHolderMap.clear();
    }

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

// Device connection callback
void handleDeviceConnected(std::shared_ptr<ob::DeviceList> connectList) {
    std::lock_guard<std::recursive_mutex> lk(pipelineHolderMutex);
    const auto                            deviceCount = connectList->deviceCount();
    std::cout << "Device connect, deviceCount: " << deviceCount << std::endl;
    for(uint32_t i = 0; i < deviceCount; i++) {
        auto uid = std::string(connectList->uid(i));
        auto itr = pipelineHolderMap.find(uid);
        if(itr != pipelineHolderMap.end()) {
            std::cout << "Device connected. device already exit. " << itr->second->deviceInfo << std::endl;
        }
        else {
            auto device     = connectList->getDevice(i);
            auto deviceInfo = device->getDeviceInfo();

            std::shared_ptr<ob::Pipeline>   pipeline(new ob::Pipeline(device));
            std::shared_ptr<PipelineHolder> holder(new PipelineHolder{ pipeline, deviceInfo, false });
            pipelineHolderMap.insert({ uid, holder });

            std::cout << "Device connected. " << deviceInfo << std::endl;
            startStream(holder);
        }
    }
}

// Device disconnect callback
void handleDeviceDisconnected(std::shared_ptr<ob::DeviceList> disconnectList) {
    std::lock_guard<std::recursive_mutex> lk(pipelineHolderMutex);
    const auto                            deviceCount = disconnectList->deviceCount();
    std::cout << "Device disconnect, deviceCount: " << deviceCount << std::endl;
    for(uint32_t i = 0; i < deviceCount; i++) {
        auto uid = std::string(disconnectList->uid(i));
        auto itr = pipelineHolderMap.find(uid);
        if(itr != pipelineHolderMap.end()) {
            auto deviceInfo = itr->second->deviceInfo;
            stopStream(itr->second);
            pipelineHolderMap.erase(uid);
            std::cout << "Device disconnected. " << deviceInfo << std::endl;
        }
        else {
            std::cout << "Device disconnect, unresolve deviceUid: " << uid << std::endl;
        }
    }
}

void rebootDevices() {
    std::cout << "Reboot devices, count: " << pipelineHolderMap.size() << std::endl;
    for(auto itr = pipelineHolderMap.begin(); itr != pipelineHolderMap.end(); itr++) {
        try {
            std::cout << "Reboot device, " << itr->second->deviceInfo << std::endl;
            stopStream(itr->second);
            itr->second->pipeline->getDevice()->reboot();
        }
        catch(ob::Error &e) {
            std::cerr << "Reboot device failed. function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage()
                      << "\ntype:" << e.getExceptionType() << std::endl;
        }
    }
}

void startStream(std::shared_ptr<PipelineHolder> holder) {
    std::shared_ptr<FramePrintInfo> printInfo(new FramePrintInfo{});
    std::string                     deviceSN = std::string(holder->deviceInfo->serialNumber());
    ob::FrameSetCallback            callback = [deviceSN, printInfo](std::shared_ptr<ob::FrameSet> frameSet) {
        // Get the depth data frame
        auto depthFrame = frameSet->depthFrame();
        if(depthFrame) {
            printInfo->depthCount++;
            if(printInfo->depthCount == 15) {
                std::cout << "=====Depth Frame Info====== SN: " << std::string(deviceSN) << ", " << depthFrame << std::endl;
                printInfo->depthCount = 0;
            }
        }

        // Get the ir data frame
        auto irFrame = frameSet->irFrame();
        if(irFrame) {
            printInfo->irCount++;
            if(printInfo->irCount == 15) {
                std::cout << "=====IR Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(irFrame)
                          << std::endl;
                printInfo->irCount = 0;
            }
        }

        // Get the ir left data frame
        auto irLeftFrame = frameSet->getFrame(OB_FRAME_IR_LEFT);
        if(irLeftFrame) {
            printInfo->irLeftCount++;
            if(printInfo->irLeftCount == 15) {
                std::cout << "=====IR Left Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(irLeftFrame)
                          << std::endl;
                printInfo->irLeftCount = 0;
            }
        }

        // Get the ir right data frame
        auto irRightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
        if(irRightFrame) {
            printInfo->irRightCount++;
            if(printInfo->irRightCount == 15) {
                std::cout << "=====IR Right Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(irRightFrame)
                          << std::endl;
                printInfo->irRightCount = 0;
            }
        }

        // Get the color data frame
        auto colorFrame = frameSet->colorFrame();
        if(colorFrame) {
            printInfo->colorCount++;
            if(printInfo->colorCount == 15) {
                std::cout << "=====Color Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(colorFrame)
                          << std::endl;
                printInfo->colorCount = 0;
            }
        }
    };

    // Start video stream according to the stream profile of the configuration file.If there is no configuration file, the first stream profile will be used.
    try {
        std::cout << "startStream " << holder->deviceInfo << std::endl;
        holder->pipeline->start(nullptr, callback);
        holder->isStarted = true;
    }
    catch(...) {
        std::cout << "Pipeline start failed!" << std::endl;
        holder->isStarted = false;
    }
}

void stopStream(std::shared_ptr<PipelineHolder> holder) {
    if(!holder->isStarted) {
        return;
    }

    try {
        std::cout << "stopStream " << holder->deviceInfo << std::endl;
        holder->isStarted = false;
        holder->pipeline->stop();
    }
    catch(ob::Error &e) {
        std::cerr << "stopStream failed., function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage()
                  << "\ntype:" << e.getExceptionType() << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const OBFrameType frameType) {
    if(frameType == OB_FRAME_COLOR) {
        os << "OB_FRAME_COLOR";
    }
    else if(frameType == OB_FRAME_IR) {
        os << "OB_FRAME_IR";
    }
    else if(frameType == OB_FRAME_IR_LEFT) {
        os << "OB_FRAME_IR_LEFT";
    }
    else if(frameType == OB_FRAME_IR_RIGHT) {
        os << "OB_FRAME_IR_RIGHT";
    }
    else if(frameType == OB_FRAME_DEPTH) {
        os << "OB_FRAME_DEPTH";
    }
    else if(frameType == OB_FRAME_ACCEL) {
        os << "OB_FRAME_ACCEL";
    }
    else if(frameType == OB_FRAME_GYRO) {
        os << "OB_FRAME_GYRO";
    }
    else {
        os << (int)frameType;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, std::shared_ptr<ob::DeviceInfo> deviceInfo) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << std::hex << "vid: 0x" << deviceInfo->vid() << ", pid: 0x" << deviceInfo->pid();
    os << oss.str() << ", sn: " << std::string(deviceInfo->serialNumber()) << ", uid: " << std::string(deviceInfo->uid());
    return os;
}

std::ostream &operator<<(std::ostream &os, std::shared_ptr<ob::DepthFrame> frame) {
    uint16_t *data       = (uint16_t *)frame->data();
    auto      width      = frame->width();
    auto      height     = frame->height();
    auto      scale      = frame->getValueScale();
    uint16_t  pixelValue = *(data + width * height / 2 + width / 2);
    os << "FrameType: " << frame->type() << ", index: " << frame->index() << ", width: " << width << ", height: " << height << ", format: " << frame->format()
       << ", timeStampUs: " << frame->timeStampUs() << "us, centerDepth: " << (int)(pixelValue * scale) << "mm";
    return os;
}

std::ostream &operator<<(std::ostream &os, std::shared_ptr<ob::VideoFrame> frame) {
    os << "FrameType: " << frame->type() << ", index: " << frame->index() << ", width: " << frame->width() << ", height: " << frame->height()
       << ", format: " << frame->format() << ", timeStampUs: " << frame->timeStampUs() << "us";
    return os;
}