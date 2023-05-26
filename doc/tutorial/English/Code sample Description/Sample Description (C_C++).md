All examples can be found in the project's Examples directory

| Name | Language | Description |
| --- | --- | --- |
| HelloOrbbec | C | Demonstrate connect to device to get SDK version and device information |
| SensorControl | C | Demonstrate the operation of device, sensor control commands |
| DepthWorkMode | C |Demonstrate get current depth work mode, obtain supported depth work mode list, switch depth work mode. |
| Hotplugin | C | Demonstrate device hot-plug monitoring, automatically connect the device to open depth streaming when the device is online, and automatically disconnect the device when it detects that the device is offline |
| PointCloud | C | Demonstrate the generation of depth point cloud or RGBD point cloud and save it as ply format file |
| HelloOrbbec | C++ | Demonstrate connect to device to get SDK version and device info |
| DepthViewer | C++ | Demonstrate using SDK to get depth data and draw display, get resolution and set, display depth image |
| ColorViewer | C++ | Demonstrate using SDK to get color data and draw display, get resolution and set, display color image |
| InfraredViewer | C++ | Demonstrate using SDK to obtain infrared data and draw display, obtain resolution and set, display infrared image |
| SyncAlignViewer | C++ | Demonstrate operations on sensor data stream alignment |
| SensorControl | C++ | Demonstrate manipulation of device and sensor control commands |
| MultiDevice | C++ | Demonstrate operation on multiple devices |
| DepthWorkMode | C++ |Demonstrate get current depth work mode, obtain supported depth work mode list, switch depth work mode. |
| HotPlugin | C++ | Demonstrate the settings of the device plug and unplug callback, and get the stream processed after plugging and unplugging |
| IMUReader | C++ | Get IMU data and output display |
| MultiDeviceSync | C++ | Demostrate how to config multiple devices synchronize config and how to start stream with this config |
| PointCloud | C++ | Demonstrate the generation of depth point cloud or RGBD point cloud and save it as ply format file |
| SaveToDisk | C++ | Get color and depth maps and save as png format |
| Recorder | C++ | Record current video stream to file |
| Playback | C++ | Load video files for playback |

# C
## HelloOrbbec
Function description: Demonstrate SDK initialization, get SDK version, get device model, get device serial number, get firmware version, and SDK release resources.
> This sample is based on C Low Level API for demonstration.

First get and print the current SDK version
```c
printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());
```
Create a Context to obtain a list of device information and create a device
```
ob_error*   error = NULL;
ob_context* ctx   = ob_create_context( &error );
```
Query the list of connected devices to obtain the number of devices in the list, and create the first device in the list through index number 0.
```
ob_device_list* dev_list = ob_query_device_list( ctx, &error );
int count = ob_device_list_device_count(dev_list, &error);
ob_device* dev = ob_device_list_get_device(dev_list, 0, &error);
```
Next you can get information from this device.
```
//Get device information
ob_device_info* dev_info = ob_device_get_device_info(dev, &error);

//Get the device name
const char* name = ob_device_info_name(dev_info, &error);
    
//Get the pid, vid, uid of the device
int pid = ob_device_info_pid(dev_info, &error);
int vid = ob_device_info_vid(dev_info, &error);
int uid = ob_device_info_uid(dev_info, &error);
    
//Get the firmware version number of the device
const char* fw_ver = ob_device_info_firmware_version(dev_info, &error);
      
//Get the serial number of the device
const char* sn = ob_device_info_serial_number(dev_info, &error);
    
//Get the list of supported sensors
ob_sensor_list* sensor_list = ob_device_get_sensor_list(dev, &error);

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
Release resources and exit the program.
```c
ob_delete_sensor_list(sensor_list, &error); //Destroy sensor list
ob_delete_device_info(dev_info, &error); //Destroy device info
ob_delete_device(dev, &error); //Destroy device
ob_delete_device_list(dev_list, &error); //Destroy device list
ob_delete_context(ctx, &error); //Destroy context
```
## SensorControl
Function description: This example mainly demonstrates the operation of device control commands, the operation of sensor control commands, and stream from sensor.
> This example is based on C Low Level API for demonstration.

Create a Context and query the list of connected devices
```c
ob_error*   error = NULL;
ob_context* ctx   = ob_create_context( &error );
ob_device_list* dev_list = ob_query_device_list( ctx, &error );
```
Print device information in the list by index number
```c
const char *name = ob_device_list_get_device_name(device_list, i, &g_error);
int pid = ob_device_list_get_device_pid(device_list, i, &g_error);
int vid = ob_device_list_get_device_vid(device_list, i, &g_error);
const char *uid = ob_device_list_get_device_uid(device_list, i, &g_error);
const char *sn = ob_device_list_get_device_serial_number(device_list, i, &g_error);
printf("%d. name: %s, pid: %d, vid: %d, uid: %s, sn: %s\n", i, name, pid, vid, uid, sn);
```
Create a device
```c
if(devCount <= 1) {
    // If a single device is inserted, the first one is selected by default.
     device = ob_device_list_get_device(dev_list, 0, &g_error);
}
else {
    // If there are multiple devices, the user inputs the selection.
    device = select_device(dev_list);  // select_device 
}
```
Get and print the currently created device information
```c
 ob_device_info *deviceInfo = ob_device_get_device_info(device, &g_error);
 const char *name = ob_device_info_name(deviceInfo, &g_error);
 int pid = ob_device_info_pid(deviceInfo, &g_error);
 int vid = ob_device_info_vid(deviceInfo, &g_error);
 const char *uid = ob_device_info_uid(deviceInfo, &g_error);
 printf("Current Device: name: %s, pid: %d, vid: %d, uid: %s\n", name, pid, vid, uid);
```
Get supported control command properties
```c
// Get the number of attributes supporting control commands
uint32_t propertySize = ob_device_get_supported_property_count(device, &g_error);
// Get the control command attribute item through index number I
ob_property_item property_item = ob_device_get_supported_property(device, i, &g_error);
```
read/write control commands
```c
// Read
bool_ret = ob_device_get_bool_property(device, property_item.id, &g_error);// bool property 
int_ret = ob_device_get_int_property(device, property_item.id, &g_error);/ int property
float_ret = ob_device_get_float_property(device, property_item.id, &g_error);// float property

// Read the value range, default value, step value and other information of the command.
ob_int_property_range   int_range;
ob_float_property_range float_range;
ob_bool_property_range  bool_range;
sprintf(str, "Bool value(min:0, max:1, step:1)"); 
int_range = ob_device_get_int_property_range(device, property_item.id, &g_error); 
float_range = ob_device_get_float_property_range(device, property_item.id, &g_error); 

