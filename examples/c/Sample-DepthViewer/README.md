# C Sample Depth Stream Viewer

# Depth Stream Viewer

Function description: Demonstrate using SDK to get depth data and draw display, get resolution and set, display depth images, and exit the program through ESC_KEY key

This example is based on the C High Level API for demonstration

## 1. create a Pipeline to connect the device and open depth streams
```c
    pipe = ob_create_pipeline( &error );
```

## 2. Create a Configuration to configure the resolution, frame rate, and format of color and depth streams

```c
    ob_config* config = ob_create_config( &error );

    //Configure Depth stream
    ob_stream_profile *     depth_profile = NULL;
    ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_DEPTH, &error);
    //ind the corresponding Profile according to the specified format, with priority given to Y16 format
    depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30, &error);
    //If the Y16 format is not found and does not match the format, search for the corresponding profile to open the stream
    if(error){
        depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
        error = nullptr;
    }
    ob_config_enable_stream(config, depth_profile, &error);  // Enable configuration
```

## 3. Start Pipeline through Configuration
```c
    ob_pipeline_start_with_config(pipe, config, &error);
```


## 4. Release resources and exit the program.
```c
    //Release frameSet and reclaim memory
    ob_delete_frame(frameset, &error);
    
    //Release depth profile
    ob_delete_stream_profile(depth_profile, &error);
    
    //Release color list
    ob_delete_stream_profile_list(profiles, &error);
    
    //Release device
    ob_delete_device(device, &error);
    
    //Release pipeline
    ob_delete_pipeline(pipe, &error);
```

## 5.Expected Output 

![image](Image/depth_viewer.png)