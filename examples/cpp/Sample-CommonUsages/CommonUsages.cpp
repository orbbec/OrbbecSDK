#include <libobsensor/ObSensor.hpp>
#include "utils.hpp"
#include "window.hpp"

#include <mutex>
#include <string>

std::shared_ptr<Window> win;

std::shared_ptr<ob::Context> ctx;

std::shared_ptr<ob::Device>   device;
std::shared_ptr<ob::Pipeline> pipeline;
std::recursive_mutex          deviceMutex;

std::thread inputWatchThread;

bool streamStarted = false;

void handleDeviceConnected(std::shared_ptr<ob::DeviceList> connectList);
void handleDeviceDisconnected(std::shared_ptr<ob::DeviceList> disconnectList);
void switchDepthWorkMode();
void turnOffHwD2d();
void setDepthUnit();
void setDepthValueRange();
void setDepthSoftFilter();

void printUsage();
void commandProcess(std::string cmd);

void handleFrameset(std::shared_ptr<ob::FrameSet> frameset);
void startStream();

void inputWatcher();

int main(int argc, char **argv) try {
    // create window for render
    win = std::make_shared<Window>("CommonUsages", 1280, 720, RENDER_GRID);

    // Set log severity, disable log, please set OB_LOG_SEVERITY_OFF.
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

    while(*win) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    inputWatchThread.join();

    if(pipeline) {
        pipeline->stop();
    }
    return 0;
}

catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

// Device connection callback
void handleDeviceConnected(std::shared_ptr<ob::DeviceList> devices) {
    // Get the number of connected devices
    if(devices->deviceCount() == 0) {
        return;
    }

    const auto deviceCount = devices->deviceCount();
    for(uint32_t i = 0; i < deviceCount; i++) {
        std::string deviceSN = devices->serialNumber(i);
        std::cout << "Found device connected, SN: " << deviceSN << std::endl;
    }

    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(!device) {
        // open default device (device index=0)
        device   = devices->getDevice(0);
        pipeline = std::make_shared<ob::Pipeline>(device);
        std::cout << "Open device success, SN: " << devices->serialNumber(0) << std::endl;

        // try to switch depth work mode
        switchDepthWorkMode();

        // try turn off hardware disparity to depth converter (switch to software d2d)
        turnOffHwD2d();

        // set depth unit
        setDepthUnit();

        // set depth value range
        setDepthValueRange();

        // set depth soft filter
        setDepthSoftFilter();

        // start stream
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
            currentDevSn                            = devInfo->serialNumber();
        }
    }
    const auto deviceCount = disconnectList->deviceCount();
    for(uint32_t i = 0; i < deviceCount; i++) {
        std::string deviceSN = disconnectList->serialNumber(i);
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
    // Get the list of camera depth modes
    auto depthModeList = device->getDepthWorkModeList();
    std::cout << "depthModeList size: " << depthModeList->count() << std::endl;
    for(int i = 0; i < depthModeList->count(); i++) {
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
    device          = deviceList->getDeviceBySN(deviceInfo->serialNumber());  // using serial number to create device
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
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
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
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    }
}

void setDepthValueRange() {
    try {
        if(device->isPropertySupported(OB_PROP_MIN_DEPTH_INT, OB_PERMISSION_WRITE)) {
            device->setIntProperty(OB_PROP_MIN_DEPTH_INT, 30);
            std::cout << "set depth value range-min to 30 mm" << std::endl;
        }
        else {
            std::cerr << "set depth value range-min is not supported." << std::endl;
        }

        if(device->isPropertySupported(OB_PROP_MAX_DEPTH_INT, OB_PERMISSION_WRITE)) {
            device->setIntProperty(OB_PROP_MAX_DEPTH_INT, 10000);
            std::cout << "set depth value range-max to 10000 mm" << std::endl;
        }
        else {
            std::cerr << "set depth value range-max is not supported." << std::endl;
        }
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    }
}

void setDepthSoftFilter() {
    try {
        if(device->isPropertySupported(OB_PROP_DEPTH_SOFT_FILTER_BOOL, OB_PERMISSION_WRITE)) {
            device->setBoolProperty(OB_PROP_DEPTH_SOFT_FILTER_BOOL, true);
            std::cout << "turn on depth soft filter" << std::endl;

            int max_diff    = device->getIntProperty(OB_PROP_DEPTH_MAX_DIFF_INT);
            int max_speckle = device->getIntProperty(OB_PROP_DEPTH_MAX_SPECKLE_SIZE_INT);

            // max_diff can be modified, the software filtering max_diff parameters are set before the stream is start, which needs to be set carefully
            // device->setIntProperty(OB_PROP_DEPTH_MAX_DIFF_INT, max_diff);

            // max_speckle can be modified, the software filtering max_speckle parameters are set before the stream is start, which needs to be set carefully
            // device->setIntProperty(OB_PROP_DEPTH_MAX_SPECKLE_SIZE_INT, max_speckle);
        }
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    }
}

