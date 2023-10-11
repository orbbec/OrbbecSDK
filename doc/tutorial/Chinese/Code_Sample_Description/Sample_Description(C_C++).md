  获取所有示例都可以在工程的Examples目录中找到

| **名称** | **语言** | **描述** |
| --- | --- | --- |
| HelloOrbbec | C | 演示连接到设备获取SDK版本和设备信息 |
| 固件升级示例 | C | 演示选择固件bin或者img文件给设备升级固件版本 |
| 传感器控制示例 | C | 演示对设备、传感器控制命令的操作 |
| 深度示例 | C | 演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像 |
| 彩色示例       | C        | 演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像 |
| 红外示例       | C        | 演示使用SDK获取红外数据并绘制显示、获取分辨率并进行设置、显示红外图像 |
| 深度模式 | C | 演示如何获取相机深度工作模式，查询支持的深度模式列表，切换模式 |
| 热拔插示例 | C | 演示设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接 |
| 点云示例 | C | 演示生成深度点云或RGBD点云并保存成ply格式文件 |
| HelloOrbbec | C++ | 演示连接到设备获取SDK版本和设备信息 |
| 深度示例 | C++ | 演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像 |
| 彩色示例 | C++ | 演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像 |
| 红外示例 | C++ | 演示使用SDK获取红外数据并绘制显示、获取分辨率并进行设置、显示红外图像 |
| 流对齐示例 | C++ | 演示对传感器数据流对齐的操作 |
| 固件升级示例 | C++ | 演示选择固件bin或者img文件给设备升级固件版本 |
| 传感器控制示例 | C++ | 演示对设备和传感器控制命令的操作 |
| 多路流同时开流示例 | C++ | 演示一个设备同时打开多路流的操作 |
| 多设备示例 | C++ | 演示对多设备进行操作 |
| 深度模式 | C++ | 演示如何获取相机深度工作模式，查询支持的深度模式列表，切换模式 |
| 热拔插示例 | C++ | 演示设备拔插回调的设置，并获取到插拔后处理的流 |
| IMU示例 | C++ | 获取IMU数据并输出显示 |
| 多机同步示例 | C++ | 演示多机同步功能 |
| 点云示例 | C++ | 演示生成深度点云或RGBD点云并保存成ply格式文件 |
| 存储示例 | C++ | 获取彩色和深度图并存储为png格式 |
| 录制示例 | C++ | 录制当前视频流到文件 |
| 回放示例 | C++ | 载入视频文件进行回放 |

# C
## HelloOrbbec

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源。
> 本示例基于C Low Level API进行演示

首先获取并打印当前SDK版本
```c
printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());
```
创建一个Context，用于获取设备信息列表和创建设备
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
接下来就可以获取和这个设备相关的信息了
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
释放资源，退出程序。
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

## 固件升级示例-FirmwareUpgrade
功能描述：本示例演示如何用固件文件给设备升级。
> 本示例基于C Low Level API进行演示，示例编译语言为C++，OrbbecSDK使用C语言API

在main函数接口通过命令参数获取固件文件
```cpp
// check_firmware_file_path()函数用于检查文件是否存在，实际代码中最好检查后缀是否为bin或者img, 以及固件文件是否与目标设备相匹配
const char *check_firmware_file_path(int argc, char **argv) {
    if(argc < 2) {
        printf("Please input firmware path.\n");
        return "";
    }

    const char *filePath = *(argv + 1);
    FILE       *file     = fopen(filePath, "r");
    if(!file) {
        printf("Open Firmware file failed. filePath: %s\n", filePath);
        return "";
    }

    fclose(file);

    return filePath;
}

int main(int argc, char **argv) {
    const char *firmware_file_path = check_firmware_file_path(argc, argv);
    if(!firmware_file_path || 0 == strlen(firmware_file_path)) {
        printf("command: \n$ ./frameware_upgrade[.exe] firmwareFile.bin\n");
        return 0;
    }

    // 接下来的业务代码

    return 0;
}
```

创建ob_context并通过ob_context获取设备，本示例假设运行firmware_upgrade[.exe]之前上位机（Windows、Ubuntu、Android平台）已经插入设备。device_changed_callback用于固件升级后监听device重启后获取被升级设备的业务处理
```cpp
// 构建ob_context对象
ob_error   *error = NULL;
ob_context *ctx   = ob_create_context(&error);
check_error(error);

// 设置设备变化监听器，device_changed_callback是管理device声明周期的关键函数，开发者必须关注该回调
ob_set_device_changed_callback(ctx, device_changed_callback, &callback_user_data_, &error);
check_error(error);

// 查询当前已经接入的设备
ob_device_list *dev_list = ob_query_device_list(ctx, &error);
check_error(error);

// 从ob_device_list中获取当前接入设备的数量
int dev_count = ob_device_list_device_count(dev_list, &error);
check_error(error);
if(dev_count == 0) {
    // 固件升级示例假设设备已经接入到上位机（Windows、Ubuntu、Android平台）
    printf("Device not found!\n");
    return -1;
}

// 获取第一个设备，index=0
ob_device *dev = ob_device_list_get_device(dev_list, 0, &error);
check_error(error);

// 打印设备信息
dump_device_info(dev);
```

获取设备当前的固件版本信息
```cpp
// 打印设备名称，SN，VID，PID以及固件版本
void dump_device_info(ob_device *device) {
    ob_error *error = NULL;

    // 获取ob_device_info对象，通过ob_device_info可以获取目标设备的基本信息
    ob_device_info *dev_info = ob_device_get_device_info(device, &error);
    check_error(error);

    // 设备名称
    const char *name = ob_device_info_name(dev_info, &error);
    check_error(error);
    printf("Device name: %s\n", name);

    // 设备VID，PID，UID
    int pid = ob_device_info_pid(dev_info, &error);
    check_error(error);
    int vid = ob_device_info_vid(dev_info, &error);
    check_error(error);
    const char *uid = ob_device_info_uid(dev_info, &error);
    check_error(error);
    printf("Device pid: %d vid: %d uid: %s\n", pid, vid, uid);

    // 设备当前的固件版本号
    const char *fw_ver = ob_device_info_firmware_version(dev_info, &error);
    check_error(error);
    printf("Firmware version: %s\n", fw_ver);

    // 设备SN
    const char *sn = ob_device_info_serial_number(dev_info, &error);
    check_error(error);
    printf("Serial number: %s\n", sn);

    // 释放资源，否则会造成内存泄漏
    ob_delete_device_info(dev_info, &error);
    check_error(error);
}
```

给目标设备升级固件
a. 实现C API的固件升级回调接口；
b. 调用固件升级接口进行升级；
```cpp
// 实现C API的固件升级回调接口；
void device_upgrade_callback(ob_upgrade_state state, const char *message, uint8_t percent, void *user_data) {
    if(state == STAT_START) {
        printf("Upgrade Firmware start\n");
    }
    else if(state == STAT_FILE_TRANSFER) {
        printf("Upgrade Firmware file transfer, percent: %u\n", (uint32_t)percent);
    }
    else if(state == STAT_IN_PROGRESS) {
        printf("Upgrade Firmware in progress, percent: %u\n", (uint32_t)percent);
    }
    else if(state == STAT_DONE) {
        // 固件升级成功
        printf("Upgrade Firmware done, percent: %u\n", (uint32_t)percent);
        is_upgrade_success_ = true;
    }
    else if(state == STAT_VERIFY_IMAGE) {
        printf("Upgrade Firmware verify image\n");
    }
    else {
        // 固件升级失败
        printf("Upgrade Firmware failed. state: %d, errMsg: %s, percent: %u \n", (int)state, message ? message : "", (uint32_t)percent);
    }
}

// 对目标设备进行固件升级
bool upgrade_firmware(ob_device *device, const char *firmwarePath) {
    const char *index     = strstr(firmwarePath, ".img");
    bool        isImgFile = (bool)index;
    index                 = strstr(firmwarePath, ".bin");
    bool isBinFile        = (bool)index;
    if(!(isImgFile || isBinFile)) {
        // 固件升级文件一般为bin或者img，实际业务中最好通过文件名称、文件MD5等信息做防呆
        printf("Upgrade Fimware failed. invalid firmware file: %s\n", firmwarePath);
        return false;
    }

    // 调用固件升级接口进行升级；
    is_upgrade_success_ = false;
    ob_error *error     = NULL;
    ob_device_upgrade(device, firmwarePath, device_upgrade_callback, false, &callback_user_data_, &error);
    check_error(error);
    return is_upgrade_success_;
}
```

固件升级成功后，需要重启设备。重启设备有两种方式，一种是拔插设备（或者操作系统重启），另一种是调用OrbbecSDK的reboot接口。等设备上线后，通过本示例的dump_device_info()函数可以查询当前设备固件版本信息

以下示例演示通过固件reboot接口重启设备
```cpp
// 重启设备
printf("Reboot device\n");
is_device_removed_       = false;
is_wait_reboot_complete_ = true;
ob_device_reboot(dev, &error);
check_error(error);

// 释放资源，防止内存泄漏
ob_delete_device(dev, &error);
check_error(error);
```

