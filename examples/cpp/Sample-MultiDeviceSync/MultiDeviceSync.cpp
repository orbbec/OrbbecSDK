#include <algorithm>
#include <condition_variable>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "../window.hpp"
#include "cJSON.h"
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#else
#include <strings.h>
#endif

#define MAX_DEVICE_COUNT 9
#define CONFIG_FILE "./MultiDeviceSyncConfig.json"

typedef struct DeviceConfigInfo_t {
  std::string deviceSN;
  OBDeviceSyncConfig syncConfig;
  bool triggleSignalOutEnable;
} DeviceConfigInfo;

std::mutex frameMutex;
std::map<uint8_t, std::shared_ptr<ob::Frame>> colorFrames;
std::map<uint8_t, std::shared_ptr<ob::Frame>> depthFrames;

std::vector<std::shared_ptr<ob::Device>> streamDevList;
std::vector<std::shared_ptr<ob::Device>> configDevList;
std::vector<std::shared_ptr<DeviceConfigInfo>> deviceConfigList;

std::condition_variable waitRebootCompleteCondition;
std::mutex rebootingDevInfoListMutex;
std::vector<std::shared_ptr<ob::DeviceInfo>> rebootingDevInfoList;

OBSyncMode textToOBSyncMode(const char *text);
std::string readFileContent(const char *filePath);
bool loadConfigFile();
int configMultiDeviceSync();
int testMultiDeviceSync();
bool checkDevicesWithDeviceConfigs(
    const std::vector<std::shared_ptr<ob::Device>> &deviceList);
int strcmp_nocase(const char *str0, const char *str1);

void startStream(std::vector<std::shared_ptr<ob::Device>> devices,
                 OBSensorType sensorType, int deviceIndexBase = 0);
void stopStream(std::vector<std::shared_ptr<ob::Device>> devices,
                OBSensorType sensorType);

void handleColorStream(int devIndex, std::shared_ptr<ob::Frame> frame);
void handleDepthStream(int devIndex, std::shared_ptr<ob::Frame> frame);

ob::Context context;

void wait_any_key() { system("pause"); }

int main(int argc, char **argv) {
  std::cout << "Please select options: " << std::endl;
  std::cout << " 0 --> config devices" << std::endl;
  std::cout << " 1 --> start stream" << std::endl;
  std::cout << "input: ";
  int index = -1;
  std::cin >> index;
  std::cout << std::endl;

  int exitValue = -1;
  if (index == 0) {
    exitValue = configMultiDeviceSync();
    // Only after the configuration is successful, the follow-up test is allowed
    // to continue
    if (exitValue == 0) {
      exitValue = testMultiDeviceSync();
    }
  } else if (index == 1) {
    exitValue = testMultiDeviceSync();
  } else {
    std::cout << "invalid index. " << std::endl;
    std::cout << "Please press any key to exit" << std::endl;
  }

  if (exitValue != 0) {
    wait_any_key();
  }

  return exitValue;
}

