// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"

#include <mutex>
#include <string>

const std::map<std::string, int> gemini_330_list = { { "gemini335", 0x0800 },  { "gemini335L", 0x0804 }, { "gemini336", 0x0803 },
                                                     { "gemini336L", 0x0807 }, { "gemini330", 0x0801 },  { "gemini330L", 0x0805 } };

bool isGemini330Series(int pid) {
    bool find = false;
    for(auto it = gemini_330_list.begin(); it != gemini_330_list.end(); ++it) {
        if(it->second == pid) {
            find = true;
            break;
        }
    }
    return find;
}

std::shared_ptr<ob_smpl::CVWindow> win;
std::shared_ptr<ob::Context>       ctx;

std::shared_ptr<ob::Device>   device;
std::shared_ptr<ob::Pipeline> pipeline;
std::recursive_mutex          deviceMutex;

std::thread inputWatchThread;

bool streamStarted = false;

std::map<OBSensorType, std::shared_ptr<ob::VideoStreamProfile>> profilesMap;
std::shared_ptr<ob::VideoStreamProfile>                         depthProfile = nullptr;
std::shared_ptr<ob::VideoStreamProfile>                         irProfile    = nullptr;

auto align = ob::FilterFactory::createFilter("Align");

void handleDeviceConnected(std::shared_ptr<ob::DeviceList> connectList);
void handleDeviceDisconnected(std::shared_ptr<ob::DeviceList> disconnectList);
void switchDepthWorkMode();
void turnOffHwD2d();
void setDepthUnit();
void setDepthSoftFilter();

void printUsage();
void commandProcess(std::string cmd);

void handleFrameset(std::shared_ptr<ob::FrameSet> frameset);
void startStream();

void inputWatcher();

int main(void) try {

    // create window for render
    win = std::make_shared<ob_smpl::CVWindow>("CommonUsages", 1280, 720, ob_smpl::ARRANGE_GRID);

    // Set log severity. disable log, please set OB_LOG_SEVERITY_OFF.
    ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_ERROR);

    // Create ob:Context.
    ctx = std::make_shared<ob::Context>();

    // Register device callback
    ctx->setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> addedList) {
        handleDeviceDisconnected(removedList);
        handleDeviceConnected(addedList);
    });

    // Query the list of connected devices.
    std::shared_ptr<ob::DeviceList> devices = ctx->queryDeviceList();

    // Handle connected devices（and open one device）
    handleDeviceConnected(devices);

    if(!device) {
        std::cout << "Waiting for connect device...";
        while(!device) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    printUsage();

    inputWatchThread = std::thread(inputWatcher);
    inputWatchThread.detach();

    while(win->run()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    if(pipeline) {
        pipeline->stop();
    }
    return 0;
}

catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

// Device connection callback
void handleDeviceConnected(std::shared_ptr<ob::DeviceList> devices) {
    // Get the number of connected devices
    if(devices->getCount() == 0) {
        return;
    }

    const auto deviceCount = devices->getCount();
    for(uint32_t i = 0; i < deviceCount; i++) {
        std::string deviceSN = devices->getSerialNumber(i);
        std::cout << "Found device connected, SN: " << deviceSN << std::endl;
    }

    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(!device) {
        // open default device (device index=0)
        device   = devices->getDevice(0);
        pipeline = std::make_shared<ob::Pipeline>(device);
        std::cout << "Open device success, SN: " << devices->getSerialNumber(0) << std::endl;

        startStream();
    }
}

// Device disconnect callback
void handleDeviceDisconnected(std::shared_ptr<ob::DeviceList> disconnectList) {
    std::string currentDevSn = "";
    {
        std::unique_lock<std::recursive_mutex> lk(deviceMutex);
        if(device) {
            std::shared_ptr<ob::DeviceInfo> devInfo = device->getDeviceInfo();
            currentDevSn                            = devInfo->getSerialNumber();
        }
    }
    const auto deviceCount = disconnectList->getCount();
    for(uint32_t i = 0; i < deviceCount; i++) {
        std::string deviceSN = disconnectList->getSerialNumber(i);
        std::cout << "Device disconnected, SN: " << deviceSN << std::endl;
        if(currentDevSn == deviceSN) {
            device.reset();    // release device
            pipeline.reset();  // release pipeline
            std::cout << "Current device disconnected" << std::endl;
        }
    }
}

void switchDepthWorkMode() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    // Check whether the camera depth working mode is supported
    if(!device->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE)) {
        return;
    }

    // Query the current camera depth mode
    auto curDepthMode = device->getCurrentDepthWorkMode();
    std::cout << "current depth work mode: " << curDepthMode.name << std::endl;

    // Get the list of camera depth modes
    auto depthModeList = device->getDepthWorkModeList();
    std::cout << "depthModeList size: " << depthModeList->getCount() << std::endl;
    for(uint32_t i = 0; i < depthModeList->getCount(); i++) {
        std::cout << "depthModeList[" << i << "]: " << (*depthModeList)[i].name << std::endl;
    }

    // switch depth work mode to default (index=0) mode, user can switch to ohter mode like this.
    device->switchDepthWorkMode((*depthModeList)[0].name);
    std::cout << "switch depth work mode to:" << (*depthModeList)[0].name << std::endl;

    // It is require to reopen the device and pipeline after switch depth work mode
    auto deviceInfo = device->getDeviceInfo();
    device.reset();
    pipeline.reset();
    auto deviceList = ctx->queryDeviceList();
    device          = deviceList->getDeviceBySN(deviceInfo->getSerialNumber());  // using serial number to create device
    pipeline        = std::make_shared<ob::Pipeline>(device);
}

