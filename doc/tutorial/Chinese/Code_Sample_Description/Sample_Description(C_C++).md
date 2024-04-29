  获取所有示例都可以在工程的Examples目录中找到

| **名称** | **语言** | **描述** |
| --- | --- | --- |
| HelloOrbbec | C | 演示连接到设备获取SDK版本和设备信息 |
| 固件升级示例 | C | 演示选择固件bin或者img文件给设备升级固件版本 |
| 深度示例 | C | 演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像 |
| 彩色示例       | C        | 演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像 |
| 热拔插示例 | C | 演示设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接 |
| 点云示例 | C | 演示生成深度点云或RGBD点云并保存成ply格式文件 |
| 流对齐处理单元 | C | 演示对传感器数据流对齐的操作 |
| 双红外示例 | C | 演示获取双红外相机图像 |
| 红外示例 | C | 演示获取红外相机图像 |
| 后处理示例 | C | 演示进行后处理操作 |
| HDR示例 | C | 演示开启HDR操作 |
| 彩色和深度示例       | C        | 演示使用SDK获取彩色和深度0数据并绘制显示、获取分辨率并进行设置、显示彩色图像 |
| HelloOrbbec | C++ | 演示连接到设备获取SDK版本和设备信息 |
| 深度示例 | C++ | 演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像 |
| 彩色示例 | C++ | 演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像、输出metadata |
| d2c对齐示例 | C++ | 演示对传感器数据流对齐的操作 |
| 固件升级示例 | C++ | 演示选择固件bin或者img文件给设备升级固件版本 |
| 多路流同时开流示例 | C++ | 演示一个设备同时打开多路流的操作 |
| 多设备示例 | C++ | 演示对多设备进行操作 |
| 热拔插示例 | C++ | 演示设备拔插回调的设置，并获取到插拔后处理的流 |
| 点云示例 | C++ | 演示生成深度点云或RGBD点云并保存成ply格式文件 |
| 存储示例 | C++ | 获取彩色和深度图并存储为png格式 |
| 录制示例 | C++ | 录制当前视频流到文件 |
| 回放示例 | C++ | 载入视频文件进行回放 |
| 深度精度示例 | C++ | 演示获取深度精度操作 |
| HDR示例 | C++ | 演示开启HDR操作 |
| 深度工作模式示例 | C++ | 演示转换深度工作模式操作 |
| 双红外示例 | C++ | 演示获取双红外相机图像 |
| 红外示例 | C++ | 演示获取红外相机图像 |
| imu读取示例 | C++ | 演示获取imu数据 |
| 多设备同步示例 | C++ | 演示多设备同步操作 |
| 后处理示例 | C++ | 演示进行后处理操作 |
| 传感器控制示例 | C++ | 演示进行传感器控制操作 |
| 流对齐处理单元 | C++ | 演示传感器数据流同步对齐的操作 |

# C
## HelloOrbbec

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源，并通过ESC_KEY键退出程序
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
## 深度示例-DepthViewer

功能描述：演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像，并通过ESC_KEY键退出程序

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

功能描述：演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像，并通过ESC_KEY键退出程序

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


## 热拔插示例-HotPlugin

功能描述： 设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接。，并通过ESC_KEY键退出程序
> 本示例基于C Low Level API进行注册设备上下线回调，基于High Level API进行开流取帧演示

创建设备连接处理函数，函数内创建pipeline并调用create_and_start_with_config开启彩色流和深度流。
```c
//设备连接处理
void handle_device_connected(ob_device_list *connect_list) {
    ob_error *error = NULL;

    std::lock_guard<std::recursive_mutex> lk(pipeline_holder_mutex);
    uint32_t                              count = ob_device_list_device_count(connect_list, &error);
    check_error(error);
    printf("Device connect: %d\n", count);
    for(uint32_t i = 0; i < count; i++) {
        const char *uid = ob_device_list_get_device_uid(connect_list, i, &error);
        check_error(error);
        std::string str_uid = std::string(uid);
        auto        itr     = pipeline_holder_map.find(str_uid);
        if(itr != pipeline_holder_map.end()) {
            printf("Deveice connect, device already connection.");
            print_device_info(itr->second->device_info);
            printf("\n");
        }
        else {
            ob_device *device = ob_device_list_get_device(connect_list, i, &error);
            check_error(error);

            ob_pipeline *pipeline = ob_create_pipeline_with_device(device, &error);
            check_error(error);

            ob_device_info *device_info = ob_device_get_device_info(device, &error);
            check_error(error);

            std::shared_ptr<pipeline_holder> holder(new pipeline_holder{ device, device_info, pipeline, false, 0, 0, 0, 0, 0 });
            start_stream(holder);
            pipeline_holder_map.insert({ str_uid, holder });

            printf("Deveice connect, ");
            print_device_info(device_info);
            printf("\n");
        }
    }
}
```
创建设备断开处理函数，函数内将pipeline停止，并将退出pipeline标记isExit设为true
```c
void handle_device_disconnected(ob_device_list *disconnect_list) {
    ob_error *error = NULL;

    std::lock_guard<std::recursive_mutex> lk(pipeline_holder_mutex);
    uint32_t                              count = ob_device_list_device_count(disconnect_list, &error);
    check_error(error);
    printf("Device disconnect: %d\n", count);

    for(uint32_t i = 0; i < count; i++) {
        const char *uid = ob_device_list_get_device_uid(disconnect_list, i, &error);
        check_error(error);
        std::string str_uid = std::string(uid);
        auto        itr     = pipeline_holder_map.find(str_uid);
        if(itr != pipeline_holder_map.end()) {
            auto holder = itr->second;
            stop_stream(holder);
            pipeline_holder_map.erase(str_uid);

            printf("Deveice discconnect, ");
            print_device_info(holder->device_info);
            printf("\n");

            delete_holder(holder);
        }
        else {
            const char *uid = ob_device_list_get_device_uid(disconnect_list, i, &error);
            check_error(error);
            printf("Device disconnect, Unsolve device, uid: %s\n", uid);
        }
    }
}
```
设备连接、断开处理函数将会在设备上下线回调函数内被调用
```c
//设备状态改变回调
void on_device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data) {
    handle_device_disconnected(removed);
    handle_device_connected(added);

    ob_error *error = NULL;

    // Need to manually destroy the device list
    // destroy device list
    ob_delete_device_list(removed, &error);
    check_error(error);

    // destroy device list
    ob_delete_device_list(added, &error);
    check_error(error);
}
```
主函数main内，创建一个Context并注册设备上下线回调
```c
 //创建上下文
ob_context* ctx = ob_create_context( &error );

//注册设备回调
ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );
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

功能描述：连接设备开流，生成深度点云或RGBD点云并保存成ply格式文件，并通过ESC_KEY键退出程序
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


//配置Color流
ob_stream_profile_list *colorProfiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
if(error) {
    printf("Current device is not support color sensor!\n");
    ob_delete_error(error);
    error = NULL;
    // Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds
    ob_config_set_align_mode(config, ALIGN_DISABLE, &error);
    check_error(error);
}
// Open the default profile of Color Sensor, which can be configured through the configuration file
if(colorProfiles) {
    color_profile = ob_stream_profile_list_get_profile(colorProfiles, OB_PROFILE_DEFAULT, &error);
}

    //配置Depth流
if(color_profile) {
    // Try find supported depth to color align hardware mode profile
    depthProfiles = ob_get_d2c_depth_profile_list(pipeline, color_profile, ALIGN_D2C_HW_MODE, &error);
    check_error(error);
    int d2cCount = ob_stream_profile_list_count(depthProfiles, &error);
    check_error(error);
    if(d2cCount > 0) {
        align_mode = ALIGN_D2C_HW_MODE;
    }
    else {
        // Try find supported depth to color align software mode profile
        depthProfiles = ob_get_d2c_depth_profile_list(pipeline, color_profile, ALIGN_D2C_SW_MODE, &error);
        check_error(error);
        d2cCount = ob_stream_profile_list_count(depthProfiles, &error);
        check_error(error);
        if(d2cCount > 0) {
            align_mode = ALIGN_D2C_SW_MODE;
        }
    }
}
else {
    depthProfiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
    check_error(error);
}
```
打开设备D2C功能
```c
// Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds
ob_config_set_align_mode(config, align_mode, &error);
```

