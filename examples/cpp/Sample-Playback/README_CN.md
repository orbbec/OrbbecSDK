# 回放示例

功能描述：连接设备开流 , 载入视频文件进行回放，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 首先创建一个用于回放文件的Pipeline
```cpp
    //创建用于回放的pipeline对象
    ob::Pipeline pipe("./OrbbecPipeline.bag");
```

## 2. 获取回放对象的回放状态回调
```cpp
    //获取回放对象设置回放状态回调
    auto playback = pipe.getPlayback();
    playback->setPlaybackStateCallback([&](OBMediaState state) {
        if(state == OB_MEDIA_BEGIN) {
            std::cout << "Playback file begin." << std::endl;
        }
        else if(state == OB_MEDIA_END) {
            std::cout << "Playback file end." << std::endl;
        }
    });
```

## 3. 从回放文件中读取设备信息与内参信息
```cpp
    //从回放文件中读取设备信息
    auto deviceInfo = playback->getDeviceInfo();
    std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
      << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;
    
    //从回放文件中读取内参信息
    auto cameraParam = pipe.getCameraParam();
    std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
                  << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;
```

## 4. 开启回放
```cpp
    //开启回放
    pipe.start(NULL);
```

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```cpp
    auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms
```

## 5. 停止Pipeline，将不再产生帧数据
```cpp
    pipe.stop();
```

程序正常退出后会释放资源

## 6.预期输出


![image](Image/Playback.png)