int configMultiDeviceSync() try {
  if (!loadConfigFile()) {
    std::cout << "load config failed" << std::endl;
    return -1;
  }

  if (deviceConfigList.empty()) {
    std::cout << "DeviceConfigList is empty. please check config file: "
              << CONFIG_FILE << std::endl;
    return -1;
  }

  // Query the list of connected devices
  auto devList = context.queryDeviceList();

  // Get the number of connected devices
  int devCount = devList->deviceCount();
  for (int i = 0; i < devCount; i++) {
    configDevList.push_back(devList->getDevice(i));
  }

  if (configDevList.empty()) {
    std::cerr << "Device list is empty. please check device connection state"
              << std::endl;
    return -1;
  }

  // write configuration to device
  for (auto config : deviceConfigList) {
    auto findItr = std::find_if(
        configDevList.begin(), configDevList.end(),
        [config](std::shared_ptr<ob::Device> device) {
          auto serialNumber = device->getDeviceInfo()->serialNumber();
          return strcmp_nocase(serialNumber, config->deviceSN.c_str()) == 0;
        });
    if (findItr != configDevList.end()) {
      auto device = (*findItr);
      auto curConfig = device->getSyncConfig();

      // Update the configuration items of the configuration file, and keep the
      // original configuration for other items
      curConfig.syncMode = config->syncConfig.syncMode;
      curConfig.irTriggerSignalInDelay =
          config->syncConfig.irTriggerSignalInDelay;
      curConfig.rgbTriggerSignalInDelay =
          config->syncConfig.rgbTriggerSignalInDelay;
      curConfig.deviceTriggerSignalOutDelay =
          config->syncConfig.deviceTriggerSignalOutDelay;
      curConfig.deviceId = config->syncConfig.deviceId;

      device->setSyncConfig(curConfig);

      // Gemini2须设置同步信号外部触发使能，星型链接时设置为false，链型链接时设置为true
      if (device->isPropertySupported(OB_PROP_SYNC_SIGNAL_TRIGGLE_OUT_BOOL,
                                      OB_PERMISSION_READ_WRITE)) {
        device->setBoolProperty(OB_PROP_SYNC_SIGNAL_TRIGGLE_OUT_BOOL,
                                config->triggleSignalOutEnable);
      }
    }
  }

  // Reboot the device
  for (auto device : configDevList) {
    rebootingDevInfoList.push_back(device->getDeviceInfo());
    std::cout << "Device sn["
              << std::string(device->getDeviceInfo()->serialNumber())
              << "] is configured, rebooting..." << std::endl;
    try {
      device->reboot();
    } catch (ob::Error &e) {
      std::cout << "Device sn["
                << std::string(device->getDeviceInfo()->serialNumber())
                << "] is not configured, skipping...";
      // The early firmware versions of some models of devices will restart
      // immediately after receiving the restart command, causing the SDK to
      // fail to receive a response to the command request and throw an
      // exception
    }
  }
  configDevList.clear();

  // Register device change monitoring, used to assist the monitoring device to
  // reconnect after restarting
  context.setDeviceChangedCallback(
      [&](std::shared_ptr<ob::DeviceList> removedList,
          std::shared_ptr<ob::DeviceList> addedList) {
        if (addedList && addedList->deviceCount() > 0) {
          auto deviceCount = addedList->deviceCount();
          for (uint32_t i = 0; i < deviceCount; i++) {
            auto device = addedList->getDevice(i);
            auto deviceInfo = device->getDeviceInfo();
            std::cout << "addedList sn: "
                      << std::string(deviceInfo->serialNumber()) << std::endl;
            auto findItr = std::find_if(
                rebootingDevInfoList.begin(), rebootingDevInfoList.end(),
                [&deviceInfo](std::shared_ptr<ob::DeviceInfo> tmp) {
                  return strcmp_nocase(tmp->serialNumber(),
                                       deviceInfo->serialNumber()) == 0;
                });

            std::lock_guard<std::mutex> lk(rebootingDevInfoListMutex);
            if (findItr != rebootingDevInfoList.end()) {
              rebootingDevInfoList.erase(findItr);
              std::cout << "Device sn["
                        << std::string(deviceInfo->serialNumber())
                        << "] reboot complete." << std::endl;

              if (rebootingDevInfoList.empty()) {
                waitRebootCompleteCondition.notify_all();
              }
            }
          }
        }
      });

  // wait for the device to restart
  {
    // wait 60s
    std::unique_lock<std::mutex> lk(rebootingDevInfoListMutex);
    waitRebootCompleteCondition.wait_for(
        lk, std::chrono::milliseconds(60000),
        [&]() { return rebootingDevInfoList.empty(); });

    // device restart failed
    if (!rebootingDevInfoList.empty()) {
      std::cerr << "Device not found after reboot. not found deviceCount: "
                << rebootingDevInfoList.size() << std::endl;
      for (auto devInfo : rebootingDevInfoList) {
        std::cout << "not found deviceSN: "
                  << std::string(devInfo->serialNumber()) << std::endl;
      }
      return -1;
    }

    // restart successfully
    std::cout << "All device update config and reboot complete." << std::endl;
  }

  // Logout callback to avoid affecting the next test multi-machine
  // synchronization
  context.setDeviceChangedCallback(nullptr);

  return 0;
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  wait_any_key();
  exit(EXIT_FAILURE);
}

