# C++ Sample Depth Stream Viewer

Function description: This example mainly demonstrates the use of SDK to get depth data and draw display, and exit the program through the ESC_KEY key

This example is based on the C++ High Level API for demonstration

## 1. Firstly, create a pipeline, through which multiple types of streams can be easily opened and closed, and a set of frame data can be obtained
```cpp
    ob::Pipeline pipe;
```

## 2. By creating config to configure which streams to enable 
```cpp
    // By creating config to configure which streams to enable or disable for the pipeline, here the depth stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableVideoStream(OB_STREAM_DEPTH);
```

## 3. Start Pipeline
```cpp
    pipe.start(config);
```

## 4. Obtain Depth frame data
Wait for a frame of data in a blocking manner which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
    auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```

## 5. Stop Pipeline, no more frame data will be generated
```cpp
    pipe.stop();
```
## 6. expected Output 

![image](Image/DepthViewer.png)