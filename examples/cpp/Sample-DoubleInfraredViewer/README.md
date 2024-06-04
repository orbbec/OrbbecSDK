# C++ Sample Double Infrared Viewer

Supported devices: Only supports Gemini 330 series, Gemini 2 XL, Gemini 2 VL

Function description: Demonstrate obtaining dual infrared camera images, displaying left and right infrared images, and exiting the program with the ESC_KEY key

This example is based on the C++ High Level API for demonstration

## 1. Create a pipeline to get devices
```cpp
    ob::Pipeline pipe;
```

## 2. Configure Left IR
```cpp
    // Get the ir_left camera configuration list
    auto irLeftProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);
    
    if(irLeftProfiles == nullptr) {
        std::cerr
            << "The obtained IR(Left) resolution list is NULL. For monocular structured light devices, try opening the IR data stream using the IR example. "
            << std::endl;
        return 0;
    }
    
    // Open the default profile of IR_LEFT Sensor, which can be configured through the configuration file
    try {
        auto irLeftProfile = irLeftProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irLeftProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "IR(Left) stream not found!" << std::endl;
    }
```
## 3.Configure Right IR
```cpp
    // Get the ir_right camera configuration list
    auto irRightProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_RIGHT);
    
    // Open the default profile of IR_RIGHT Sensor, which can be configured through the configuration file
    try {
        auto irRightProfile = irRightProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irRightProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "IR(Right) stream not found!" << std::endl;
    }
```
## 4.Start Pipeline through Configuration
```cpp
    pipe.start(config);
```
## 5.Get frame data
```cpp
    auto leftFrame  = frameSet->getFrame(OB_FRAME_IR_LEFT);
    auto rightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
```
## 6.Stop pipeline
```cpp
    pipe.stop();
```
## 7.expected Output 
![image](Image/double_infrared_viewer.png)