int testMultiDeviceSync() try {
  streamDevList.clear();
  // Query the list of connected devices
  auto devList = context.queryDeviceList();

  // Get the number of connected devices
  int devCount = devList->deviceCount();
  for (int i = 0; i < devCount; i++) {
    streamDevList.push_back(devList->getDevice(i));
  }

  if (streamDevList.empty()) {
    std::cerr << "Device list is empty. please check device connection state"
              << std::endl;
    return -1;
  }

  // traverse the device list and create the device
  std::vector<std::shared_ptr<ob::Device>> primary_devices;
  std::vector<std::shared_ptr<ob::Device>> secondary_devices;
  for (auto dev : streamDevList) {
    auto config = dev->getSyncConfig();
    if (config.syncMode == OB_SYNC_MODE_PRIMARY ||
        config.syncMode == OB_SYNC_MODE_PRIMARY_MCU_TRIGGER ||
        config.syncMode == OB_SYNC_MODE_PRIMARY_IR_TRIGGER ||
        config.syncMode == OB_SYNC_MODE_PRIMARY_SOFT_TRIGGER) {
      primary_devices.push_back(dev);
    } else {
      secondary_devices.push_back(dev);
    }
  }

  if (primary_devices.empty()) {
    std::cerr << "WARNING primary_devices is empty!!!" << std::endl;
  }

  // Start the multi-device time synchronization function
  context.enableMultiDeviceSync(60000);  // update and sync every minute

  std::cout << "Secondary devices start..." << std::endl;
  startStream(secondary_devices, OB_SENSOR_DEPTH);
  startStream(secondary_devices, OB_SENSOR_COLOR);

  // Delay and wait for 5s to ensure that the initialization of the slave device
  // is completed
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));

  std::cout << "Primary device start..." << std::endl;
  startStream(primary_devices, OB_SENSOR_DEPTH, secondary_devices.size());
  startStream(primary_devices, OB_SENSOR_COLOR, secondary_devices.size());

  // Create a window for rendering and set the resolution of the window
  Window app("MultiDeviceViewer", 1600, 900);
  app.setShowInfo(true);

  while (app) {
    // Get the key value of the key event
    auto key = app.getKey();

    if (key == 'S' || key == 's') {
      std::cout << "syncDevicesTime..." << std::endl;
      context.enableMultiDeviceSync(60000);  // Manual update synchronization
    }

    std::vector<std::shared_ptr<ob::Frame>> framesVec;
    {
      std::lock_guard<std::mutex> lock(frameMutex);
      for (int i = 0; i < MAX_DEVICE_COUNT; i++) {
        if (depthFrames[i] != nullptr) {
          framesVec.emplace_back(depthFrames[i]);
        }
        if (colorFrames[i] != nullptr) {
          framesVec.emplace_back(colorFrames[i]);
        }
      }
    }
    // Render a set of frame in the window, where the depth and color frames of
    // all devices will be rendered. RENDER_GRID means to render all frames in a
    // grid
    app.render(framesVec, RENDER_GRID);
  }

  // close data stream
  stopStream(primary_devices, OB_SENSOR_COLOR);
  stopStream(primary_devices, OB_SENSOR_DEPTH);

  stopStream(secondary_devices, OB_SENSOR_COLOR);
  stopStream(secondary_devices, OB_SENSOR_DEPTH);

  std::lock_guard<std::mutex> lock(frameMutex);
  depthFrames.clear();
  colorFrames.clear();

  return 0;
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  wait_any_key();
  exit(EXIT_FAILURE);
}

void startStream(std::vector<std::shared_ptr<ob::Device>> devices,
                 OBSensorType sensorType, int deviceIndexBase) {
  for (auto &&dev : devices) {
    // get camera list
    auto sensorList = dev->getSensorList();
    for (uint32_t i = 0; i < sensorList->count(); i++) {
      auto sensor = sensorList->getSensor(i);
      if (sensorType == sensor->type()) {
        auto profiles = sensor->getStreamProfileList();
        auto profile = profiles->getProfile(0);
        switch (sensorType) {
          case OB_SENSOR_DEPTH:
            if (profile) {
              sensor->start(
                  profile, [deviceIndexBase](std::shared_ptr<ob::Frame> frame) {
                    handleDepthStream(deviceIndexBase, frame);
                  });
            }
            break;
          case OB_SENSOR_COLOR:
            if (profile) {
              sensor->start(
                  profile, [deviceIndexBase](std::shared_ptr<ob::Frame> frame) {
                    handleColorStream(deviceIndexBase, frame);
                  });
            }
            break;
          default:
            break;
        }
      }
    }
    deviceIndexBase++;
  }
}

