# 后处理示例

支持设备： Gemini 330系列相机，例如：Gemini G335

功能描述：演示进行后处理操作，显示经过后处理的图像，并通过ESC_KEY键退出程序

本示例基于C++ high Level API进行演示

## 1. 获取pipeline，进行流配置
```cpp
    // Create a pipeline with default device
    ob::Pipeline pipe;
    
    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    
    std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
    try {
        // Find the corresponding profile according to the specified format, first look for the y16 format
        depthProfile = profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
    }
    catch(ob::Error &e) {
        // If the specified format is not found, search for the default profile to open the stream
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    
    // By creating config to configure which streams to enable or disable for the pipeline, here the depth stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(depthProfile);
```

## 2. 获取深度后处理过滤器列表
```cpp
    auto obFilterList = pipe.getDevice()->getSensor(OB_SENSOR_DEPTH)->getRecommendedFilters();
    
    std::shared_ptr<ob::DecimationFilter> decFilter;
    for(int i = 0; i < obFilterList->count(); i++) {
        auto postProcessorfilter =obFilterList->getFilter(i);
        std::cout << "Depth recommended post processor filter type: " << postProcessorfilter->type() << std::endl;
        if(postProcessorfilter->is<ob::DecimationFilter>()) {
            decFilter = postProcessorfilter->as<ob::DecimationFilter>();
        }
    }
```

## 3. 开启pipeline
```cpp
    pipe.start(config);
```

## 4. 预期输出


![image](Image/post_processing.png)