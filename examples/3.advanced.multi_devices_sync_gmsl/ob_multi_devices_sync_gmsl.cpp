// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

/*
Notes: ob_multi_devices_sync_gmsl.cpp for GMSL device
on the nvidia arm64 xavier/orin platform ,this example demo sync multi gmsl devices.
*/
#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"
#include "utils_opencv.hpp"
#include "utils/cJSON.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <chrono>

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <cstdlib>
#include <strings.h>
#endif

#define MAX_DEVICE_COUNT 9
#define CONFIG_FILE "./ob_multi_devices_sync_gmsl_config.json"
#define DEVICE_PATH "/dev/camsync"
#define KEY_ESC 27

static bool quitStreamPreview = false;

const std::string GMSL2_DEVICE_TAG= "GMSL2";

const std::map<std::string, uint16_t> gemini_330_list = {
    { "gemini335", 0x0800 }, { "gemini335L", 0x0804 }, { "gemini336", 0x0803 }, { "gemini336L", 0x0807 }, { "gemini335Lg", 0x080B }
};

typedef struct DeviceConfigInfo_t {
    std::string             deviceSN;
    OBMultiDeviceSyncConfig syncConfig;
} DeviceConfigInfo;

typedef struct PipelineHolder_t {
    std::shared_ptr<ob::Pipeline> pipeline;
    OBSensorType                  sensorType;
    int                           deviceIndex;
    std::string                   deviceSN;
} PipelineHolder;
std::ostream &operator<<(std::ostream &os, const PipelineHolder &holder);
std::ostream &operator<<(std::ostream &os, std::shared_ptr<PipelineHolder> holder) {
    return os << *holder;
}

std::mutex                                    frameMutex;
std::map<uint8_t, std::shared_ptr<ob::Frame>> colorFrames;
std::map<uint8_t, std::shared_ptr<ob::Frame>> depthFrames;

std::vector<std::shared_ptr<ob::Device>>       streamDevList;
std::vector<std::shared_ptr<ob::Device>>       configDevList;
std::vector<std::shared_ptr<DeviceConfigInfo>> deviceConfigList;

std::condition_variable                      waitRebootCompleteCondition;
std::mutex                                   rebootingDevInfoListMutex;
std::vector<std::shared_ptr<ob::DeviceInfo>> rebootingDevInfoList;
std::vector<std::shared_ptr<PipelineHolder>> pipelineHolderList;

bool loadConfigFile();
int  configMultiDeviceSync();
int  testMultiDeviceSync();

void startStream(std::shared_ptr<PipelineHolder> pipelineHolder);
void stopStream(std::shared_ptr<PipelineHolder> pipelineHolder);

void handleColorStream(int devIndex, std::shared_ptr<ob::Frame> frame);
void handleDepthStream(int devIndex, std::shared_ptr<ob::Frame> frame);

std::string           OBSyncModeToString(const OBMultiDeviceSyncMode syncMode);
OBMultiDeviceSyncMode stringToOBSyncMode(const std::string &modeString);

OBFrameType mapFrameType(OBSensorType sensorType);
std::string readFileContent(const char *filePath);

int  strcmp_nocase(const char *str0, const char *str1);
bool checkDevicesWithDeviceConfigs(const std::vector<std::shared_ptr<ob::Device>> &deviceList);

std::shared_ptr<PipelineHolder> createPipelineHolder(std::shared_ptr<ob::Device> device, OBSensorType sensorType, int deviceIndex);

ob::Context context;

bool IsGemini330Series(uint16_t pid) {
    bool find = false;
    for(auto it = gemini_330_list.begin(); it != gemini_330_list.end(); ++it) {
        if(it->second == pid) {
            // std::cout << "find gemini 330 series : " << it->first << std::endl;
            find = true;
            break;
        }
    }
    return find;
}


int triggerFd = -1;
int hardwareTriggerFps = 0;  // 0 means hardware trigger is disabled
typedef struct {
    uint8_t  mode;
    uint16_t fps;
} cs_param_t;

