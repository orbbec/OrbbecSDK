# HelloOrbbec示例


功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源，并通过ESC_KEY键退出程序

本示例基于C++ Low Level API进行演示

## 1. 打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号
```cpp
    std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
```
## 2. 首先需要创建一个Context，用于获取设备信息列表和创建设备
```cpp
    ob::Context ctx;
```
## 3. 查询已经接入设备的列表
```cpp
    auto devList = ctx.queryDeviceList();
    
    //获取接入设备的数量
    if(devList->deviceCount() == 0) {
        std::cerr << "Device not found!" << std::endl;
        return -1;
    }
```
## 4. 创建设备
```cpp
    //创建设备，0表示第一个设备的索引
    auto dev = devList->getDevice(0);
```

## 5.获取当前preset信息
当前只有Gemini 330 系列支持
```cpp
    auto devicePresetList = dev->getAvailablePresetList();
    auto devicePreset     = dev->getCurrentPresetName();
    std::cout << "Available preset list: " << std::endl;
    for(uint32_t i = 0; i < devicePresetList->count(); i++) {
        auto name = devicePresetList->getName(i);
        if(std::string(name) == std::string(devicePreset)) {
            std::cout << "\t" << name << " (current preset)" << std::endl;
        }
        else {
            std::cout << "\t" << name << std::endl;
        }
```    

## 6.设置当前preset
当前只有Gemini 330 系列支持
```cpp
    auto newPreset = "High Accuracy";
    if(devicePresetList->hasPreset(newPreset)) {
        dev->loadPreset(newPreset);
        std::cout << "Current preset is changed to " << newPreset << std::endl;
    }
```

## 7. 获取和这个设备相关的信息：设备信息、设备的名称、获取设备的pid, vid, uid、设备的固件版本号、设备的序列号
```cpp
    //获取设备信息
    auto devInfo = dev->getDeviceInfo();
    
    //获取设备的名称
    std::cout << "Device name: " << devInfo->name() << std::endl;
    
    //获取设备的pid, vid, uid
    std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;
    
    //获取设备的固件版本号
    auto fwVer = devInfo->firmwareVersion();
    std::cout << "Firmware version: " << fwVer << std::endl;
    
    //获取设备的序列号
    auto sn = devInfo->serialNumber();
    std::cout << "Serial number: " << sn << std::endl;
    
    // By getting the connection type of the device
    auto connectType = devInfo->connectionType();
```
## 8.获取支持的传感器列表
```cpp
    //获取支持的传感器列表
    std::cout << "Sensor types: " << std::endl;
    auto sensorList = dev->getSensorList();
    for(uint32_t i = 0; i < sensorList->count(); i++) {
        auto sensor = sensorList->getSensor(i);
        switch(sensor->type()) {
        case OB_SENSOR_COLOR:
            std::cout << "\tColor sensor" << std::endl;
            break;
        case OB_SENSOR_DEPTH:
            std::cout << "\tDepth sensor" << std::endl;
            break;
        case OB_SENSOR_IR:
            std::cout << "\tIR sensor" << std::endl;
            break;
        case OB_SENSOR_IR_LEFT:
            std::cout << "\tIR Left sensor" << std::endl;
            break;
        case OB_SENSOR_IR_RIGHT:
            std::cout << "\tIR Right sensor" << std::endl;
            break;
        case OB_SENSOR_GYRO:
            std::cout << "\tGyro sensor" << std::endl;
            break;
        case OB_SENSOR_ACCEL:
            std::cout << "\tAccel sensor" << std::endl;
            break;
        default:
            break;
        }
```    


## 9.预期输出


![image](Image/HelloOrbbec.png)