在ob_device_changed_callback可以监听设备重启时下线、上线的事件，详情可以看热拔插示例
```cpp
// 监听设备变化
void device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data) {
    ob_error *error = NULL;

    // 通过added处理上线的设备

    // 通过removed处理下线的设备

    // 释放资源，避免内存泄漏
    ob_delete_device_list(removed, &error);
    check_error(error);

    // 释放资源，避免内存泄漏
    ob_delete_device_list(added, &error);
    check_error(error);
}
```

## 传感器控制示例-SensorControl

功能描述：本示例主要演示了对device控制命令的操作、对Sensor控制命令的操作、对Sensor进行流操作。
> 本示例基于C Low Level API进行演示

创建一个Context，并查询已经接入设备的列表
```c
ob_error*   error = NULL;
ob_context* ctx   = ob_create_context( &error );
ob_device_list* dev_list = ob_query_device_list( ctx, &error );
```
通过索引号打印列表内设备信息
```c
const char *name = ob_device_list_get_device_name(device_list, i, &g_error);
int pid = ob_device_list_get_device_pid(device_list, i, &g_error);
int vid = ob_device_list_get_device_vid(device_list, i, &g_error);
const char *uid = ob_device_list_get_device_uid(device_list, i, &g_error);
const char *sn = ob_device_list_get_device_serial_number(device_list, i, &g_error);
printf("%d. name: %s, pid: %d, vid: %d, uid: %s, sn: %s\n", i, name, pid, vid, uid, sn);
```
创建一个设备
```c
if(devCount <= 1) {
    // 如果插入单个设备，默认选择第一个
     device = ob_device_list_get_device(dev_list, 0, &g_error);
}
else {
    // 如果有多个设备，用户输入选择
    device = select_device(dev_list);  // select_device 代码请参考实例源码
}
```
获取并打印当前创建的设备信息
```c
 ob_device_info *deviceInfo = ob_device_get_device_info(device, &g_error);
 const char *name = ob_device_info_name(deviceInfo, &g_error);
 int pid = ob_device_info_pid(deviceInfo, &g_error);
 int vid = ob_device_info_vid(deviceInfo, &g_error);
 const char *uid = ob_device_info_uid(deviceInfo, &g_error);
 printf("Current Device: name: %s, pid: %d, vid: %d, uid: %s\n", name, pid, vid, uid);
```
获取支持的控制命令属性
```c
// 获取支持控制命令属性个数
uint32_t propertySize = ob_device_get_supported_property_count(device, &g_error);
// 通过索引号i获取控制命令属性项
ob_property_item property_item = ob_device_get_supported_property(device, i, &g_error);
```
读/写控制命令
```c
// 读取
bool_ret = ob_device_get_bool_property(device, property_item.id, &g_error);// bool型参数
int_ret = ob_device_get_int_property(device, property_item.id, &g_error);/ int 型参数
float_ret = ob_device_get_float_property(device, property_item.id, &g_error);// float型参数

// 读取命令的值范围、默认值、步进值等信息
ob_int_property_range   int_range;
ob_float_property_range float_range;
ob_bool_property_range  bool_range;
sprintf(str, "Bool value(min:0, max:1, step:1)"); // bool 型
int_range = ob_device_get_int_property_range(device, property_item.id, &g_error); // int 型
float_range = ob_device_get_float_property_range(device, property_item.id, &g_error); // float型

// 写入
ob_device_set_bool_property(device, property_item.id, bool_value, &g_error); // bool型参数
ob_device_set_int_property(device, property_item.id, int_value, &g_error); // int 型参数
ob_device_set_float_property(device, property_item.id, float_value, &g_error); // float型参数
```
最后程序退出时需要释放相应的已创建资源。
```c
//销毁context
ob_delete_context(ctx, &g_error);
//销毁device list
ob_delete_device_list(dev_list, &g_error);
//销毁device
ob_delete_device(device, &g_error);
```
## 深度示例-DepthViewer

功能描述：演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像
> 本示例基于C High Level API进行演示

首先需要创建Pipeline，用于连接设备后打开彩色和深度流
```c
pipe = ob_create_pipeline( &error );
```
创建Config，用于配置彩色和深度流的分辨率、帧率、格式
```c
ob_config* config = ob_create_config( &error );
```
配置流
```c
//配置Depth流
ob_stream_profile *     depth_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_DEPTH, &error);
//根据指定的格式查找对应的Profile,优先查找Y16格式
depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
//没找到Y16格式后不匹配格式查找对应的Profile进行开流
if(error){
    depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
    error = nullptr;
}
ob_config_enable_stream(config, depth_profile, &error);  // 使能配置
```
通过Config，启动Pipeline
```c
ob_pipeline_start_with_config(pipe, config, &error);
```
释放资源，退出程序。
```c
//销毁frameSet，回收内存
ob_delete_frame(frameset, &error);

//销毁profile
ob_delete_stream_profile(depth_profile, &error);

//销毁profile list
ob_delete_stream_profile_list(profiles, &error);

//销毁device
ob_delete_device(device, &error);

//销毁pipeline
ob_delete_pipeline(pipe, &error);
```
## 彩色示例-ColorViewer

功能描述：演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像

> 本示例基于C High Level API进行演示

首先需要创建Pipeline，用于连接设备后打开彩色和深度流

```c
pipe = ob_create_pipeline( &error );
```

创建Config，用于配置彩色和深度流的分辨率、帧率、格式

```c
ob_config* config = ob_create_config( &error );
```

配置流

```c
//配置Color流
ob_stream_profile *     color_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_Color, &error);
//根据指定的格式查找对应的Profile,优先选择RGB888格式
color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30, &error);
//没找到RGB888格式后不匹配格式查找对应的Profile进行开流
if(error){
	color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
	error = nullptr;
}
ob_config_enable_stream(config, depth_profile, &error);  // 使能配置
```

通过Config，启动Pipeline

```c
ob_pipeline_start_with_config(pipe, config, &error);
```

释放资源，退出程序。

```c
//销毁frameSet，回收内存
ob_delete_frame(frameset, &error);

//销毁profile
ob_delete_stream_profile(color_profile, &error);

//销毁profile list
ob_delete_stream_profile_list(profiles, &error);

//销毁device
ob_delete_device(device, &error);

//销毁pipeline
ob_delete_pipeline(pipe, &error);
```

## 红外示例-InfraredViewer

功能描述：演示使用SDK获取红外数据并绘制显示、获取分辨率并进行设置、显示红外图像

> 本示例基于C High Level API进行演示

首先需要创建Pipeline，用于连接设备后打开彩色和深度流

```c
pipe = ob_create_pipeline( &error );
```

创建Config，用于配置彩色和深度流的分辨率、帧率、格式

```c
ob_config* config = ob_create_config( &error );
```

配置流

```c
//配置IR流
ob_stream_profile *     ir_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_IR, &error);
//根据指定的格式查找对应的Profile,优先查找Y16格式
ir_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
//没找到Y16格式后不匹配格式查找对应的Profile进行开流
if(error) {
    ir_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
    error = nullptr;
}
ob_config_enable_stream(config, depth_profile, &error);  // 使能配置
```

配置IR输出源
**说明：** 该功能仅Gemini2产品支持（OrbbecSDK1.5.5开始支持Gemini2产品），Gemini2产品默认左IR sensor输出数据流
```c
//判断是否支持切换左右IR通道
if(ob_device_is_property_supported(device, OB_PROP_IR_CHANNEL_DATA_SOURCE_INT, OB_PERMISSION_READ_WRITE, &error)) {
    // Gemini2产品支持SENSOR_IR选择sensor输出，0是左IR，1是右IR。
    int32_t dataSource = 0;
    ob_device_set_int_property(device, OB_PROP_IR_CHANNEL_DATA_SOURCE_INT, dataSource, &error);
}
```

通过Config，启动Pipeline

```c
ob_pipeline_start_with_config(pipe, config, &error);
```

释放资源，退出程序。

```c
//销毁frameSet，回收内存
ob_delete_frame(frameset, &error);

//销毁profile
ob_delete_stream_profile(ir_profile, &error);

//销毁profile list
ob_delete_stream_profile_list(profiles, &error);

//销毁device
ob_delete_device(device, &error);

//销毁pipeline
ob_delete_pipeline(pipe, &error);
```

## 切换相机深度工作模式-DepthWorkMode
功能描述：本示例在OrbbecSDK1.5.5开始支持，仅支持Gemini2的设备。主要演示切换相机深度模式，先查询深度模式列表，然后选择对应的相机深度模式，调用接口切换

首先获取设备
```cpp
//创建一个Context，与Pipeline不同，Context是底层API的入口，在开关流等常用操作上
//使用低级会稍微复杂一些，但是底层API可以提供更多灵活的操作，如获取多个设备，读写
//设备及相机的属性等
ob_error *  error = NULL;
ob_context *ctx   = ob_create_context(&error);
check_error(error);

//查询已经接入设备的列表
ob_device_list *dev_list = ob_query_device_list(ctx, &error);
check_error(error);

//获取接入设备的数量
int dev_count = ob_device_list_device_count(dev_list, &error);
check_error(error);
if(dev_count == 0) {
    printf("Device not found!\n");
    return -1;
}

//创建设备，0表示第一个设备的索引
ob_device *dev = ob_device_list_get_device(dev_list, 0, &error);
check_error(error);
```