// cs_param_t rd_par = {0, 0}, param = {1, 3000}; //30
int openSocSyncPwmTrigger(uint16_t fps) {
    const char *devicePath           = DEVICE_PATH;
    const int   TRIGGER_MODE_ENABLE  = 1;
    const int   TRIGGER_MODE_DISABLE = 0;

    int        ret    = -1;
    cs_param_t param  = { TRIGGER_MODE_ENABLE, fps };
    cs_param_t rd_par = { TRIGGER_MODE_DISABLE, 0 };

    if(access(devicePath, F_OK) != 0) {
        std::cerr << "Device node " << devicePath << " does not exist." << std::endl;
        return ret;
    }
    triggerFd = open(DEVICE_PATH, O_RDWR);
    if(triggerFd < 0) {
        perror("open device failed\n");
        return triggerFd;
    }

    std::cout << "Written param mode=" << param.mode << ", fps=" << param.fps << std::endl;
    ret = write(triggerFd, &param, sizeof(param));
    if(ret < 0) {
        perror("write device failed\n");
        close(triggerFd);
        return ret;
    }

    ret = read(triggerFd, &rd_par, sizeof(rd_par));
    if(ret < 0) {
        perror("read device failed\n");
        close(triggerFd);
        return ret;
    }
    std::cout << "Read param mode=" << rd_par.mode << ", fps=" << rd_par.fps << std::endl;

    std::cout << "Start hardware triggering..." << std::endl;

    return 0;
}
int closeSocSyncPwmTrigger() {
    if(triggerFd >= 0) {
        close(triggerFd);
        triggerFd = -1;  // Reset file descriptors
        std::cout << "close camSync success" << std::endl;
        return 0;
    }
    return -1;
}

