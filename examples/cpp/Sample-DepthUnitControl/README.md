# C++ Sample Depth Unit Control

Function description: Demonstrate the operation of obtaining depth accuracy, display depth images, output depth accuracy, and exit the program through the ESC_KEY key

> This example is based on the C++High Level API for demonstration

## 1.Create a pipeline and stream configuration, and get devices through the pipeline
```cpp
    // Create a pipeline with default device
    ob::Pipeline pipe;
    
    // Get the device inside the pipeline
    auto device = pipe.getDevice();
```
## 2.Check the way to get depth accuracy
```cpp
    bool changeDepthUnitFlag = false;
    // check the way to adjust depth unit
    if(device->isPropertySupported(OB_PROP_DEPTH_PRECISION_LEVEL_INT, OB_PERMISSION_READ_WRITE)) {
        // change depth unit by adjust the depth precision level
        changeDepthUnitFlag = changeDepthUnit(device);
    }
    else if(device->isPropertySupported(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT, OB_PERMISSION_READ_WRITE)) {
        // for some models (Such as G330 series), user can adjust the depth unit by adjusting the depth unit flexible adjustment property
        changeDepthUnitFlag = changeDepthUnitFlexible(device);
    }
    else {
        std::cout << "Current device not support adjust depth unit, use the default depth unit 1mm" << std::endl;
        changeDepthUnitFlag = true;
    }
```
## 3.Calculate depth value
```cpp
    // For Y16 format depth frame, print the distance of the center pixel every 30 frames
    if(depthFrame->index() % 30 == 0 && depthFrame->format() == OB_FORMAT_Y16) {
        // Convert depth values to 1mm precision
        auto rstImageData = convertDepthFrameUnitTo1mm(depthFrame);
    
        uint32_t  width          = depthFrame->width();
        uint32_t  height         = depthFrame->height();
        uint16_t  centerDistance = rstImageData.data()[width * height / 2 + width / 2];
    
        // Attention: if the distance is 0, it means that the depth camera cannot detect the object（may be out of detection range）
        std::cout << "The depth frame center value is " << centerDistance << " mm. " << std::endl;
    }
```
## 4.Release resources and exit the program
```cpp
        pipe.stop();
```

## 5. expected Output 

![image](Image/DepthUnitControl.png)