开流
```c
ob_pipeline_start_with_config(pipeline, config, &error);
```

在开流后创建点云filter。点云filter用于将depth和color帧数据转换成点云数据。点云filter在开流后创建目的是让SDK内部自动根据当前开流配置设置好相机参数。当然也可以自行设置。
```c
// Create a point cloud Filter object (device parameters will be obtained inside the Pipeline when the point cloud filter is created, so try to configure
// the device before creating the filter)
ob_filter *point_cloud = ob_create_pointcloud_filter(&error);
check_error(error);

// Obtain the current open-stream camera parameters from the pipeline and pass them to the point cloud filter
ob_camera_param camera_param = ob_pipeline_get_camera_param(pipeline, &error);
check_error(error);
ob_pointcloud_filter_set_camera_param(point_cloud, camera_param, &error);
check_error(error);
```

启动主循环，循环内根据用户按键调用点云filter生成深度点云或RGBD点云数据，并保存成ply文件。
```c
//等待一帧数据，超时时间为100ms
ob_frame *frameset = ob_pipeline_wait_for_frameset(pipeline, 100, &error);
check_error(error);
if(frameset != NULL) {
    // get depth value scale
    ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
    check_error(error);
    if(depth_frame == NULL) {
        continue;
    }

    // get depth value scale
    float depth_value_scale = ob_depth_frame_get_value_scale(depth_frame, &error);
    check_error(error);

    // delete depth frame
    ob_delete_frame(depth_frame, &error);
    check_error(error);

    // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
    // millimeter)
    ob_pointcloud_filter_set_position_data_scale(point_cloud, depth_value_scale, &error);
    check_error(error);

    ob_pointcloud_filter_set_point_format(point_cloud, OB_FORMAT_RGB_POINT, &error);
    check_error(error);
    ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
    check_error(error);
    if(pointsFrame != NULL) {
        save_rgb_points_to_ply(pointsFrame, "rgb_points.ply");
        printf("rgb_points.ply Saved\n");
        ob_delete_frame(pointsFrame, &error);
        check_error(error);
        points_created = true;
    }
    ob_delete_frame(frameset, &error);  // Destroy frameSet to reclaim memory
    check_error(error);
    if(points_created) {
        break;
    }
}
```
主循环退出后，停流并销毁回收资源
```c
// stop pipeline
ob_pipeline_stop(pipeline, &error);
check_error(error);

// destroy pipeline
ob_delete_pipeline(pipeline, &error);
check_error(error);

// destroy config
ob_delete_config(config, &error);
check_error(error);

// destroy profile
ob_delete_stream_profile(depth_profile, &error);
check_error(error);

// destroy profile
ob_delete_stream_profile(color_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(colorProfiles, &error);
check_error(error);

ob_delete_stream_profile_list(depthProfiles, &error);
check_error(error);
```

## 流对齐处理单元-AlignFilterViewer

支持设备： G3系列相机，例如：Gemini G335

功能描述：演示使用SDK进行流对齐演示，显示对齐后的图像，并通过ESC_KEY键退出程序

>本示例基于C High Level API进行演示

创建Pipeline及流配置
```c
// Create a pipeline to open the depth stream after connecting the device
pipeline = ob_create_pipeline(&error);
check_error(error);

// Create config to configure the resolution, frame rate, and format of the depth stream
ob_config *config = ob_create_config(&error);
check_error(error);

// Configure the depth stream
ob_stream_profile      *depth_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
check_error(error);

// Find the corresponding profile according to the specified format, first look for the y16 format
depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
// If the specified format is not found, search for the default profile to open the stream
if(error) {
    depth_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    ob_delete_error(error);
    error = nullptr;
}

// enable stream
ob_config_enable_stream(config, depth_profile, &error);
check_error(error);

// Configure the color stream
ob_stream_profile *     color_profile = nullptr;
ob_stream_profile_list *color_rofiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
if(error) {
    printf("Current device is not support color sensor!\n");
    exit(EXIT_FAILURE);
}

// Find the corresponding Profile according to the specified format, and choose the RGB888 format first
color_profile = ob_stream_profile_list_get_video_stream_profile(color_rofiles, 1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30, &error);
// If the specified format is not found, search for the default Profile to open the stream
if(error) {
    color_profile = ob_stream_profile_list_get_profile(color_rofiles, OB_PROFILE_DEFAULT, &error);
    ob_delete_error(error);
    error = nullptr;
}

// enable stream
ob_config_enable_stream(config, color_profile, &error);
check_error(error);
```

配置流对齐方式
```c
/* Config depth align to color or color align to depth.
ob_stream_type align_to_stream = OB_STREAM_DEPTH; */
ob_stream_type align_to_stream = OB_STREAM_COLOR;
ob_filter *    align_filter    = ob_create_align(&error, align_to_stream);
```

通过Config，启动Pipeline
```c
// Start the pipeline with config
ob_pipeline_start_with_config(pipeline, config, &error);
check_error(error);
```

获取对齐后的数据
```c
ob_frame *new_frame_set = ob_filter_process(align_filter, frameset, &error);
check_error(error);

new_depth_frame = ob_frameset_depth_frame(new_frame_set, &error);
check_error(error);

new_color_frame = ob_frameset_color_frame(new_frame_set, &error);
check_error(error);
```

释放资源，退出程序。

```c
ob_delete_filter(align_filter, &error);
check_error(error);

// stop the pipeline
ob_pipeline_stop(pipeline, &error);
check_error(error);

// destroy the window
delete win;

// destroy profile
ob_delete_stream_profile(depth_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(profiles, &error);
check_error(error);

// destroy profile
ob_delete_stream_profile(color_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(color_rofiles, &error);
check_error(error);

// destroy the pipeline
ob_delete_pipeline(pipeline, &error);
check_error(error);
```

## 双红外示例-DoubleInfraredViewer

