# 彩色示例

功能描述：演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像，并通过ESC_KEY键退出程序

本示例基于C High Level API进行演示

## 1. 首先创建Pipeline，用于连接设备后打开彩色流
```c
    pipe = ob_create_pipeline( &error );
```

## 2. 创建Config，用于配置彩色流的分辨率、帧率、格式
```c
    ob_config* config = ob_create_config( &error );


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

## 3. 启动Pipeline
```c
    ob_pipeline_start_with_config(pipe, config, &error);
```

## 4. 释放资源，退出程序
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
    ob_delete_pipeline(pipe, &error)
```

## 5. 预期输出

![image](Image/color_viewer.png)