int main(void) try {
    int                       choice;
    int                       exitValue      = 0;
    constexpr std::streamsize maxInputIgnore = 10000;

    while(true) {
        std::cout << "\n--------------------------------------------------\n";
        std::cout << "Please select options: \n";
        std::cout << " 0 --> config devices sync mode. \n";
        std::cout << " 1 --> start stream \n";
        std::cout << " 2 --> start hardwareTrigger (Dependent on soc trigger source) \n";
        std::cout << " 3 --> close hardwareTrigger & Exit program \n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "Please select input: ";
        // std::cin >> choice;
        if(!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(maxInputIgnore, '\n');
            std::cout << "Invalid input. Please enter a number [0~3]." << std::endl;
            continue;
        }
        std::cout << std::endl;

        switch(choice) {
        case 0:
            exitValue = configMultiDeviceSync();
            if(exitValue == 0) {
                std::cout << "Config MultiDeviceSync Success. \n" << std::endl;
            }
            break;
        case 1:
            std::cout << "\nStart Slave Devices video stream." << std::endl;
            testMultiDeviceSync();
            break;
        case 2:
            if(!loadConfigFile()) {
                std::cout << "load config failed" << std::endl;
                return -1;
            }
            if(hardwareTriggerFps > 0) {
                std::cout << "\nStart HardwareTrigger by soc-trigger-source. triggerFps:" << hardwareTriggerFps << std::endl;
                openSocSyncPwmTrigger(hardwareTriggerFps);
            }
            else {
                std::cout << "Please repeat set hardwareTriggerFps (0 to disable): ";
                std::cin >> hardwareTriggerFps;
                std::cout << "You has set hardwareTriggerFps: " << hardwareTriggerFps << std::endl;
            }
            break;
        case 3:
            closeSocSyncPwmTrigger();
            std::cout << "Program exit & close device! " << std::endl;
            exit(0);
            return 0;

        default:
            std::cout << "-input Invalid index. \n"
                      << "-Please re-select and input valid param. \n";
        }
    }
    return exitValue;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

int configMultiDeviceSync() {
    try {
        if(!loadConfigFile()) {
            std::cout << "load config failed" << std::endl;
            return -1;
        }

        if(deviceConfigList.empty()) {
            std::cout << "DeviceConfigList is empty. please check config file: " << CONFIG_FILE << std::endl;
            return -1;
        }

        // Query the list of connected devices
        auto devList  = context.queryDeviceList();
        int  devCount = devList->deviceCount();
        for(int i = 0; i < devCount; i++) {
            std::shared_ptr<ob::Device> device = devList->getDevice(i);
            auto pid = device->getDeviceInfo()->getPid();
            if(!IsGemini330Series(pid)){
                std::cout << "Device pid: " << pid << " is not Gemini 330 series, skip" << std::endl;
                continue;
            }
            auto ConnectionType = device->getDeviceInfo()->getConnectionType();
            std::cout << "Device ConnectionType: " << ConnectionType << std::endl;
            if( ConnectionType != GMSL2_DEVICE_TAG) {
                std::cout << "Device ConnectionType: " << ConnectionType << " is not GMSL2 devices, skip" << std::endl;
                continue;
            }

            configDevList.push_back(devList->getDevice(i));
        }

        if(configDevList.empty()) {
            std::cerr << "Device list is empty. please check device connection state" << std::endl;
            return -1;
        }

        // write configuration to device
        for(auto config: deviceConfigList) {
            auto findItr = std::find_if(configDevList.begin(), configDevList.end(), [config](std::shared_ptr<ob::Device> device) {
                auto serialNumber = device->getDeviceInfo()->serialNumber();
                return strcmp_nocase(serialNumber, config->deviceSN.c_str()) == 0;
            });
            if(findItr != configDevList.end()) {
                auto device    = (*findItr);
                auto curConfig = device->getMultiDeviceSyncConfig();
                // Update the configuration items of the configuration file, and keep the original configuration for other items
                curConfig.syncMode             = config->syncConfig.syncMode;
                curConfig.depthDelayUs         = config->syncConfig.depthDelayUs;
                curConfig.colorDelayUs         = config->syncConfig.colorDelayUs;
                curConfig.trigger2ImageDelayUs = config->syncConfig.trigger2ImageDelayUs;
                curConfig.triggerOutEnable     = config->syncConfig.triggerOutEnable;
                curConfig.triggerOutDelayUs    = config->syncConfig.triggerOutDelayUs;
                curConfig.framesPerTrigger     = config->syncConfig.framesPerTrigger;
                std::cout << "-Config Device syncMode:" << curConfig.syncMode << ", syncModeStr:" << OBSyncModeToString(curConfig.syncMode) << std::endl;
                device->setMultiDeviceSyncConfig(curConfig);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return 0;
    }
    catch(ob::Error &e) {
        std::cerr << "configMultiDeviceSync failed! \n";
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
        return -1;
    }
}

void startDeviceStreams(const std::vector<std::shared_ptr<ob::Device>> &devices, int startIndex) {
    std::vector<OBSensorType> sensorTypes = { OB_SENSOR_DEPTH, OB_SENSOR_COLOR };
    for(auto &dev: devices) {
        for(auto sensorType: sensorTypes) {
            auto holder = createPipelineHolder(dev, sensorType, startIndex);
            pipelineHolderList.push_back(holder);
            startStream(holder);
        }
        startIndex++;
    }
    quitStreamPreview = false;
}

// key press event processing
void handleKeyPress(ob_smpl::CVWindow &win, int key) {
    //Get the key value
    if(key == KEY_ESC) {
        if(!quitStreamPreview){
            win.setShowInfo(false);
            win.setShowSyncTimeInfo(false);
            quitStreamPreview = true;
            win.close();
            win.destroyWindow();
            std::cout << "press ESC quitStreamPreview" << std::endl;
        }
    }
    else if(key == 'S' || key == 's') {
        std::cout << "syncDevicesTime..." << std::endl;
        context.enableDeviceClockSync(3600000);  // Manual update synchronization
    }
    else if(key == 'T' || key == 't') {
        // software trigger
        std::cout << "check software trigger mode" << std::endl;
        for(auto &dev: streamDevList) {
            auto multiDeviceSyncConfig = dev->getMultiDeviceSyncConfig();
            if(multiDeviceSyncConfig.syncMode == OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING) {
                std::cout << "software trigger..." << std::endl;
                dev->triggerCapture();
            }
        }
    }
}

int testMultiDeviceSync() {
    try {
        streamDevList.clear();
        // Query the list of connected devices
        auto devList  = context.queryDeviceList();
        int  devCount = devList->deviceCount();
        for(int i = 0; i < devCount; i++) {
            streamDevList.push_back(devList->getDevice(i));
        }

        if(streamDevList.empty()) {
            std::cerr << "Device list is empty. please check device connection state" << std::endl;
            return -1;
        }

        // traverse the device list and create the device
        std::vector<std::shared_ptr<ob::Device>> primary_devices;
        std::vector<std::shared_ptr<ob::Device>> secondary_devices;
        for(auto dev: streamDevList) {
            auto config = dev->getMultiDeviceSyncConfig();
            if(config.syncMode == OB_MULTI_DEVICE_SYNC_MODE_PRIMARY) {
                primary_devices.push_back(dev);
            }
            else {
                secondary_devices.push_back(dev);
            }
        }

        std::cout << "Secondary devices start..." << std::endl;
        startDeviceStreams(secondary_devices, 0);

        // Delay and wait for 5s to ensure that the initialization of the slave device is completed
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        if(primary_devices.empty()) {
            std::cerr << "WARNING primary_devices is empty!!!" << std::endl;
        } else {
            std::cout << "Primary device start..." << std::endl;
            startDeviceStreams(primary_devices, secondary_devices.size());
        }

        // Start the multi-device time synchronization function
        context.enableDeviceClockSync(60000);  // update and sync every minitor

        // Create a window for rendering and set the resolution of the window
        ob_smpl::CVWindow win("MultiDeviceSyncViewer", 1600, 900, ob_smpl::ARRANGE_GRID);

        // set key prompt
        win.setKeyPrompt("'S': syncDevicesTime, 'T': software triiger");
        // set the callback function for the window to handle key press events
        win.setKeyPressedCallback([&](int key){ handleKeyPress(win, key); });

        win.setShowInfo(true);
        win.setShowSyncTimeInfo(true);
        while(win.run() && !quitStreamPreview) {
            if(quitStreamPreview){
                break;
            }
            std::vector<std::pair<std::shared_ptr<ob::Frame>, std::shared_ptr<ob::Frame>>> framePairs;
            {
                std::lock_guard<std::mutex> lock(frameMutex);
                for(int i = 0; i < std::min(MAX_DEVICE_COUNT, (int)depthFrames.size()); i++) {
                    if(depthFrames[i] != nullptr && colorFrames[i] != nullptr) {
                        framePairs.emplace_back(depthFrames[i], colorFrames[i]);
                    }
                }
            }
            auto groudID = 0;
            for(const auto &pair: framePairs) {
                groudID++;
                win.pushFramesToView({ pair.first, pair.second }, groudID);
            }
        }

        // Stop streams and clear resources
        for(auto &holder: pipelineHolderList) {
            stopStream(holder);
        }
        pipelineHolderList.clear();
        depthFrames.clear();
        colorFrames.clear();      

        // Release resource
        streamDevList.clear();
        configDevList.clear();
        deviceConfigList.clear();
        return 0;
    }
    catch(ob::Error &e) {
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
        std::cout << "\nPress any key to exit.";
        ob_smpl::waitForKeyPressed();
        exit(EXIT_FAILURE);
        return -1;
    }
}

std::shared_ptr<PipelineHolder> createPipelineHolder(std::shared_ptr<ob::Device> device, OBSensorType sensorType, int deviceIndex) {
    auto holder         = std::make_shared<PipelineHolder>();
    holder->pipeline    = std::make_shared<ob::Pipeline>(device);
    holder->sensorType  = sensorType;
    holder->deviceIndex = deviceIndex;
    holder->deviceSN    = device->getDeviceInfo()->serialNumber();
    return holder;
}

void processFrame(std::shared_ptr<ob::FrameSet> frameSet, OBFrameType frameType, int deviceIndex) {
    if(!frameSet) {
        std::cerr << "Invalid frameSet received." << std::endl;
        return;
    }

    if(quitStreamPreview){
        //std::cerr << "press ESC quit Stream ProcessFrame." << std::endl;
        return;
    }

    auto frame = frameSet->getFrame(frameType);
    if(frame) {
        if(frameType == OB_FRAME_COLOR) {
            handleColorStream(deviceIndex, frame);
        }
        else if(frameType == OB_FRAME_DEPTH) {
            handleDepthStream(deviceIndex, frame);
        }
    }
}

void handleStreamError(const ob::Error &e) {
    // Separate error handling logic
    std::cerr << "Function: " << e.getName() << "\nArgs: " << e.getArgs() << "\nMessage: " << e.getMessage() << "\nType: " << e.getExceptionType() << std::endl;
}

void startStream(std::shared_ptr<PipelineHolder> holder) {
    std::cout << "startStream. " << holder << std::endl;
    try {
        auto pipeline      = holder->pipeline;
        auto profileList   = pipeline->getStreamProfileList(holder->sensorType);
        auto streamProfile = profileList->getProfile(OB_PROFILE_DEFAULT)->as<ob::VideoStreamProfile>();
        auto frameType     = mapFrameType(holder->sensorType);
        auto deviceIndex   = holder->deviceIndex;

        std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
        config->enableStream(streamProfile);

        pipeline->start(config, [frameType, deviceIndex](std::shared_ptr<ob::FrameSet> frameSet) { processFrame(frameSet, frameType, deviceIndex); });
    }
    catch(ob::Error &e) {
        std::cerr << "starting stream failed: " << holder << std::endl;
        handleStreamError(e);
    }
}

void stopStream(std::shared_ptr<PipelineHolder> holder) {
    try {
        std::cout << "stopStream " << holder << std::endl;
        holder->pipeline->stop();
    }
    catch(ob::Error &e) {
        std::cerr << "stopping stream failed: " << holder << std::endl;
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    }
}

void handleStream(int devIndex, std::shared_ptr<ob::Frame> frame, const char *frameType) {
    std::lock_guard<std::mutex> lock(frameMutex);
    std::cout << "Device#" << devIndex << ", " << frameType << " frame "
              << ", frame timestamp=" << frame->timeStamp() << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

    if(strcmp(frameType, "color") == 0) {
        colorFrames[devIndex] = frame;
    }
    else if(strcmp(frameType, "depth") == 0) {
        depthFrames[devIndex] = frame;
    }
}

void handleColorStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
    handleStream(devIndex, frame, "color");
}

void handleDepthStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
    handleStream(devIndex, frame, "depth");
}
std::string readFileContent(const char *filePath) {
    std::ostringstream oss;
    std::ifstream      file(filePath, std::fstream::in);
    if(!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }
    oss << file.rdbuf();
    file.close();
    return oss.str();
}

bool loadConfigFile() {
    int                               deviceCount   = 0;
    std::shared_ptr<DeviceConfigInfo> devConfigInfo = nullptr;
    cJSON *                           deviceElem    = nullptr;

    auto content = readFileContent(CONFIG_FILE);
    if(content.empty()) {
        std::cerr << "load config file failed." << std::endl;
        return false;
    }

    cJSON *rootElem = cJSON_Parse(content.c_str());
    if(rootElem == nullptr) {
        const char *errMsg = cJSON_GetErrorPtr();
        std::cout << std::string(errMsg) << std::endl;
        cJSON_Delete(rootElem);
        return true;
    }

    cJSON *devicesElem = cJSON_GetObjectItem(rootElem, "devices");
    cJSON_ArrayForEach(deviceElem, devicesElem) {
        devConfigInfo = std::make_shared<DeviceConfigInfo>();
        memset(&devConfigInfo->syncConfig, 0, sizeof(devConfigInfo->syncConfig));
        devConfigInfo->syncConfig.syncMode = OB_MULTI_DEVICE_SYNC_MODE_FREE_RUN;

        cJSON *snElem = cJSON_GetObjectItem(deviceElem, "sn");
        if(cJSON_IsString(snElem) && snElem->valuestring != nullptr) {
            devConfigInfo->deviceSN = std::string(snElem->valuestring);
        }
        cJSON *deviceConfigElem = cJSON_GetObjectItem(deviceElem, "syncConfig");
        if(cJSON_IsObject(deviceConfigElem)) {
            cJSON *numberElem = nullptr;
            cJSON *strElem    = nullptr;
            cJSON *bElem      = nullptr;
            strElem           = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "syncMode");
            if(cJSON_IsString(strElem) && strElem->valuestring != nullptr) {
                devConfigInfo->syncConfig.syncMode = stringToOBSyncMode(strElem->valuestring);
                std::cout << "config[" << (deviceCount++) << "]: SN=" << std::string(devConfigInfo->deviceSN) << ", mode=" << strElem->valuestring << std::endl;
            }
            numberElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "depthDelayUs");
            if(cJSON_IsNumber(numberElem)) {
                devConfigInfo->syncConfig.depthDelayUs = numberElem->valueint;
            }
            numberElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "colorDelayUs");
            if(cJSON_IsNumber(numberElem)) {
                devConfigInfo->syncConfig.colorDelayUs = numberElem->valueint;
            }
            numberElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "trigger2ImageDelayUs");
            if(cJSON_IsNumber(numberElem)) {
                devConfigInfo->syncConfig.trigger2ImageDelayUs = numberElem->valueint;
            }
            numberElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "triggerOutDelayUs");
            if(cJSON_IsNumber(numberElem)) {
                devConfigInfo->syncConfig.triggerOutDelayUs = numberElem->valueint;
            }
            bElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "triggerOutEnable");
            if(cJSON_IsBool(bElem)) {
                devConfigInfo->syncConfig.triggerOutEnable = (bool)bElem->valueint;
            }
            bElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "framesPerTrigger");
            if(cJSON_IsNumber(bElem)) {
                devConfigInfo->syncConfig.framesPerTrigger = bElem->valueint;
            }
        }
        cJSON *hardwareTriggerFpsElem = nullptr;
        hardwareTriggerFpsElem        = cJSON_GetObjectItemCaseSensitive(rootElem, "hardwareTriggerFps");
        if(cJSON_IsNumber(hardwareTriggerFpsElem)) {
            hardwareTriggerFps = hardwareTriggerFpsElem->valueint;
            std::cout << "From json get HardwareTriggerFps=" << hardwareTriggerFps << std::endl;
        }

        if(OB_MULTI_DEVICE_SYNC_MODE_FREE_RUN != devConfigInfo->syncConfig.syncMode) {
            deviceConfigList.push_back(devConfigInfo);
        }
        else {
            std::cerr << "Invalid sync mode of deviceSN: " << devConfigInfo->deviceSN << std::endl;
        }
        devConfigInfo = nullptr;
    }
    cJSON_Delete(rootElem);
    return true;
}