void startStream() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(pipeline) {
        if(streamStarted) {
            std::cout << "Try to restart stream!" << std::endl;
            pipeline->stop();
            std::cout << "Stream stoped!" << std::endl;
        }

        // create config to configure pipeline
        auto config = std::make_shared<ob::Config>();

        std::shared_ptr<ob::VideoStreamProfile> colorProfile;
        try {
            auto colorProfileList = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
            if(colorProfileList->count() > 0) {
                // get default (index=0) stream profile
                auto profile = colorProfileList->getProfile(OB_PROFILE_DEFAULT);

                colorProfile = profile->as<ob::VideoStreamProfile>();
                std::cout << "color profile: " << colorProfile->width() << "x" << colorProfile->height() << " @ " << colorProfile->fps() << "fps" << std::endl;

                // enable color stream
                config->enableStream(colorProfile);
            }
        }
        catch(...) {
            // dose not support color stream or other error
        }

        std::shared_ptr<ob::VideoStreamProfile> depthProfile;
        try {
            std::shared_ptr<ob::StreamProfileList> depthProfileList;
            OBAlignMode                            alignMode = ALIGN_DISABLE;
            do {
                if(colorProfile) {
                    // Try find supported depth to color align hardware mode profile
                    depthProfileList = pipeline->getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
                    if(depthProfileList->count() > 0) {
                        alignMode = ALIGN_D2C_HW_MODE;
                        break;
                    }

                    // Try find supported depth to color align software mode profile
                    depthProfileList = pipeline->getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
                    if(depthProfileList->count() > 0) {
                        alignMode = ALIGN_D2C_SW_MODE;
                        break;
                    }
                }

                // Try get depth stream profile list
                depthProfileList = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
            } while(0);

            if(depthProfileList->count() > 0) {
                std::shared_ptr<ob::StreamProfile> profile;

                try {
                    // Select the profile with the same frame rate as color.
                    profile = depthProfileList->getVideoStreamProfile(OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
                }
                catch(...) {
                    profile = nullptr;
                }

                if(!profile) {
                    // If no matching profile is found, select the default profile.
                    profile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
                }

                depthProfile = profile->as<ob::VideoStreamProfile>();
                std::cout << "depth profile: " << depthProfile->width() << "x" << depthProfile->height() << " @ " << depthProfile->fps() << "fps" << std::endl;

                // enable depth stream
                config->enableStream(depthProfile);

                // set align mode
                config->setAlignMode(alignMode);
            }
        }
        catch(...) {
            // dose not support depth stream or other error
        }

        std::shared_ptr<ob::VideoStreamProfile> irProfile;
        try {
            auto irProfileList = pipeline->getStreamProfileList(OB_SENSOR_IR);
            if(irProfileList->count() > 0) {
                std::shared_ptr<ob::StreamProfile> profile;
                if(depthProfile) {
                    // Try to get same resolution and frame rate with depth, due to same model ir stream resolution and frame rate must be same with depth
                    try {
                        profile = irProfileList->getVideoStreamProfile(depthProfile->width(), depthProfile->height(), OB_FORMAT_ANY, depthProfile->fps());
                    }
                    catch(...) {
                        // dose not support same resolution and frame rate
                        profile = nullptr;
                    }
                }

                if(!profile) {
                    // If no matching profile is found, select the default profile.
                    profile = irProfileList->getProfile(OB_PROFILE_DEFAULT);
                }

                irProfile = profile->as<ob::VideoStreamProfile>();

                std::cout << "ir profile: " << irProfile->width() << "x" << irProfile->height() << " @ " << irProfile->fps() << "fps" << std::endl;

                // enable ir stream
                config->enableStream(irProfile);
            }
        }
        catch(...) {
            // dose not support ir stream or other error
        }

        // start pipeline
        pipeline->start(config, handleFrameset);
        streamStarted = true;
        std::cout << "Stream started!" << std::endl;
    }
}

void handleFrameset(std::shared_ptr<ob::FrameSet> frameset) {
    static std::shared_ptr<ob::Frame>       depthFrame;
    static std::shared_ptr<ob::Frame>       colorFrame;
    static std::shared_ptr<ob::Frame>       irFrame;
    std::vector<std::shared_ptr<ob::Frame>> frames;

    try {
        auto frame = frameset->depthFrame();
        if(frame) {
            depthFrame = frame;
        }
    }
    catch(...) {
        // get depth frame failed.
    }

    try {
        auto frame = frameset->colorFrame();
        if(frame) {
            colorFrame = frame;
        }
    }
    catch(...) {
        // get color frame failed.
    }

    try {
        auto frame = frameset->irFrame();
        if(frame) {
            irFrame = frame;
        }
    }
    catch(...) {
        // get ir frame failed.
    }

    if(depthFrame) {
        frames.push_back(depthFrame);
    }
    if(colorFrame) {
        frames.push_back(colorFrame);
    }
    if(irFrame) {
        frames.push_back(irFrame);
    }

    if(!frames.empty()) {
        win->addToRender(frames);
    }
}