// Write
ob_device_set_bool_property(device, property_item.id, bool_value, &g_error); // bool property
ob_device_set_int_property(device, property_item.id, int_value, &g_error); // int property
ob_device_set_float_property(device, property_item.id, float_value, &g_error); // float property
```
Finally, when the program exits, the corresponding created resources need to be released.
```c
//Delete context
ob_delete_context(ctx, &g_error);
//Delete device list
ob_delete_device_list(dev_list, &g_error);        
//Delete device
ob_delete_device(device, &g_error);
```
## ColorDepthViewer
Function description: This example mainly demonstrates the use of the pipeline advanced interface to operate the device, obtain the resolution list and set the resolution.
> This example is based on C High Level API for demonstration.

First, we need to create a PipeLine to open the Color and Depth streams after connecting the device.
```
pipe = ob_create_pipeline( &error );
```
Create a Config to configure the resolution, frame rate and format of Color or Depth streams.
```
ob_config* config = ob_create_config( &error );
```
Configure stream
```c
// 2.1 Configure depth stream
ob_stream_profile *     depth_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_DEPTH, &error);
//Find the corresponding Profile according to the specified format.
depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_Y16, 30, &error);
if(!depth_profile) {  // If the specified format profile is not found, the first profile in the profile list is used.
     depth_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
}
ob_config_enable_stream(config, depth_profile, &error);  // Enable stream

// 2.2 Configure color stream
ob_stream_profile *color_profile = NULL;
profiles                         = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_COLOR, &error);
//Find the corresponding Profile according to the specified format.
color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_MJPG, 30, &error);
if(!color_profile) {  // If the specified format profile is not found, the first profile in the profile list is used.
    color_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
}
ob_config_enable_stream(config, color_profile, &error);  // Enable stream
```
Turn off sync
```
ob_pipeline_disable_frame_sync(pipe, &error);  // Turn off frame synchronization
```
Start the pipeline through config
```
ob_pipeline_start_with_config(pipe, config, &error);
```
Stop and destroy
```
// Stop pipeline
ob_pipeline_stop(pipe, &error); 

// Destroy pipeline  
ob_delete_pipeline(pipe, &error);
```
## DepthWorkMode
Function Description: This example mainly demonstrates switching camera depth mode. First, the depth mode list is queried, and then the corresponding camera depth mode is selected to call interface switching

First get device
```C
// Create Context. Unlike Pipeline, Context is the entry point to the low-level API, which is slightly more complicated to use for common operations such as switching streams,
// but the low-level API can provide more flexible operations such as getting multiple devices, reading and writing device and camera properties, etc
ob_error *  error = NULL;
ob_context *ctx   = ob_create_context(&error);
check_error(error);

//Get the list of connected devices
ob_device_list *dev_list = ob_query_device_list(ctx, &error);
check_error(error);

//Obtain the number of connected devices
int dev_count = ob_device_list_device_count(dev_list, &error);
check_error(error);
if(dev_count == 0) {
    printf("Device not found!\n");
    return -1;
}

//Create a device. 0 indicates the index of the first device
ob_device *dev = ob_device_list_get_device(dev_list, 0, &error);
check_error(error);
```

Check whether the device supports camera depth mode. Currently, only Gemini2 supports camera depth mode
```C
// Check to see if depth mode is supported. Currently (5 December 2022), only Gemini2 binocular cameras support depth mode
if(!ob_device_is_property_supported(dev, OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE, &error)) {
    printf("FAILED!!, Device not support depth work mode");
    check_error(error);
    return -1;
}
check_error(error);
```

Get the current depth work mode of the device
```C
// Get the current depth work mode of the device
ob_depth_work_mode cur_work_mode = ob_device_get_current_depth_work_mode(dev, &error);
check_error(error);
printf("Current depth work mode: %s\n", cur_work_mode.name);
```

the list of camera depth work modes supported by the device
```C
// Get number contain in mode_list
uint32_t mode_count = ob_depth_work_mode_list_count(mode_list, &error);
printf("Support depth work mode list count: %u\n", mode_count);

int cur_mode_index = -1;
for(uint32_t i = 0; i < mode_count; i++) {
    ob_depth_work_mode mode = ob_depth_work_mode_list_get_item(mode_list, i, &error);
    check_error(error);
    printf("depth work mode[%u], name: %s", i, mode.name);
}
```

Switch depth work mode
```C
// Switch device to a new depth work mode
ob_device_switch_depth_work_mode_by_name(dev, mode.name, &error);
check_error(error);
```

At the end of switching camera depth mode, you can open the camera to stream using pipeline
Cautions： 
1. If you want to switch camera depth mode, you must open the data stream after switching depth mode. Each camera depth mode supports different effective resolutions
2. If the data stream has been opened with a pipeline, the original applied pipeline must be released before the switch depth work mode 
   Re-create pipeline after switching camera depth mode; otherwise, wild pointer or memory leak will be caused;


Final release of resources
```C
// Release mode_list
ob_delete_depth_work_mode_list(mode_list, &error);
check_error(error);

//Release device
ob_delete_device(dev, &error);
check_error(error);

//Release device list
ob_delete_device_list(dev_list, &error);
check_error(error);

//Release context
ob_delete_context(ctx, &error);
check_error(error);
```

## HotPlugin
Function description: Monitor device hot-plug, if the device is online, then automatically connects to the device to open the depth stream, or if the device is offline and then automatically disconnects the device.
> This sample is based on C Low Level API for registering device up/down callback, and based on High Level API for open stream fetching demo.

Create a device connection processing function, create a pipeline in the function and call create_and_start_with_config to start RGB and Depth streams.
```
//Device connection processing
void device_connect_callback( ob_device_list* connectList ) {
    uint32_t count = ob_device_list_device_count( connectList, &error );
    check_error( error );
    printf( "Device connect: %d\n", count );
    if ( count > 0 ) {
        if ( pipeline == NULL ) {
            pipeline = ob_create_pipeline( &error );
            check_error( error );
            create_and_start_with_config();
        }
    }
}
```
Create a device disconnection processing function, stop the pipeline in the function, and set the exit pipeline flag isExit as true.
```
//Device disconnection processing
void device_disconnect_callback( ob_device_list* disconnectList ) {
    uint32_t count = ob_device_list_device_count( disconnectList, &error );
    check_error( error );
    printf( "Device disconnect: %d\n", count );
    if ( count > 0 ) {
        ob_pipeline_stop( pipeline, &error );
        check_error( error );
        isExit = true;
    }
}
```
The device connection and disconnection processing function will be called within the device up/down callback function.
```
//Device status change callback
void on_device_changed_callback( ob_device_list* removed, ob_device_list* added, void* pCallback ) {
    device_disconnect_callback( removed );
    device_connect_callback( added );
}
```
In the main function, first we need to create a Context and set the device up/down callback
```
//Creat context
ob_context* ctx = ob_create_context( &error );  