void stopStream(std::vector<std::shared_ptr<ob::Device>> devices,
                OBSensorType sensorType) {
  for (auto &&dev : devices) {
    // get camera list
    auto sensorList = dev->getSensorList();
    for (uint32_t i = 0; i < sensorList->count(); i++) {
      if (sensorList->type(i) == sensorType) {
        sensorList->getSensor(i)->stop();
        break;
      }
    }
  }
}

void handleColorStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
  std::cout << "Device#" << devIndex << ", color frame index=" << frame->index()
            << ", timestamp=" << frame->timeStamp()
            << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

  std::lock_guard<std::mutex> lock(frameMutex);
  colorFrames[devIndex] = frame;
}

void handleDepthStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
  std::cout << "Device#" << devIndex << ", depth frame index=" << frame->index()
            << ", timestamp=" << frame->timeStamp()
            << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

  std::lock_guard<std::mutex> lock(frameMutex);
  depthFrames[devIndex] = frame;
}

std::string readFileContent(const char *filePath) {
  std::ostringstream oss;

  long length = 0;
  long readSum = 0;
  int readSize = 0;
  char buf[512];
  bool isOpened = false;
  bool success = false;
  std::ifstream file;
  file.exceptions(std::fstream::badbit | std::fstream::failbit);
  try {
    file.open(filePath, std::fstream::in);
    isOpened = true;
    file.seekg(0, std::fstream::end);
    length = file.tellg();
    file.seekg(0, std::fstream::beg);

    while (!file.eof() && readSum < length) {
      readSize = (std::min)((long)512, length - readSum);
      file.read(buf, readSize);
      if (file.gcount() > 0) {
        oss << std::string(buf, file.gcount());
        readSum += file.gcount();
      }
    }
    success = true;
  } catch (std::fstream::failure e) {
    if ((file.rdstate() & std::fstream::failbit) != 0 &&
        (file.rdstate() & std::fstream::eofbit) != 0) {
      if (readSize > 0 && file.gcount() > 0) {
        oss << std::string(buf, file.gcount());
        readSum += file.gcount();
      }
      success = true;
    } else {
      std::string errorMsg = (nullptr != e.what() ? std::string(e.what()) : "");
      std::cerr << "open or reading file: " << std::string(filePath)
                << ", errorMsg: " << errorMsg << std::endl;
    }
  }

  if (isOpened) {
    try {
      file.close();
    } catch (std::fstream::failure e) {
      std::string errorMsg = (nullptr != e.what() ? std::string(e.what()) : "");
      std::cerr << "close file: " << std::string(filePath)
                << ", errorMsg: " << errorMsg << std::endl;
    }
  }

  return success ? oss.str() : "";
}