功能描述：演示获取双红外相机图像，显示左右红外图像，并通过ESC_KEY键退出程序

>本示例基于C High Level API进行演示

创建pipeline和config
```c
// Create a pipeline to open the Infrared stream after connecting the device
pipeline = ob_create_pipeline(&error);
check_error(error);

// Create config to configure the resolution, frame rate, and format of the Infrared stream
ob_config *config = ob_create_config(&error);
check_error(error);
```

获取左IR流配置
```c
// Configure the infrared stream(IR_LEFT)
ob_stream_profile      *ir_left_profile  = NULL;
ob_stream_profile_list *ir_left_profiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_IR_LEFT, &error);
check_error(error);

if(ir_left_profiles == nullptr) {
    printf("The obtained IR(Left) resolution list is NULL. For monocular structured light devices, try opening the IR data stream using the IR example. ");
    return 0;
}

// Find the corresponding profile according to the specified format, first look for the y16 format
ir_left_profile = ob_stream_profile_list_get_profile(ir_left_profiles, OB_PROFILE_DEFAULT, &error);
check_error(error);

// enable stream
ob_config_enable_stream(config, ir_left_profile, &error);
check_error(error);
```

获取右IR流配置
```c
// Configure the infrared stream(IR_RIGHT)
ob_stream_profile      *ir_right_profile  = NULL;
ob_stream_profile_list *ir_right_profiles = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_IR_RIGHT, &error);
check_error(error);

// Find the corresponding profile according to the specified format, first look for the y16 format
ir_right_profile = ob_stream_profile_list_get_profile(ir_right_profiles, OB_PROFILE_DEFAULT, &error);
check_error(error);

// enable stream
ob_config_enable_stream(config, ir_right_profile, &error);
check_error(error);
```

通过config开启pipeline
```c
ob_pipeline_start_with_config(pipeline, config, &error);
check_error(error);
```

获取左右IR帧数据
```c
ob_frame *ir_left_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_LEFT, &error);
check_error(error);

ob_frame *ir_right_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_RIGHT, &error);
check_error(error);
```

// 停止 pipeline
```c
ob_pipeline_stop(pipeline, &error);
check_error(error);
```

释放相关资源
```c
// destroy the window
delete win;

// destroy profile
ob_delete_stream_profile(ir_left_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(ir_left_profiles, &error);
check_error(error);

// destroy profile
ob_delete_stream_profile(ir_right_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(ir_right_profiles, &error);
check_error(error);

// destroy the pipeline
ob_delete_pipeline(pipeline, &error);
check_error(error);
```

## 红外示例-InfraredViewer

功能描述：演示获取红外相机图像，显示红外图像，并通过ESC_KEY键退出程序

>本示例基于C High Level API进行演示

创建pipeline，进行流配置
```c
// Create a pipeline to open the Infrared stream after connecting the device
pipeline = ob_create_pipeline(&error);
check_error(error);

// Create config to configure the resolution, frame rate, and format of the Infrared stream
ob_config *config = ob_create_config(&error);
check_error(error);

// Configure the infrared stream
// Please adjust the sensor according to the actual product, some device types only have OB_SENSOR_IR_LEFT and OB_SENSOR_IR_RIGHT.
ob_stream_profile      *ir_profile = NULL;
ob_stream_profile_list *profiles   = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_IR, &error);
check_error(error);

if(profiles == nullptr) {
    printf(
        "The obtained IR resolution list is NULL. For binocular structured light devices, try using the doubleIr example to turn on the ir data stream. ");
    return 0;
}

// Find the corresponding profile according to the specified format, first look for the y16 format
ir_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
// If the specified format is not found, search for the default profile to open the stream
if(error) {
    ir_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    ob_delete_error(error);
    error = nullptr;
}

// enable stream
ob_config_enable_stream(config, ir_profile, &error);
check_error(error);
```

通过config开启pipeline
```c
// Start the pipeline with config
ob_pipeline_start_with_config(pipeline, config, &error);
check_error(error);
```

释放相关资源
```c
// stop the pipeline
ob_pipeline_stop(pipeline, &error);
check_error(error);

// destroy the window
delete win;

// destroy profile
ob_delete_stream_profile(ir_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(profiles, &error);
check_error(error);

// destroy the pipeline
ob_delete_pipeline(pipeline, &error);
check_error(error);
```

## 后处理示例-Post-Processing

支持设备： G3系列相机，例如：Gemini G335

功能描述：演示使用后处理操作，显示后处理后的图像，并通过ESC_KEY键退出程序

>本示例基于C High Level API进行演示

创建pipeline，进行流配置
```c
// Create a pipeline to open the depth stream after connecting the device
pipeline = ob_create_pipeline(&error);
check_error(error);

// Create config to configure the resolution, frame rate, and format of the depth stream
ob_config *config = ob_create_config(&error);
check_error(error);

// Configure the depth stream
ob_stream_profile      *depth_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
check_error(error);

// Find the corresponding profile according to the specified format, first look for the y16 format
depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
// If the specified format is not found, search for the default profile to open the stream
if(error) {
    depth_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    ob_delete_error(error);
    error = nullptr;
}

// enable stream
ob_config_enable_stream(config, depth_profile, &error);
check_error(error);
```

获取传感器
```c
// Get device
ob_device *device = ob_pipeline_get_device(pipeline, &error);
check_error(error);

// Get depth sensor
ob_sensor *depthSensor = ob_device_get_sensor(device, OB_SENSOR_DEPTH, &error);
check_error(error);
```

获取后处理列表
```c
ob_filter_list *filterList = ob_sensor_get_recommended_filter_list(depthSensor, &error);
check_error(error);
```

后处理
```c
ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
check_error(error);

if(depth_frame != nullptr) {
    for(uint32_t i = 0; i < count; i++) {
        ob_frame * new_depth_frame = nullptr;
        ob_filter *obFilter = filters[i];
        bool enable = ob_filter_is_enable(obFilter, &error);
        check_error(error);
        if(enable) {
            new_depth_frame = ob_filter_process(obFilter, depth_frame, &error);
            check_error(error);

            ob_delete_frame(depth_frame, &error);
            check_error(error);

            depth_frame = new_depth_frame;
        }
    }
}
```

释放相关资源
```c
for(uint32_t i = 0; i < count; i++) {
    ob_filter *obFilter = filters[i];
    ob_delete_filter(obFilter, &error);
    check_error(error);
}

// destroy profile filterList
ob_delete_filter_list(filterList, &error);
check_error(error);

// stop the pipeline
ob_pipeline_stop(pipeline, &error);
check_error(error);

// destroy the window
delete win;

// destroy profile
ob_delete_stream_profile(depth_profile, &error);
check_error(error);

// destroy profile list
ob_delete_stream_profile_list(profiles, &error);
check_error(error);

// destroy the pipeline
ob_delete_pipeline(pipeline, &error);
check_error(error);
```

## HDR示例-HdrMerge

支持设备： G3系列相机，例如：Gemini G335

功能描述：演示使用HDR操作，显示HDR处理后的图像，并通过ESC_KEY键退出程序

