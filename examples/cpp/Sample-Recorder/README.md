# C++ Sample Recorder

Function description: Connect the device to start streaming, record the current video stream to a file, and exit the program with the ESC_KEY key

This example is based on the C++High Level API for demonstration

## 1. create a pipeline, through which multiple types of streams can be easily opened and closed, and a set of frame data can be obtained
```cpp
    ob::Pipeline pipe;
```

## 2. Get the stream configuration of the depth camera
```cpp
    depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
```

Configure which streams to enable or disable in the Pipeline by creating a Configuration, where depth streams will be enabled
```cpp
    // Configure the stream types opened by Pipeline
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(depthProfile);
```

## 3. Start the stream configured in Configuration
 If no parameters are passed，the default configuration start stream will be activated, and the recording switch will be activated
```cpp
    pipe.start(config);
    pipe.startRecord("./OrbbecPipeline.bag");
```
Wait for a frame of data in a blocking manner, which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
    auto frameSet = pipe.waitForFrames(100);	// Set the waiting time to 100ms
```

## 4. Stop Pipeline, no more frame data will be generated
```cpp
    pipe.stop();
```