//Set device callback 
ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );
```
The main task of the main loop is to obtain the data of frame from the pipeline when the pipeline is created and started after the device connected, and print out the Color and Depth information from the data frame. 
```

//Waiting for a frame of data, the timeout period is 100ms
ob_frame* frameset = ob_pipeline_wait_for_frames( pipeline, 100, &error );
if ( frameset ) {
    //Get the depth data frame
    ob_frame* depth_frame = ob_frame_set_depth_frame( frameset, &error );
    if ( depth_frame ) {
        printf( "=====Depth Frame Info======Index: %lld TimeStamp: %lld\n", ob_frame_index( depth_frame, &error ), ob_frame_time_stamp( depth_frame, &error ) );
        //Release the depth data frame
        ob_delete_frame( depth_frame, &error ); 
    }
    //Get the Color data frame
    ob_frame* color_frame = ob_frame_set_color_frame( frameset, &error );
    if ( color_frame ) {
        printf( "=====Color Frame Info======Index: %lld TimeStamp: %lld\n", ob_frame_index( color_frame, &error ), ob_frame_time_stamp( color_frame, &error ) );
        //Release the Color data frame
        ob_delete_frame( color_frame, &error );
    }
    //Release frameSet
    ob_delete_frame( frameset, &error );
}
```
After exiting the main loop, relevant resources need to be released.
```
    if ( pipeline ) {
        //Stop pipeline
        ob_pipeline_stop( pipeline, &error );
        //Delete pipeline
        ob_delete_pipeline( pipeline, &error );
    }
    // Delete dev_list
    if ( dev_list ) {
        ob_delete_device_list( dev_list, &error );
    }
    // Delete ctx
    if ( ctx ) {
        ob_delete_context( ctx, &error );
    }
```
## PointCloud
Function description: Connect the device to open the stream, generate depth point cloud or RGBD point cloud and save it as ply format file.
> This sample is based on C++ High Level API for demonstration.

Create a point cloud and save it as a ply format file function. The detailed description of the ply file format can be viewed on the Internet.
```
//Save point cloud data to ply
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

//Save color point cloud data to ply
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
Create pipeline and stream profile
```
    //Create a PipeLine to open the Color and Depth streams after connecting the device
    pipeline = ob_create_pipeline( &error );
    //Create Config to configure the resolution, frame rate and format of Color and Depth streams
    ob_config* config = ob_create_config( &error );

    //Configure depth stream
    ob_stream_profile*  depth_profile = NULL;
    ob_stream_profile** profiles      = ob_pipeline_get_stream_profiles( pipeline, OB_SENSOR_DEPTH, &depth_profile_count, &error );
    //Find the corresponding Profile according to the specified format
    depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_Y16, 30, &error);
    ob_config_enable_stream(config, depth_profile, &error);  // Enable configuration

    //Configure color stream
    ob_stream_profile *color_profile = NULL;
    profiles                         = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
    //Find the corresponding Profile according to the specified format
    color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_YUYV, 0, &error);
    //If YUYV is not found, look for I420 format
    if(!color_profile) {  
        color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_I420, 0, &error);
        check_error(error);
    }
    ob_config_enable_stream( config, color_profile, &error );  // Enable configuration
```
Get device information pipeline to enable the D2C function of the device.
```
   //Get device handle
   ob_device* device = ob_pipeline_get_device( pipeline, &error );
   // Enable D2C alignment, required for RGBD point cloud generation.
    if ( device && ob_device_is_property_supported( device, OB_DEVICE_PROPERTY_DEPTH_ALIGN_HARDWARE_BOOL, &error ) ) {
       // Turn on hardware D2c alignment 
       ob_device_set_bool_property( device, OB_DEVICE_PROPERTY_DEPTH_ALIGN_HARDWARE_BOOL, true, &error );
    }
```
Open the stream, and create a point cloud filter for converting the depth and color frame data into point cloud data. The point cloud filter is created after opening the stream to allow the SDK to automatically set the camera parameters according to the current stream configuration. You can also set it yourself.
```
// Create a point cloud filter object (when the point cloud filter is created, the device parameters will be obtained inside the Pipeline, so try to configure the device before the filter is created)
     ob_filter* point_cloud = ob_pipeline_create_pointcloud_filter( pipeline, &error );
// Set filter parameters by yourself
     if (device && ob_device_is_property_supported( device, OB_DATA_TYPE_CAMERA_PARA, &error)) {
         CAMERA_PARA cameraParam = {0};
         uint32_t len;
         ob_device_get_structured_data( device, OB_DATA_TYPE_CAMERA_PARA, &cameraParam,&len, &error );
         ob_pointcloud_filter_set_camera_parameter(point_cloud,cameraParam,error);
     }
```
Start the main loop, call the point cloud filter according to the user's key in the loop to generate depth point cloud or RGBD point cloud data, and save it as a ply file.
```
//Waiting for a frame of data, the timeout period is 100ms.
ob_frame* frameset = ob_pipeline_wait_for_frames( pipeline, 100, &error );
if ( frameset != NULL ) 
{
    //Press R to save ply data
    if ( ( key == 'R' || key == 'r' ) && frameset != NULL ) {
        ob_pointcloud_filter_set_point_format( point_cloud, OB_FORMAT_RGB_POINT, &error );
        ob_filter_process( point_cloud, frameset, &error );
        ob_frame* pointsFrame = ob_frame_set_points_frame( frameset, &error );
        save_rgb_points_to_ply( pointsFrame, "rgb_points.ply" );
        ob_delete_frame( pointsFrame, &error );
    }
    else if ( ( key == 'D' || key == 'd' ) && frameset != NULL ) {
        //Generate and save pointcloud
        ob_pointcloud_filter_set_point_format( point_cloud, OB_FORMAT_POINT, &error );
        ob_filter_process( point_cloud, frameset, &error );
        ob_frame* pointsFrame = ob_frame_set_points_frame( frameset, &error );
        save_points_to_ply( pointsFrame, "points.ply" );
        ob_delete_frame( pointsFrame, &error );
    }
    ob_delete_frame( frameset, &error );  // Delete frameSet  Reclaim memory
}
```
After the main loop exits, stop the stream and release the resources.
```
    // Stop pipeline
    ob_pipeline_stop( pipeline, &error );

    // Delete pipeline
    ob_delete_pipeline( pipeline, &error );

    ob_delete_context( ctx, &error );
```
# C++
## HelloOrbbec
Function description: Demonstrate SDK initialization, get SDK version, get device model, get device serial number, get firmware version, and SDK release resources.
> This example is based on C++ Low Level API for demonstration.