OBMultiDeviceSyncMode stringToOBSyncMode(const std::string &modeString) {
    static const std::unordered_map<std::string, OBMultiDeviceSyncMode> syncModeMap = {
        { "OB_MULTI_DEVICE_SYNC_MODE_FREE_RUN", OB_MULTI_DEVICE_SYNC_MODE_FREE_RUN },
        { "OB_MULTI_DEVICE_SYNC_MODE_STANDALONE", OB_MULTI_DEVICE_SYNC_MODE_STANDALONE },
        { "OB_MULTI_DEVICE_SYNC_MODE_PRIMARY", OB_MULTI_DEVICE_SYNC_MODE_PRIMARY },
        { "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY", OB_MULTI_DEVICE_SYNC_MODE_SECONDARY },
        { "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY_SYNCED", OB_MULTI_DEVICE_SYNC_MODE_SECONDARY_SYNCED },
        { "OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING", OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING },
        { "OB_MULTI_DEVICE_SYNC_MODE_HARDWARE_TRIGGERING", OB_MULTI_DEVICE_SYNC_MODE_HARDWARE_TRIGGERING }
    };
    auto it = syncModeMap.find(modeString);
    if(it != syncModeMap.end()) {
        return it->second;
    }
    // Constructing exception messages with stringstream
    std::stringstream ss;
    ss << "Unrecognized sync mode: " << modeString;
    throw std::invalid_argument(ss.str());
}

