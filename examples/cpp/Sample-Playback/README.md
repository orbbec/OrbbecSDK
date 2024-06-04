# C++ Sample Playback

Function description: Connect the device to start streaming, load video files for playback, and exit the program with the ESC_KEY key

This example is based on the C++High Level API for demonstration

## 1. Firstly, you need to create a Pipeline for playing back files
```cpp
    // Create a pipeline object for playback
    ob::Pipeline pipe("./OrbbecPipeline.bag");
```

## 2. Retrieve the playback status callback of the playback object
```cpp
    // Get playback object, set playback status callback
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

## 3. Reading device information and internal parameter information from playback files
```cpp
    // Reading device information from playback files
    auto deviceInfo = playback->getDeviceInfo();
    std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
      << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;
    
    // Reading internal parameter information from playback files
    auto cameraParam = pipe.getCameraParam();
    std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
                  << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;
```
## 4. Enable playback
```cpp
    //Enable playback
    pipe.start(NULL);
```

Wait for a frame of data in a blocking manner, which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
    auto frameSet = pipe.waitForFrames(100);	// Set the waiting time to 100ms
```

## 5. Stop Pipeline, no more frame data will be generated
```cpp
    pipe.stop();
```
## 6.expected Output 


![image](Image/Playback.png)