Get SDK version. The SDK version number is divided into major version number, minor version number and revision number.
```
std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
```
First, we need to create a Context to obtain a list of device information and create a device.
```
ob::Context ctx;
```
Get the list of connected devices
```
auto devList = ctx.queryDeviceList();

//Get the number of connected devices
devList->deviceCount()
```
Create device
```
//Create a device, 0 means the index of the first device
auto dev = devList->getDevice(0);
```
Next we can get the information of this device: device information, device name, get device pid, vid, uid, device firmware version, device serial number.
```
//Get device information
auto devInfo = dev->getDeviceInfo();

//Get the device name
std::cout << "Device name: " << devInfo->name() << std::endl;

//Get the pid, vid, uid of the device
std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

//Get the fireware version number of the device 
auto fwVer = devInfo->firmwareVersion();
std::cout << "Firmware version: " << fwVer << std::endl;

//Get the serial number of the device
auto sn = devInfo->serialNumber();
std::cout << "Serial number: " << sn << std::endl;
```
Get the list of supported sensors
```
// Get a list of supported sensors
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
The console result output is as follows:![1](.\Orbbec SDK Sample Description (CC++)img\1.png)
## DepthViewer
Function description: This example mainly demonstrates the use of SDK to obtain depth data, set resolution and select to set, display depth image, and finally exit by ESC key.
> This example is based on C++ High Level API for demonstration.

First, we need to create a Pipeline. Through Pipelie, you can easily open and close multiple types of streams and obtain a set of frame data.
```
ob::Pipeline pipe;
```
Get all stream profile list of the depth camera, including stream resolution, frame rate, and frame format.
```
auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

// Get the frame format to be used by traversing the profile of the depth stream. The Y16 format will be used here to open the stream.
std::shared_ptr<ob::StreamProfile> depthProfile;
    for (int i = 0; i < profiles->count(); i++)
    {   
        auto profile = profiles->getProfile(i);
        if(profile->format() == OB_FORMAT_Y16){
            depthProfile = profile;
            break;
        }
    }
    
// You can also set the item of interest to return the first Profile in the corresponding Profile list
// auto depthProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);