std::string OBSyncModeToString(const OBMultiDeviceSyncMode syncMode) {
    static const std::unordered_map<OBMultiDeviceSyncMode, std::string> modeToStringMap = {
        { OB_MULTI_DEVICE_SYNC_MODE_FREE_RUN, "OB_MULTI_DEVICE_SYNC_MODE_FREE_RUN" },
        { OB_MULTI_DEVICE_SYNC_MODE_STANDALONE, "OB_MULTI_DEVICE_SYNC_MODE_STANDALONE" },
        { OB_MULTI_DEVICE_SYNC_MODE_PRIMARY, "OB_MULTI_DEVICE_SYNC_MODE_PRIMARY" },
        { OB_MULTI_DEVICE_SYNC_MODE_SECONDARY, "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY" },
        { OB_MULTI_DEVICE_SYNC_MODE_SECONDARY_SYNCED, "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY_SYNCED" },
        { OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING, "OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING" },
        { OB_MULTI_DEVICE_SYNC_MODE_HARDWARE_TRIGGERING, "OB_MULTI_DEVICE_SYNC_MODE_HARDWARE_TRIGGERING" }
    };

    auto it = modeToStringMap.find(syncMode);
    if(it != modeToStringMap.end()) {
        return it->second;
    }
    std::stringstream ss;
    ss << "Unmapped sync mode value: " << static_cast<int>(syncMode) << ". Please check the sync mode value.";
    throw std::invalid_argument(ss.str());
}

