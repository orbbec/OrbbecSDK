# 红外示例-InfraredViewer

功能描述：演示获取红外相机图像，显示红外图像，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 创建Pipeline, 配置流信息
```cpp
    // Create a pipeline with default device
    ob::Pipeline pipe;
    
    // Get all stream profiles of the infrared camera, including stream resolution, frame rate, and frame format
    // Please adjust the sensor according to the actual product, some device types only have OB_SENSOR_IR_LEFT and OB_SENSOR_IR_RIGHT.
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_IR);
    
    if(profiles == nullptr) {
        std::cerr
            << "The obtained IR resolution list is NULL. For binocular structured light devices, try using the doubleIr example to turn on the ir data stream. "
            << std::endl;
        return 0;
    }
    
    std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
    try {
        // Find the corresponding profile according to the specified format, first look for the y16 format
        irProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y8, 30);
    }
    catch(ob::Error &e) {
        // If the specified format is not found, search for the default profile to open the stream
        irProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    
    // By creating config to configure which streams to enable or disable for the pipeline, here the infrared stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(irProfile);
```

## 2. 开启pipeline
```cpp
    pipe.start(config);
```

## 3. 获取帧数据
```cpp
    auto frameSet = pipe.waitForFrames(100);
```

## 4. 停止pipeline
```cpp
    pipe.stop();
```

## 5. 预期输出

![image](Image/InfraredViewer.png)