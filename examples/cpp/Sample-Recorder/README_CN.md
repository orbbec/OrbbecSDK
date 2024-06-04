# 录制示例-Recorder

功能描述：连接设备开流 , 录制当前视频流到文件，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 首先创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
    ob::Pipeline pipe;
```

## 2. 获取深度相机的流配置
```
    depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
```

## 3. 通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用深度流
```cpp
    //配置Pipeline打开的流类型
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(depthProfile);
```

启动在Config中配置的流，如果不传参数，将启动默认配置启动流，启动录制开关
```cpp
    pipe.start(config);
    pipe.startRecord("./OrbbecPipeline.bag");
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```cpp
    auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms
```

## 4. 停止Pipeline，将不再产生帧数据
```cpp
    pipe.stop();
```
程序正常退出后会释放资源