检查设备是否支持相机深度工作模式，当前只有Gemini2支持相机深度工作模式
```cpp
// 检查是否支持相机深度工作模式，目前（2022年12月5日）仅Gemini2双目摄像头支持深度工作模式
if(!ob_device_is_property_supported(dev, OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE, &error)) {
    printf("FAILED!!, Device not support depth work mode");
    check_error(error);
    return -1;
}
check_error(error);
```

查询设备的当前深度工作模式
```cpp
// 查询当前的深度工作模式
ob_depth_work_mode cur_work_mode = ob_device_get_current_depth_work_mode(dev, &error);
check_error(error);
printf("Current depth work mode: %s\n", cur_work_mode.name);
```

查询设备支持的相机深度模式列表
```cpp
// 获取列表长度
uint32_t mode_count = ob_depth_work_mode_list_count(mode_list, &error);
printf("Support depth work mode list count: %u\n", mode_count);

int cur_mode_index = -1;
for(uint32_t i = 0; i < mode_count; i++) {
    ob_depth_work_mode mode = ob_depth_work_mode_list_get_item(mode_list, i, &error);
    check_error(error);
    printf("depth work mode[%u], name: %s", i, mode.name);
}
```

切换相机深度模式
```cpp
// 切换到新的相机深度模式
ob_device_switch_depth_work_mode_by_name(dev, mode.name, &error);
check_error(error);
```

到此切换相机深度模式结束，可以用pipeline进行打开相机取流
注意：
1. 如果需要切换相机深度模式，那么打开数据流必须在切换深度工作模式之后；每个相机深度模式下支持的有效分辨率不同
2. 如果已经用pipeline打开数据流，那么切换相机深度工作模式前必须把原来申请的pipeline释放；
   切换相机深度工作模式后重新创建pipeline，否则会造成野指针或者内存泄露；


最后释放资源
```cpp
// 销毁mode_list
ob_delete_depth_work_mode_list(mode_list, &error);
check_error(error);

//销毁device
ob_delete_device(dev, &error);
check_error(error);

//销毁device list
ob_delete_device_list(dev_list, &error);
check_error(error);

//销毁context
ob_delete_context(ctx, &error);
check_error(error);
```


## 热拔插示例-HotPlugin

功能描述： 设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接。
> 本示例基于C Low Level API进行注册设备上下线回调，基于High Level API进行开流取帧演示

创建设备连接处理函数，函数内创建pipeline并调用create_and_start_with_config开启彩色流和深度流。
```c
//设备连接处理
void device_connect_callback( ob_device_list* connectList ) {
    uint32_t count = ob_device_list_device_count(connectList, &error);
    check_error(error);
    printf("Device connect: %d\n", count);
    if(count > 0) {
        if(pipeline == NULL) {
            pipeline = ob_create_pipeline(&error);
            check_error(error);
            create_and_start_with_config();
        }
    }
}
```
创建设备断开处理函数，函数内将pipeline停止，并将退出pipeline标记isExit设为true
```c
//设备断开处理
void device_disconnect_callback( ob_device_list* disconnectList ) {
    uint32_t count = ob_device_list_device_count(disconnectList, &error);
    check_error(error);
    printf("Device disconnect: %d\n", count);
    if(count > 0) {
        isExit = true;
    }
}
```
设备连接、断开处理函数将会在设备上下线回调函数内被调用
```c
//设备状态改变回调
void on_device_changed_callback( ob_device_list* removed, ob_device_list* added, void* pCallback ) {
    device_disconnect_callback( removed );
    device_connect_callback( added );
}
```
主函数main内，首先需要创建一个Context并注册设备上下线回调
```c
 //创建上下文
ob_context* ctx = ob_create_context( &error );

//注册设备回调
ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );
```
主循环主要任务是，当pipeline在设备上线后已创建启动，从pipeline获取数据帧集合，并将数据帧集合内的Color和Depth相关信息打印输出。
```c
//等待一帧数据，超时时间为100ms
ob_frame* frameset = ob_pipeline_wait_for_frameset( pipeline, 100, &error );
if ( frameset ) {
    //获取深度数据帧
    ob_frame* depth_frame = ob_frameset_depth_frame( frameset, &error );
    if ( depth_frame ) {
        printf( "=====Depth Frame Info======Index: %lld TimeStamp: %lld\n", ob_frame_index( depth_frame, &error ), ob_frame_time_stamp( depth_frame, &error ) );
        //释放深度数据帧
        ob_delete_frame( depth_frame, &error );
    }
    //获取Color数据帧
    ob_frame* color_frame = ob_frameset_color_frame( frameset, &error );
    if ( color_frame ) {
        printf( "=====Color Frame Info======Index: %lld TimeStamp: %lld\n", ob_frame_index( color_frame, &error ), ob_frame_time_stamp( color_frame, &error ) );
        //释放Color数据帧
        ob_delete_frame( color_frame, &error );
    }
    //释放frameSet
    ob_delete_frame( frameset, &error );
}
```
退出主循环后，需要将相关资源释放
```c
if ( pipeline ) {
    //停止pipeline
    ob_pipeline_stop( pipeline, &error );
    // 销毁pipeline
    ob_delete_pipeline( pipeline, &error );
}
// 销毁dev_list
if ( dev_list ) {
    ob_delete_device_list( dev_list, &error );
}
// 销毁ctx
if ( ctx ) {
    ob_delete_context( ctx, &error );
}
```
## 点云示例-PointCloud

功能描述：连接设备开流，生成深度点云或RGBD点云并保存成ply格式文件。
> 本示例基于C++ High Level API进行演示

创建点云保存成ply格式文件函数，ply文件格式详细描述可在网络上查看
```c
//保存点云数据到ply
void save_points_to_ply( ob_frame* frame, const char* fileName ) {
    int pointsSize = ob_frame_data_size( frame, &error ) / sizeof( ob_point );
    check_error( error );

    FILE* fp = fopen( fileName, "wb+" );
    fprintf( fp, "ply\n" );
    fprintf( fp, "format ascii 1.0\n" );
    fprintf( fp, "element vertex %d\n", pointsSize );
    fprintf( fp, "property float x\n" );
    fprintf( fp, "property float y\n" );
    fprintf( fp, "property float z\n" );
    fprintf( fp, "end_header\n" );

    ob_point* point = ( ob_point* )ob_frame_data( frame, &error );
    check_error( error );
    for ( int i = 0; i < pointsSize; i++ ) {
        fprintf( fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z );
        point++;
    }

    fflush( fp );
    fclose( fp );
}

//保存彩色点云数据到ply
void save_rgb_points_to_ply( ob_frame* frame, const char* fileName ) {
    int pointsSize = ob_frame_data_size( frame, &error ) / sizeof( ob_color_point );
    check_error( error );

    FILE* fp = fopen( fileName, "wb+" );
    fprintf( fp, "ply\n" );
    fprintf( fp, "format ascii 1.0\n" );
    fprintf( fp, "element vertex %d\n", pointsSize );
    fprintf( fp, "property float x\n" );
    fprintf( fp, "property float y\n" );
    fprintf( fp, "property float z\n" );
    fprintf( fp, "property uchar red\n" );
    fprintf( fp, "property uchar green\n" );
    fprintf( fp, "property uchar blue\n" );
    fprintf( fp, "end_header\n" );

    ob_color_point* point = ( ob_color_point* )ob_frame_data( frame, &error );
    check_error( error );

    for ( int i = 0; i < pointsSize; i++ ) {
        fprintf( fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, ( int )point->r, ( int )point->g, ( int )point->b );
        point++;
    }

    fflush( fp );
    fclose( fp );
}
```
创建Pipeline及流配置
```c
    //创建pipeline 用于连接设备后打开Color和Depth流
    pipeline = ob_create_pipeline( &error );
    //创建config，用于配置 Color 和 Depth 流的 分辨率、帧率、格式
    ob_config* config = ob_create_config( &error );

     //配置Depth流
    ob_stream_profile *     depth_profile = NULL;
    ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
     //根据指定的格式查找对应的Profile,优先查找Y16格式
    depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
    //没找到Y16格式后不匹配格式查找对应的Profile进行开流
    if(error){
        depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
        error = NULL;
    }

    //配置Color流
    ob_stream_profile *color_profile = NULL;
    profiles                         = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
    if(error){
        printf("Current device is not support color sensor!\n");
        //如果不存在Color Sensor 点云转换分辨率配置为深度分辨率
        ob_config_set_d2c_target_resolution(config,ob_video_stream_profile_width(depth_profile, &error),ob_video_stream_profile_height(depth_profile, &error),&error);
        ob_config_set_depth_scale_require(config,false,&error);
        error = NULL;
    }
    //根据指定的格式查找对应的Profile,优先选择RGB888格式
    if(profiles){
        color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30, &error);
    }
    //没找到RGB888格式后不匹配格式查找对应的Profile进行开流
    if(profiles && error){
        color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
        error = NULL;
    }
    ob_config_enable_stream( config, color_profile, &error );  // 使能配置
```
从Pipeline上获取当前打开的设备，用以打开设备D2C功能
```c
    //获取device句柄
    ob_device *device = ob_pipeline_get_device(pipeline, &error);
    // 开启D2C对齐, 生成RGBD点云时需要开启
    ob_config_set_align_mode(config,ALIGN_D2C_HW_MODE,&error);

```
开流，并在开流后创建点云filter。点云filter用于将depth和color帧数据转换成点云数据。点云filter在开流后创建目的是让SDK内部自动根据当前开流配置设置好相机参数。当然也可以自行设置。
```c
    //创建点云Filter对象（点云Filter创建时会在Pipeline内部获取设备参数, 所以尽量在Filter创建前配置好设备）
    ob_filter *point_cloud = ob_create_pointcloud_filter(&error);
    //从pipeline获取当前开流的相机参数，并传入到点云filter
    ob_camera_param camera_param = ob_pipeline_get_camera_param(pipeline, &error);
    ob_pointcloud_filter_set_camera_param(point_cloud, camera_param, &error);
```
启动主循环，循环内根据用户按键调用点云filter生成深度点云或RGBD点云数据，并保存成ply文件。
```c
//等待一帧数据，超时时间为100ms
ob_frame* frameset = ob_pipeline_wait_for_frames( pipeline, 100, &error );
if ( frameset != NULL )
{
    //按R键保存ply数据
    if ( ( key == 'R' || key == 'r' ) && frameset != NULL ) {
        ob_pointcloud_filter_set_point_format( point_cloud, OB_FORMAT_RGB_POINT, &error );
        ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
        save_rgb_points_to_ply( pointsFrame, "rgb_points.ply" );
        ob_delete_frame( pointsFrame, &error );
    }
    else if ( ( key == 'D' || key == 'd' ) && frameset != NULL ) {
        //生成点云并保存
        ob_pointcloud_filter_set_point_format( point_cloud, OB_FORMAT_POINT, &error );
        ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
        save_points_to_ply( pointsFrame, "points.ply" );
        ob_delete_frame( pointsFrame, &error );
    }
    ob_delete_frame( frameset, &error );  // 销毁frameSet 回收内存
}
```
主循环退出后，停流并销毁回收资源
```c
    // 停止pipeline
    ob_pipeline_stop( pipeline, &error );

    // 销毁pipeline
    ob_delete_pipeline( pipeline, &error );

    ob_delete_context( ctx, &error );
```
# C++
## HelloOrbbec

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源。
> 本示例基于C++ Low Level API进行演示

