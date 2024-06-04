# C++ Sample Hot Plugin

Function description: This example mainly demonstrates the setting of device unplugging callback, as well as the processing of the obtained stream after unplugging, and exiting the program through the ESC_KEY key

This example is based on the C++ High Level API for demonstration

## 1.Firstly, it is necessary to create a Context for obtaining a list of device information and creating a device
```cpp
    ob::Context ctx;
```
## 2.Register device callback and execute relevant functions during device unplugging and unplugging
```cpp
    ctx.setDeviceChangedCallback( []( std::shared_ptr< ob::DeviceList > removedList, std::shared_ptr< ob::DeviceList > addedList ) {
            DeviceDisconnectCallback( removedList );
            DeviceConnectCallback( addedList );
        } );
```

## 3.Enable streaming
```cpp
    void startStream(std::shared_ptr<PipelineHolder> holder) {
    std::shared_ptr<FramePrintInfo> printInfo(new FramePrintInfo{});
    std::string                     deviceSN = std::string(holder->deviceInfo->serialNumber());
    ob::FrameSetCallback            callback = [deviceSN, printInfo](std::shared_ptr<ob::FrameSet> frameSet) {
        // Get the depth data frame
        auto depthFrame = frameSet->depthFrame();
        if(depthFrame) {
            printInfo->depthCount++;
            if(printInfo->depthCount == 15) {
                std::cout << "=====Depth Frame Info====== SN: " << std::string(deviceSN) << ", " << depthFrame << std::endl;
                printInfo->depthCount = 0;
            }
        }
    
        // Get the ir data frame
        auto irFrame = frameSet->irFrame();
        if(irFrame) {
            printInfo->irCount++;
            if(printInfo->irCount == 15) {
                std::cout << "=====IR Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(irFrame)
                            << std::endl;
                printInfo->irCount = 0;
            }
        }
    
        // Get the ir left data frame
        auto irLeftFrame = frameSet->getFrame(OB_FRAME_IR_LEFT);
        if(irLeftFrame) {
            printInfo->irLeftCount++;
            if(printInfo->irLeftCount == 15) {
                std::cout << "=====IR Left Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(irLeftFrame)
                            << std::endl;
                printInfo->irLeftCount = 0;
            }
        }
    
        // Get the ir right data frame
        auto irRightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
        if(irRightFrame) {
            printInfo->irRightCount++;
            if(printInfo->irRightCount == 15) {
                std::cout << "=====IR Right Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(irRightFrame)
                            << std::endl;
                printInfo->irRightCount = 0;
            }
        }
    
        // Get the color data frame
        auto colorFrame = frameSet->colorFrame();
        if(colorFrame) {
            printInfo->colorCount++;
            if(printInfo->colorCount == 15) {
                std::cout << "=====Color Frame Info====== SN: " << std::string(deviceSN) << ", " << std::dynamic_pointer_cast<ob::VideoFrame>(colorFrame)
                            << std::endl;
                printInfo->colorCount = 0;
            }
        }
    };
    
    // Start video stream according to the stream profile of the configuration file.If there is no configuration file, the first stream profile will be used.
    try {
        std::cout << "startStream " << holder->deviceInfo << std::endl;
        holder->pipeline->start(nullptr, callback);
        holder->isStarted = true;
    }
    catch(...) {
        std::cout << "Pipeline start failed!" << std::endl;
        holder->isStarted = false;
    }
    }
 ```   
    
 ## 4. Stop streaming
```cpp
void stopStream(std::shared_ptr<PipelineHolder> holder) {

    if(!holder->isStarted) {
        return;
    }

    try {
        std::cout << "stopStream " << holder->deviceInfo << std::endl;
        holder->isStarted = false;
        holder->pipeline->stop();
    }
    catch(ob::Error &e) {
        std::cerr << "stopStream failed., function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage()
                  << "\ntype:" << e.getExceptionType() << std::endl;
    }

} 
```