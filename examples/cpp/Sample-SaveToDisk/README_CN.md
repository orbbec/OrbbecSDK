# 存储示例

功能描述：连接设备开流 , 获取彩色和深度图并存储为png格式。

本示例基于C++ High Level API进行演示

## 1. 创建两个函数来用于将获取的图片保存到文件
```cpp
    //保存深度图为png格式
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
    
    //保存彩色图为png格式
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

## 2. 创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
    ob::Pipeline pipeline;
```

然后可以通过Pipeline来获取彩色流和深度流的所有配置, 包括流的分辨率 ,帧率 ,以及流的格式，配置所需要彩色和深度流
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

## 3. 启动流
```cpp
    pipeline.start( config );
```

## 4. 通过等待的方式来获取帧的数据
```cpp
    auto frameset = pipeline.waitForFrames( 100 )
```

获取单种类型的帧的数据
```cpp
    auto colorFrame = frameset->colorFrame();
    auto depthFrame = frameset->depthFrame();
```

创建格式转换Filter对彩色图像进行转换成RGB格式后保存
```cpp
    //创建格式转换Filter
    ob::FormatConvertFilter formatConverFilter;
    
    formatConverFilter.setFormatConvertType(FORMAT_MJPEG_TO_RGB888);
    colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
    formatConverFilter.setFormatConvertType(FORMAT_RGB_TO_BGR);
    colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
```

通过开头的编辑的存储函数来存储获得的数据
```cpp
    saveColor( colorFrame );
    saveDepth( depthFrame );
```

## 5. 停止Pipeline
```cpp
    pipeline.stop();
```

程序正常退出后会释放资源