打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号
```cpp
std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
```
首先需要创建一个Context，用于获取设备信息列表和创建设备
```cpp
ob::Context ctx;
```
查询已经接入设备的列表
```cpp
auto devList = ctx.queryDeviceList();

//获取接入设备的数量
devList->deviceCount()
```
创建设备
```cpp
//创建设备，0表示第一个设备的索引
auto dev = devList->getDevice(0);
```
接下来就可以获取和这个设备相关的信息：设备信息、设备的名称、获取设备的pid, vid, uid、设备的固件版本号、设备的序列号
```cpp
//获取设备信息
auto devInfo = dev->getDeviceInfo();

//获取设备的名称
std::cout << "Device name: " << devInfo->name() << std::endl;

//获取设备的pid, vid, uid
std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

//获取设备的固件版本号
auto fwVer = devInfo->firmwareVersion();
std::cout << "Firmware version: " << fwVer << std::endl;

//获取设备的序列号
auto sn = devInfo->serialNumber();
std::cout << "Serial number: " << sn << std::endl;
```
获取支持的传感器列表
```
//获取支持的传感器列表
std::cout << "Sensor types: " << std::endl;
auto sensorList = dev->getSensorList();
for(uint32_t i = 0; i < sensorList->count(); i++) {
    auto sensor = sensorList->getSensor(i);
    switch(sensor->type()) {
    case OB_SENSOR_COLOR:
        std::cout << "\tColor sensor" << std::endl;
        break;
    case OB_SENSOR_DEPTH:
        std::cout << "\tDepth sensor" << std::endl;
        break;
    case OB_SENSOR_IR:
        std::cout << "\tIR sensor" << std::endl;
        break;
    case OB_SENSOR_GYRO:
        std::cout << "\tGyro sensor" << std::endl;
        break;
    case OB_SENSOR_ACCEL:
        std::cout << "\tAccel sensor" << std::endl;
        break;
    default:
        break;
    }
}
```
控制台结果输出如下：<br />

![image1](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage001.png)

## 深度示例-DepthViewer

功能描述：本示例主要演示了使用SDK获取深度数据并绘制显示、获取分辨率并选择进行设置、显示深度图像，并通过ESC键退出。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
```cpp
//获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try{
	//根据指定的格式查找对应的Profile,优先查找Y16格式
	depthProfile = profiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_Y16,30);
}catch(ob::Error &e){
	//没找到Y16格式后不匹配格式查找对应的Profile进行开流
	depthProfile = profiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
}
```
通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用深度流
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```
启动在Config中配置的流，如果不传参数，将启动默认配置启动流
```cpp
pipe.start(config);
```
设置镜像模式，先判断设备是否有可读可写的权限，再进行设置
```cpp
//获取镜像属性是否有可写的权限
if(pipe.getDevice()->isPropertySupported(OB_PROP_DEPTH_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
    //设置镜像
    pipe.getDevice()->setBoolProperty(OB_PROP_DEPTH_MIRROR_BOOL, true);
}
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```cpp
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
最终的深度图显示如下<br />![image2](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage002.png)<br />程序正常退出之后资源将会自动释放

## 彩色示例-ColorViewer

功能描述：本示例主要演示了使用SDK获取彩色数据并绘制显示、获取分辨率并选择进行设置、显示彩色图像，并通过ESC键退出。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
```cpp
auto profiles = pipe.getStreamProfiles(OB_SENSOR_COLOR);
try{
	//获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
	auto profiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);

	std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
	try{
		//根据指定的格式查找对应的Profile,优先选择RGB888格式
		colorProfile = profiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_RGB,30);
	}catch(ob::Error &e){
		//没找到RGB888格式后不匹配格式查找对应的Profile进行开流
		colorProfile = profiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
	}
	config->enableStream(colorProfile);
}catch(ob::Error &e){
	std::cerr<<"Current device is not support color sensor!"<<std::endl;
	exit(EXIT_FAILURE);
}
```
通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(colorProfile);
```
启动在Config中配置的流
```cpp
pipe.start(config);
```
设置镜像模式，先判断设备是否有可读可写的权限，再进行设置
```cpp
//获取镜像属性是否有可写的权限
if(pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
    //设置镜像
    pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
}
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```cpp
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
最终的彩色图显示如下<br />![image3](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage003.png)<br />程序正常退出之后资源将会自动释放
## 红外示例-InfraredViewer

功能描述：本示例主要演示了使用SDK获取红外数据并绘制显示、获取分辨率并选择进行设置、显示红外图像，并通过ESC键退出。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
通过输入的分辨率，格式，帧率等感兴趣项来获取红外相机的流配置
```cpp
//获取红外相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto profiles = pipe.getStreamProfileList(OB_SENSOR_IR);

std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
try{
	//根据指定的格式查找对应的Profile,优先查找Y16格式
	irProfile = profiles-getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16,30);
}catch(ob::Error &e){
	//没找到Y16格式后不匹配格式查找对应的Profile进行开流
	irProfile = profiles-getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_ANY,30);
}
```
通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用红外流
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(irProfile);
```
配置IR输出源
**说明：** 该功能仅Gemini2产品支持（OrbbecSDK1.5.5开始支持Gemini2产品)，Gemini2产品默认左IR sensor输出数据流
```c
//判断是否支持切换左右IR通道
if (pipe.getDevice()->isPropertySupported(OB_PROP_IR_CHANNEL_DATA_SOURCE_INT, OB_PERMISSION_READ_WRITE)) {
    // Gemini2产品支持SENSOR_IR选择sensor输出，0是左IR，1是右IR。
    int32_t dataSource = 0;
    pipe.getDevice()->setIntProperty(OB_PROP_IR_CHANNEL_DATA_SOURCE_INT, dataSource);
}
```

启动在Config中配置的流，如果不传参数，将启动默认配置启动流
```cpp
pipe.start(config);
```
设置镜像模式，先判断设备是否有可读可写的权限，再进行设置
```cpp
//获取镜像属性是否有可写的权限
if(pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
    //设置镜像
    pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
}
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
程序正常退出之后资源将会自动释放
## 流对齐示例-SyncAlignViewer

功能描述：本示例主要演示了对Sensor数据流控制对齐的操作。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
获取彩色相机和深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
```cpp
try{
    //获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
    auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try{
        //根据指定的格式查找对应的Profile,优先选择RGB888格式
        colorProfile = colorProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_RGB,30);
    }catch(ob::Error &e){
        //没找到RGB888格式后不匹配格式查找对应的Profile进行开流
        colorProfile = colorProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
    }
    config->enableStream(colorProfile);
}catch(...){
    std::cerr<<"Current device is not support color sensor!"<<std::endl;
    exit(EXIT_FAILURE);
}

//获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try{
    //根据指定的格式查找对应的Profile,优先查找Y16格式
    depthProfile = depthProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_Y16,30);
}catch(ob::Error &e){
    //没找到Y16格式后不匹配格式查找对应的Profile进行开流
    depthProfile = depthProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
}
```
通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流和深度流
```
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(colorProfile);
config->enableStream(depthProfile);
```
控制流对齐，此处开启软件对齐
```
// 配置对齐模式为软件D2C对齐
config->setAlignMode(ALIGN_D2C_SW_MODE);
```
启动在Config中配置的流，如果不传参数，将启动默认配置启动流
```
pipe.start(config);
```
 停止Pipeline，将不再产生帧数据
