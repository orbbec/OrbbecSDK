# 双红外示例

支持设备：仅支持Gemini 330系列，Gemini 2 XL，Gemini 2 VL

功能描述：演示获取双红外相机图像，显示左右红外图像，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 创建Pipeline, 获取设备
```cpp
    ob::Pipeline pipe
```
## 2. 配置左IR
```cpp
    // Get the ir_left camera configuration list
    auto irLeftProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);
    
    if(irLeftProfiles == nullptr) {
        std::cerr
            << "The obtained IR(Left) resolution list is NULL. For monocular structured light devices, try opening the IR data stream using the IR example. "
            << std::endl;
        return 0;
    }
    
    // Open the default profile of IR_LEFT Sensor, which can be configured through the configuration file
    try {
        auto irLeftProfile = irLeftProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irLeftProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "IR(Left) stream not found!" << std::endl;
    }
```
## 3. 配置右IR
```cpp
    // Get the ir_right camera configuration list
    auto irRightProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_RIGHT);
    
    // Open the default profile of IR_RIGHT Sensor, which can be configured through the configuration file
    try {
        auto irRightProfile = irRightProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irRightProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "IR(Right) stream not found!" << std::endl;
    }
```
## 4. 开启pipeline
```cpp
    pipe.start(config);
```
## 5. 获取帧数据
```cpp
    auto leftFrame  = frameSet->getFrame(OB_FRAME_IR_LEFT);
    auto rightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT)
```
## 6. 停止pipeline
```cpp
    pipe.stop();
```
## 7.预期输出

![image](Image/double_infrared_viewer.png)