void getDeviceInformation() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        auto info = device->getDeviceInfo();
        // Get the name of the device
        std::cout << "-Device name: " << info->name() << std::endl;
        // Get the pid, vid, uid of the device
        std::cout << "-Device pid: " << info->pid() << " vid: " << info->vid() << " uid: " << info->uid() << std::endl;
        // By getting the firmware version number of the device
        auto fwVer = info->firmwareVersion();
        std::cout << "-Firmware version: " << fwVer << std::endl;
        // By getting the serial number of the device
        auto sn = info->serialNumber();
        std::cout << "-Serial number: " << sn << std::endl;
        // By getting the connection type of the device
        auto connectType = info->connectionType();
        std::cout << "-ConnectionType: " << connectType << std::endl;
    }
}

void getCameraParams() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(pipeline) {
        try {
            auto cameraParam = pipeline->getCameraParam();

            std::cout << "depthIntrinsic fx:" << cameraParam.depthIntrinsic.fx << ", fy" << cameraParam.depthIntrinsic.fy
                      << ", cx:" << cameraParam.depthIntrinsic.cx << ", cy:" << cameraParam.depthIntrinsic.cy << " ,width:" << cameraParam.depthIntrinsic.width
                      << ", height:" << cameraParam.depthIntrinsic.height << std::endl;

            std::cout << "rgbIntrinsic fx:" << cameraParam.rgbIntrinsic.fx << ", fy" << cameraParam.rgbIntrinsic.fy << ", cx:" << cameraParam.rgbIntrinsic.cx
                      << ", cy:" << cameraParam.rgbIntrinsic.cy << ", width:" << cameraParam.rgbIntrinsic.width
                      << ", height:" << cameraParam.rgbIntrinsic.height << std::endl;

            std::cout << "depthDistortion k1:" << cameraParam.depthDistortion.k1 << ", k2:" << cameraParam.depthDistortion.k2
                      << ", k3:" << cameraParam.depthDistortion.k3 << ", k4:" << cameraParam.depthDistortion.k4 << ", k5:" << cameraParam.depthDistortion.k5
                      << ", k6:" << cameraParam.depthDistortion.k6 << ", p1:" << cameraParam.depthDistortion.p1 << ", p2:" << cameraParam.depthDistortion.p2
                      << std::endl;

            std::cout << "rgbDistortion k1:" << cameraParam.rgbDistortion.k1 << ", k2:" << cameraParam.rgbDistortion.k2
                      << ", k3:" << cameraParam.rgbDistortion.k3 << ", k4:" << cameraParam.rgbDistortion.k4 << ", k5:" << cameraParam.rgbDistortion.k5
                      << ", k6:" << cameraParam.rgbDistortion.k6 << ", p1:" << cameraParam.rgbDistortion.p1 << ", p2:" << cameraParam.rgbDistortion.p2
                      << std::endl;

            OBD2CTransform transform = cameraParam.transform;
            std::cout << "transform-rot: [";
            for(int j = 0; j < 9; j++) {
                std::cout << transform.rot[j];
                if(j == 8) {
                    std::cout << "]" << std::endl;
                }
                else {
                    std::cout << ", ";
                }
            }

            std::cout << "transform-trans: [";
            for(int k = 0; k < 3; k++) {
                std::cout << " " << transform.trans[k];
                if(k == 2) {
                    std::cout << "]" << std::endl;
                }
                else {
                    std::cout << ", ";
                }
            }
        }
        catch(ob::Error &e) {
            std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType()
                      << std::endl;
        }
    }
}

void switchLaser() {
    std::unique_lock<std::recursive_mutex> lk(deviceMutex);
    if(device) {
        try {
            if(device->isPropertySupported(OB_PROP_LASER_BOOL, OB_PERMISSION_READ)) {
                bool value = device->getBoolProperty(OB_PROP_LASER_BOOL);
                if(device->isPropertySupported(OB_PROP_LASER_BOOL, OB_PERMISSION_WRITE)) {
                    device->setBoolProperty(OB_PROP_LASER_BOOL, !value);
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
            std::cerr << "Laser switch property is not supported." << std::endl;
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
            std::cerr << "LDP switch property is not supported." << std::endl;
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
            std::cerr << "LDP switch property is not supported." << std::endl;
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
            std::cerr << "Depth Auto-Exposure switch property is not supported." << std::endl;
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
            std::cerr << "Color Auto-Exposure switch property is not supported." << std::endl;
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
            std::cerr << "Depth mirror switch property is not supported." << std::endl;
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
            std::cerr << "IR  mirror switch property is not supported." << std::endl;
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
            std::cerr << "Color mirror switch property is not supported." << std::endl;
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
            std::cerr << "Depth AE set property is not supported." << std::endl;
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
            std::cerr << "Color AE set property is not supported." << std::endl;
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
            std::cerr << "Depth gain set property is not supported." << std::endl;
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
            std::cerr << "Color gain set property is not supported.: " << std::endl;
        }
    }
}

void printUsage() {
    std::cout << "Support commands:" << std::endl;
    std::cout << "    stream / s - (re)start stream" << std::endl;
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
    if(cmd == "stream" || cmd == "s") {
        startStream();
    }
    else if(cmd == "info" || cmd == "i") {
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
