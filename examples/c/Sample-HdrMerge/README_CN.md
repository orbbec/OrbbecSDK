# HDR示例

支持设备： Gemini 330系列相机，例如：Gemini G335

功能描述：演示使用HDR操作，显示HDR处理后的图像，并通过ESC_KEY键退出程序

本示例基于C High Level API进行演示

## 1. 配置 HDR
```c
    // configure Hdr
    ob_hdr_config hdr_config;
    hdr_config.enable     = true;  // enable Hdr
    hdr_config.exposure_1 = 7500;
    hdr_config.gain_1     = 16;
    hdr_config.exposure_2 = 100;
    hdr_config.gain_2     = 1;
    ob_device_set_structured_data(dev, OB_STRUCT_DEPTH_HDR_CONFIG, &hdr_config, sizeof(ob_hdr_config), &error);
    check_error(error);
```
## 2. 创建pipeline，配置depth和ir流
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

## 3. 获取device
```c
    // Get the device through the pipeline
    ob_device *dev = ob_pipeline_get_device(pipeline, &error);
    check_error(error);
```

## 4. 开启pipeline
```c
    ob_pipeline_start_with_config(pipeline, config, &error);
```

## 5. 获取HDR处理后的图像
```c
    auto new_frame_set = ob_filter_process(hdr_merge_filter, frameset, &error);
    check_error(error);
    
    auto new_depth_frame = ob_frameset_depth_frame(new_frame_set, &error);
    check_error(error);
    if(new_depth_frame != nullptr) {
        frames.push_back(new_depth_frame);
    }
```

## 6. 停止pipeline
```c
    ob_pipeline_stop(pipeline, &error);
```

## 7.预期输出

![image](Image/hdr_merge_viewer.png)