bool loadConfigFile() {
  auto content = readFileContent(CONFIG_FILE);
  if (content.empty()) {
    std::cerr << "load config file failed." << std::endl;
    return false;
  }

  int deviceCount = 0;

  cJSON *rootElem = cJSON_Parse(content.c_str());
  if (rootElem == nullptr) {
    const char *errMsg = cJSON_GetErrorPtr();
    std::cout << std::string(errMsg) << std::endl;
    cJSON_Delete(rootElem);
    return true;
  }

  std::shared_ptr<DeviceConfigInfo> devConfigInfo = nullptr;
  cJSON *deviceElem = nullptr;
  cJSON *devicesElem = cJSON_GetObjectItem(rootElem, "devices");
  cJSON_ArrayForEach(deviceElem, devicesElem) {
    devConfigInfo = std::make_shared<DeviceConfigInfo>();
    memset(&devConfigInfo->syncConfig, 0, sizeof(devConfigInfo->syncConfig));
    devConfigInfo->syncConfig.syncMode = OB_SYNC_MODE_UNKNOWN;

    cJSON *snElem = cJSON_GetObjectItem(deviceElem, "sn");
    if (cJSON_IsString(snElem) && snElem->valuestring != nullptr) {
      devConfigInfo->deviceSN = std::string(snElem->valuestring);
    }

    cJSON *deviceConfigElem = cJSON_GetObjectItem(deviceElem, "syncConfig");
    if (cJSON_IsObject(deviceConfigElem)) {
      cJSON *numberElem = nullptr;
      cJSON *strElem = nullptr;
      cJSON *bElem = nullptr;
      strElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "syncMode");
      if (cJSON_IsString(strElem) && strElem->valuestring != nullptr) {
        devConfigInfo->syncConfig.syncMode =
            textToOBSyncMode(strElem->valuestring);
        std::cout << "config[" << (deviceCount++)
                  << "]: SN=" << std::string(devConfigInfo->deviceSN)
                  << ", mode=" << strElem->valuestring << std::endl;
      }

      numberElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem,
                                                    "irTriggerSignalInDelay");
      if (cJSON_IsNumber(numberElem)) {
        devConfigInfo->syncConfig.irTriggerSignalInDelay = numberElem->valueint;
      }

      numberElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem,
                                                    "rgbTriggerSignalInDelay");
      if (cJSON_IsNumber(numberElem)) {
        devConfigInfo->syncConfig.rgbTriggerSignalInDelay =
            numberElem->valueint;
      }

      numberElem = cJSON_GetObjectItemCaseSensitive(
          deviceConfigElem, "deviceTriggerSignalOutDelay");
      if (cJSON_IsNumber(numberElem)) {
        devConfigInfo->syncConfig.deviceTriggerSignalOutDelay =
            numberElem->valueint;
      }

      bElem = cJSON_GetObjectItemCaseSensitive(deviceConfigElem,
                                               "triggleSignalOutEnable");
      if (cJSON_IsBool(bElem)) {
        devConfigInfo->triggleSignalOutEnable = (bool)bElem->valueint;
      }

      numberElem =
          cJSON_GetObjectItemCaseSensitive(deviceConfigElem, "deviceId");
      if (cJSON_IsNumber(numberElem)) {
        devConfigInfo->syncConfig.deviceId = numberElem->valueint;
      }
    }

    if (OB_SYNC_MODE_UNKNOWN != devConfigInfo->syncConfig.syncMode) {
      deviceConfigList.push_back(devConfigInfo);
    } else {
      std::cerr << "invalid sync mode of deviceSN: " << devConfigInfo->deviceSN
                << std::endl;
    }

    devConfigInfo = nullptr;
  }

  cJSON_Delete(rootElem);
  return true;
}

OBSyncMode textToOBSyncMode(const char *text) {
  if (strcmp(text, "OB_SYNC_MODE_CLOSE") == 0) {
    return OB_SYNC_MODE_CLOSE;
  } else if (strcmp(text, "OB_SYNC_MODE_STANDALONE") == 0) {
    return OB_SYNC_MODE_STANDALONE;
  } else if (strcmp(text, "OB_SYNC_MODE_PRIMARY") == 0) {
    return OB_SYNC_MODE_PRIMARY;
  } else if (strcmp(text, "OB_SYNC_MODE_SECONDARY") == 0) {
    return OB_SYNC_MODE_SECONDARY;
  } else if (strcmp(text, "OB_SYNC_MODE_PRIMARY_MCU_TRIGGER") == 0) {
    return OB_SYNC_MODE_PRIMARY_MCU_TRIGGER;
  } else if (strcmp(text, "OB_SYNC_MODE_PRIMARY_IR_TRIGGER") == 0) {
    return OB_SYNC_MODE_PRIMARY_IR_TRIGGER;
  } else if (strcmp(text, "OB_SYNC_MODE_PRIMARY_SOFT_TRIGGER") == 0) {
    return OB_SYNC_MODE_PRIMARY_SOFT_TRIGGER;
  } else if (strcmp(text, "OB_SYNC_MODE_SECONDARY_SOFT_TRIGGER") == 0) {
    return OB_SYNC_MODE_SECONDARY_SOFT_TRIGGER;
  } else {
    return OB_SYNC_MODE_UNKNOWN;
  }
}

int strcmp_nocase(const char *str0, const char *str1) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  return _strcmpi(str0, str1);
#else
  return strcasecmp(str0, str1);
#endif
}
