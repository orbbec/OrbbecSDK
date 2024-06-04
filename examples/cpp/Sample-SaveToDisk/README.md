# C++ Sample Save To Disk

Function description: Connect the device to open the stream, get color and depth maps, and store them in PNG format

This example is based on the C++ High Level API for demonstration

## 1. Create two functions to save the obtained image to a file
```cpp
    // Save depth map in PNG format
    void saveDepth( std::shared_ptr< ob::DepthFrame > depthFrame ) {
        std::vector< int > compression_params;
        compression_params.push_back( cv::IMWRITE_PNG_COMPRESSION );
        compression_params.push_back( 0 );
        compression_params.push_back( cv::IMWRITE_PNG_STRATEGY );
        compression_params.push_back( cv::IMWRITE_PNG_STRATEGY_DEFAULT );
        std::string depthName = "Depth_" + std::to_string( depthFrame->timeStamp() ) + ".png";
        cv::Mat     depthMat( depthFrame->height(), depthFrame->width(), CV_16UC1, depthFrame->data() );
        cv::imwrite( depthName, depthMat, compression_params );
        std::cout << "Depth saved:" << depthName << std::endl;
    }
    
    // Save color images in PNG format
    void saveColor( std::shared_ptr< ob::ColorFrame > colorFrame ) {
        std::vector< int > compression_params;
        compression_params.push_back( cv::IMWRITE_PNG_COMPRESSION );
        compression_params.push_back( 0 );
        compression_params.push_back( cv::IMWRITE_PNG_STRATEGY );
        compression_params.push_back( cv::IMWRITE_PNG_STRATEGY_DEFAULT );
        std::string colorName = "Color_" + std::to_string( colorFrame->timeStamp() ) + ".png";
        cv::Mat     colorRawMat( 1, colorFrame->dataSize(), CV_8UC1, colorFrame->data() );
        cv::Mat     colorMat = cv::imdecode( colorRawMat, 1 );
        cv::imwrite( colorName, colorMat, compression_params );
        std::cout << "Color saved:" << colorName << std::endl;
    }
```

## 2. Create a pipeline that makes it easy to open and close multiple types of streams and get a set of frame data
```cpp
    ob::Pipeline pipeline;
```
Then, all configurations of color and depth streams can be obtained through the Pipeline, including the resolution, frame rate, and format of the stream, as well as the required color and depth stream configurations
```cpp
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto                                    colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
        std::shared_ptr<ob::VideoStreamProfile> colorProfile  = nullptr;
        if(colorProfiles) {
            colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(colorProfile);
    }
    catch(ob::Error &e) {
        // no Color Sensor
        colorCount = -1;
        std::cerr << "Current device is not support color sensor!" << std::endl;
    }
    
    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto                                    depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);
```

## 3. Start Pipeline 
```cpp
    pipeline.start( config );
```

## 4. Get frame data 
```cpp
    auto frameset = pipeline.waitForFrames( 100 );
```

Get data for a single type of frame
```cpp
    auto colorFrame = frameset->colorFrame();
    auto depthFrame = frameset->depthFrame();
```

Create a format conversion filter to convert color images to RGB format and save them
```cpp
    // Create Format Conversion Filter
    ob::FormatConvertFilter formatConverFilter;
    
    formatConverFilter.setFormatConvertType(FORMAT_MJPEG_TO_RGB888);
    colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
    formatConverFilter.setFormatConvertType(FORMAT_RGB_TO_BGR);
    colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
```
Store the obtained data through the storage function edited at the beginning
```cpp
    saveColor( colorFrame );
    saveDepth( depthFrame );
```

## 5. Stop Pipeline
```cpp
    pipeline.stop();
```