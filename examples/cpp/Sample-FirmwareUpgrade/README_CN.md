# 固件升级示例

功能描述：本示例演示如何用固件文件给设备升级。

本示例基于C++ Low Level API进行演示

## 1. 在main函数接口通过命令参数获取固件文件
```cpp
    // checkFirmwareFilePath()函数用于检查文件是否存在，实际代码中最好检查后缀是否为bin或者img, 以及固件文件是否与目标设备相匹配
    std::string checkFirmwareFilePath(int argc, char **argv) {
        if(argc < 2) {
            std::cout << "Please input firmware path." << std::endl;
            return "";
        }
    
        std::string   filePath = std::string(*(argv + 1));
        std::ifstream fs(filePath);
        if(!fs.is_open()) {
            std::cout << "Open Firmware file failed. filePath: " << filePath << std::endl;
            return "";
        }
    
        fs.close();
        return filePath;
    }
    
    int main(int argc, char **argv) try {
        std::string firmwareFilePath = checkFirmwareFilePath(argc, argv);
        if(firmwareFilePath.empty()) {
            std::cout << "command: " << std::endl << "$ ./FirmwareUpgrade[.exe] firmwareFile.bin" << std::endl;
            return 0;
        }
    
        // 接下来的业务代码
    
        return 0;
    }
    catch(ob::Error &e) {
        // 处理OrbbecSDK接口调用异常，示例为了简洁连续几个接口一起try-catch，实际业务中推荐单独一个接口一个try-catch
        std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
        exit(EXIT_FAILURE);
    }
```
创建ob::Context并通过ob::Context获取设备，本示例假设运行FirmwareUpgrade\[.exe\]之前上位机（Windows、Ubuntu、Android平台）已经插入设备。ob::DeviceChangedCallback用于固件升级后监听device重启后获取被升级设备的业务处理
```cpp
    // 构建ob::Context对象
    ob::Context ctx;
    // 设置设备变化监听器，device_changed_callback是管理device声明周期的关键函数，开发者必须关注该回调
    ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> addedList) {
        if(isWaitRebootComplete_) {
            if(addedList && addedList->deviceCount() > 0) {
                auto device = addedList->getDevice(0);
                if(isDeviceRemoved_ && deviceSN_ == std::string(device->getDeviceInfo()->serialNumber())) {
                    rebootedDevice_       = device;
                    isWaitRebootComplete_ = false;
    
                    std::unique_lock<std::mutex> lk(waitRebootMutex_);
                    waitRebootCondition_.notify_all();
                }
            }
    
            if(removedList && removedList->deviceCount() > 0) {
                if(deviceUid_ == std::string(removedList->uid(0))) {
                    isDeviceRemoved_ = true;
                }
            }
        }  // if isWaitRebootComplete_
    });
    
    // 查询当前已经接入的设备
    auto devList = ctx.queryDeviceList();
    
    // 从ob::DeviceList中获取当前接入设备的数量
    if(devList->deviceCount() == 0) {
         // 固件升级示例默认设备已经接入到上位机（Windows、Ubuntu、Android平台）
        std::cerr << "Device not found!" << std::endl;
        return -1;
    }
    
    // 获取第一个设备，index=0
    auto dev = devList->getDevice(0);
    // 打印设备信息
    dumpDeviceInfo(dev);
```
## 2. 获取设备当前的固件版本信息
```cpp
    // 打印设备名称，SN，VID，PID以及固件版本
    void dumpDeviceInfo(std::shared_ptr<ob::Device> device) {
        // 获取ob::DeviceInfo对象，通过ob::DeviceInfo可以获取目标设备的基本信息
        auto devInfo = device->getDeviceInfo();
    
        // 设备名称
        std::cout << "Device name: " << devInfo->name() << std::endl;
    
        // 设备VID，PID，UID
        std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;
    
        // 设备当前的固件版本号
        auto fwVer = devInfo->firmwareVersion();
        std::cout << "Firmware version: " << fwVer << std::endl;
    
        // 设备SN
        auto sn = devInfo->serialNumber();
        std::cout << "Serial number: " << sn << std::endl;
    
        // devInfo资源会自动释放
```    

给目标设备升级固件
a. 实现C API的固件升级回调接口；
b. 调用固件升级接口进行升级；
```cpp
    // 对目标设备进行固件升级
    bool upgradeFirmware(std::shared_ptr<ob::Device> device, std::string firmwarePath) {
        auto index     = firmwarePath.find_last_of(".img");
        bool isImgFile = index != std::string::npos;
        index          = firmwarePath.find_last_of(".bin");
        bool isBinFile = index != std::string::npos;
        if(!(isImgFile || isBinFile)) {
            // 固件升级文件一般为bin或者img，实际业务中最好通过文件名称、文件MD5等信息做防呆
            std::cout << "Upgrade Fimware failed. invalid firmware file: " << firmwarePath << std::endl;
            return false;
        }
    
        bool isUpgradeSuccess = false;
        try {
            // 调用固件升级接口进行升级；
            device->deviceUpgrade(
                firmwarePath.c_str(),
                [=, &isUpgradeSuccess](OBUpgradeState state, const char *message, uint8_t percent) {
                    if(state == STAT_START) {
                        std::cout << "Upgrade Firmware start" << std::endl;
                    }
                    else if(state == STAT_FILE_TRANSFER) {
                        std::cout << "Upgrade Firmware file transfer, percent: " << (uint32_t)percent << std::endl;
                    }
                    else if(state == STAT_IN_PROGRESS) {
                        std::cout << "Upgrade Firmware in progress, percent: " << (uint32_t)percent << std::endl;
                    }
                    else if(state == STAT_DONE) {
                        // 固件升级成功
                        std::cout << "Upgrade Firmware done, percent: " << (uint32_t)percent << std::endl;
                        isUpgradeSuccess = true;
                    }
                    else if(state == STAT_VERIFY_IMAGE) {
                        std::cout << "Upgrade Firmware verify image" << std::endl;
                    }
                    else {
                        // 固件升级失败
                        std::string errMsg = (nullptr != message ? std::string(message) : "");
                        std::cout << "Upgrade Firmware failed. state: " << std::to_string(state) << ", errMsg: " << errMsg << ", percent: " << (uint32_t)percent
                                  << std::endl;
                    }
                },
                false);
        }
        catch(ob::Error &e) {
            std::cerr << "Upgrade Firmware ob error. function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage()
                      << "\ntype:" << e.getExceptionType() << std::endl;
        }
        catch(std::exception &e) {
            if(e.what()) {
                std::cout << "Upgrade Firmware Exception. what: " << std::string(e.what()) << std::endl;
            }
        }
    
        return isUpgradeSuccess;
    }
```
固件升级成功后，需要重启设备。重启设备有两种方式，一种是拔插设备（或者操作系统重启），另一种是调用OrbbecSDK的reboot接口。等设备上线后，通过本示例的dump\_device\_info()函数可以查询当前设备固件版本信息。

以下示例演示通过固件reboot接口重启设备
```cpp
    // 重启设备
    std::cout << "Reboot device" << std::endl;
    isDeviceRemoved_      = false;
    isWaitRebootComplete_ = true;
    dev->reboot();
    // 释放资源，设备重启后ob::Device对象就不能用了
    dev     = nullptr;
```
程序正常退出之后资源将会自动释放

## 3.预期输出

![image](Image/FirmwareUpgrade.png)
