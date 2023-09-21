# MultiDeviceSync

本示例用于演示如何完成多设备同步的配置和使用。

## 操作说明

首先需要先参考产品说明书连接好设备，然后按照下面步骤完成设备配置和开启设备数据流。

### 设备配置

1. 在程序文件夹内找到配置文件`MultiDeviceSyncConfig.json`，依据实际情况修改配置文件内容：

``` json
// MultiDeviceSyncConfig.json
{
  "sn": "CL2LC2P0089", // device serial number
  "syncConfig": {  // 详细信息请参考 ObTypes.h 文件内的 OBMultiDeviceSyncConfig 结构体定义和说明
    "syncMode": "OB_MULTI_DEVICE_SYNC_MODE_PRIMARY",  // 同步模式配置，常用模式：“OB_MULTI_DEVICE_SYNC_MODE_PRIMARY、OB_MULTI_DEVICE_SYNC_MODE_SECONDARY、OB_MULTI_DEVICE_SYNC_MODE_SECONDARY_SYNCED”, 在include\libobsensor\h\ObTypes.h中定义的枚举
    "depthDelayUs": 160,// ir/depth/tof 触发信号输入延时，单位微秒；为防止激光干扰，建议不同设备间通过将该延时配置，使激光错开160us
    "colorDelayUs": 0, // rgb 触发信号输入延时，单位微秒
    "triggerSignalOutputEnable": true, // 设备触发信号输出延迟的使能开关
    "triggerSignalOutputDelayUs": 0,  // 设备触发信号输出延时，单位微秒
    "deviceId": 0  // 设备ID，可用于设备编号
  }
}
```

2. 运行`MultiDeviceSync.exe`, 输入选项`0`后回车，等待设备完成配置后重启。

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

### 启动设备

​启动方法有两种，除非需要重新配置设备同步，否则推荐使用第二种

1. 设备同步配置完成后，会自动开启设备的深度和 RGB 数据流。

2. 也可以在运行`MultiDeviceSync.exe`, 输入选项`1`后回车，即可开启设备设备的深度和 RGB 数据流。

   ```bash

   Please select options:
    0 --> config devices
    1 --> start stream
   input: 1

   start secondary devices...
   start primary devices...
   ```

### 注意事项

1. 设备同步配置会完成会将参数写入到设备内Flash，不需要每次使用都配置，频繁配置会损耗Flash使用寿命；
2. 部分型号设备在配置完成后的自动重启时间会比较长，请耐心等待；
3. 启动设备后，请在图像预览窗按`ESC`键可停止数据流并退出程序，非正常程序退出可能会导致设备关流不完全，从而导致从设备一直被触发（可重启设备解决）。

## 关键代码说明

### 设备时钟同步

设备时钟同步主要目的是保证不同设备间数据帧时间戳的同步性，方便后续对数据帧进行同步匹配。设备时钟同步使用了异步授时方案，即PC电脑给各连接设备授时，可通过下列代码完成：

```cpp
context.enableDeviceClockSync(3600000); // 该操作会立即将所有已创建的设备进行授时同步，并且每过一小时（3600000秒）会自动授时一次
```

### 判断设备主从类型
区分主从设备对于处理frame数据非常有帮助，判断主机和从机方法如下
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

### 设备同步配置

*信息内容请参考 `ObTypes.h` 文件内的 `OBMultiDeviceSyncConfig `结构体定义和说明*

```cpp
dev->getMultiDeviceSyncConfig(); // 获取当前设备的同步配置
dev->setMultiDeviceSyncConfig(syncConfig); // 设置设备同步配置，该操作会将参数写入到设备Flash，设备重启后生效
```

### 主从设备开流

通常主从设备开流顺序可以随意设置，如产品说明书为特殊说明，建议先开启从设备数据流，再开启主设备数据流。并且由于开流后设备需要一定时间初始化，在从设备开流后，需要延时等待一段时间后再开启主设备，否则从设备将可能丢失部分触发信号。

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

