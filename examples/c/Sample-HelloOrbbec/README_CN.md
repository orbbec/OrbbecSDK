# HelloOrbbec

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源，并通过ESC_KEY键退出程序

本示例基于C Low Level API进行演示

## 1. 首先获取并打印当前SDK版本
```cpp
    printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());
```
## 2. 创建一个Context，用于获取设备信息列表和创建设备
```c
    ob_error*   error = NULL;
    ob_context* ctx   = ob_create_context( &error );
```

查询已经接入设备的列表，从而获取列表内设备数量，并通过索引号0创建获取列表内第一个设备
```c
    ob_device_list* dev_list = ob_query_device_list( ctx, &error );
    int dev_count = ob_device_list_device_count(dev_list, &error);
    if(dev_count == 0) {
        printf("Device not found!\n");
        return -1;
    }
    ob_device* dev = ob_device_list_get_device(dev_list, 0, &error);
```

## 3. 获取和这个设备相关的信息
```c
    //获取设备信息
    ob_device_info* dev_info = ob_device_get_device_info(dev, &error);
    
    //获取设备名称
    const char* name = ob_device_info_name(dev_info, &error);
    
    //获取设备的pid, vid, uid
    int pid = ob_device_info_pid(dev_info, &error);
    int vid = ob_device_info_vid(dev_info, &error);
    int uid = ob_device_info_uid(dev_info, &error);
    
    //通过获取设备的固件版本号
    const char* fw_ver = ob_device_info_firmware_version(dev_info, &error);
    
    //通过获取设备的序列号
    const char* sn = ob_device_info_serial_number(dev_info, &error);
    
    //获取支持的传感器列表
    ob_sensor_list* sensor_list = ob_device_get_sensor_list(dev, &error);
    
    //获取传感器数量
    int count = ob_sensor_list_get_sensor_count(sensor_list, &error);
    for(int i = 0; i < count; i++)
    {
        ob_sensor_type sensor_type = ob_sensor_list_get_sensor_type(sensor_list， i, &error);
        switch (sensor_type)
        {
          case OB_SENSOR_COLOR:
              break;
            case OB_SENSOR_DEPTH:
              break;
            case OB_SENSOR_IR:
              break;
        }
    }
```

## 4. 释放资源，退出程序。
```c
    //销毁sensor list
    ob_delete_sensor_list(sensor_list, &error);
    //销毁device info
    ob_delete_device_info(dev_info, &error);
    //销毁device
    ob_delete_device(dev, &error);
    //销毁device list
    ob_delete_device_list(dev_list, &error);
    //销毁context
    ob_delete_context(ctx, &error);
```

## 5. 预期输出

![image](Image/hello_orbbec.png)