// If no suitable flow configuration is found, open the first configuration in the list
if(!depthProfile) {
    depthProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
By creating Config to configure which streams to be enabled or disabled by Pipeline, depth stream will be enabled here.
```
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```
Start the stream profile in Config, if no parameters are set, the default configuration startup stream will be started.
```
pipe.start(config);
```
To set the mirror mode, first determine whether the device has readable and writable permissions, and then set
```cpp
// Gets whether the image attribute has writable permission
if(pipe.getDevice()->isPropertySupported(OB_PROP_DEPTH_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
    //Set mirror
    pipe.getDevice()->setBoolProperty(OB_PROP_DEPTH_MIRROR_BOOL, true);
}
```
Waiting for a frame of data in a blocking manner. The frame is a composite frame, which contains the frame data of all streams enabled in the configuration. Set the frame timeout time.
```
auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```
Stop Pipeline, no more frame data will be generated.
```
pipe.stop();
```
The final depth map is shown below.<br />![2](.\Orbbec SDK Sample Description (CC++)img\2.png)<br />Resources will be automatically released after the program exits normally.
## ColorViewer
Function description: This example mainly demonstrates the use of SDK to obtain color data, set resolution, display color image, and exit by ESC key.
> This example is based on C++ High Level API for demonstration.

First, we need to create a Pipeline. Through Pipelie, you can easily open and close multiple types of streams and obtain a set of frame data.
```
ob::Pipeline pipe;
```
Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format.
```
auto profiles = pipe.getStreamProfiles(OB_SENSOR_COLOR);

//Get the frame format to be used by traversing the profile of the color stream. The MJPG format will be used here to open the stream.
std::shared_ptr< ob::StreamProfile > colorProfile;
for (int i = 0; i < profiles->count(); i++) {
    auto profile = profiles->getProfile(i);
    if (profile->format() == OB_FORMAT_MJPG) {
        colorProfile = profile;
        break;
    }
}

// You can also set the item of interest through the interface to return the first Profile in the corresponding Profile list
// auto colorProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_MJPG);

//If no suitable flow configuration is found, open the first configuration in the list
if(!colorProfile) {
    colorProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
By creating Config to configure which streams to be enabled or disabled by Pipeline, color stream will be enabled here.
```
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(colorProfile);
```
Start the stream configured in Config.
```
pipe.start(config);
```
To set the mirror mode, first determine whether the device has readable and writable permissions, and then set
```cpp
//Gets whether the image attribute has writable permission
if(pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
    //Set mirror
    pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
}
```
Waiting for a frame of data in a blocking manner. The frame is a composite frame, which contains the frame data of all streams enabled in the configuration. Set the frame timeout time.
```
auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```
Stop Pipeline, no more frame data will be generated.
```
pipe.stop();
```
The final color map is shown below.<br />![3](.\Orbbec SDK Sample Description (CC++)img\3.png)<br />Resources will be automatically released after the program exits normally.
## InfraredViewer
Function description: This example mainly demonstrates the use of SDK to obtain infrared data, set the resolution, display the infrared image, and exit through the ESC key.
> This example is based on C++ High Level API for demonstration.

First, we need to create a Pipeline. Through Pipelie, you can easily open and close multiple types of streams and obtain a set of frame data.
```
ob::Pipeline pipe;
```
Get all stream profiles of the infrared camera, including stream resolution, frame rate, and frame format.
```
// Set the item of interest through the interface and return the first profile in the corresponding profile list
auto irProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);
if(!irProfile) {
    irProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
By creating Config to configure which streams to be enabled or disabled by Pipeline, depth stream will be enabled here.
```
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(irProfile);
```
Start the stream configured in Config, if no parameters are set, the default configuration stream will be started.
```
pipe.start(config);
```
To set the mirror mode, first determine whether the device has readable and writable permissions, and then set
```cpp
//Gets whether the image attribute has writable permission
if(pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
    //Set mirror
    pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
}
```
Waiting for a frame of data in a blocking manner. The frame is a composite frame, which contains the frame data of all streams enabled in the configuration. Set the frame timeout time..
```
auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```
Stop Pipeline, no more frame data will be generated.
```
pipe.stop();
```
Resources will be automatically released after the program exits normally.
## SyncAlignViewer
Function description: This example mainly demonstrates the operation of sensor data stream alignment.
> This example is based on C++ High Level API for demonstration.

First, we need to create a Pipeline. Through Pipelie, you can easily open and close multiple types of streams and obtain a set of frame data.
```
ob::Pipeline pipe;
```
Get all stream profiles of color camera and depth camera, including stream resolution, frame rate, and frame format.
```
//Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format.
auto colorProfiles = pipe.getStreamProfiles(OB_SENSOR_COLOR);

//Set the item of interest through the interface and return the first profile in the corresponding profile list
auto colorProfile = colorProfiles->getVideoStreamProfile(colorWidth, colorHeight, OB_FORMAT_MJPG, 30);
if(!colorProfile) {
    colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
}

//Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format.
auto depthProfiles = pipe.getStreamProfiles(OB_SENSOR_DEPTH);

//Set the item of interest through the interface and return the first profile in the corresponding profile list
auto depthProfile = depthProfiles->getVideoStreamProfile(depthWidth, depthHeight, OB_FORMAT_Y16, 30);
if(!depthProfile) {
    depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
By creating Config to configure which streams to enable or disable in Pipeline, color stream and depth stream will be enabled here.
```
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(colorProfile);
config->enableStream(depthProfile);
```
Control stream alignment，software alignment is enabled here
```
// Configure the alignment mode to software D2c alignment
config->setAlignMode(ALIGN_D2C_SW_MODE);
```
Start the stream configured in Config, if no parameters are set, the default configuration stream will be started.
```
pipe.start(config);
```
 Stop Pipeline, no more frame data will be generated.
```
pipe.stop();
```
Resources will be automatically released after the program exits normally.
## SensorControl
Function description: This example mainly demonstrates the operation of device control commands, Sensor control commands, and the stream of Sensor.
> This example is based on C++ Low Level API for demonstration.

First, we need to create a Context to obtain a list of device information and create a device.
```
ob::Context ctx;
```
Get device information list
```
auto devList = ctx.queryDeviceList();
```
Select a device to operate, if a single device is plugged in, it is selected and turned on by default.
```
//Select a device to operate
std::shared_ptr<ob::Device> device = nullptr;
if(deviceList->deviceCount() > 0) {
    if(deviceList->deviceCount() <= 1) {
    //If you insert a single device, the first one is selected by default
        device = deviceList->getDevice(0);
    }
    else {
        device = selectDevice(deviceList);
    }
}
```
Control command control, get the scope of the Property, set the property, get the property
```
//Gets the scope of the property
OBBoolPropertyRange  bool_range = device->getBoolPropertyRange(property_item.id)
OBIntPropertyRange   int_range = device->getIntPropertyRange(property_item.id)
OBFloatPropertyRange float_range = device->getFloatPropertyRange(property_item.id)

//set a property
device->setBoolProperty(propertyItem.id, bool_value);
device->setIntProperty(propertyItem.id, int_value);
device->setFloatProperty(propertyItem.id, float_value);

//get a property
bool bool_ret = device->getBoolProperty(propertyItem.id);
int int_ret = device->getIntProperty(propertyItem.id);
float float_ret = device->getFloatProperty(propertyItem.id);
```
Resources will be automatically released after the program exits normally.
## MultiDevice
Function description: This example mainly demonstrates the operation of multiple devices.
> This example is based on C++ Low Level API for demonstration.

First, we need to create a Context to obtain a list of device information for creating devices and pipelines.
```
ob::Context ctx;
//Query the list of connected devices
auto devList = ctx.queryDeviceList();
//Get the number of access devices
int devCount = devList->deviceCount();
```
Create a device, and create a pipeline through the device.
```
//Traverse the device list and create a device
std::vector<std::shared_ptr<ob::Device>> devices;
for (int i = 0; i < devCount; i++)
{
    //Get device and create pipeline
		auto dev  = devList->getDevice( i );
		auto pipe = std::make_shared< ob::Pipeline >( dev );
		pipes.push_back( pipe );
}
```
Start stream
```
for ( auto&& pipe : pipes ) {
        //Get depth camera profile list
        auto depthProfileList = pipe->getStreamProfileList( OB_SENSOR_DEPTH );

 //Set the item of interest through the interface and return the first profile in the corresponding profile list
    auto depthProfile = depthProfileList->getVideoStreamProfile(640, 480, 
OB_FORMAT_Y16)->as<ob::VideoStreamProfile>();
    if(!depthProfile) {
        depthProfile = depthProfileList->getProfile(0)->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);
    
 //Get color camera profile list
 auto colorProfileList = pipe->getStreamProfileList( OB_SENSOR_COLOR );
 
 //Set the item of interest through the interface and return the first profile in the corresponding profile list
   auto colorProfile = colorProfileList->getVideoStreamProfile(640, 480,
OB_FORMAT_MJPG)->as<ob::VideoStreamProfile>();
    if(!colorProfile) {
       colorProfile = colorProfileList->getProfile(0)->as<ob::VideoStreamProfile>();
    }
    config->enableStream(colorProfile);
                
 //Start the pipeline and pass in the configuration
   pipe->start(config, [i](std::shared_ptr<ob::FrameSet> frameSet) {
        std::lock_guard<std::mutex> lock(frameMutex);
        if(frameSet->colorFrame() && frameSet->depthFrame()) {
            colorFrames[i] = frameSet->colorFrame();
            depthFrames[i] = frameSet->depthFrame();
        }
    });
    i++;
}
```
Stop stream
```

void StopStream( std::vector< std::shared_ptr< ob::Pipeline > > pipes) {
    int i = 0;
    for ( auto&& pipe : pipes ) {
        if(colorFrames[i])
            colorFrames->reset();
        if(depthFrames[i])
            depthFrames->reset();
        if(irFrames[i])
            irFrames->reset();
        //Stop pipeline
        pipe->stop();
        i++;
    }
}
```
Resources will be automatically released after the program exits normally.

## DepthWorkMode
Function Description: This example mainly demonstrates switching camera depth mode. First, the depth mode list is queried, and then the corresponding camera depth mode is selected to call interface switching

First get device
```C
// Create Context. Unlike Pipeline, Context is the entry point to the low-level API, which is slightly more complicated to use for common operations such as switching streams,
// but the low-level API can provide more flexible operations such as getting multiple devices, reading and writing device and camera properties, etc
ob::Context ctx;

//Get the list of connected devices
auto devList = ctx.queryDeviceList();

//Obtain the number of connected devices
if(devList->deviceCount() == 0) {
    pressKeyExit("Device not found!");
    return -1;
}

//Create a device. 0 indicates the index of the first device
auto device = devList->getDevice(0);
```

Check whether the device supports camera depth mode. Currently, only Gemini2 supports camera depth mode
```C
// Check to see if depth mode is supported. Currently (5 December 2022), only Gemini2 binocular cameras support depth mode
if (!device->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE)) {
    pressKeyExit("Current device not support depth work mode!");
    return -1;
}
```

Get current depth work mode
```C
// Get current depth work mode
auto curDepthMode = device->getCurrentDepthWorkMode();
```

Get the list of depth work modes supported by the device
```C
// Gets a list of depth work modes
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

