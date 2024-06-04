# C++快速开始示例

这个快速入门示例演示了使用最少的API来获取数据流。

## 1. 使用默认设备创建Pipeline
```cpp
ob::Pipeline pipe;
```

## 2.  使用默认配置开启Pipeline,更多信息请参阅 OrbbecSDKConfig_v1.0.xml。默认情况下仅启用深度和彩色流。通过修改配置文件，可以启用IR流。
```cpp
 pipe.start();
```

## 3. 获取帧数据
```cpp
   auto frameSet = pipe.waitForFrames();
   auto colorFrame = frameSet->colorFrame();
   auto depthFrame = frameSet->depthFrame();
```

## 4. 停止Pipeline
```cpp

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();
```