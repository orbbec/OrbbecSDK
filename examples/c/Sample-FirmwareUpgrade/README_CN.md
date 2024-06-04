# 固件升级示例

功能描述：本示例演示如何用固件文件给设备升级。

本示例基于C Low Level API进行演示，示例编译语言为C++，Orbbec SDK使用C语言API

## 1. 在main函数接口通过命令参数获取固件文件
```c
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

## 2. 创建ob_context并通过ob_context获取设备
本示例假设运行firmware_upgrade[.exe]之前，上位机（Windows、Ubuntu、Arm平台）已经插入设备。device_changed_callback用于固件升级后监听device重启后获取被升级设备的业务处理
```c
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

## 3. 获取设备当前的固件版本信息
```c
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

## 4. 给目标设备升级固件

a. 实现C API的固件升级回调接口；

b. 调用固件升级接口进行升级；
```c
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

固件升级成功后，需要重启设备。重启设备有两种方式，一种是拔插设备（或者操作系统重启），另一种是调用OrbbecSDK的reboot接口。等设备上线后，通过本示例的dump_device_info()函数可以查询当前设备固件版本信息.

以下示例演示通过固件reboot接口重启设备
```c
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
```c
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
## 5. 预期输出：

![image](Image/firmware_upgrade.png)