>本示例基于C High Level API进行演示

创建pipeline，配置depth和ir流
```c
// Create a pipeline to open the depth stream after connecting the device
pipeline = ob_create_pipeline(&error);
check_error(error);

// Create config to configure the resolution, frame rate, and format of the depth stream
ob_config *config = ob_create_config(&error);
check_error(error);

// Configure the depth stream
ob_stream_profile      *depth_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
check_error(error);

// Find the corresponding profile according to the specified format, first look for the y16 format
depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
// If the specified format is not found, search for the default profile to open the stream
if(error) {
    depth_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    ob_delete_error(error);
    error = nullptr;
}

// enable stream
ob_config_enable_stream(config, depth_profile, &error);
check_error(error);

// Configure the infrared stream
// Please adjust the sensor according to the actual product, some device types only have OB_SENSOR_IR_LEFT and OB_SENSOR_IR_RIGHT.
ob_stream_profile *     ir_profile = NULL;
ob_stream_profile_list *ir_profiles   = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_IR_LEFT, &error);
check_error(error);

if(profiles == nullptr) {
    printf(
        "The obtained IR resolution list is NULL. For binocular structured light devices, try using the doubleIr example to turn on the ir data stream. ");
    return 0;
}

// Find the corresponding profile according to the specified format, first look for the y16 format
ir_profile = ob_stream_profile_list_get_video_stream_profile(ir_profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y8, 30, &error);
// If the specified format is not found, search for the default profile to open the stream
if(error) {
    ir_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    ob_delete_error(error);
    error = nullptr;
}

// enable stream
ob_config_enable_stream(config, ir_profile, &error);
check_error(error);
```

获取device
```c
// Get the device through the pipeline
ob_device *dev = ob_pipeline_get_device(pipeline, &error);
check_error(error);
```

打开HDR处理
```c
// Create HdrMerage post processor
ob_filter *hdr_merge_filter = ob_create_hdr_merge(&error);
check_error(error);

bool is_supported = ob_device_is_property_supported(dev, OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE, &error);
check_error(error);
// open hdr merage
if(is_supported) {
    // Get depth exposure value range,the exposure_1 and exposure_2 in OBHdrConfig can be adjusted.
    ob_int_property_range depth_exp_range = ob_device_get_int_property_range(dev, OB_PROP_DEPTH_EXPOSURE_INT, &error);
    check_error(error);

    // Get depth gain value range,,the gain_1 and gain_1 in OBHdrConfig can be adjusted.
    ob_int_property_range depth_gain_range = ob_device_get_int_property_range(dev, OB_PROP_DEPTH_GAIN_INT, &error);
    check_error(error);

    ob_hdr_config hdr_config;
    uint32_t      dataSize = sizeof(ob_hdr_config);
    ob_device_get_structured_data(dev, OB_STRUCT_DEPTH_HDR_CONFIG, &hdr_config, &dataSize, &error);
    check_error(error);
    // open hdr
    hdr_config.enable = true;

    ob_device_set_structured_data(dev, OB_STRUCT_DEPTH_HDR_CONFIG, &hdr_config, dataSize, & error);
    check_error(error);
}
```

开启pipeline
```c
ob_pipeline_start_with_config(pipeline, config, &error);
```

获取HDR处理后的图像
```c
auto new_frame_set = ob_filter_process(hdr_merge_filter, frameset, &error);
check_error(error);

auto new_depth_frame = ob_frameset_depth_frame(new_frame_set, &error);
check_error(error);
if(new_depth_frame != nullptr) {
    frames.push_back(new_depth_frame);
}
```

停止pipeline
```c
ob_pipeline_stop(pipeline, &error);
```

## 彩色和深度示例-ColorDepthViewer

功能描述：演示使用SDK获取彩色和深度0数据并绘制显示、获取分辨率并进行设置、显示彩色图像，并通过ESC_KEY键退出程序

>本示例基于C High Level API进行演示

创建pipeline，配置流信息
```c
// 1. Create a pipeline to open the Color and Depth streams after connecting the device
pipe = ob_create_pipeline(&error);
check_error(error);

// 2. Create config to configure the resolution, frame rate, and format of Color and Depth streams
ob_config *config = ob_create_config(&error);
check_error(error);

// 2.1 Configure Depth stream
ob_stream_profile      *depth_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_DEPTH, &error);
check_error(error);
// Find the corresponding Profile according to the specified format
depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, OB_WIDTH_ANY, OB_HEIGHT_ANY, , OB_FORMAT_Y16, 30, &error);
check_error(error);
if(!depth_profile) {  // If the specified profile is not found, use the first profile in the profile list
    depth_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    check_error(error);
}
ob_config_enable_stream(config, depth_profile, &error);  // enable stream
check_error(error);

// 2.2 Configure Color stream
ob_stream_profile *color_profile = NULL;
profiles                         = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_COLOR, &error);
check_error(error);
// Find the corresponding Profile according to the specified format
color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_MJPG, 30, &error);
check_error(error);
if(!color_profile) {  // If the specified format profile is not found, use the first profile in the profile list
    color_profile = ob_stream_profile_list_get_profile(profiles, OB_PROFILE_DEFAULT, &error);
    check_error(error);
}
ob_config_enable_stream(config, color_profile, &error);  // enable stream
check_error(error);
```

关闭同步
```c
ob_pipeline_disable_frame_sync(pipe, &error);
check_error(error);
```

通过配置开启pipeline
```c
ob_pipeline_start_with_config(pipe, config, &error);
check_error(error);
```

获取帧数据
```c
ob_frame *color_frame = ob_frameset_color_frame(frameset, &error);
ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
```

停止pipeline
```c
ob_pipeline_stop(pipe, &error);
```

释放相关资源
```c
ob_delete_stream_profile(depth_profile, &error);
check_error(error);

ob_delete_stream_profile(color_profile, &error);
check_error(error);

ob_delete_stream_profile_list(profiles, &error);
check_error(error);

ob_delete_pipeline(pipe, &error);
check_error(error);
```

# C++
## HelloOrbbec

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源，并通过ESC_KEY键退出程序
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
if(devList->deviceCount() == 0) {
    std::cerr << "Device not found!" << std::endl;
    return -1;
}
```

创建设备
```cpp
//创建设备，0表示第一个设备的索引
auto dev = devList->getDevice(0);
```

获取当前preset信息
```cpp
auto devicePresetList = dev->getAvailablePresetList();
auto devicePreset     = dev->getCurrentPresetName();
std::cout << "Available preset list: " << std::endl;
for(uint32_t i = 0; i < devicePresetList->count(); i++) {
    auto name = devicePresetList->getName(i);
    if(std::string(name) == std::string(devicePreset)) {
        std::cout << "\t" << name << " (current preset)" << std::endl;
    }
    else {
        std::cout << "\t" << name << std::endl;
    }
}
```

设置当前preset
```cpp
auto newPreset = "High Accuracy";
if(devicePresetList->hasPreset(newPreset)) {
    dev->loadPreset(newPreset);
    std::cout << "Current preset is changed to " << newPreset << std::endl;
}
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

