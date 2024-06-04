# 双红外示例

支持设备：仅支持Gemini 330系列，Gemini 2 XL，Gemini 2 VL

功能描述：演示获取双红外相机图像，显示左右红外图像，并通过ESC_KEY键退出程序

本示例基于C High Level API进行演示

## 1. 创建pipeline和config
```c
    // Create a pipeline to open the Infrared stream after connecting the device
    pipeline = ob_create_pipeline(&error);
    check_error(error);
    
    // Create config to configure the resolution, frame rate, and format of the Infrared stream
    ob_config *config = ob_create_config(&error);
    check_error(error);
```

## 2. 获取左IR流配置
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

## 3. 获取右IR流配置
```
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

## 4. 通过config开启pipeline
```c
    ob_pipeline_start_with_config(pipeline, config, &error);
    check_error(error)
```

## 5. 获取左右IR帧数据
```c
    ob_frame *ir_left_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_LEFT, &error);
    check_error(error);
    
    ob_frame *ir_right_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_RIGHT, &error);
    check_error(error);
```

## 6. 停止 pipeline
```c
    ob_pipeline_stop(pipeline, &error);
    check_error(error);
```

## 7. 释放相关资源
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

## 预期输出

![image](Image/double_infrared_viewer.png)