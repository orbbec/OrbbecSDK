# NetDevice 示例
Femto Mega 和 Gemini 2 XL 支持网络功能，这两个设备都支持设备发现，只要确保PC和设备在同一个网络，使用网络功能就像使用USB 一样，接口没有区别。 

这个示例用来演示通过网络获取帧数据，通过IP地址和端口号连接设备。

**注意事项：** 

1、Femto Mega 支持网络DHCP, Gemini 2 XL 不支持DHCP。  
2、首先通过Orbbec Viewer工具，在USB连接下，可以查看设备的IP地址，然后将PC 的IP 设置和 设备同一个网络内，确保能ping通。


## 输入IP地址 
```cpp
    // Enter the device ip address (currently only FemtoMega devices support network connection, and its default ip address is 192.168.1.10)
    std::string ip;
    std::cout << "Input your device ip(default: 192.168.1.10):";
    std::getline(std::cin, ip);
```

## 通过IP地址创建设备
```cpp
    // Create a network device through ip (the default port number is: 8090, devices that currently support network mode do not support modifying the port
    // number)
    auto device = ctx.createNetDevice(ip.c_str(), 8090);

```

## 通过配置使能Depth和Color流
```cpp
    // Create Config for configuring Pipeline work
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Get the depth camera configuration list
    auto depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
    // use default configuration
    auto depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
    // enable depth stream
    config->enableStream(depthProfile);

    // Get the color camera configuration list
    auto colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
    // use default configuration
    auto colorProfile = colorProfileList->getProfile(OB_PROFILE_DEFAULT);
    // enable depth stream
    config->enableStream(colorProfile);
```

## 调用Pipeline回调的接口开流
```cpp
    // Pass in the configuration and start the pipeline
    pipe->start(config, [&](std::shared_ptr<ob::FrameSet> frameSet) {
        std::lock_guard<std::mutex> lock(frameSetMutex);
        currentFrameSet = frameSet;
    });


```
## 获取Depth和Color帧数据
```cpp
 std::shared_ptr<ob::FrameSet> frameSet;
        {  // Define the scope through curly braces, and the lock will be automatically unlocked after exiting the scope. Unlocking in time can prevent the
           // frameset output thread of the pipeline from being blocked for too long, resulting in an increase in internal cache and an increase in data frame
           // delay
            std::lock_guard<std::mutex> lock(frameSetMutex);
            frameSet = currentFrameSet;
        }
        if(frameSet) {
            auto colorFrame = frameSet->getFrame(OB_FRAME_COLOR);
            auto depthFrame = frameSet->getFrame(OB_FRAME_DEPTH);
        }
```


## 停止Pipeline
```cpp
    // stop the pipeline
    pipe->stop();
```