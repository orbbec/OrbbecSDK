# HDR示例

支持设备： Gemini 330系列相机，例如：Gemini G335

功能描述：演示使用HDR操作，显示HDR处理后的图像，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 配置HDR
```cpp
    // configure and enable Hdr stream
    OBHdrConfig obHdrConfig;
    obHdrConfig.enable     = true;  // enable HDR merge
    obHdrConfig.exposure_1 = 7500;
    obHdrConfig.gain_1     = 24;
    obHdrConfig.exposure_2 = 100;
    obHdrConfig.gain_2     = 16;
    device->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
```
## 2.创建Pipeline,进行流配置
```cpp
    // Create a pipeline with default device
    ob::Pipeline pipe;
    
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    
    std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
    try {
        // Get all stream profiles of the ir camera, including stream resolution, frame rate, and frame format
        auto irProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);
        if(irProfiles) {
            irProfile = std::const_pointer_cast<ob::StreamProfile>(irProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(irProfile);
    }
    catch(...) {
        std::cerr << "Current device is not support ir sensor!" << std::endl;
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
## 3.开启pipeline
```cpp
    pipe.start(config);
```

## 4.获取HDR处理后的图像
```cpp
    auto leftIRFrame = frameSet->getFrame(OB_FRAME_IR_LEFT);
    if(leftIRFrame) {
        framesForRender.push_back(leftIRFrame);
    }
    
    auto depthFrame = frameSet->depthFrame();
    if(depthFrame != nullptr) {
        auto newFrame = hdrMerge.process(frameSet);
        auto newFrameSet = newFrame->as<ob::FrameSet>();
        if(newFrameSet) {
            depthFrame = newFrameSet->depthFrame();
            if(depthFrame) {
                framesForRender.push_back(depthFrame);
            }
        }
    }
```

## 5.停止pipeline
```cpp
    pipe.stop();
```
## 6.关闭Hdr处理
```cpp
        obHdrConfig.enable = false;
        device->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
```

## 7.预期输出


![image](Image/HdrMerge.png)