// By getting the connection type of the device
auto connectType = devInfo->connectionType();
```

获取支持的传感器列表
```cpp
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
    case OB_SENSOR_IR_LEFT:
        std::cout << "\tIR Left sensor" << std::endl;
        break;
    case OB_SENSOR_IR_RIGHT:
        std::cout << "\tIR Right sensor" << std::endl;
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

功能描述：本示例主要演示了使用SDK获取深度数据并绘制显示、获取分辨率并选择进行设置、显示深度图像，并通过ESC_KEY键退出程序
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
	depthProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16, 30);
}catch(ob::Error &e){
	//没找到Y16格式后不匹配格式查找对应的Profile进行开流
	depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}
```

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用深度流
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```

选择序列ID
```cpp
    ob::SequenceIdFilter sequenceIdFilter;
    sequenceIdFilter.selectSequenceId(0);
```

启动在Config中配置的流，如果不传参数，将启动默认配置启动流
```cpp
pipe.start(config);
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

功能描述：本示例主要演示了使用SDK获取彩色数据并绘制显示、获取分辨率并选择进行设置、显示彩色图像，并通过ESC_KEY键退出程序
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Configure which streams to enable or disable for the Pipeline by creating a Config
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
try {
    // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    try {
        // Find the corresponding Profile according to the specified format, and choose the RGB888 format first
        colorProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_YUYV, 30);
    }
    catch(ob::Error &e) {
        // If the specified format is not found, select the first one (default stream profile)
        colorProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(colorProfile);
}
catch(ob::Error &e) {
    std::cerr << "Current device is not support color sensor!" << std::endl;
    exit(EXIT_FAILURE);
}
```
```
启动在Config中配置的流
```cpp
pipe.start(config);
```

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间
```cpp
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
每30帧打印一次metadata
```cpp
// print metadata every 30 frames
auto index = colorFrame->index();
if(index % 30 == 0) {
    std::cout << "*************************** Color Frame #" << index << " Metadata List ********************************" << std::endl;
    for(int metaDataType = 0; metaDataType < OB_FRAME_METADATA_TYPE_COUNT; metaDataType++) {
        // Check if it is supported metaDataType for current frame
        if(colorFrame->hasMetadata((OBFrameMetadataType)metaDataType)) {
            // Get the value of the metadata
            std::cout << metaDataTypes[metaDataType] << ": " << colorFrame->getMetadataValue((OBFrameMetadataType)metaDataType) << std::endl;
        }
        else {
            std::cout << metaDataTypes[metaDataType] << ": " << "unsupported" << std::endl;
        }
    }
    std::cout << "********************************************************************************" << std::endl << std::endl;
}
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
最终的彩色图显示如下<br />![image3](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage003.png)<br />程序正常退出之后资源将会自动释放

## d2c对齐示例-SyncAlignViewer

功能描述：本示例演示了对Sensor数据流控制对齐的操作，显示对齐后的图像，并通过ESC_KEY键退出程序
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
获取彩色相机和深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
```cpp
std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
try {
    // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
    auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    if(colorProfiles) {
        colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(colorProfile);
}
catch(...) {
    std::cerr << "Current device is not support color sensor!" << std::endl;
    exit(EXIT_FAILURE);
}

// Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
if(depthProfiles) {
    depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}
config->enableStream(depthProfile);
```
通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流和深度流
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(colorProfile);
config->enableStream(depthProfile);
```
控制流对齐，此处开启软件对齐
```cpp
// 配置对齐模式为软件D2C对齐
config->setAlignMode(ALIGN_D2C_SW_MODE);
```
启动在Config中配置的流，如果不传参数，将启动默认配置启动流
```cpp
pipe.start(config);
```
 停止Pipeline，将不再产生帧数据
```cpp
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

## 多路流同时开流示例-MultiStream
功能描述：本示例主要演示用device同时打开多个sensor流的操作，显示多路流图像，并通过ESC_KEY键退出程序
> 本示例基于C++ high Level API进行演示

首先需要创建一个pipeline
```cpp
ob::Pipeline pipe;
```

枚举设备，并进行流配置
```cpp
// enumerate and config all sensors
auto device     = pipe.getDevice();
auto sensorList = device->getSensorList();
for(int i = 0; i < sensorList->count(); i++) {
    auto sensorType = sensorList->type(i);
    if(sensorType == OB_SENSOR_GYRO || sensorType == OB_SENSOR_ACCEL) {
        continue;
    }
    auto profiles = pipe.getStreamProfileList(sensorType);
    auto profile  = profiles->getProfile(OB_PROFILE_DEFAULT);
    config->enableStream(profile);
}
```

通过配置开启pipeline
```cpp
// Start the pipeline with config
std::mutex                                        frameMutex;
std::map<OBFrameType, std::shared_ptr<ob::Frame>> frameMap;
pipe.start(config, [&](std::shared_ptr<ob::FrameSet> frameset) {
    auto count = frameset->frameCount();
    for(int i = 0; i < count; i++) {
        auto                         frame = frameset->getFrame(i);
        std::unique_lock<std::mutex> lk(frameMutex);
        frameMap[frame->type()] = frame;
    }
});
```

The IMU frame rate is much faster than the video, so it is advisable to use a separate pipeline to obtain IMU data.
```cpp
auto                                              dev         = pipe.getDevice();
auto                                              imuPipeline = std::make_shared<ob::Pipeline>(dev);
std::mutex                                        imuFrameMutex;
std::map<OBFrameType, std::shared_ptr<ob::Frame>> imuFrameMap;
try {
    auto                        accelProfiles = imuPipeline->getStreamProfileList(OB_SENSOR_ACCEL);
    auto                        gyroProfiles  = imuPipeline->getStreamProfileList(OB_SENSOR_GYRO);
    auto                        accelProfile  = accelProfiles->getProfile(OB_PROFILE_DEFAULT);
    auto                        gyroProfile   = gyroProfiles->getProfile(OB_PROFILE_DEFAULT);
    std::shared_ptr<ob::Config> imuConfig     = std::make_shared<ob::Config>();
    imuConfig->enableStream(accelProfile);
    imuConfig->enableStream(gyroProfile);
    imuPipeline->start(imuConfig, [&](std::shared_ptr<ob::FrameSet> frameset) {
        auto count = frameset->frameCount();
        for(int i = 0; i < count; i++) {
            auto                         frame = frameset->getFrame(i);
            std::unique_lock<std::mutex> lk(imuFrameMutex);
            imuFrameMap[frame->type()] = frame;
        }
    });
}
catch(...) {
    std::cout << "IMU sensor not found!" << std::endl;
    imuPipeline.reset();
}
```

关闭pipeline
```cpp
pipe.stop();
```

关闭IMU pipeline
```cpp
imuPipeline->stop();
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
void StartStream(std::vector<std::shared_ptr<ob::Pipeline>> pipes) {
    int i = 0;
    for(auto &&pipe: pipes) {
        std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
        // Get the depth camera configuration list
        auto                                    depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
        std::shared_ptr<ob::VideoStreamProfile> depthProfile     = nullptr;
        if(depthProfileList) {
            // Open the default profile of Depth Sensor, which can be configured through the configuration file
            depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfileList->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(depthProfile);

        // Get the color camera configuration list
        try {
            auto                                    colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
            std::shared_ptr<ob::VideoStreamProfile> colorProfile     = nullptr;
            if(colorProfileList) {
                // Open the default profile of Color Sensor, which can be configured through the configuration file
                colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfileList->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
            }
            config->enableStream(colorProfile);
        }
        catch(ob::Error &e) {
            std::cerr << "Current device is not support color sensor!" << std::endl;
        }

        // Start the pipeline and pass in the configuration
        pipe->start(config, [i](std::shared_ptr<ob::FrameSet> frameSet) {
            std::lock_guard<std::mutex> lock(frameMutex);
            if(frameSet->colorFrame()) {
                colorFrames[i] = frameSet->colorFrame();
            }
            if(frameSet->depthFrame()) {
                depthFrames[i] = frameSet->depthFrame();
            }
        });
        i++;
    }
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

## 热拔插示例-HotPlugin

功能描述：本示例主要演示设备拔插回调的设置，以及拔插之后处理获取到的流，并通过ESC_KEY键退出程序
> 本示例基于C++ High Level API进行演示

首先需要创建一个Context，用于获取设备信息列表和创建设备
```cpp
ob::Context ctx;
```
注册设备回调，分别在设备拔插的时候执行相关函数
```cpp
ctx.setDeviceChangedCallback( []( std::shared_ptr< ob::DeviceList > removedList, std::shared_ptr< ob::DeviceList > addedList ) {
        DeviceDisconnectCallback( removedList );
        DeviceConnectCallback( addedList );
    } );
```

开流
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


停流
```cpp
void stopStream(std::shared_ptr<PipelineHolder> holder) {
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
程序正常退出之后资源将会自动释放

## 点云示例-PointCloud

功能描述：连接设备开流 ,生成深度点云或RGBD点云并保存成ply格式文件，并通过ESC_KEY键退出程序
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
配置color流
```cpp
auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
if(colorProfiles) {
    auto profile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
    colorProfile = profile->as<ob::VideoStreamProfile>();
}
config->enableStream(colorProfile);
```

配置深度流
```cpp
std::shared_ptr<ob::StreamProfileList> depthProfileList;
OBAlignMode                            alignMode = ALIGN_DISABLE;
if(colorProfile) {
    // Try find supported depth to color align hardware mode profile
    depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
    if(depthProfileList->count() > 0) {
        alignMode = ALIGN_D2C_HW_MODE;
    }
    else {
        // Try find supported depth to color align software mode profile
        depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
        if(depthProfileList->count() > 0) {
            alignMode = ALIGN_D2C_SW_MODE;
        }
    }
}
else {
    depthProfileList = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
}

if(depthProfileList->count() > 0) {
    std::shared_ptr<ob::StreamProfile> depthProfile;
    try {
        // Select the profile with the same frame rate as color.
        if(colorProfile) {
            depthProfile = depthProfileList->getVideoStreamProfile(OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
        }
    }
    catch(...) {
        depthProfile = nullptr;
    }

    if(!depthProfile) {
        // If no matching profile is found, select the default profile.
        depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
    }
    config->enableStream(depthProfile);
}
```

开启D2C对齐, 生成RGBD点云时需要开启
```cpp
// 开启D2C对齐, 生成RGBD点云时需要开启
config->setAlignMode(ALIGN_D2C_HW_MODE);
```

启动Pipeline
```cpp
pipeline.start( config );
```

创建点云Filter对象，并且设置相机内参
```cpp
// 创建点云Filter对象（点云Filter创建时会在Pipeline内部获取设备参数, 所以尽量在Filter创建前配置好设备）
ob::PointCloudFilter pointCloud;

//获取相机内参传入点云Filter中
auto cameraParam = pipeline.getCameraParam();
pointCloud.setCameraParam(cameraParam);
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
```cpp
ob::Pipeline pipeline;
```
然后可以通过Pipeline来获取彩色流和深度流的所有配置, 包括流的分辨率 ,帧率 ,以及流的格式，配置所需要彩色和深度流
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
formatConverFilter.setFormatConvertType(FORMAT_RGB_TO_BGR);
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

功能描述：连接设备开流 , 录制当前视频流到文件，并通过ESC_KEY键退出程序
> 本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
ob::Pipeline pipe;
```
获取深度相机的流配置
```cpp
depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
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
```cpp
auto frameSet = pipe.waitForFrames(100);	//设置等待时间为100ms
```
停止Pipeline，将不再产生帧数据
```cpp
pipe.stop();
```
程序正常退出后会释放资源
## 回放示例-Playback

功能描述：连接设备开流 , 载入视频文件进行回放，并通过ESC_KEY键退出程序
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

## 深度精度示例-DepthUnitControl

功能描述：演示获取深度精度操作，显示深度图像，输出深度精度，并通过ESC_KEY键退出程序

>本示例基于C++ High Level API进行演示

创建Pipeline及流配置,并通过Pipeline获取设备
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Get the device inside the pipeline
auto device = pipe.getDevice();
```

检查获取深度精度的方式
```cpp
bool changeDepthUnitFlag = false;
// check the way to adjust depth unit
if(device->isPropertySupported(OB_PROP_DEPTH_PRECISION_LEVEL_INT, OB_PERMISSION_READ_WRITE)) {
    // change depth unit by adjust the depth precision level
    changeDepthUnitFlag = changeDepthUnit(device);
}
else if(device->isPropertySupported(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT, OB_PERMISSION_READ_WRITE)) {
    // for some models (Such as G300 series), user can adjust the depth unit by adjusting the depth unit flexible adjustment property
    changeDepthUnitFlag = changeDepthUnitFlexible(device);
}
else {
    std::cout << "Current device not support adjust depth unit, use the default depth unit 1mm" << std::endl;
    changeDepthUnitFlag = true;
}
```

计算深度值
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

释放资源，退出程序。
```cpp
    pipe.stop();
```

## HDR示例-HdrMerge

支持设备： G3系列相机，例如：Gemini G335

功能描述：演示使用HDR操作，显示HDR处理后的图像，并通过ESC_KEY键退出程序

>本示例基于C++ High Level API进行演示

创建Pipeline,进行流配置
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Configure which streams to enable or disable for the Pipeline by creating a Config
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
try {
    // Get all stream profiles of the ir camera, including stream resolution, frame rate, and frame format
    auto irProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);
    if(irProfiles) {
        irProfile = std::const_pointer_cast<ob::StreamProfile>(irProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(irProfile);
}
catch(...) {
    std::cerr << "Current device is not support ir sensor!" << std::endl;
    exit(EXIT_FAILURE);
}

// Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
if(depthProfiles) {
    depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}
config->enableStream(depthProfile);
```

打开HDR处理
```cpp
// Create HdrMerage post processor
ob::HdrMerge hdrMerge;

// open hdr merage
if(pipe.getDevice()->isPropertySupported(OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE)) {
    // Get depth exposure value range,the exposure_1 and exposure_2 in OBHdrConfig can be adjusted.
    OBIntPropertyRange depthExpRange = pipe.getDevice()->getIntPropertyRange(OB_PROP_DEPTH_EXPOSURE_INT);
    // Get depth gain value range,,the gain_1 and gain_1 in OBHdrConfig can be adjusted.
    OBIntPropertyRange depthGainRange = pipe.getDevice()->getIntPropertyRange(OB_PROP_DEPTH_GAIN_INT);

    OBHdrConfig obHdrConfig;
    uint32_t    dataSize = sizeof(OBHdrConfig);
    pipe.getDevice()->getStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, &dataSize);
    // open hdr
    obHdrConfig.enable = true;

    pipe.getDevice()->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
}
```

开启pipeline
```cpp
pipe.start(config);
```

获取HDR处理后的图像
```cpp
auto leftIRFrame = frameSet->getFrame(OB_FRAME_IR_LEFT);
if(leftIRFrame) {
    framesForRender.push_back(leftIRFrame);
}

auto depthFrame = frameSet->depthFrame();
if(depthFrame != nullptr) {
    auto newFrame = hdrMerge.process(frameSet);
    auto newFrameSet = newFrame->as<ob::FrameSet>();
    if(newFrameSet) {
        depthFrame = newFrameSet->depthFrame();
        if(depthFrame) {
            framesForRender.push_back(depthFrame);
        }
    }
}
```

关闭pipeline
```cpp
pipe.stop();
```

关闭Hdr处理
```cpp
if(pipe.getDevice()->isPropertySupported(OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE)) {
    OBHdrConfig obHdrConfig;
    uint32_t    dataSize = sizeof(OBHdrConfig);
    pipe.getDevice()->getStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, &dataSize);
    obHdrConfig.enable = false;
    pipe.getDevice()->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
}
```

## 深度工作模式示例-DepthWorkMode

支持的设备： G2系列，G3系列相机，Astra2系列相机

功能描述：演示转换深度工作模式操作。

>本示例基于C++ High Level API进行演示

创建Pipeline, 获取设备
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Get the device inside the pipeline
auto device = pipe.getDevice();
```

检查是否支持工作模式转换
```cpp
// Check whether the camera depth working mode is supported, currently (December 5, 2022) only the Gemini2 binocular camera supports the depth working mode
if(!device->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE)) {
    pressKeyExit("Current device not support depth work mode!");
    return -1;
}
```

获取深度工作模式列表
```cpp
// Query the current camera depth mode
auto curDepthMode = device->getCurrentDepthWorkMode();
// Get the list of camera depth modes
auto depthModeList = device->getDepthWorkModeList();
std::cout << "depthModeList size: " << depthModeList->count() << std::endl;
for(uint32_t i = 0; i < depthModeList->count(); i++) {
    std::cout << "depthModeList[" << i << "]: " << (*depthModeList)[i];
    if(strcmp(curDepthMode.name, (*depthModeList)[i].name) == 0) {
        std::cout << "  (Current WorkMode)";
    }

    std::cout << std::endl;
}
```

设置深度工作模式
```cpp
// Let the user choose a mode, then switch
if(depthModeList->count() > 0) {
    uint32_t index = 0;
    std::cout << "Please input the index from above depthModeList, newIndex = ";
    std::cin >> index;
    if(index >= 0 && index < depthModeList->count()) {  // legitimacy check
        device->switchDepthWorkMode((*depthModeList)[index].name);

        // Check whether the mode changes after the display is switched
        curDepthMode = device->getCurrentDepthWorkMode();
        if(strcmp(curDepthMode.name, (*depthModeList)[index].name) == 0) {
            std::cout << "Switch depth work mode success! currentDepthMode: " << curDepthMode << std::endl;
        }
        else {
            std::cout << "Switch depth work mode failed!" << std::endl;
        }
    }
    else {
        std::cout << "switchDepthMode faild. invalid index: " << index << std::endl;
    }
}
```

## 双红外示例-DoubleInfraredViewer

支持设备：OpenNI协议相机例如Dabia系列，Astra/Astra2系列，Femto系列相机

功能描述：演示获取双红外相机图像，显示左右红外图像，并通过ESC_KEY键退出程序

>本示例基于C++ High Level API进行演示

创建Pipeline, 获取设备
```cpp
    ob::Pipeline pipe;
```

配置左IR
```cpp
// Get the ir_left camera configuration list
auto irLeftProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);

if(irLeftProfiles == nullptr) {
    std::cerr
        << "The obtained IR(Left) resolution list is NULL. For monocular structured light devices, try opening the IR data stream using the IR example. "
        << std::endl;
    return 0;
}

// Open the default profile of IR_LEFT Sensor, which can be configured through the configuration file
try {
    auto irLeftProfile = irLeftProfiles->getProfile(OB_PROFILE_DEFAULT);
    config->enableStream(irLeftProfile->as<ob::VideoStreamProfile>());
}
catch(...) {
    std::cout << "IR(Left) stream not found!" << std::endl;
}
```

配置右IR
```cpp
// Get the ir_right camera configuration list
auto irRightProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_RIGHT);

// Open the default profile of IR_RIGHT Sensor, which can be configured through the configuration file
try {
    auto irRightProfile = irRightProfiles->getProfile(OB_PROFILE_DEFAULT);
    config->enableStream(irRightProfile->as<ob::VideoStreamProfile>());
}
catch(...) {
    std::cout << "IR(Right) stream not found!" << std::endl;
}
```

开启pipeline
```cpp
pipe.start(config);
```

获取帧数据
```cpp
auto leftFrame  = frameSet->getFrame(OB_FRAME_IR_LEFT);
auto rightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
```

停止pipeline
```cpp
pipe.stop();
```

## 红外示例-InfraredViewer
功能描述：演示获取红外相机图像，显示红外图像，并通过ESC_KEY键退出程序

>本示例基于C++ High Level API进行演示

创建Pipeline, 配置流信息
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Get all stream profiles of the infrared camera, including stream resolution, frame rate, and frame format
// Please adjust the sensor according to the actual product, some device types only have OB_SENSOR_IR_LEFT and OB_SENSOR_IR_RIGHT.
auto profiles = pipe.getStreamProfileList(OB_SENSOR_IR);

if(profiles == nullptr) {
    std::cerr
        << "The obtained IR resolution list is NULL. For binocular structured light devices, try using the doubleIr example to turn on the ir data stream. "
        << std::endl;
    return 0;
}

std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
try {
    // Find the corresponding profile according to the specified format, first look for the y16 format
    irProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y8, 30);
}
catch(ob::Error &e) {
    // If the specified format is not found, search for the default profile to open the stream
    irProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}

// By creating config to configure which streams to enable or disable for the pipeline, here the infrared stream will be enabled
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(irProfile);
```

开启pipeline
```cpp
pipe.start(config);
```

获取帧数据
```cpp
auto frameSet = pipe.waitForFrames(100);
```

停止pipeline
```cpp
pipe.stop();
```

## imu读取示例-ImuReader
功能描述：演示获取imu数据

>本示例基于C++ low Level API进行演示，输出imu数据，并通过ESC_KEY键退出程序

获取设备
```cpp
// Create a Context.
ob::Context ctx;

// Query the list of connected devices
auto devList = ctx.queryDeviceList();

if(devList->deviceCount() == 0) {
    std::cerr << "Device not found!" << std::endl;
    return -1;
}

// Create a device, 0 represents the index of the first device
auto dev = devList->getDevice(0);
```

获取陀螺仪数据
```cpp
// Get Gyroscope Sensor
gyroSensor = dev->getSensorList()->getSensor(OB_SENSOR_GYRO);
if(gyroSensor) {
    // Get configuration list
    auto profiles = gyroSensor->getStreamProfileList();
    // Select the first profile to open stream
    auto profile = profiles->getProfile(OB_PROFILE_DEFAULT);
    gyroSensor->start(profile, [](std::shared_ptr<ob::Frame> frame) {
        std::unique_lock<std::mutex> lk(printerMutex);
        auto                         timeStamp = frame->timeStamp();
        auto                         index     = frame->index();
        auto                         gyroFrame = frame->as<ob::GyroFrame>();
        if(gyroFrame != nullptr && (index % 50) == 2) {  //( timeStamp % 500 ) < 2: Reduce printing frequency
            auto value = gyroFrame->value();
            std::cout << "Gyro Frame: \n\r{\n\r"
                        << "  tsp = " << timeStamp << "\n\r"
                        << "  temperature = " << gyroFrame->temperature() << "\n\r"
                        << "  gyro.x = " << value.x << " rad/s"
                        << "\n\r"
                        << "  gyro.y = " << value.y << " rad/s"
                        << "\n\r"
                        << "  gyro.z = " << value.z << " rad/s"
                        << "\n\r"
                        << "}\n\r" << std::endl;
        }
    });
}
```

获取加速度计
```cpp
accelSensor = dev->getSensorList()->getSensor(OB_SENSOR_ACCEL);
if(accelSensor) {
    // Get configuration list
    auto profiles = accelSensor->getStreamProfileList();
    // Select the first profile to open stream
    auto profile = profiles->getProfile(OB_PROFILE_DEFAULT);
    accelSensor->start(profile, [](std::shared_ptr<ob::Frame> frame) {
        std::unique_lock<std::mutex> lk(printerMutex);
        auto                         timeStamp  = frame->timeStamp();
        auto                         index      = frame->index();
        auto                         accelFrame = frame->as<ob::AccelFrame>();
        if(accelFrame != nullptr && (index % 50) == 0) {
            auto value = accelFrame->value();
            std::cout << "Accel Frame: \n\r{\n\r"
                        << "  tsp = " << timeStamp << "\n\r"
                        << "  temperature = " << accelFrame->temperature() << "\n\r"
                        << "  accel.x = " << value.x << " m/s^2"
                        << "\n\r"
                        << "  accel.y = " << value.y << " m/s^2"
                        << "\n\r"
                        << "  accel.z = " << value.z << " m/s^2"
                        << "\n\r"
                        << "}\n\r" << std::endl;
        }
    });
}
```

```cpp
// turn off the flow
if(gyroSensor) {
    gyroSensor->stop();
}
if(accelSensor) {
    accelSensor->stop();
}
```

## 多设备同步示例-MultiDeviceSync
功能描述：演示多设备同步操作。

>本示例基于C++ low Level API进行演示

配置程序模式
```cpp
std::cout << "Please select options: " << std::endl;
std::cout << " 0 --> config devices" << std::endl;
std::cout << " 1 --> start stream" << std::endl;
std::cout << "input: ";
int index = -1;
std::cin >> index;
```

配置多设备同步
```cpp
exitValue = configMultiDeviceSync();
```

区分主从设备
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

开启从设备
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

开启主设备
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

设置多设备同步间隔时间
```cpp
// Start the multi-device time synchronization function
context.enableDeviceClockSync(3600000);  // update and sync every hour
```

进行多设备测试
```cpp
testMultiDeviceSync();
```

关闭数据流
```cpp
// close data stream
for(auto itr = pipelineHolderList.begin(); itr != pipelineHolderList.end(); itr++) {
    stopStream(*itr);
}
```

## 后处理示例-PostProcessing

支持设备： G3系列相机，例如：Gemini G335

功能描述：演示进行后处理操作，显示经过后处理的图像，并通过ESC_KEY键退出程序

>本示例基于C++ high Level API进行演示

获取pipeline，进行流配置
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try {
    // Find the corresponding profile according to the specified format, first look for the y16 format
    depthProfile = profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
}
catch(ob::Error &e) {
    // If the specified format is not found, search for the default profile to open the stream
    depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}

// By creating config to configure which streams to enable or disable for the pipeline, here the depth stream will be enabled
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```

获取深度后处理过滤器列表
```cpp
auto obFilterList = pipe.getDevice()->getSensor(OB_SENSOR_DEPTH)->getRecommendedFilters();

std::shared_ptr<ob::DecimationFilter> decFilter;
for(int i = 0; i < obFilterList->count(); i++) {
    auto postProcessorfilter =obFilterList->getFilter(i);
    std::cout << "Depth recommended post processor filter type: " << postProcessorfilter->type() << std::endl;
    if(postProcessorfilter->is<ob::DecimationFilter>()) {
        decFilter = postProcessorfilter->as<ob::DecimationFilter>();
    }
}
```

开启pipeline
```cpp
pipe.start(config);
```


## 传感器控制示例-SensorControl

功能描述：本示例演示了对device控制命令的操作、对Sensor控制命令的操作、对Sensor进行流操作。
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
```cpp
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


## 流对齐处理单元-AlignFilterViewer

支持设备： G3系列相机，例如：Gemini G335

功能描述：演示传感器数据流同步对齐的操作，显示对齐后的图像，并通过ESC_KEY键退出程序

>本示例基于C++ hign Level API进行演示

创建pipeline
```cpp
ob::Pipeline pipe;
```

开启color流
```cpp
auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
if(colorProfiles) {
    colorProfile = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30);
}
config->enableStream(colorProfile);
```

开启深度流
```cpp
auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
if(depthProfiles) {
depthProfile = depthProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
//depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}
config->enableStream(depthProfile);
```

设置对齐模式
```cpp
/* Config depth align to color or color align to depth.
OBStreamType align_to_stream = OB_STREAM_DEPTH; */
OBStreamType align_to_stream = OB_STREAM_COLOR;
ob::Align align(align_to_stream);
```

开启pipeline
```cpp
pipe.start(config);
```

获取帧数据
```cpp
auto colorFrame = frameSet->colorFrame();
auto depthFrame = frameSet->depthFrame();
```

进行对齐处理
```cpp
aif(align_to_stream == OB_STREAM_COLOR) {
    app.resize(colorFrame->width(), colorFrame->height());
}
else {
    app.resize(depthFrame->width(), depthFrame->height());
}
```

关闭pipeline
```cpp
pipe.stop();
```