```
pipe.stop();
```
程序正常退出之后资源将会自动释放

## 固件升级示例-FirmwareUpgrade
功能描述：本示例演示如何用固件文件给设备升级。
> 本示例基于C++ Low Level API进行演示

在main函数接口通过命令参数获取固件文件
```cpp
// checkFirmwareFilePath()函数用于检查文件是否存在，实际代码中最好检查后缀是否为bin或者img, 以及固件文件是否与目标设备相匹配
std::string checkFirmwareFilePath(int argc, char **argv) {
    if(argc < 2) {
        std::cout << "Please input firmware path." << std::endl;
        return "";
    }

    std::string   filePath = std::string(*(argv + 1));
    std::ifstream fs(filePath);
    if(!fs.is_open()) {
        std::cout << "Open Firmware file failed. filePath: " << filePath << std::endl;
        return "";
    }

    fs.close();
    return filePath;
}

int main(int argc, char **argv) try {
    std::string firmwareFilePath = checkFirmwareFilePath(argc, argv);
    if(firmwareFilePath.empty()) {
        std::cout << "command: " << std::endl << "$ ./FirmwareUpgrade[.exe] firmwareFile.bin" << std::endl;
        return 0;
    }

    // 接下来的业务代码

    return 0;
}
catch(ob::Error &e) {
    // 处理OrbbecSDK接口调用异常，示例为了简洁连续几个接口一起try-catch，实际业务中推荐单独一个接口一个try-catch
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
```

创建ob::Context并通过ob::Context获取设备，本示例假设运行FirmwareUpgrade[.exe]之前上位机（Windows、Ubuntu、Android平台）已经插入设备。ob::DeviceChangedCallback用于固件升级后监听device重启后获取被升级设备的业务处理
```cpp
// 构建ob::Context对象
ob::Context ctx;
// 设置设备变化监听器，device_changed_callback是管理device声明周期的关键函数，开发者必须关注该回调
ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> addedList) {
    if(isWaitRebootComplete_) {
        if(addedList && addedList->deviceCount() > 0) {
            auto device = addedList->getDevice(0);
            if(isDeviceRemoved_ && deviceSN_ == std::string(device->getDeviceInfo()->serialNumber())) {
                rebootedDevice_       = device;
                isWaitRebootComplete_ = false;

                std::unique_lock<std::mutex> lk(waitRebootMutex_);
                waitRebootCondition_.notify_all();
            }
        }

        if(removedList && removedList->deviceCount() > 0) {
            if(deviceUid_ == std::string(removedList->uid(0))) {
                isDeviceRemoved_ = true;
            }
        }
    }  // if isWaitRebootComplete_
});

// 查询当前已经接入的设备
auto devList = ctx.queryDeviceList();

// 从ob::DeviceList中获取当前接入设备的数量
if(devList->deviceCount() == 0) {
     // 固件升级示例默认设备已经接入到上位机（Windows、Ubuntu、Android平台）
    std::cerr << "Device not found!" << std::endl;
    return -1;
}

// 获取第一个设备，index=0
auto dev = devList->getDevice(0);
// 打印设备信息
dumpDeviceInfo(dev);
```

获取设备当前的固件版本信息
```cpp
// 打印设备名称，SN，VID，PID以及固件版本
void dumpDeviceInfo(std::shared_ptr<ob::Device> device) {
    // 获取ob::DeviceInfo对象，通过ob::DeviceInfo可以获取目标设备的基本信息
    auto devInfo = device->getDeviceInfo();

    // 设备名称
    std::cout << "Device name: " << devInfo->name() << std::endl;

    // 设备VID，PID，UID
    std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

    // 设备当前的固件版本号
    auto fwVer = devInfo->firmwareVersion();
    std::cout << "Firmware version: " << fwVer << std::endl;

    // 设备SN
    auto sn = devInfo->serialNumber();
    std::cout << "Serial number: " << sn << std::endl;

    // devInfo资源会自动释放
}
```

给目标设备升级固件
a. 实现C API的固件升级回调接口；
b. 调用固件升级接口进行升级；
```cpp
// 对目标设备进行固件升级
bool upgradeFirmware(std::shared_ptr<ob::Device> device, std::string firmwarePath) {
    auto index     = firmwarePath.find_last_of(".img");
    bool isImgFile = index != std::string::npos;
    index          = firmwarePath.find_last_of(".bin");
    bool isBinFile = index != std::string::npos;
    if(!(isImgFile || isBinFile)) {
        // 固件升级文件一般为bin或者img，实际业务中最好通过文件名称、文件MD5等信息做防呆
        std::cout << "Upgrade Fimware failed. invalid firmware file: " << firmwarePath << std::endl;
        return false;
    }

    bool isUpgradeSuccess = false;
    try {
        // 调用固件升级接口进行升级；
        device->deviceUpgrade(
            firmwarePath.c_str(),
            [=, &isUpgradeSuccess](OBUpgradeState state, const char *message, uint8_t percent) {
                if(state == STAT_START) {
                    std::cout << "Upgrade Firmware start" << std::endl;
                }
                else if(state == STAT_FILE_TRANSFER) {
                    std::cout << "Upgrade Firmware file transfer, percent: " << (uint32_t)percent << std::endl;
                }
                else if(state == STAT_IN_PROGRESS) {
                    std::cout << "Upgrade Firmware in progress, percent: " << (uint32_t)percent << std::endl;
                }
                else if(state == STAT_DONE) {
                    // 固件升级成功
                    std::cout << "Upgrade Firmware done, percent: " << (uint32_t)percent << std::endl;
                    isUpgradeSuccess = true;
                }
                else if(state == STAT_VERIFY_IMAGE) {
                    std::cout << "Upgrade Firmware verify image" << std::endl;
                }
                else {
                    // 固件升级失败
                    std::string errMsg = (nullptr != message ? std::string(message) : "");
                    std::cout << "Upgrade Firmware failed. state: " << std::to_string(state) << ", errMsg: " << errMsg << ", percent: " << (uint32_t)percent
                              << std::endl;
                }
            },
            false);
    }
    catch(ob::Error &e) {
        std::cerr << "Upgrade Firmware ob error. function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage()
                  << "\ntype:" << e.getExceptionType() << std::endl;
    }
    catch(std::exception &e) {
        if(e.what()) {
            std::cout << "Upgrade Firmware Exception. what: " << std::string(e.what()) << std::endl;
        }
    }

    return isUpgradeSuccess;
}
```

固件升级成功后，需要重启设备。重启设备有两种方式，一种是拔插设备（或者操作系统重启），另一种是调用OrbbecSDK的reboot接口。等设备上线后，通过本示例的dump_device_info()函数可以查询当前设备固件版本信息

以下示例演示通过固件reboot接口重启设备
```cpp
// 重启设备
std::cout << "Reboot device" << std::endl;
isDeviceRemoved_      = false;
isWaitRebootComplete_ = true;
dev->reboot();
// 释放资源，设备重启后ob::Device对象就不能用了
dev     = nullptr;
```

程序正常退出之后资源将会自动释放

## 传感器控制示例-SensorControl

功能描述：本示例主要演示了对device控制命令的操作、对Sensor控制命令的操作、对Sensor进行流操作。
> 本示例基于C++ Low Level API进行演示

首先需要创建一个Context，用于获取设备信息列表和创建设备
```
ob::Context ctx;
```
查询设备信息列表
```
auto devList = ctx.queryDeviceList();
```
选择一个设备进行操作，如果插入单个设备默认选择并打开，如果存在多个设备提供选择
```
//选择一个设备进行操作
std::shared_ptr<ob::Device> device = nullptr;
if(deviceList->deviceCount() > 0) {
    if(deviceList->deviceCount() <= 1) {
    //如果插入单个设备，默认选择第一个
        device = deviceList->getDevice(0);
    }
    else {
        device = selectDevice(deviceList);
    }
}
```
控制命令控制，获取Property的范围，设置属性，获取属性
```
//获取Property的范围
OBBoolPropertyRange  bool_range = device->getBoolPropertyRange(property_item.id)
OBIntPropertyRange   int_range = device->getIntPropertyRange(property_item.id)
OBFloatPropertyRange float_range = device->getFloatPropertyRange(property_item.id)

//设置属性
device->setBoolProperty(propertyItem.id, bool_value);
device->setIntProperty(propertyItem.id, int_value);
device->setFloatProperty(propertyItem.id, float_value);

//获取属性
bool bool_ret = device->getBoolProperty(propertyItem.id);
int int_ret = device->getIntProperty(propertyItem.id);
float float_ret = device->getFloatProperty(propertyItem.id);
```
程序正常退出之后资源将会自动释放

## 多路流同时开流示例-MultiStream
功能描述：本示例主要演示用device同时打开多个sensor流的操作
> 本示例基于C++ Low Level API进行演示

首先需要创建一个Context，用于获取设备信息列表和创建设备
```cpp
ob::Context ctx;
```
查询设备信息列表
```cpp
auto devList = ctx.queryDeviceList();
```
选择一个设备进行操作，如果插入单个设备默认选择并打开，如果存在多个设备提供选择
```cpp
//选择一个设备进行操作
std::shared_ptr<ob::Device> device = nullptr;
if(deviceList->deviceCount() < 1) {
    // 示例默认设备已经插入到上位机
    return 0;
}
//默认选择第一个
device = deviceList->getDevice(0);
```

