# C++ Sample Hdr Merge


Supported devices: Gemini 330 series cameras, such as Gemini G335

Function description: Demonstrate using HDR operation, display HDR processed images, and exit the program with ESC_KEY key

This example is based on the C++High Level API for demonstration

## 1. Configure HDR
```cpp
    // configure and enable Hdr stream
    OBHdrConfig obHdrConfig;
    obHdrConfig.enable     = true;  // enable HDR merge
    obHdrConfig.exposure_1 = 7500;
    obHdrConfig.gain_1     = 24;
    obHdrConfig.exposure_2 = 100;
    obHdrConfig.gain_2     = 16;
    device->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
```

## 2. Create a pipeline and configure the stream
```cpp
    // Create a pipeline with default device
    ob::Pipeline pipe;
    
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    
    std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
    try {
        // Get all stream profiles of the ir camera, including stream resolution, frame rate, and frame format
        auto irProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);
        if(irProfiles) {
            irProfile = std::const_pointer_cast<ob::StreamProfile>(irProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(irProfile);
    }
    catch(...) {
        std::cerr << "Current device is not support ir sensor!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);
```
## 3. Start Pipeline through Configuration
```cpp
    pipe.start(config);
```

## 4.Get HDR processed images
```cpp
    auto leftIRFrame = frameSet->getFrame(OB_FRAME_IR_LEFT);
    if(leftIRFrame) {
        framesForRender.push_back(leftIRFrame);
    }
    
    auto depthFrame = frameSet->depthFrame();
    if(depthFrame != nullptr) {
        auto newFrame = hdrMerge.process(frameSet);
        auto newFrameSet = newFrame->as<ob::FrameSet>();
        if(newFrameSet) {
            depthFrame = newFrameSet->depthFrame();
            if(depthFrame) {
                framesForRender.push_back(depthFrame);
            }
        }
    }
```
## 5.Stop pipeline

    pipe.stop();

## 6.Turn off HDR processing
```cpp
    obHdrConfig.enable = false;
    device->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
```
## 7.expected Output

![image](Image/HdrMerge.png)