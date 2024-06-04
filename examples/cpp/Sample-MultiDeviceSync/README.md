# C++ Sample Multi Device Sync

Function description: Demonstrate multi device synchronization operation

This example is based on the C++ low level API for demonstration

## 1. Configure program
```cpp
    std::cout << "Please select options: " << std::endl;
    std::cout << " 0 --> config devices" << std::endl;
    std::cout << " 1 --> start stream" << std::endl;
    std::cout << "input: ";
    int index = -1;
    std::cin >> index;
```
## 2.Configure multi device synchronization
```cpp
    exitValue = configMultiDeviceSync();
```
## 3. Distinguishing primary-secondary devices
```cpp
    streamDevList.clear();
    // Query the list of connected devices
    auto devList = context.queryDeviceList();
    
    // Get the number of connected devices
    int devCount = devList->deviceCount();
    for(int i = 0; i < devCount; i++) {
        streamDevList.push_back(devList->getDevice(i));
    }
    
    if(streamDevList.empty()) {
        std::cerr << "Device list is empty. please check device connection state" << std::endl;
        return -1;
    }
    
    // traverse the device list and create the device
    std::vector<std::shared_ptr<ob::Device>> primary_devices;
    std::vector<std::shared_ptr<ob::Device>> secondary_devices;
    for(auto dev: streamDevList) {
        auto config = dev->getMultiDeviceSyncConfig();
        if(config.syncMode == OB_MULTI_DEVICE_SYNC_MODE_PRIMARY) {
            primary_devices.push_back(dev);
        }
        else {
            secondary_devices.push_back(dev);
        }
    }
```
## 4. Enable slave devices
```cpp
    std::cout << "Secondary devices start..." << std::endl;
    int deviceIndex = 0;  // Sencondary device display first
    for(auto itr = secondary_devices.begin(); itr != secondary_devices.end(); itr++) {
        auto depthHolder = createPipelineHolder(*itr, OB_SENSOR_DEPTH, deviceIndex);
        pipelineHolderList.push_back(depthHolder);
        startStream(depthHolder);
    
        auto colorHolder = createPipelineHolder(*itr, OB_SENSOR_COLOR, deviceIndex);
        pipelineHolderList.push_back(colorHolder);
        startStream(colorHolder);
    
        deviceIndex++;
    }
```
## 5. Turn on the main device
```cpp
    std::cout << "Primary device start..." << std::endl;
    deviceIndex = secondary_devices.size();  // Primary device display after primary devices.
    for(auto itr = primary_devices.begin(); itr != primary_devices.end(); itr++) {
        auto depthHolder = createPipelineHolder(*itr, OB_SENSOR_DEPTH, deviceIndex);
        startStream(depthHolder);
        pipelineHolderList.push_back(depthHolder);
    
        auto colorHolder = createPipelineHolder(*itr, OB_SENSOR_COLOR, deviceIndex);
        startStream(colorHolder);
        pipelineHolderList.push_back(colorHolder);
    
        deviceIndex++;
    }
```

## 6. Set the synchronization interval time for multiple device
```cpp
    // Start the multi-device time synchronization function
    context.enableDeviceClockSync(3600000);  // update and sync every hour
```

## 7. Conduct multi device testing
```cpp
    testMultiDeviceSync();
```

## 8. Close data stream
```cpp
    // close data stream
    for(auto itr = pipelineHolderList.begin(); itr != pipelineHolderList.end(); itr++) {
        stopStream(*itr);
    }
```