使用device创建ob::Pipeline对象
```cpp
ob::Pipeline pipe(device);
```

创建ob::Config用于配置pipeline要打开的stream流，目前pipeline仅支持Color,IR,Depth等UVC类型的视频流，不支持IMU类型的Gyro, Accel的流。
```cpp
    // 构建ob::Config配置对象，后续pipeline开流需要该对象
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // 尝试配置Color流
    try {
        auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
        auto colorProfile  = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(colorProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "color stream not found!" << std::endl;
    }

    // 尝试配置Depth流
    try {
        auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
        auto depthProfile  = depthProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(depthProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "depth stream not found!" << std::endl;
    }

    // 尝试配置IR流
    try {
        auto irProfiles = pipe.getStreamProfileList(OB_SENSOR_IR);
        auto irProfile  = irProfiles->getProfile(OB_PROFILE_DEFAULT);
        config->enableStream(irProfile->as<ob::VideoStreamProfile>());
    }
    catch(...) {
        std::cout << "ir stream not found!" << std::endl;
    }

    // 可选，非必须。打开硬件D2C对齐
    config->setAlignMode(ALIGN_D2C_HW_MODE);
```

将配置好的ob::Config传入到ob::Pipeline#start开流并监听回调
```cpp
// Pipeline#start需要传入config配置和FrameSetCallback帧回调对象
pipe.start(config, [&](std::shared_ptr<ob::FrameSet> frameset) {
    std::unique_lock<std::mutex> lk(videoFrameMutex);
    // 获取对应的帧数据，注意，colorFrame, depthFrame, irFrame可能为null
    colorFrame = frameset->colorFrame();
    depthFrame = frameset->depthFrame();
    irFrame    = frameset->irFrame();
});
```

关闭pipeline
```cpp
pipe.stop();
```

开启IMU流，陀螺仪和加速度计目前仅支持用sensor开流，不支持ob::Pipeline开流
开流步骤如下：
a. 从device中获取GyroSensor(陀螺仪)和AccelSensor(加速度计)，对象为ob::Sensor
b. 分别从GyroSensor和AccelSensor获取对应的配置GyroStreamProfile和AccelStreamProfile
c. 用ob::Sensor#start进行开流，参数为StreamProfile;
```cpp
std::shared_ptr<ob::Sensor> accelSensor;
std::shared_ptr<ob::Sensor> gyroSensor;
try {
    // 通过device构建IMU sensor对象
    accelSensor = device->getSensor(OB_SENSOR_ACCEL);
    gyroSensor  = device->getSensor(OB_SENSOR_GYRO);
}
catch(...) {
    std::cout << "IMU sensor not found!" << std::endl;
}

if(accelSensor && gyroSensor) {
    // 获取AccelStreamProfile
    auto accelProfiles = accelSensor->getStreamProfileList();
    auto accelProfile  = accelProfiles->getProfile(OB_PROFILE_DEFAULT);
    // 用ob::Sensor#start函数开流，需要传入StreamProfile和FrameCallback
    // 在FrameCallback帧回调函数中处理device返回的数据帧
    accelSensor->start(accelProfile, [&](std::shared_ptr<ob::Frame> frame) {
        std::unique_lock<std::mutex> lk(accelFrameMutex);
        accelFrame = frame;
    });

    // 获取GyroStreamProfile
    auto gyroProfiles = gyroSensor->getStreamProfileList();
    auto gyroProfile  = gyroProfiles->getProfile(OB_PROFILE_DEFAULT);
    // 用ob::Sensor#start函数开流，需要传入StreamProfile和FrameCallback
    // 在FrameCallback帧回调函数中处理device返回的数据帧
    gyroSensor->start(gyroProfile, [&](std::shared_ptr<ob::Frame> frame) {
        std::unique_lock<std::mutex> lk(gyroFrameMutex);
        gyroFrame = frame;
    });
}
```

关闭IMU sensor数据流
```cpp
accelSensor->stop();
gyroSensor->stop();
```

## 多设备示例-MultiDevice

功能描述：本示例主要演示了对多设备进行操作。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Context，获取设备信息列表用于创建设备及Pipeline
```
ob::Context ctx;
//查询已经接入设备的列表
auto devList = ctx.queryDeviceList();
//获取接入设备的数量
int devCount = devList->deviceCount();
```
创建设备，并通过设备创建Pipeline
```cpp
//遍历设备列表并创建设备
std::vector<std::shared_ptr<ob::Device>> devices;
for (int i = 0; i < devCount; i++)
{
    //获取设备并创建pipeline
	auto dev  = devList->getDevice( i );
	auto pipe = std::make_shared< ob::Pipeline >( dev );
	pipes.push_back( pipe );
}
```
打开多个设备的深度和彩色流
```cpp
for(auto &&pipe: pipes) {
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    //获取深度相机配置列表
    auto depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
    try{
        //根据指定的格式查找对应的Profile,优先查找Y16格式
        depthProfile = depthProfileList-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_Y16,30);
    }catch(ob::Error &e){
        //没找到Y16格式后不匹配格式查找对应的Profile进行开流
        depthProfile = depthProfileList-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
    }
    config->enableStream(depthProfile);

    //获取彩色相机配置列表
    try{
        auto colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
        std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;

        try{
            //根据指定的格式查找对应的Profile,优先选择RGB888格式
            colorProfile = colorProfileList-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_RGB,30);
        }catch(ob::Error &e){
            //没找到RGB888格式后不匹配格式查找对应的Profile进行开流
            colorProfile = colorProfileList-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
        }
        config->enableStream(colorProfile);
    }catch(ob::Error &e){
        std::cerr<<"Current device is not support color sensor!"<<std::endl;
    }

    //启动pipeline，并传入配置
    pipe->start(config, [i](std::shared_ptr<ob::FrameSet> frameSet) {
        std::lock_guard<std::mutex> lock(frameMutex);
        if(frameSet->colorFrame()){
            colorFrames[i] = frameSet->colorFrame();
        }
        if(frameSet->depthFrame()) {
            depthFrames[i] = frameSet->depthFrame();
        }
    });
    i++;
}
```
停止所有设备已打开的流
```cpp

void StopStream( std::vector< std::shared_ptr< ob::Pipeline > > pipes) {
    int i = 0;
    for ( auto&& pipe : pipes ) {
        if(colorFrames[i])
            colorFrames->reset();
        if(depthFrames[i])
            depthFrames->reset();
        if(irFrames[i])
            irFrames->reset();
        //停止pipeline
        pipe->stop();
        i++;
    }
}
```
程序正常退出之后资源将会自动释放

## 切换相机深度模式-DepthWorkMode
功能描述：本示例在OrbbecSDK1.5.5开始支持，仅支持Gemini2的设备。本示例主要演示切换相机深度模式，先查询深度模式列表，然后选择对应的相机深度模式，调用接口切换

首先获取设备
```cpp
//创建一个Context，与Pipeline不通，Context是底层API的入口，在开关流等常用操作上
//使用低级会稍微复杂一些，但是底层API可以提供更多灵活的操作，如获取多个设备，读写
//设备及相机的属性等
ob::Context ctx;

//查询已经接入设备的列表
auto devList = ctx.queryDeviceList();

//获取接入设备的数量
if(devList->deviceCount() == 0) {
    pressKeyExit("Device not found!");
    return -1;
}

// 获取设备列表中的第一个设备
auto device = devList->getDevice(0);
```

检查设备是否支持相机深度工作模式，当前只有Gemini2支持相机深度工作模式
```cpp
// 检查是否支持相机深度工作模式，目前（2022年12月5日）仅Gemini2双目摄像头支持深度工作模式
if (!device->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE)) {
    pressKeyExit("Current device not support depth work mode!");
    return -1;
}
```

查询设备的当前深度工作模式
```cpp
// 查询当前的相机深度模式
auto curDepthMode = device->getCurrentDepthWorkMode();
```

查询设备支持的相机深度模式列表
```cpp
// 获取相机深度模式列表
auto depthModeList = device->getDepthWorkModeList();
std::cout << "depthModeList size: " << depthModeList->count() << std::endl;
for(uint32_t i = 0; i < depthModeList->count(); i++) {
    std::cout << "depthModeList[" << i << "]: " << (*depthModeList)[i];
    if (strcmp(curDepthMode.name, (*depthModeList)[i].name) == 0) {
        std::cout << "  (Current WorkMode)";
    }

    std::cout << std::endl;
}
```

切换相机深度模式
```cpp
// 切换到新的相机深度模式
OBDepthMode depthMode = (*depthModeList)[index];
device->switchDepthWorkMode(depthMode.name);
check_error(error);
```

到此切换相机深度模式结束，可以用pipeline进行打开相机取流
注意：
1. 如果需要切换相机深度模式，那么打开数据流必须在切换深度工作模式之后；每个相机深度模式下支持的有效分辨率不同
2. 如果已经用pipeline打开数据流，那么切换相机深度工作模式前必须把原来申请的pipeline释放；
   切换相机深度工作模式后重新创建pipeline，否则会造成野指针或者内存泄露；

## 热拔插示例-HotPlugin

