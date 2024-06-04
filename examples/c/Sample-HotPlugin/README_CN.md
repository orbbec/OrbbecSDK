# 热拔插示例

功能描述： 设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接。，并通过ESC_KEY键退出程序

本示例基于C Low Level API进行注册设备上下线回调，基于High Level API进行开流取帧演示


## 1. 创建设备连接处理函数，函数内创建pipeline并调用create_and_start_with_config开启彩色流和深度流。
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

## 2. 创建设备断开处理函数，函数内将pipeline停止，并将退出pipeline标记isExit设为true
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
## 3. 设备连接、断开处理函数将会在设备上下线回调函数内被调用
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
## 4. 主函数main内，创建一个Context并注册设备上下线回调
```c
     //创建上下文
    ob_context* ctx = ob_create_context( &error );
    
    //注册设备回调
    ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );
```

## 5. 退出主循环后，需要将相关资源释放
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