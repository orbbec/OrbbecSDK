# C++ Sample Multi Device

Function description: This example mainly demonstrates the operation of multiple devices.

This example is based on the C++High Level API for demonstration

## 1. Firstly, it is necessary to create a Context to get a list of device information for creating devices and pipelines
```cpp
    ob::Context ctx;
    // Query the list of connected devices
    auto devList = ctx.queryDeviceList();
    // Get the number of connected devices
    int devCount = devList->deviceCount();
```

## 2. Create a device and create a pipeline through the device
```cpp
    // Traverse device list and create devices
    std::vector<std::shared_ptr<ob::Device>> devices;
    for (int i = 0; i < devCount; i++)
    {
        // Get the device and create a pipeline
    	auto dev  = devList->getDevice( i );
    	auto pipe = std::make_shared< ob::Pipeline >( dev );
    	pipes.push_back( pipe );
    }
```

## 3. Open depth and color streams for multiple devices
```cpp
    void StartStream(std::vector<std::shared_ptr<ob::Pipeline>> pipes) {
        int i = 0;
        for(auto &&pipe: pipes) {
            std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
            // Get the depth camera configuration list
            auto                                    depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
            std::shared_ptr<ob::VideoStreamProfile> depthProfile     = nullptr;
            if(depthProfileList) {
                // Open the default profile of Depth Sensor, which can be configured through the configuration file
                depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfileList->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
            }
            config->enableStream(depthProfile);
    
            // Get the color camera configuration list
            try {
                auto                                    colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
                std::shared_ptr<ob::VideoStreamProfile> colorProfile     = nullptr;
                if(colorProfileList) {
                    // Open the default profile of Color Sensor, which can be configured through the configuration file
                    colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfileList->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
                }
                config->enableStream(colorProfile);
            }
            catch(ob::Error &e) {
                std::cerr << "Current device is not support color sensor!" << std::endl;
            }
    
            // Start the pipeline and pass in the configuration
            pipe->start(config, [i](std::shared_ptr<ob::FrameSet> frameSet) {
                std::lock_guard<std::mutex> lock(frameMutex);
                if(frameSet->colorFrame()) {
                    colorFrames[i] = frameSet->colorFrame();
                }
                if(frameSet->depthFrame()) {
                    depthFrames[i] = frameSet->depthFrame();
                }
            });
            i++;
        }
    }
```
## 4. Stop all open streams on devices
```cpp
    void StopStream( std::vector< std::shared_ptr< ob::Pipeline > > pipes) {
        int i = 0;
        for ( auto&& pipe : pipes ) {
            if(colorFrames[i])
                colorFrames->reset();
            if(depthFrames[i])
                depthFrames->reset();
            if(irFrames[i])
                irFrames->reset();
            // stop pipeline
            pipe->stop();
            i++;
        }
    }
```
## 5. expected Output 

![image](Image/MultiDevice.png)