功能描述：本示例主要演示设备拔插回调的设置，以及拔插之后处理获取到的流。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Context，用于获取设备信息列表和创建设备
```
ob::Context ctx;
```
注册设备回调，分别在设备拔插的时候执行相关函数
```
ctx.setDeviceChangedCallback( []( std::shared_ptr< ob::DeviceList > removedList, std::shared_ptr< ob::DeviceList > addedList ) {
        DeviceDisconnectCallback( removedList );
        DeviceConnectCallback( addedList );
    } );
```
按照配置文件中配置的分辨率和格式进行开流
```
//按配置文件的流配置启动流，如果没有配置文件，将使用第0个流配置启动流
try{
  pipeline->start(nullptr);
}catch(...){
  std::cout<<"Pipeline start failed!"<<std::endl;
}
//获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto depthProfiles = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
//获取对应流配置列表的第0个Profile，如果有配置文件，配置文件里面流配置是流配置列表的第0个
auto depthProfile = depthProfiles->getProfile(OB_PROFILE_DEFAULT)->as<ob::VideoStreamProfile>();
//获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto colorProfiles = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
//获取对应流配置列表的第0个Profile，如果有配置文件，配置文件里面流配置是流配置列表的第0个
auto colorProfile = colorProfiles->getProfile(OB_PROFILE_DEFAULT)->as<ob::VideoStreamProfile>();
//获取帧率
colorFps = colorProfile->fps();
depthFps = depthProfile->fps();
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```
pipe.stop();
```
程序正常退出之后资源将会自动释放
## IMU示例-ImuReader

功能描述：本示例主要演示了使用SDK获取IMU数据并输出显示，并通过ESC键退出。
> 本示例基于C++ Low Level API进行演示

打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号
```cpp
//打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号
std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
```
首先需要创建一个Context，用于获取设备信息列表和创建设备
```cpp
ob::Context ctx;
```
查询已经接入设备的列表
```cpp
auto devList = ctx.queryDeviceList();

//获取接入设备的数量
devList->deviceCount()
```
创建设备
```cpp
//创建设备，0表示第一个设备的索引
auto dev = devList->getDevice(0);
```
获取陀螺仪传感器和加速度传感器
```cpp
auto gyroSensor = dev->getSensorList()->getSensor( OB_SENSOR_GYRO );

auto accelSensor = dev->getSensorList()->getSensor( OB_SENSOR_ACCEL );
```
获取陀螺仪传感器的配置列表并选择第一个配置开流，在开流的回调里获取帧的数据，加速度传感器同理
```cpp
auto profiles = gyroSensor->getStreamProfileList();
auto profile = profiles->getProfile( 0 );
gyroSensor->start( profile, []( std::shared_ptr< ob::Frame > frame ) {
		auto timeStamp = frame->timeStamp();
    auto gyroFrame = frame->as< ob::GyroFrame >();
    if ( gyroFrame != nullptr && ( timeStamp % 500 ) < 2 ) {  //  ( timeStamp % 500 ) < 2: 目的时减少打印频率
    		std::cout << "Gyro Frame: \n{\n";
        std::cout << "  tsp = " << timeStamp << std::endl;
        std::cout << "  temperature = " << gyroFrame->temperature() << std::endl;
        auto value = gyroFrame->value();
        std::cout << "  gyro.x = " << value.x << " dps" << std::endl;
        std::cout << "  gyro.y = " << value.y << " dps" << std::endl;
        std::cout << "  gyro.z = " << value.z << " dps" << std::endl;
        std::cout << "}" << std::endl << std::endl;
    }
 } );
```
关流
```cpp
gyroSensor->stop();
accelSensor->stop();
```
程序正常退出之后资源将会自动释放

## 多机同步示例-MultiDeviceSync
功能描述：演示配置多机同步参数，并开启多机同步

多机同步sample的步骤：
1. 读取配置文件，配置设备；
2. 重启设备；
3. 等待所有设备启动完毕，开始多机同步预览；

构建OrbbecSDK的上下文
```cpp
OBContext context;
```

#### 读取配置文件，配置设备
获取设备列表
```cpp
// 查询已经接入设备的列表
auto devList = context.queryDeviceList();

// 获取接入设备的数量
int devCount = devList->deviceCount();
for(int i = 0; i < devCount; i++) {
    configDevList.push_back(devList->getDevice(i));
}
```

下发多机同步配置
```cpp
for(auto config: deviceConfigList) {
    auto findItr = std::find_if(configDevList.begin(), configDevList.end(), [config](std::shared_ptr<ob::Device> device) {
        auto serialNumber = device->getDeviceInfo()->serialNumber();
        auto cmpSize      = (std::min)(strlen(serialNumber), config->deviceSN.size());
        return strncmp(serialNumber, config->deviceSN.c_str(), cmpSize) == 0;
    });
    if(findItr != configDevList.end()) {
        auto curConfig = (*findItr)->getSyncConfig();

        // 将配置文件的配置项更新，其他项保留原有配置
        curConfig.syncMode                    = config->syncConfig.syncMode;
        curConfig.irTriggerSignalInDelay      = config->syncConfig.irTriggerSignalInDelay;
        curConfig.rgbTriggerSignalInDelay     = config->syncConfig.rgbTriggerSignalInDelay;
        curConfig.deviceTriggerSignalOutDelay = config->syncConfig.deviceTriggerSignalOutDelay;
        curConfig.deviceId                    = config->syncConfig.deviceId;

        (*findItr)->setSyncConfig(curConfig);
    }
    else {
        std::cerr << "Device sn[" << config->deviceSN << "] is not connected. Set sync config failed" << std::endl;
        notMatchCount++;
    }
}
```

重启设备
```cpp
// 重启设备
for(auto device: configDevList) {
    rebootingDevInfoList.push_back(device->getDeviceInfo());
    device->reboot();
}
configDevList.clear();
```

等待所有设备重启完毕，后进行多机同步开流


#### 多机同步开流
多机同步开流示例演示了多个设备通过信号连接器连接然后同步触发出图的功能。

主要关键步骤：
1. 设置多机同步配置
2. 重启设备
3. 等待设备重启完毕，枚举设备并打开color和depth相机，并处理数据
备注：以下说明仅列关键代码，具体请参考Example\cpp\Sample-MultiDeviceSync\MultiDeviceSync.cpp


设置多机同步配置信息
```cpp
auto curConfig = device->getMultiDeviceSyncConfig();

// Update the configuration items of the configuration file, and keep the original configuration for other items
curConfig.syncMode                   = config->syncConfig.syncMode;
curConfig.depthDelayUs               = config->syncConfig.depthDelayUs;
curConfig.colorDelayUs               = config->syncConfig.colorDelayUs;
curConfig.trigger2ImageDelayUs       = config->syncConfig.trigger2ImageDelayUs;
curConfig.triggerOutEnable  = config->syncConfig.triggerOutEnable;
curConfig.triggerOutDelayUs = config->syncConfig.triggerOutDelayUs;

device->setMultiDeviceSyncConfig(curConfig);
```

重启所有设备
```cpp
std::cout << "Device sn[" << std::string(device->getDeviceInfo()->serialNumber()) << "] is configured, rebooting..." << std::endl;
try {
    device->reboot();
}
catch(ob::Error &e) {
    std::cout << "Device sn[" << std::string(device->getDeviceInfo()->serialNumber()) << "] is not configured, skipping...";
    // The early firmware versions of some models of devices will restart immediately after receiving the restart command, causing the SDK to fail to
    // receive a response to the command request and throw an exception
}
```

等待设备重启完毕，不同产品等待的时间不同


获取设备列表
```cpp
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
```

从设备读取多机同步配置，区分主机、从机
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

if(primary_devices.empty()) {
    std::cerr << "WARNING primary_devices is empty!!!" << std::endl;
}
```

设置时间同步频率
```cpp
// 启动多设备时间同步功能
context.enableDeviceClockSync(3600000);  // 每一小时更新同步一次
```

pipeline类PipelineHolder
因为一个pipeline关联一个sensorType，所以用PipelineHolder关联device和sensor信息，方便处理frame数据。
```cpp
typedef struct PipelineHolder_t {
    std::shared_ptr<ob::Pipeline> pipeline;
    OBSensorType                  sensorType;
    int                           deviceIndex;
    std::string                   deviceSN;
} PipelineHolder;

std::shared_ptr<PipelineHolder> createPipelineHolder(std::shared_ptr<ob::Device> device, OBSensorType sensorType, int deviceIndex) {
    PipelineHolder *pHolder = new PipelineHolder();
    pHolder->pipeline       = std::shared_ptr<ob::Pipeline>(new ob::Pipeline(device));
    pHolder->sensorType     = sensorType;
    pHolder->deviceIndex    = deviceIndex;
    pHolder->deviceSN       = std::string(device->getDeviceInfo()->serialNumber());

    return std::shared_ptr<PipelineHolder>(pHolder);
}
```

打开设备数据流
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

处理开关流的函数实现
```cpp
void handleColorStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
    std::cout << "Device#" << devIndex << ", color frame index=" << frame->index() << ", timestamp=" << frame->timeStamp()
              << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

    std::lock_guard<std::mutex> lock(frameMutex);
    colorFrames[devIndex] = frame;
}

void handleDepthStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
    std::cout << "Device#" << devIndex << ", depth frame index=" << frame->index() << ", timestamp=" << frame->timeStamp()
              << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

    std::lock_guard<std::mutex> lock(frameMutex);
    depthFrames[devIndex] = frame;
}
```
在这里我们就可以根据colorFrames和depthFrames处理多台设备输出的数据。

