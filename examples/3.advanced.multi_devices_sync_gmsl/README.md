# C++ Sample: 3.advanced.multi_devices_sync_gmsl

## Overview

Function description: Demonstrate multi GMSL devices synchronization operation on nvidia platform

This example is based on the C++ low level API for demonstration

**Note:**

1.support nvidia xavier/orin platform config hardware_trigger_source

2.Currently, only G335Lg GMSL devices are supported, and other GMSL devices will be supported in the near future.

## Knowledge

Multi-machine GMSL devices  perform frame synchronization of multi-machine depth/color/ir streams through software synchronization or hardware syncronization signal methods.

## Code overview

### 1.Configure multi device synchronization

```cpp
    configMultiDeviceSync();
```

### 2.Distinguishing secondary devices

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

### 3.Enable secondary devices

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

### 4.Enable Primary device

note: G335Lg GMSL device current don`t  support primary.

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

### 5.Set software synchronization interval time

```cpp
    // Start the multi-device time synchronization function
    context.enableDeviceClockSync(3600000);  // update and sync every hour
```

### 6.Conduct multi device testing

```cpp
    testMultiDeviceSync();
```

### 7.Close data stream

```cpp
    // close data stream
    for(auto itr = pipelineHolderList.begin(); itr != pipelineHolderList.end(); itr++) {
        stopStream(*itr);
    }
```

### 8.Triggering Mode

#### Software Triggering Mode

Set the device synchronization mode to `OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING` after opening the stream, and the device will wait for the trigger signal (command) sent by the upper layer after opening the stream. The number of frames to be triggered for triggering mode can be configured through `framesPerTrigger`. The method for triggering images:

```c++
auto multiDeviceSyncConfig = dev->getMultiDeviceSyncConfig();
if(multiDeviceSyncConfig.syncMode == OB_MULTI_DEVICE_SYNC_MODE_SOFTWARE_TRIGGERING)
{
    dev->triggerCapture();
}
```

*Press `t` in the render window to trigger a capture once.*

#### Hardware Triggering Mode

Set the device synchronization mode to `OB_MULTI_DEVICE_SYNC_MODE_HARDWARE_TRIGGERING` after opening the stream, and the device will wait for the external hardware trigger signal input after opening the stream. The number of frames to be triggered for triggering mode can be configured through `framesPerTrigger`.

```
openSocSyncPwmTrigger(uint16_tfps)
```

**support nvidia xavier/orin platform config hardware_trigger_source**

```
--------------------------------------------------
Please select options: 
 0 --> config devices sync mode. 
 1 --> start stream 
 2 --> start hardwareTrigger (Dependent on soc trigger source) 
 3 --> close hardwareTrigger & Exit program 
--------------------------------------------------
Please select input: 2
```

start soc pwm hardware_triggering by config HZ.

config soc hardware_trigger_source hz refefence  MultiDeviceSyncConfigGmsl.json

for example: "hardwareTriggerFps":3000

## Run Sample

```cpp
$ ./ob_multi_devices_sync_gmsl

--------------------------------------------------
Please select options: 
 0 --> config devices sync mode. 
 1 --> start stream 
 2 --> start hardwareTrigger (Dependent on soc trigger source) 
 3 --> close hardwareTrigger & Exit program 
--------------------------------------------------
Please select input: 0

config[0]: SN=CP4H74D0003D, mode=OB_MULTI_DEVICE_SYNC_MODE_SECONDARY
Line:576get HardwareTriggerFps:3000 from json
config[1]: SN=GMSL00000005, mode=OB_MULTI_DEVICE_SYNC_MODE_SECONDARY
Line:576get HardwareTriggerFps:3000 from json
1-Config Device syncMode:8, syncModeStr:OB_MULTI_DEVICE_SYNC_MODE_SECONDARY
Config MultiDeviceSync Success. 


--------------------------------------------------
Please select options: 
 0 --> config devices sync mode. 
 1 --> start stream 
 2 --> start hardwareTrigger (Dependent on soc trigger source) 
 3 --> close hardwareTrigger & Exit program 
--------------------------------------------------
Please select input:
```

0:Configure program

1:Start multi-machine frame synchronization and synchronization frame preview

2:Start hardware trigger  frame synchronization

3.Close hardware trigger and exit program.

## Configuration file parameter description

config file :  ob_multi_devices_sync_gmsl_config.json

```
{    "version": "1.0.0",
    "configTime": "2023/01/01",
    "devices": [
        {
            "sn": "CP4H74D0003D",
            "syncConfig": {
                "syncMode": "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY",
                "depthDelayUs": 0,
                "colorDelayUs": 0,
                "trigger2ImageDelayUs": 0,
                "triggerOutEnable": false,
                "triggerOutDelayUs": 0,
                "framesPerTrigger": 30
            }
        },
        {
            "sn": "GMSL00000005",
            "syncConfig": {
                "syncMode": "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY",
                "depthDelayUs": 0,
                "colorDelayUs": 0,
                "trigger2ImageDelayUs": 0,
                "triggerOutEnable": false,
                "triggerOutDelayUs": 0,
                "framesPerTrigger": 30
            }
        }
    ],
    "hardwareTriggerFps": 3000
}
```


**Key parameter description**

"syncConfig": {
                "syncMode": "OB_MULTI_DEVICE_SYNC_MODE_SECONDARY",   //set multi gmsl devices sync mode
                "depthDelayUs": 0,
                "colorDelayUs": 0,
                "trigger2ImageDelayUs": 0,
                "triggerOutEnable": false,	//current gmsl device don`t support.
                "triggerOutDelayUs": 0,
                "framesPerTrigger": 30	//set default software trigger 30 frames per  softwaretrigger.
            }

"hardwareTriggerFps": 3000  //set default hardware soc trigger source 30FPS
