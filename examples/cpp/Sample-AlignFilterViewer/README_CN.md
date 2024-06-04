# 流对齐处理单元-AlignFilterViewer

支持设备： Gemini 330系列相机，例如：Gemini G335

功能描述：演示传感器数据流同步对齐的操作，显示对齐后的图像，并通过ESC_KEY键退出程序

本示例基于C++ hign Level API进行演示

## 1. 创建pipeline
```cpp
    ob::Pipeline pipe;
```
## 2. 开启color流
```cpp
    auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    if(colorProfiles) {
        colorProfile = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30);
    }
    config->enableStream(colorProfile);
```
## 3. 开启深度流
```cpp
    auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
    depthProfile = depthProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
    //depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);
```
## 4. 设置对齐模式
```cpp
    /* Config depth align to color or color align to depth.
    OBStreamType align_to_stream = OB_STREAM_DEPTH; */
    OBStreamType align_to_stream = OB_STREAM_COLOR;
    ob::Align align(align_to_stream);
```
## 5. 开启pipeline
```cpp
    pipe.start(config);
```

## 6. 获取帧数据
```cpp
    auto colorFrame = frameSet->colorFrame();
    auto depthFrame = frameSet->depthFrame();
```

## 7. 进行对齐处理
```cpp
    auto newFrame    = align.process(frameSet);
    auto newFrameSet = newFrame->as<ob::FrameSet>();
    colorFrame       = newFrameSet->colorFrame();
    depthFrame       = newFrameSet->depthFrame();
```

## 8. 关闭pipeline
```cpp
    pipe.stop();
```
## 9. 预期输出

![image](Image/alignFilter_viewer.png)