# C Sample Post Processing

Supported devices: Gemini 330 series cameras, such as Gemini G335

Function description: Demonstrate the use of post-processing operations, display post-processing images, and exit the program using the ESC_KEY key

This example is based on the C High Level API for demonstration

## 1. Create a pipeline for stream configuration
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

## 2. Get sensors
```c
    // Get device
    ob_device *device = ob_pipeline_get_device(pipeline, &error);
    check_error(error);
    
    // Get depth sensor
    ob_sensor *depthSensor = ob_device_get_sensor(device, OB_SENSOR_DEPTH, &error);
    check_error(error);
```

## 3. Get post-processing list
```c
    ob_filter_list *filterList = ob_sensor_get_recommended_filter_list(depthSensor, &error);
    check_error(error);
```

## 4. Post processing
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

## 5. Release relevant resources
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

## 6. expected Output 

![image](Image/post_processing.png)