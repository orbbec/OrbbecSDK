# C++ NetDevice sample
Femto Mega and Gemini 2 XL support network functionality. Both devices support device discovery. As long as the PC and the device are on the same network, using the network functionality is the same as using USB, with no difference in the interface.

This example demonstrates obtaining frame data over the network by connecting to the device via IP address and port number.

**Notes:**

1. Femto Mega supports network DHCP, while Gemini 2 XL does not support DHCP.
2. First, use the Orbbec Viewer tool to view the device's IP address while connected via USB. Then, set the PC's IP address to be in the same network as the device, ensuring that you can ping the device successfully.

## Enter the IP address
```cpp
    // Enter the device IP address (currently only Femto Mega devices support network connection, and its default IP address is 192.168.1.10)
    std::string ip;
    std::cout << "Input your device IP (default: 192.168.1.10):";
    std::getline(std::cin, ip);
```

## Create a device via IP address
```cpp
    // Create a network device through IP (the default port number is 8090; devices that currently support network mode do not support modifying the port number)
    auto device = ctx.createNetDevice(ip.c_str(), 8090);
```

## Enable Depth and Color streams via configuration
```cpp
    // Create Config for configuring Pipeline work
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Get the depth camera configuration list
    auto depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
    // Use default configuration
    auto depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
    // Enable depth stream
    config->enableStream(depthProfile);

    // Get the color camera configuration list
    auto colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
    // Use default configuration
    auto colorProfile = colorProfileList->getProfile(OB_PROFILE_DEFAULT);
    // Enable color stream
    config->enableStream(colorProfile);
```

## Start the pipeline with a callback interface
```cpp
    // Pass in the configuration and start the pipeline
    pipe->start(config, [&](std::shared_ptr<ob::FrameSet> frameSet) {
        std::lock_guard<std::mutex> lock(frameSetMutex);
        currentFrameSet = frameSet;
    });
```

## Obtain Depth and Color frame data
```cpp
    std::shared_ptr<ob::FrameSet> frameSet;
    {  // Define the scope through curly braces, and the lock will be automatically released after exiting the scope. Timely unlocking can prevent the 
       // frameset output thread of the pipeline from being blocked for too long, which could result in increased internal cache and data frame delay.
        std::lock_guard<std::mutex> lock(frameSetMutex);
        frameSet = currentFrameSet;
    }
    if(frameSet) {
        auto colorFrame = frameSet->getFrame(OB_FRAME_COLOR);
        auto depthFrame = frameSet->getFrame(OB_FRAME_DEPTH);
    }
```

## Stop the pipeline
```cpp
    // Stop the pipeline
    pipe->stop();
```