Switch depth work mode
```C
// Switch device to a new depth work mode
OBDepthMode depthMode = (*depthModeList)[index];
device->switchDepthWorkMode(depthMode.name);
check_error(error);
```

At the end of switching camera depth mode, you can open the camera to stream using pipeline
Cautions： 
1. If you want to switch camera depth mode, you must open the data stream after switching depth mode. Each camera depth mode supports different effective resolutions
2. If the data stream has been opened with a pipeline, the original applied pipeline must be released before the switch depth work mode 
   Re-create pipeline after switching camera depth mode; otherwise, wild pointer or memory leak will be caused;

## HotPlugin
Function description: This example mainly demonstrates the settings of the device plug-in callback, and processes the obtained stream after replug-in.
> This example is based on C++ High Level API for demonstration.

First, we need to create a Context to obtain a list of device information and create a device.
```
ob::Context ctx;
```
Register the device callback, and execute related functions when the device is plugged and unplugged.
```
ctx.setDeviceChangedCallback( []( std::shared_ptr< ob::DeviceList > removedList, std::shared_ptr< ob::DeviceList > addedList ) {
        DeviceDisconnectCallback( removedList );
        DeviceConnectCallback( addedList );
    } );
```
Open stream according to the resolution and format configured in the configuration file.
```
//Start the stream by the stream configuration of the configuration file, if there is no configuration file, the stream will be started with the 0th stream configuration
try{
  pipeline->start(nullptr);
}catch(...){
  std::cout<<"Pipeline start failed!"<<std::endl;
}
//Get all stream configurations of the depth camera, including stream resolution, frame rate, and frame format
auto depthProfiles = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
//Get the 0th Profile in the corresponding stream configuration list. If there is a configuration file, the stream configuration in the configuration file is the 0th in the stream configuration list.
auto depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
//Get all stream configurations for color cameras, including stream resolution, frame rate, and frame format
auto colorProfiles = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
//Get the 0th Profile in the corresponding stream configuration list. If there is a configuration file, the stream configuration in the configuration file is the 0th in the stream configuration list.
auto colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
//get frame rate
colorFps = colorProfile->fps();
depthFps = depthProfile->fps();
```
Waiting for a frame of data in a blocking manner. The frame is a composite frame, which contains the frame data of all streams enabled in the configuration. Set the frame timeout time.
```
auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```
Stop Pipeline, will no longer generate frame data.
```
pipe.stop();
```
Resources will be automatically released after the program exits normally.
## ImuReader
Function description: This example mainly demonstrates the use of SDK to obtain IMU data, and exit by ESC key.
> This example is based on C++ Low Level API for demonstration.

Print the SDK version number. The SDK version number is divided into major version number, minor version number and revision number.
```
//Print the SDK version number. The SDK version number is divided into major version number, minor version number and revision number.
std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
```
First, we need to create a Context to obtain a list of device information and create a device.
```
ob::Context ctx;
```
Get the list of connected devices
```
auto devList = ctx.queryDeviceList();

//Get the number of connected devices.
devList->deviceCount()
```
Create device
```
//Create a device, 0 means the index of the first device.
auto dev = devList->getDevice(0);
```
Get the gyroscope sensor and acceleration sensor
```
auto gyroSensor = dev->getSensorList()->getSensor( OB_SENSOR_GYRO );

auto accelSensor = dev->getSensorList()->getSensor( OB_SENSOR_ACCEL );
```
Get the profile list of the gyroscope sensor and select the first profile to open stream, get the frame data in the open stream callback, the same as the acceleration sensor.
```
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
Stop stream
```
gyroSensor->stop();
accelSensor->stop();
```
Resources will be automatically released after the program exits normally.

## MultiDeviceSync
Function Description: Demonstrates how to set synchronization parameters, start devices stream and handle frames

Steps for synchronizing multiple devices:
1. Load the configuration file and configure the device;
2. Reboot devices;
3. Wait for all devices to reboot complete, start all devices's color and depth sensor to handle it frames;

Create OBContext
```C
OBContext context;
```

#### Load the configuration file and configure the device;
Get connected device list
```C
// Get connected device list
auto devList = context.queryDeviceList();