void turnOffHwD2d() {
    try {
        // Some models dose not support this feature
        if(device->isPropertySupported(OB_PROP_DISPARITY_TO_DEPTH_BOOL, OB_PERMISSION_WRITE)) {
            device->setBoolProperty(OB_PROP_DISPARITY_TO_DEPTH_BOOL, false);
            std::cout << "turn off hardware disparity to depth converter (Turn on Software D2D)" << std::endl;
        }
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void setDepthUnit() {
    try {
        if(device->isPropertySupported(OB_PROP_DEPTH_PRECISION_LEVEL_INT, OB_PERMISSION_WRITE)) {
            device->setIntProperty(OB_PROP_DEPTH_PRECISION_LEVEL_INT, OB_PRECISION_1MM);
            std::cout << "set depth unit to 1mm" << std::endl;
        }
        else {
            std::cerr << "Depth precision level switch is not supported." << std::endl;
        }
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void setDepthSoftFilter() {
    try {
        if(device->isPropertySupported(OB_PROP_DEPTH_SOFT_FILTER_BOOL, OB_PERMISSION_WRITE)) {
            device->setBoolProperty(OB_PROP_DEPTH_SOFT_FILTER_BOOL, true);
            std::cout << "turn on depth soft filter" << std::endl;
        }
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void startStream() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);

    device          = pipeline->getDevice();
    auto sensorList = device->getSensorList();

    // Configure which streams to enable or disable for the Pipeline by creating a Config.
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    for(uint32_t index = 0; index < sensorList->getCount(); index++) {
        // Query all supported infrared sensor type and enable the infrared stream.
        // For dual infrared device, enable the left and right infrared streams.
        // For single infrared device, enable the infrared stream.
        OBSensorType sensorType = sensorList->getSensorType(index);
        if(sensorType == OB_SENSOR_IR || sensorType == OB_SENSOR_IR_LEFT || sensorType == OB_SENSOR_IR_RIGHT || sensorType == OB_SENSOR_COLOR
           || sensorType == OB_SENSOR_DEPTH) {
            try {
                auto sensor           = sensorList->getSensor(sensorType);
                auto colorProfileList = sensor->getStreamProfileList();
                if(colorProfileList->getCount() > 0) {
                    // get default (index=0) stream profile
                    auto profile = colorProfileList->getProfile(OB_PROFILE_DEFAULT);

                    auto colorProfile = profile->as<ob::VideoStreamProfile>();
                    profilesMap.insert(std::make_pair(sensorType, colorProfile));
                    std::cout << "color profile: " << colorProfile->getWidth() << "x" << colorProfile->getHeight() << " @ " << colorProfile->getFps() << "fps"
                              << std::endl;

                    // enable color stream.
                    config->enableStream(colorProfile);
                }
            }
            catch(ob::Error &e) {
                std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                          << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    // start pipeline
    pipeline->start(config, handleFrameset);
    streamStarted = true;
    std::cout << "Stream started!" << std::endl;
}
std::shared_ptr<ob::FrameSet> fileterAlign(std::shared_ptr<ob::FrameSet> frameset) {
    auto newFrame    = align->process(frameset);
    auto newFrameSet = newFrame->as<ob::FrameSet>();
    return newFrameSet;
}
void handleFrameset(std::shared_ptr<ob::FrameSet> frameset) {
    auto alignFrameSet = fileterAlign(frameset);
    //If no depthframe is present, it is discarded
    if(frameset->getCount() < 3) {
        return;
    }
    win->pushFramesToView(alignFrameSet);
}

void getDeviceInformation() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        auto info = device->getDeviceInfo();
        // Get the name of the device
        std::cout << "-Device name: " << info->getName() << std::endl;
        // Get the pid, vid, uid of the device
        std::cout << "-Device pid: " << info->getPid() << " vid: " << info->getVid() << " uid: " << info->getUid() << std::endl;
        // By getting the firmware version number of the device
        auto fwVer = info->getFirmwareVersion();
        std::cout << "-Firmware version: " << fwVer << std::endl;
        // By getting the serial number of the device
        auto sn = info->getSerialNumber();
        std::cout << "-Serial number: " << sn << std::endl;
        // By getting the connection type of the device
        auto connectType = info->getConnectionType();
        std::cout << "-ConnectionType: " << connectType << std::endl;
    }
}

void getCameraParams() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(pipeline) {
        try {
            for(const auto &item: profilesMap) {
                auto profile    = item.second;
                auto type       = item.first;
                auto intrinsics = profile->getIntrinsic();
                auto distortion = profile->getDistortion();
                std::cout << type << " intrinsics - " << "fx:" << intrinsics.fx << ", fy: " << intrinsics.fy << ", cx: " << intrinsics.cx
                          << ", cy: " << intrinsics.cy << " ,width: " << intrinsics.width << ", height: " << intrinsics.height << std::endl;

                std::cout << "rgbDistortion k1:" << distortion.k1 << ", k2:" << distortion.k2 << ", k3:" << distortion.k3 << ", k4:" << distortion.k4
                          << ", k5:" << distortion.k5 << ", k6:" << distortion.k6 << ", p1:" << distortion.p1 << ", p2:" << distortion.p2 << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void switchLaser() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            auto         pid        = device->getDeviceInfo()->getPid();
            OBPropertyID propertyId = OB_PROP_LASER_BOOL;
            if(isGemini330Series(pid)) {
                propertyId = OB_PROP_LASER_CONTROL_INT;
            }

            if(device->isPropertySupported(propertyId, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(propertyId);
                if(device->isPropertySupported(propertyId, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(propertyId, !value);
                    if(!value) {
                        std::cout << "laser turn on!" << std::endl;
                    }
                    else {
                        std::cout << "laser turn off!" << std::endl;
                    }
                }
            }
            else {
                std::cerr << "Laser switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void switchLDP() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_LDP_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_LDP_BOOL);
                if(device->isPropertySupported(OB_PROP_LDP_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_LDP_BOOL, !value);
                    if(!value) {
                        std::cout << "LDP turn on!" << std::endl;
                    }
                    else {
                        std::cout << "LDP turn off!" << std::endl;
                    }
                    std::cout << "Attention: For some models, it is require to restart depth stream after turn on/of LDP. Input \"stream\" command "
                                 "to restart stream!"
                              << std::endl;
                }
            }
            else {
                std::cerr << "LDP switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void getLDPStatus() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_LDP_STATUS_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_LDP_STATUS_BOOL);
                std::cout << "LDP status:" << value << std::endl;
            }
            else {
                std::cerr << "LDP status property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void switchDepthAE() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL);
                if(device->isPropertySupported(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL, !value);
                    if(!value) {
                        std::cout << "Depth Auto-Exposure on!" << std::endl;
                    }
                    else {
                        std::cout << "Depth Auto-Exposure off!" << std::endl;
                    }
                }
            }
            else {
                std::cerr << "Depth Auto-Exposure switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}
void switchColorAE() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL);
                if(device->isPropertySupported(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL, !value);
                    if(!value) {
                        std::cout << "Color Auto-Exposure on!" << std::endl;
                    }
                    else {
                        std::cout << "Color Auto-Exposure off!" << std::endl;
                    }
                }
            }
            else {
                std::cerr << "Color Auto-Exposure switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void switchDepthMirror() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_DEPTH_MIRROR_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_DEPTH_MIRROR_BOOL);
                if(device->isPropertySupported(OB_PROP_DEPTH_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_DEPTH_MIRROR_BOOL, !value);
                    if(!value) {
                        std::cout << "Depth mirror on!" << std::endl;
                    }
                    else {
                        std::cout << "Depth mirror off!" << std::endl;
                    }
                }
            }
            else {
                std::cerr << "Depth mirror switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void switchIRMirror() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_IR_MIRROR_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_IR_MIRROR_BOOL);
                if(device->isPropertySupported(OB_PROP_IR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_IR_MIRROR_BOOL, !value);
                    if(!value) {
                        std::cout << "IR mirror on!" << std::endl;
                    }
                    else {
                        std::cout << "IR  mirror off!" << std::endl;
                    }
                }
            }
            else {
                std::cerr << "IR  mirror switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void switchColorMirror() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_COLOR_MIRROR_BOOL);
                if(device->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, !value);
                    if(!value) {
                        std::cout << "Color mirror on!" << std::endl;
                    }
                    else {
                        std::cout << "Color mirror off!" << std::endl;
                    }
                }
            }
            else {
                std::cerr << "Color mirror switch property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void setDepthExposureValue(bool increase) {
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL, OB_PERMISSION_READ_WRITE)) {
                bool value = device->getBoolProperty(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL);
                if(value) {
                    device->setBoolProperty(OB_PROP_DEPTH_AUTO_EXPOSURE_BOOL, false);
                    std::cout << "Depth AE close." << std::endl;
                }
            }
            if(device->isPropertySupported(OB_PROP_DEPTH_EXPOSURE_INT, OB_PERMISSION_READ)) {
                // get the value range
                OBIntPropertyRange valueRange = device->getIntPropertyRange(OB_PROP_DEPTH_EXPOSURE_INT);
                std::cout << "Depth current exposure max:" << valueRange.max << ", min:" << valueRange.min << std::endl;

                int value = device->getIntProperty(OB_PROP_DEPTH_EXPOSURE_INT);
                std::cout << "Depth current exposure:" << value << std::endl;
                if(device->isPropertySupported(OB_PROP_DEPTH_EXPOSURE_INT, OB_PERMISSION_WRITE)) {
                    if(increase) {
                        value += (valueRange.max - valueRange.min) / 10;
                        if(value > valueRange.max) {
                            value = valueRange.max;
                        }
                    }
                    else {
                        value -= (valueRange.max - valueRange.min) / 10;
                        if(value < valueRange.min) {
                            value = valueRange.min;
                        }
                    }

                    // Ensure that the value meet the step value requirements
                    value = valueRange.min + (value - valueRange.min) / valueRange.step * valueRange.step;

                    std::cout << "Set depth exposure:" << value << std::endl;
                    device->setIntProperty(OB_PROP_DEPTH_EXPOSURE_INT, value);
                }
                else {
                    std::cerr << "Depth exposure set property is not supported." << std::endl;
                }
            }
            else {
                std::cerr << "Depth exposure get property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void setColorExposureValue(bool increase) {
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL, OB_PERMISSION_READ_WRITE)) {
                bool value = device->getBoolProperty(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL);
                if(value) {
                    device->setBoolProperty(OB_PROP_COLOR_AUTO_EXPOSURE_BOOL, false);
                    std::cout << "Color AE close." << std::endl;
                }
            }

            if(device->isPropertySupported(OB_PROP_COLOR_EXPOSURE_INT, OB_PERMISSION_READ)) {
                // get the value range
                OBIntPropertyRange valueRange = device->getIntPropertyRange(OB_PROP_COLOR_EXPOSURE_INT);
                std::cout << "Color current exposure max:" << valueRange.max << ", min:" << valueRange.min << std::endl;

                int value = device->getIntProperty(OB_PROP_COLOR_EXPOSURE_INT);
                std::cout << "Color current exposure:" << value << std::endl;

                if(device->isPropertySupported(OB_PROP_COLOR_EXPOSURE_INT, OB_PERMISSION_WRITE)) {
                    if(increase) {
                        value += (valueRange.max - valueRange.min) / 10;
                        if(value > valueRange.max) {
                            value = valueRange.max;
                        }
                    }
                    else {
                        value -= (valueRange.max - valueRange.min) / 10;
                        if(value < valueRange.min) {
                            value = valueRange.min;
                        }
                    }

                    // Ensure that the value meet the step value requirements
                    value = valueRange.min + (value - valueRange.min) / valueRange.step * valueRange.step;

                    std::cout << "Set color exposure:" << value << std::endl;
                    device->setIntProperty(OB_PROP_COLOR_EXPOSURE_INT, value);
                }
                else {
                    std::cerr << "Color exposure set property is not supported." << std::endl;
                }
            }
            else {
                std::cerr << "Color exposure get property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void setDepthGainValue(bool increase) {
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_DEPTH_GAIN_INT, OB_PERMISSION_READ)) {
                OBIntPropertyRange valueRange = device->getIntPropertyRange(OB_PROP_DEPTH_GAIN_INT);
                std::cout << "Depth current gain max:" << valueRange.max << ", min:" << valueRange.min << std::endl;
                int value = device->getIntProperty(OB_PROP_DEPTH_GAIN_INT);
                std::cout << "Depth current gain:" << value << std::endl;
                if(device->isPropertySupported(OB_PROP_DEPTH_GAIN_INT, OB_PERMISSION_WRITE)) {
                    if(increase) {
                        value += (valueRange.max - valueRange.min) / 10;
                        if(value > valueRange.max) {
                            value = valueRange.max;
                        }
                    }
                    else {
                        value -= (valueRange.max - valueRange.min) / 10;
                        if(value < valueRange.min) {
                            value = valueRange.min;
                        }
                    }
                    // Ensure that the value meet the step value requirements
                    value = valueRange.min + (value - valueRange.min) / valueRange.step * valueRange.step;

                    std::cout << "Set depth gain:" << value << std::endl;
                    device->setIntProperty(OB_PROP_DEPTH_GAIN_INT, value);
                }
                else {
                    std::cerr << "Depth gain set property is not supported." << std::endl;
                }
            }
            else {
                std::cerr << "Depth gain get property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void setColorGainValue(bool increase) {
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_COLOR_GAIN_INT, OB_PERMISSION_READ)) {
                OBIntPropertyRange valueRange = device->getIntPropertyRange(OB_PROP_COLOR_GAIN_INT);
                std::cout << "Color current gain max:" << valueRange.max << ", min:" << valueRange.min << std::endl;
                int value = device->getIntProperty(OB_PROP_COLOR_GAIN_INT);
                std::cout << "Color current gain:" << value << std::endl;
                if(device->isPropertySupported(OB_PROP_COLOR_GAIN_INT, OB_PERMISSION_WRITE)) {
                    if(increase) {
                        value += (valueRange.max - valueRange.min) / 10;
                        if(value > valueRange.max) {
                            value = valueRange.max;
                        }
                    }
                    else {
                        value -= (valueRange.max - valueRange.min) / 10;
                        if(value < valueRange.min) {
                            value = valueRange.min;
                        }
                    }

                    // Ensure that the value meet the step value requirements
                    value = valueRange.min + (value - valueRange.min) / valueRange.step * valueRange.step;

                    std::cout << "Set color gain:" << value << std::endl;
                    device->setIntProperty(OB_PROP_COLOR_GAIN_INT, value);
                }
                else {
                    std::cerr << "Color gain set property is not supported." << std::endl;
                }
            }
            else {
                std::cerr << "Color gain get property is not supported." << std::endl;
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType()
                      << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void printUsage() {
    std::cout << "Support commands:" << std::endl;
    std::cout << "    info / i - get device information" << std::endl;
    std::cout << "    param / p - get camera parameter" << std::endl;
    std::cout << "    laser / l - on/off laser" << std::endl;
    std::cout << "    ldp / d - on/off LDP" << std::endl;
    std::cout << "    ldp status / ds - get LDP status" << std::endl;
    std::cout << "    color ae / ca - on/off Color auto exposure" << std::endl;
    std::cout << "    inc color value / cei - increase Color exposure value" << std::endl;
    std::cout << "    dec color value / ced - decrease Color exposure value" << std::endl;
    std::cout << "    inc color gain / cgi - increase Color gain value" << std::endl;
    std::cout << "    dec color gain / cgd - decrease Color gain value" << std::endl;
    std::cout << "    color mirror / cm - on/off color mirror" << std::endl;
    std::cout << "    depth ae / da - on/off Depth/IR auto exposure" << std::endl;
    std::cout << "    depth mirror / dm - on/off Depth mirror" << std::endl;
    std::cout << "    inc depth value / dei - increase Depth/IR exposure value" << std::endl;
    std::cout << "    dec depth value / ded - decrease Depth/IR exposure value" << std::endl;
    std::cout << "    inc depth gain / dgi - increase Depth/IR gain value" << std::endl;
    std::cout << "    dec depth gain / dgd - decrease Depth/IR gain value" << std::endl;
    std::cout << "    ir mirror / im - on/off Ir mirror" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << "    help / ? - print usage" << std::endl;
    std::cout << "    quit / q- quit application" << std::endl;
}

void commandProcess(std::string cmd) {
    if(cmd == "info" || cmd == "i") {
        getDeviceInformation();
    }
    else if(cmd == "param" || cmd == "p") {
        getCameraParams();
    }
    else if(cmd == "laser" || cmd == "l") {
        switchLaser();
    }
    else if(cmd == "ldp" || cmd == "d") {
        switchLDP();
    }
    else if(cmd == "ldp status" || cmd == "ds") {
        getLDPStatus();
    }
    else if(cmd == "color ae" || cmd == "ca") {
        switchColorAE();
    }
    else if(cmd == "inc color value" || cmd == "cei") {
        setColorExposureValue(true);
    }
    else if(cmd == "dec color value" || cmd == "ced") {
        setColorExposureValue(false);
    }
    else if(cmd == "inc color gain" || cmd == "cgi") {
        setColorGainValue(true);
    }
    else if(cmd == "dec color gain" || cmd == "cgd") {
        setColorGainValue(false);
    }
    else if(cmd == "inc depth value" || cmd == "dei") {
        setDepthExposureValue(true);
    }
    else if(cmd == "dec depth value" || cmd == "ded") {
        setDepthExposureValue(false);
    }
    else if(cmd == "inc depth gain" || cmd == "dgi") {
        setDepthGainValue(true);
    }
    else if(cmd == "dec depth gain" || cmd == "dgd") {
        setDepthGainValue(false);
    }
    else if(cmd == "depth ae" || cmd == "da") {
        switchDepthAE();
    }
    else if(cmd == "color mirror" || cmd == "cm") {
        switchColorMirror();
    }
    else if(cmd == "depth mirror" || cmd == "dm") {
        switchDepthMirror();
    }
    else if(cmd == "ir mirror" || cmd == "im") {
        switchIRMirror();
    }
    else if(cmd == "help" || cmd == "?") {
        printUsage();
    }
    else {
        std::cerr << "Unsupported command received! Input \"help\" to get usage" << std::endl;
    }
}

void inputWatcher() {
    while(true) {
        std::string cmd;
        std::cout << "\nInput command:  ";
        std::getline(std::cin, cmd);
        if(cmd == "quit" || cmd == "q") {
            win->close();
            break;
        }
        else {
            commandProcess(cmd);
        }
    }
}