关闭设备数据流
```cpp
// close data stream
for(auto itr = pipelineHolderList.begin(); itr != pipelineHolderList.end(); itr++) {
    stopStream(*itr);
}
pipelineHolderList.clear();
```


## 点云示例-PointCloud

功能描述：连接设备开流 ,生成深度点云或RGBD点云并保存成ply格式文件。
> 本示例基于C++ High Level API进行演示

创建点云保存成ply格式文件函数，ply文件格式详细描述可在网络上查看<br />首先创建两个函数来保存从流里面获取到的点云数据，这是保存普通点云数据的函数
```cpp
//保存点云数据到ply
void savePointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "end_header\n");

    OBPoint *point = (OBPoint *)frame->data();
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
        point++;
    }

    fflush(fp);
    fclose(fp);
}
```
再创建一个保存彩色点云数据的函数用于存储彩色点云数据
```cpp
//保存彩色点云数据到ply
void saveRGBPointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "property uchar red\n");
    fprintf(fp, "property uchar green\n");
    fprintf(fp, "property uchar blue\n");
    fprintf(fp, "end_header\n");

    OBColorPoint *point = (OBColorPoint *)frame->data();
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
        point++;
    }

    fflush(fp);
    fclose(fp);
}
```
设置Log等级，避免过多Info等级的Log影响点云输出的结果
```cpp
ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_ERROR);
```
创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipeline;
```
然后可以通过Pipeline来获取彩色流和深度流的所有配置, 包括流的分辨率 ,帧率 ,以及流的格式，配置所需要彩色和深度流
```cpp
//通过创建Config来配置Pipeline要启用或者禁用哪些流
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
//获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try{
    //根据指定的格式查找对应的Profile,优先查找Y16格式
    depthProfile = depthProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_Y16,30);
}catch(ob::Error &e){
    //没找到Y16格式后不匹配格式查找对应的Profile进行开流
    depthProfile = depthProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
}
config->enableStream(depthProfile);

try{
    //获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
    auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try{
        //根据指定的格式查找对应的Profile,优先选择RGB888格式
        colorProfile = colorProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_RGB,30);
    }catch(ob::Error &e){
        //没找到RGB888格式后不匹配格式查找对应的Profile进行开流
        colorProfile = colorProfiles-getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
    }
    config->enableStream(colorProfile);
}catch(ob::Error &e){
    //如果不存在Color Sensor 点云转换分辨率配置为深度分辨率
    config->setDepthScaleRequire(false);
    config->setD2CTargetResolution(depthProfile->width(),depthProfile->height());
    std::cerr<<"Current device is not support color sensor!"<<std::endl;
}
```
开启D2C对齐, 生成RGBD点云时需要开启
```cpp
// 开启D2C对齐, 生成RGBD点云时需要开启
config->setAlignMode(ALIGN_D2C_HW_MODE);
```
创建点云Filter对象，并且设置相机内参
```cpp
// 创建点云Filter对象（点云Filter创建时会在Pipeline内部获取设备参数, 所以尽量在Filter创建前配置好设备）
ob::PointCloudFilter pointCloud;

//获取相机内参传入点云Filter中
auto cameraParam = pipeline.getCameraParam();
pointCloud.setCameraParam(cameraParam);
```
启动Pipeline
```cpp
pipeline.start( config );
```
设置些操作提示
```cpp
 std::cout << "Press R to create rgbd pointCloud and save to ply file! " << std::endl;
 std::cout << "Press d to create depth pointCloud and save to ply file! " << std::endl;
 std::cout << "Press ESC to exit! " << std::endl;
```
设置主流程通过上面创建的点云Filter对象获取并保存点云数据
```cpp
if(key == 'R' || key == 'r') {
  count = 0;
  //限制最多重复10次
  while(count++ < 10) {
    //等待一帧数据，超时时间为100ms
    auto frameset = pipeline.waitForFrames(100);
    if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
      try {
        //生成彩色点云并保存
        std::cout << "Save RGBD PointCloud ply file..." << std::endl;
        pointCloud.setCreatePointFormat(OB_FORMAT_RGB_POINT);
        std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
        saveRGBPointsToPly(frame, "RGBPoints.ply");
        std::cout << "RGBPoints.ply Saved" << std::endl;
      }
      catch(std::exception &e) {
        std::cout << "Get point cloud failed" << std::endl;
      }
      break;
    }
  }
}
else if(key == 'D' || key == 'd') {
  count = 0;
  //限制最多重复10次
  while(count++ < 10) {
    //等待一帧数据，超时时间为100ms
    auto frameset = pipeline.waitForFrames(100);
    if(frameset != nullptr && frameset->depthFrame() != nullptr) {
      try {
        //生成点云并保存
        std::cout << "Save Depth PointCloud to ply file..." << std::endl;
        pointCloud.setCreatePointFormat(OB_FORMAT_POINT);
        std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
        savePointsToPly(frame, "DepthPoints.ply");
        std::cout << "DepthPoints.ply Saved" << std::endl;
      }
      catch(std::exception &e) {
        std::cout << "Get point cloud failed" << std::endl;
      }
      break;
    }
  }
}

```
最后通过Pipeline来停止流
```cpp
 pipeline.stop();
```
程序正常退出后会释放资源
## 存储示例-SaveToDisk

功能描述：连接设备开流 , 获取彩色和深度图并存储为png格式。
> 本示例基于C++ High Level API进行演示

创建两个函数来用于将获取的图片保存到文件中
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
创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```
ob::Pipeline pipeline;
```
然后可以通过Pipeline来获取彩色流和深度流的所有配置, 包括流的分辨率 ,帧率 ,以及流的格式，配置所需要彩色和深度流
```cpp
try{
    //获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
    auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try{
        //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
        colorProfile = colorProfiles->getVideoStreamProfile(640,OB_HEIGHT_ANY,OB_FORMAT_MJPG,30);
    }catch(ob::Error &e){
        colorProfile = colorProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
    }
    config->enableStream(colorProfile);
}catch(ob::Error &e) {
    //没有Color Sensor
    std::cerr<<"Current device is not support color sensor!"<<std::endl;
}

//获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
auto depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try{
    //根据指定的格式查找对应的Profile,优先查找Y16格式
    depthProfile = depthProfiles->getVideoStreamProfile(640,OB_HEIGHT_ANY,OB_FORMAT_Y16,30);
}catch(ob::Error &e){
    //没找到Y16格式后不匹配格式查找对应的Profile进行开流
    depthProfile = depthProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY,OB_FORMAT_ANY,30);
}
config->enableStream(depthProfile);
```
启动流
```cpp
pipeline.start( config );
```
通过等待的方式来获取帧的数据
```cpp
auto frameset = pipeline.waitForFrames( 100 );
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
formatConverFilter.setFormatConvertType(FORMAT_RGB888_TO_BGR);
colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
```
通过开头的编辑的存储函数来存储获得的数据
```cpp
saveColor( colorFrame );
saveDepth( depthFrame );
```
停止Pipeline
```cpp
pipeline.stop();
```
程序正常退出后会释放资源
## 录制示例-Recorder

功能描述：连接设备开流 , 录制当前视频流到文件。
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
通过输入的分辨率，格式，帧率等感兴趣项来获取深度相机的流配置
```cpp
//获取深度流配置
auto profiles     = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try {
    //根据指定的格式查找对应的Profile,优先查找Y16格式
    depthProfile = profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
}
catch (ob::Error& e) {
    //没找到Y16格式后不匹配格式查找对应的Profile进行开流
    depthProfile = profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30);
}
```
通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用深度流
```cpp
//配置Pipeline打开的流类型
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```
启动在Config中配置的流，如果不传参数，将启动默认配置启动流，启动录制开关
```cpp
pipe.start(config);
pipe.startRecord("./OrbbecPipeline.bag");
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
程序正常退出后会释放资源
## 回放示例-Playback

功能描述：连接设备开流 , 载入视频文件进行回放。
> 本示例基于C++ High Level API进行演示

首先需要创建一个用于回放文件的Pipeline
```cpp
//创建用于回放的pipeline对象
ob::Pipeline pipe("./OrbbecPipeline.bag");
```
获取回放对象的回放状态回调
```cpp
//获取回放对象设置回放状态回调
auto playback = pipe.getPlayback();
playback->setPlaybackStateCallback([&](OBMediaState state) {
    if(state == OB_MEDIA_BEGIN) {
        std::cout << "Playback file begin." << std::endl;
    }
    else if(state == OB_MEDIA_END) {
        std::cout << "Playback file end." << std::endl;
    }
});
```
从回放文件中读取设备信息与内参信息
```cpp
//从回放文件中读取设备信息
auto deviceInfo = playback->getDeviceInfo();
std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
  << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;

//从回放文件中读取内参信息
auto cameraParam = pipe.getCameraParam();
std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
              << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;
```
开启回放
```cpp
//开启回放
pipe.start(NULL);
```
以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
程序正常退出后会释放资源
