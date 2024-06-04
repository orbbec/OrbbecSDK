# C++ Sample Align Filter Viewer

Supported devices: Gemini 330 series cameras, such as Gemini G335

Function description: Demonstrate the synchronization and alignment of sensor data streams, display the aligned image, and exit the program using the ESC_KEY key

This example is based on the C++ high level API for demonstration

## 1. Create pipeline
```cpp
    ob::Pipeline pipe;
```

## 2. Enable color stream
```cpp
    auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    if(colorProfiles) {
        colorProfile = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30);
    }
    config->enableStream(colorProfile);
```

## 3. Enable depth stream
```cpp
    auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
    depthProfile = depthProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
    //depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);
```

## 4. Set alignment mode
```cpp
    /* Config depth align to color or color align to depth.
    OBStreamType align_to_stream = OB_STREAM_DEPTH; */
    OBStreamType align_to_stream = OB_STREAM_COLOR;
    ob::Align align(align_to_stream);
```

## 5. Start Pipeline through Configuration
```cpp
    pipe.start(config);
```

## 6. Get frame data
```cpp
    auto colorFrame = frameSet->colorFrame();
    auto depthFrame = frameSet->depthFrame();
```

## 7. Perform alignment processing
```cpp
    auto newFrame    = align.process(frameSet);
    auto newFrameSet = newFrame->as<ob::FrameSet>();
    colorFrame       = newFrameSet->colorFrame();
    depthFrame       = newFrameSet->depthFrame();
```

## 8. Stop pipeline
```cpp
    pipe.stop();
```

## 9. expected Output 
![AlignFilterViewer-Output](Image/alignFilter_viewer.png)