// Get the number contain in devList
int devCount = devList->deviceCount();
for(int i = 0; i < devCount; i++) {
    configDevList.push_back(devList->getDevice(i));
}
```

Setting multiple device config to each device.
```C
for(auto config: deviceConfigList) {
    auto findItr = std::find_if(configDevList.begin(), configDevList.end(), [config](std::shared_ptr<ob::Device> device) {
        auto serialNumber = device->getDeviceInfo()->serialNumber();
        auto cmpSize      = (std::min)(strlen(serialNumber), config->deviceSN.size());
        return strncmp(serialNumber, config->deviceSN.c_str(), cmpSize) == 0;
    });
    if(findItr != configDevList.end()) {
        auto curConfig = (*findItr)->getSyncConfig();

        // Update the configuration items in the configuration file and retain the original configurations for other items
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

Reboot devices
```C
// Reboot devices
for(auto device: configDevList) {
    rebootingDevInfoList.push_back(device->getDeviceInfo());
    device->reboot();
}
configDevList.clear();
```

Wait for all devices to restart, and then start stream on multiple devices.


#### Start stream on connected devices

Get connected devices
```C
// Get connected device list
auto devList = context.queryDeviceList();

// Get the number contain in devList
int devCount = devList->deviceCount();
for(int i = 0; i < devCount; i++) {
    streamDevList.push_back(devList->getDevice(i));
}

if(streamDevList.empty()) {
    std::cerr << "Device list is empty. please check device connection state" << std::endl;
    return -1;
}
```

Read the multi-device synchronization configuration from the device, Distinguish between host and slave
```C
// Iterate through the list of devices and create devices
std::vector<std::shared_ptr<ob::Device>> primary_devices;
std::vector<std::shared_ptr<ob::Device>> common_devices;
for(auto dev: streamDevList) {
    auto config = dev->getSyncConfig();
    if(config.syncMode == OB_SYNC_MODE_PRIMARY || config.syncMode == OB_SYNC_MODE_PRIMARY_MCU_TRIGGER || config.syncMode == OB_SYNC_MODE_PRIMARY_IR_TRIGGER
        || config.syncMode == OB_SYNC_MODE_PRIMARY_SOFT_TRIGGER) {
        primary_devices.push_back(dev);
    }
    else {
        common_devices.push_back(dev);
    }
}

if(primary_devices.empty()) {
    std::cerr << "WARNING primary_devices is empty!!!" << std::endl;
}
```

Set the time synchronization frequency
```C
// Enable time synchronization among multiple devices
context.enableMultiDeviceSync(60000);  // Updates are synchronized every minute
```

Open the device data stream of color sensor and depth sensor
```C
// Open the stream of the main mode device first, and then open the stream of other mode devices. (This sequence is required for devices of other models. For details, please refer to the related device documentation.)
startStream(primary_devices, OB_SENSOR_COLOR);
std::this_thread::sleep_for(std::chrono::milliseconds(500));
startStream(primary_devices, OB_SENSOR_DEPTH);
std::this_thread::sleep_for(std::chrono::milliseconds(500));
startStream(common_devices, OB_SENSOR_DEPTH, primary_devices.size());
startStream(common_devices, OB_SENSOR_COLOR, primary_devices.size());
```

Implementation of start stream.
```C
void startStream(std::vector<std::shared_ptr<ob::Device>> devices, OBSensorType sensorType, int deviceIndexBase) {
    for(auto &&dev: devices) {
        // Get sensor list from device
        auto sensorList = dev->getSensorList();
        for(uint32_t i = 0; i < sensorList->count(); i++) {
            auto sensor = sensorList->getSensor(i);
            if(sensorType == sensor->type()) {
                auto profiles = sensor->getStreamProfileList();
                auto profile  = profiles->getProfile(0);
                switch(sensorType) {
                case OB_SENSOR_DEPTH:
                    if(profile) {
                        sensor->start(profile, [deviceIndexBase](std::shared_ptr<ob::Frame> frame) { handleDepthStream(deviceIndexBase, frame); });
                    }
                    break;
                case OB_SENSOR_COLOR:
                    if(profile) {
                        sensor->start(profile, [deviceIndexBase](std::shared_ptr<ob::Frame> frame) { handleColorStream(deviceIndexBase, frame); });
                    }
                    break;
                default:
                    break;
                }
            }
        }
        deviceIndexBase++;
    }
}

void stopStream(std::vector<std::shared_ptr<ob::Device>> devices, OBSensorType sensorType) {
    for(auto &&dev: devices) {
        // Get sensor list from device
        auto sensorList = dev->getSensorList();
        for(uint32_t i = 0; i < sensorList->count(); i++) {
            if(sensorList->type(i) == sensorType) {
                sensorList->getSensor(i)->stop();
                break;
            }
        }
    }
}
```

Process sensor frame data
```C
void handleColorStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
    std::lock_guard<std::mutex> lock(frameMutex);
    std::cout << "Device#" << devIndex << ", color frame index=" << frame->index() << ", timestamp=" << frame->timeStamp()
              << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

    colorFrames[devIndex] = frame;
}

void handleDepthStream(int devIndex, std::shared_ptr<ob::Frame> frame) {
    std::lock_guard<std::mutex> lock(frameMutex);
    std::cout << "Device#" << devIndex << ", depth frame index=" << frame->index() << ", timestamp=" << frame->timeStamp()
              << ", system timestamp=" << frame->systemTimeStamp() << std::endl;

    depthFrames[devIndex] = frame;
}
```
Here we can process the output of multiple devices according to colorFrames and depthFrames.

Stop sensor stream
```C
// stop stream
stopStream(primary_devices, OB_SENSOR_COLOR);
stopStream(primary_devices, OB_SENSOR_DEPTH);
stopStream(common_devices, OB_SENSOR_COLOR);
stopStream(common_devices, OB_SENSOR_DEPTH);

```

## PointCloud
Function description: Connect the device to open the stream, generate a depth point cloud or RGBD point cloud and save it as a ply format file.
> This example is based on C++ High Level API for demonstration.

Create a point cloud and save it as a ply format file function. The detailed description of the ply file format can be viewed on the Internet.<br />First, create two functions to save the point cloud data obtained from the stream. This is a function to save point cloud data.
```
//Save point cloud data to ply
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
Create another function to save the color point cloud data.
```
//Save color point cloud data to ply
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
Set the Log level to avoid too many Logs of Info level affecting the output of the point cloud
```cpp
ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_ERROR);
```
Create a Pipeline, through pipeline, you can easily open and close multiple types of streams and obtain a set of frame data.
```
 ob::Pipeline pipeline;
```
Then we can get all the profile of the color stream through the Pipeline, including the resolution of the stream, the frame rate, and the format of the stream.
```
auto colorProfiles = pipeline.getStreamProfileList( OB_SENSOR_COLOR );
```
Then select the profile of the stream we need through the profile list of the color stream obtained above.
```
//Set the item of interest through the interface, and return the first Profile corresponding to the Profile list
auto colorProfile = colorProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_YUYV);
if(!colorProfile) {
  colorProfile = colorProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_I420);
  if(!colorProfile)
      colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
According to the method of setting the profile of the color stream above, set the profile of the depth stream in the same way.
```
//Get all stream configurations of the depth camera, including stream resolution, frame rate, and frame format
auto depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
//Set the item of interest through the interface, and return the first Profile corresponding to the Profile list
auto depthProfile = depthProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);
if(!depthProfile) {
    depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
By creating Config to configure which streams to be enabled or disabled in Pipeline, color stream and depth stream will be enabled here.
```
  std::shared_ptr< ob::Config > config = std::make_shared< ob::Config >();
  config->enableStream( colorProfile );
  config->enableStream( depthProfile );
```
D2C alignment need to be turned on when generating RGBD point cloud
```
//Enable D2C alignment, which needs to be enabled when generating RGBD point clouds
config->setAlignMode(ALIGN_D2C_HW_MODE);
```
Create a point cloud Filter object and set camera internal parameters
```
// Create a point cloud Filter object (when the point cloud Filter is created, the device parameters will be obtained inside the Pipeline, so try to configure the device before the Filter is created)
ob::PointCloudFilter pointCloud;

//Get the camera internal parameters and pass them into the point cloud Filter
auto cameraParam = pipeline.getCameraParam();
pointCloud.setCameraParam(cameraParam);
```
Start Pipeline
```
pipeline.start( config );
```
Set up some operation tips
```
 std::cout << "Press R to create rgbd pointCloud and save to ply file! " << std::endl;
 std::cout << "Press d to create depth pointCloud and save to ply file! " << std::endl;
 std::cout << "Press ESC to exit! " << std::endl;
```
Set the main process to obtain and save point cloud data through the point cloud Filter object created above.
```
if(key == 'R' || key == 'r') {
  count = 0;
  //Limit up to 10 repetitions
  while(count++ < 10) {
    //Waiting for a frame of data, the timeout is 100ms
    auto frameset = pipeline.waitForFrames(100);
    if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
      try {
        //Generate color point cloud and save
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
  //Limit up to 10 repetitions
  while(count++ < 10) {
    //Waiting for a frame of data, the timeout is 100ms
    auto frameset = pipeline.waitForFrames(100);
    if(frameset != nullptr && frameset->depthFrame() != nullptr) {
      try {
        //Generate point cloud and save
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
Finally, stop the stream through the Pipeline.
```
pipeline.stop();
```
Resources will be released after the program exits normally.
## SaveToDisk
Function description: Connect the device to open the stream, get the color and depth map and save as png format.
> This example is based on C++ high Level API for demonstration.

Create two functions to save the acquired picture to a file.
```
//Save the depth image in png format.
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

//Save the color image in png format.
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
Create a Pipeline, through Pipelie, you can easily open and close multiple types of streams and obtain a set of frame data.
```
ob::Pipeline pipeline;
```
Then we can get all the profile of the color stream through the Pipeline, including the resolution of the stream, the frame rate, and the format of the stream.
```
auto colorProfiles = pipeline.getStreamProfileList( OB_SENSOR_COLOR );
```
Then select the profile of the stream, we need to get the profile list of the color stream obtained above.
```
//Set the item of interest through the interface, and return the first Profile corresponding to the Profile list
auto colorProfile = colorProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_MJPG, 30);
if(!colorProfile){
  colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
Configure the depth stream in the same way as getting the color stream configuration
```cpp
//Get all stream configurations of the depth camera, including stream resolution, frame rate, and frame format
auto depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);

//Set the item of interest through the interface, and return the first Profile corresponding to the Profile list
auto depthProfile = depthProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16, 30);
if(!depthProfile){
    depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
}
```
Configure the stream we want to open by creating Config.
```
std::shared_ptr< ob::Config > config = std::make_shared< ob::Config >();
    config->enableStream( colorProfile );
    config->enableStream( depthProfile )
