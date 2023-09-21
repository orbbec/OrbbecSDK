# MultiDeviceSync

This sample demonstrates how to complete the configuration and use of multi-device synchronization.

## Operating instructions

First, you need to refer to the product manual to connect the device, and then follow the steps below to complete the device configuration and enable device data flow.

### Device configuration

1. Find the configuration file 'MultiDeviceSyncConfig.json' in the program folder and modify the content of the configuration file according to the actual situation:

``` json
// MultiDeviceSyncConfig.json
{
      "sn": "CL2LC2P0089", // device serial number
      "syncConfig": {  // For more information, refer to the OBMultiDeviceSyncConfig structure definition and description in the ObTypes.h file
        "syncMode": "OB_MULTI_DEVICE_SYNC_MODE_PRIMARY",  // device sync mode：“OB_MULTI_DEVICE_SYNC_MODE_PRIMARY, OB_MULTI_DEVICE_SYNC_MODE_SECONDARY，OB_MULTI_DEVICE_SYNC_MODE_SECONDARY_SYNCED etc.” Enum define at include\libobsensor\h\ObTypes.h
        "depthDelayUs": 160,// The delay time of the color image capture after receiving the capture command or trigger signal in microseconds.; In order to prevent laser interference, it is recommended that the laser be staggered by 160us by configuring this delay between different devices
        "colorDelayUs": 0, // Rgb trigger signal input delay in microseconds
        "triggerSignalOutputEnable": true, // Device trigger signal output delay enable flag.
        "triggerSignalOutputDelayUs": 0,  // Device trigger signal output delay, in microseconds
        "deviceId": 0  // The device ID, which can be used for the device number
      }
    }
```

2. Run 'MultiDeviceSync.exe', enter option '0', press Enter, wait for the device to complete configuration and then restart.

```bash
Please select options:
 0 --> config devices
 1 --> start stream
input: 0

config[0]: SN=CL2LC2P0089, mode=OB_MULTI_DEVICE_SYNC_MODE_PRIMARY
config[1]: SN=CL2LC2P00C0, mode=OB_MULTI_DEVICE_SYNC_MODE_SECONDARY
Device sn[CL2LC2P0089] is configured, rebooting...
Device sn[CL2LC2P00C0] is configured, rebooting...
```

### Start devices

​	There are two methods, and the second is recommended unless device synchronization needs to be reconfigured

1. After the device synchronization configuration is completed, the depth and RGB data flow of the device is automatically turned on.

2. You can also run 'MultiDeviceSync.exe', enter option '1' and press enter to open the depth and RGB data streams of the device device.

   ```bash

   Please select options:
    0 --> config devices
    1 --> start stream
   input: 1

   start secondary devices...
   start primary devices...
   ```

### Attention

1. The device synchronization configuration will be completed and the parameters will be written to the Flash in the device, and there is no need to configure it every time it is used, and frequent configuration will lose the service life of Flash。
2. The automatic restart time of some models of devices after the configuration is completed will be relatively long, please be patient;
3. After starting the device, press the 'ESC' key in the image preview window to stop the data flow and exit the program, abnormal program exit may cause the device to shut down incompletely, resulting in the slave being triggered all the time (You can reboot you devices).

## Key code descriptions

### Device clock synchronization

The main purpose of device clock synchronization is to ensure the synchronization of data frame timestamps between different devices, and facilitate subsequent synchronization and matching of data frames. Device clock synchronization uses an asynchronous timing scheme, that is, the PC gives time to each connected device, which can be done by the following code:

```cpp
context.enableDeviceClockSync(3600000); // This operation will immediately synchronize all created devices, and will automatically time every hour (3,600,000 seconds).
```

### Determine the device Primary-Secondary type
Distinguishing between Primary and Secondary devices is very helpful for processing frame data. The method of judging the Primary and Secondary is as follows
```cpp
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

### Device synchronization configuration

*For information, please refer to the 'OBMultiDeviceSyncConfig' structure definition and description in the 'ObTypes.h' file*

```cpp
dev->getMultiDeviceSyncConfig(); // Gets the device's current sync configuration
dev->setMultiDeviceSyncConfig(SyncConfig); // Set the device synchronization configuration, which writes parameters to the device Flash, and the device takes effect after restarting
```

### Start devices

Usually the order of Primary-Secondary device stream can be start arbitrarily, such as the product manual is a special description, it is recommended to start the secondary device data stream first, and then start the primary device data stream. And because the device needs a certain amount of time to initialize after the stream is started, after the slave device start stream, it is necessary to delay waiting for a period of time before start the primary device, otherwise the secondary device may lose part of the trigger signal.

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

// Delay and wait for 5s to ensure that the initialization of the slave device is completed
std::this_thread::sleep_for(std::chrono::milliseconds(5000));

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