int strcmp_nocase(const char *str0, const char *str1) {
    return strcasecmp(str0, str1);
}

OBFrameType mapFrameType(OBSensorType sensorType) {
    switch(sensorType) {
    case OB_SENSOR_COLOR:
        return OB_FRAME_COLOR;
    case OB_SENSOR_IR:
        return OB_FRAME_IR;
    case OB_SENSOR_IR_LEFT:
        return OB_FRAME_IR_LEFT;
    case OB_SENSOR_IR_RIGHT:
        return OB_FRAME_IR_RIGHT;
    case OB_SENSOR_DEPTH:
        return OB_FRAME_DEPTH;
    default:
        return OBFrameType::OB_FRAME_UNKNOWN;
    }
}

std::ostream &operator<<(std::ostream &os, const PipelineHolder &holder) {
    if(os.good()) {
        os << "deviceSN: " << holder.deviceSN << ", sensorType: ";
        switch(holder.sensorType) {
        case OB_SENSOR_COLOR:
            os << "OB_SENSOR_COLOR";
            break;
        case OB_SENSOR_DEPTH:
            os << "OB_SENSOR_DEPTH";
            break;
        default:
            os << static_cast<int>(holder.sensorType);
            break;
        }
        os << ", deviceIndex: " << holder.deviceIndex;
    }
    return os;
}