```
Start stream
```
pipeline.start( config );
```
Obtain frame data by waiting
```
 auto frameset = pipeline.waitForFrames( 100 );
```
Get the data of a single type of frame
```
auto colorFrame = frameset->colorFrame();
auto depthFrame = frameset->depthFrame();
```
Create a format conversion Filter to convert the color image to RGB format and save it
```cpp
//Create Format Conversion Filter
ob::FormatConvertFilter formatConverFilter;

formatConverFilter.setFormatConvertType(FORMAT_MJPEG_TO_RGB888);
colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
formatConverFilter.setFormatConvertType(FORMAT_RGB888_TO_BGR);
colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
```
Store the data through the edited storage function above
```
saveColor( colorFrame );
saveDepth( depthFrame );
```
Stop Pipeline
```
pipeline.stop();
```
Resources will be released after the program exits normally.
## Recorder
Function description: Connect the device to open the stream, and record the current video stream to a file.
> This example is based on C++ high Level API for demonstration.

First, you need to create a Pipeline, through which you can easily open and close multiple types of streams and obtain a set of frame data.
```cpp
ob::Pipeline pipe;
```
Obtain the stream configuration of the depth camera through the input resolution, format, frame rate and other interesting items.
```cpp
//Get depth stream configuration
auto profiles     = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
auto depthProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);
```
Configure which streams the Pipeline wants to enable or disable by creating a Config, where the depth stream will be enabled
```cpp
//Configure the type of stream opened by Pipeline
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```
Start the stream configured in Config. If no parameters are passed, the default configuration startup stream will be started, and the recording switch will be enabled.
```cpp
pipe.start(config);
pipe.startRecord("./OrbbecPipeline.bag");
```
Wait for a frame of data in a blocking manner, which is a composite frame, which contains frame data of all streams enabled in the configuration, and sets the frame's waiting timeout time.
```
auto frameSet = pipe.waitForFrames(100);	//Set the wait time to 100ms
```
Stop Pipeline
```cpp
pipe.stop();
```
Resources will be released after the program exits normally.
## Playback

Function description: Connect the device to open the stream, and load the video file for playback.
> This example is based on C++ high Level API for demonstration.

```cpp
//Create a pipeline object for playback
ob::Pipeline pipe("./OrbbecPipeline.bag");
```
Get the playback status callback of the playback object
```cpp
//Get playback object set playback status callback
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
Read device information and internal parameter information from playback files
```cpp
//Read device information from playback file
auto deviceInfo = playback->getDeviceInfo();
std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
  << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;

//Read internal parameter information from playback file
auto cameraParam = pipe.getCameraParam();
std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
              << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;
```
Start playback
```cpp
//Start playback
pipe.start(NULL);
```
Wait for a frame of data in a blocking manner, which is a composite frame, which contains frame data of all streams enabled in the configuration, and sets the frame's waiting timeout time.
```
auto frameSet = pipe.waitForFrames(100);	//Set the wait time to 100ms
```
Stop Pipeline
```cpp
pipe.stop();
```
Resources will be released after the program exits normally.
