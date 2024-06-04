# C Sample Hot Plugin

Function description: Device hot plug monitoring, automatically connects the device when it is online and opens depth stream, and automatically disconnects the device when it is disconnected., And exit the program through the ESC_KEY key

This example is based on the C Low Level API for device registration and offline callback, and the High Level API for open stream frame retrieval demonstration

## 1. Create a device connection processing function, create a pipeline within the function, and call create_and_start_with_config configuration to enable color and depth streams.
```c
    //Device connection processing
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

## 2. Create a device disconnection handling function that stops the pipeline and sets the exit pipeline flag isExit to true
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

## 3. The device connection and disconnection handling functions will be called within the device's online and offline callback functions
```c
    //Device status change callback
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

## 4. Within the main function, create a Context and register device online and offline callbacks
```c
     //Create Context
    ob_context* ctx = ob_create_context( &error );
    
    //Register device callback
    ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );
```

## 5. After exiting the main loop, relevant resources need to be released
```c
    if ( pipeline ) {
        // Release pipeline
        ob_pipeline_stop( pipeline, &error );
        // Release pipeline
        ob_delete_pipeline( pipeline, &error );
    }
    // Release dev_list
    if ( dev_list ) {
        ob_delete_device_list( dev_list, &error );
    }
    // Release ctx
    if ( ctx ) {
        ob_delete_context( ctx, &error );
    }
```