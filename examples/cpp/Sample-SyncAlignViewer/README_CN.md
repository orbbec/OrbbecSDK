# 同步对齐示例

功能描述：本示例演示了对Sensor数据流控制对齐的操作，显示对齐后的图像，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 首先创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
    ob::Pipeline pipe;
```

## 2. 获取彩色相机和深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
```cpp 
    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(colorProfile);
    }
    catch(...) {
        std::cerr << "Current device is not support color sensor!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);
```

## 3. 通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流和深度流
```cpp
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(colorProfile);
    config->enableStream(depthProfile);
```

## 4. 控制流对齐，此处开启软件对齐
```cpp
    // 配置对齐模式为软件D2C对齐
    config->setAlignMode(ALIGN_D2C_SW_MODE);
```

## 5. 启动在Config中配置的流，如果不传参数，将启动默认配置启动流
```cpp

    pipe.start(config);
```

## 6. 停止Pipeline，将不再产生帧数据
```cpp 
    pipe.stop();
```
程序正常退出之后资源将会自动释放

## 7. 预期输出

![image](Image/SyncAlignViewer.png)