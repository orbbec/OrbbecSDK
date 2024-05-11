All examples can be found in the Examples directory of the project

|Name| Language| Description|
|--- | --- | ---|
|HelloOrbbec | C | Demo connecting to a device to get SDK version and device information|
|FirmwareUpgrade | C | Demonstration of Selecting Firmware Bin or img Files to Upgrade Firmware Version for Devices|
|DepthViewer | C | Demo using SDK to get depth data, draw display, get resolution, set and display depth images|
|ColorViewer | C | Demo Using SDK to Get Color Data and Draw Display, Get Resolution and Set, Display Color Images|
|HotPlugin | C | Demo device hot plug monitoring, automatically connect to device upon detection of device online, open deep stream, automatically disconnect device upon detection of device disconnection|
|PointCloud | C | Demonstration of generating deep point clouds or RGBD point clouds and saving them as ply format files|
|AlignFilterViewer | C | Demonstrate the operation of aligning sensor data streams|
|DoubleInfraredViewer | C | Demonstration of Obtaining Dual Infrared Camera Images|
|InfraredViewer | C | Demonstration of Obtaining Infrared Camera Images|
|Post-Processing | C | Demonstration of post-processing operations|
|HdrMerge | C | Demonstration of Enabling HDR Operation|
|ColorDepthViewer | C | Demo using SDK to get color and depth 0 data and draw display, get resolution and set, display color images|
|HelloOrbbec | C++| Demo connecting to a device to get SDK version and device information|
|DepthViewer | C++| Demo Using SDK to Get Depth Data and Draw Display, Get Resolution and Set, Display Depth Images|
|ColorViewer | C++| Demo using SDK to get color data and draw display, get resolution and set, display color images, and output metadata|
|SyncAlignViewer | C++| Demonstrate the operation of aligning sensor data streams|
|FirmwareUpgrade | C++| Demonstration of selecting firmware bin or img files to upgrade firmware versions to devices|
|MultiStream | C++| Demonstrate the operation of a device opening multiple streams simultaneously|
|MultiDevice | C++| Demonstrate operating on multiple devices|
|HotPlugin | C++| Demonstrate the setting of device plug callback and get the stream of post plug processing|
|PointCloud | C++| Demonstration of generating deep point clouds or RGBD point clouds and saving them as ply format files|
|SaveToDisk | C++| Get color and depth maps and store them in PNG format|
|Recorder | C++| Record the current video stream to a file|
|Playback | C++| Loading Video Files for Playback|
|DepthUnitControl | C++| Demonstration of Obtaining Depth Precision Operations|
|HdrMerge | C++| Demonstration of Enabling HDR Operations|
|DepthWorkMode | C++| Demonstration of Converting Deep Work Mode Operations|
|DoubleInfraredViewer | C++| Demo Obtaining Dual Infrared Camera Images|
|InfraredViewer | C++| Demo Obtaining Infrared Camera Images|
|ImuReader | C++| Demonstration of Obtaining IMU Data|
|MultiDeviceSync | C++| Demonstration of multi device synchronization operation|
|PostProcessing | C++| demonstrate performing post-processing operations|
|SensorControl | C++| Demonstration of Sensor Control Operations|
|AlignFilterViewer | C++| Demonstrate the operation of synchronous alignment of sensor data streams|

# C
## HelloOrbbec

Function Description: Used to demonstrate SDK initialization, get SDK version, get device model, get device serial number, get firmware version number, SDK resource release, and exit the program through the ESC_KEY key
> This example is based on the C Low Level API for demonstration

Firstly, get and print the current SDK version
```c
printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());
```
Create a Context for obtaining a list of device information and creating devices
```c
ob_error*   error = NULL;
ob_context* ctx   = ob_create_context( &error );
```
Query the list of connected devices to get the number of devices in the list, and use index number 0 to create the first device in the list
```c
ob_device_list* dev_list = ob_query_device_list( ctx, &error );
int dev_count = ob_device_list_device_count(dev_list, &error);
if(dev_count == 0) {
	printf("Device not found!\n");
	return -1;
}
ob_device* dev = ob_device_list_get_device(dev_list, 0, &error);
```
Next, you can get information related to this device
```c
//Get device information
ob_device_info* dev_info = ob_device_get_device_info(dev, &error);

//Get device name
const char* name = ob_device_info_name(dev_info, &error);

//Get pid, vid, uid of the device
int pid = ob_device_info_pid(dev_info, &error);
int vid = ob_device_info_vid(dev_info, &error);
int uid = ob_device_info_uid(dev_info, &error);

//By get the firmware version number of the device
const char* fw_ver = ob_device_info_firmware_version(dev_info, &error);

//By get the serial number of the device
const char* sn = ob_device_info_serial_number(dev_info, &error);

//Get a list of supported sensors
ob_sensor_list* sensor_list = ob_device_get_sensor_list(dev, &error);

//Get the number of sensors
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
//Release sensor list
ob_delete_sensor_list(sensor_list, &error);
//Release device info
ob_delete_device_info(dev_info, &error);
//Release device
ob_delete_device(dev, &error);
//Release device list
ob_delete_device_list(dev_list, &error);
//Release context
ob_delete_context(ctx, &error);
```

## FirmwareUpgrade
Function Description: This example demonstrates how to upgrade a device using fir
> This example is based on the C Low Level API for demonstration. The example is compiled in C++and the OrbbecSDK uses the C language API

Get firmware files through command parameters in the main function interface
```cpp
// The check_firmware_file_path() function is used to check if a file exists. In actual code, it is best to check if the suffix is bin or img, and if the firmware file matches the target device
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

    // The following business code

    return 0;
}
```

Create an ob_context and get the device through it. This example assumes that the upper computer (Windows, Ubuntu, Android platforms) has already inserted the device before running firmware_upgrade [. exe]. Device_changed_callback is used to listen for business processing of the upgraded device after firmware upgrade and device restart
```cpp
// Building an ob_context object
ob_error   *error = NULL;
ob_context *ctx   = ob_create_context(&error);
check_error(error);

// Set up a device change listener, device_changed_callback is a key function for managing device declaration cycles, and developers must pay attention to this callback
ob_set_device_changed_callback(ctx, device_changed_callback, &callback_user_data_, &error);
check_error(error);

// Query the currently connected devices
ob_device_list *dev_list = ob_query_device_list(ctx, &error);
check_error(error);

// Retrieve the current number of connected devices from ob_device_list
int dev_count = ob_device_list_device_count(dev_list, &error);
check_error(error);
if(dev_count == 0) {
    // Firmware upgrade example assumes that the device has been connected to the upper computer (Windows, Ubuntu, Android platforms)
    printf("Device not found!\n");
    return -1;
}

// Get the first device, index=0
ob_device *dev = ob_device_list_get_device(dev_list, 0, &error);
check_error(error);

// Print device information
dump_device_info(dev);
```

Get the current firmware version information of the device
```cpp
// Print device name, SN, VID, PID, and firmware version
void dump_device_info(ob_device *device) {
    ob_error *error = NULL;

    // Get the ob_device_info object, through which the basic information of the target device can be obtained
    ob_device_info *dev_info = ob_device_get_device_info(device, &error);
    check_error(error);

    // Device Name
    const char *name = ob_device_info_name(dev_info, &error);
    check_error(error);
    printf("Device name: %s\n", name);

    // VID，PID，UID of the device
    int pid = ob_device_info_pid(dev_info, &error);
    check_error(error);
    int vid = ob_device_info_vid(dev_info, &error);
    check_error(error);
    const char *uid = ob_device_info_uid(dev_info, &error);
    check_error(error);
    printf("Device pid: %d vid: %d uid: %s\n", pid, vid, uid);

    // The current firmware version number of the device
    const char *fw_ver = ob_device_info_firmware_version(dev_info, &error);
    check_error(error);
    printf("Firmware version: %s\n", fw_ver);

    // SN of the device
    const char *sn = ob_device_info_serial_number(dev_info, &error);
    check_error(error);
    printf("Serial number: %s\n", sn);

    // Release resources, otherwise it will cause memory leakage
    ob_delete_device_info(dev_info, &error);
    check_error(error);
}
```

Upgrade firmware for the target device
a. Implement firmware upgrade callback interface for C API;
b. Call the firmware upgrade interface for upgrading;
```cpp
// Implement firmware upgrade callback interface for C API;
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
        // Firmware upgrade successful
        printf("Upgrade Firmware done, percent: %u\n", (uint32_t)percent);
        is_upgrade_success_ = true;
    }
    else if(state == STAT_VERIFY_IMAGE) {
        printf("Upgrade Firmware verify image\n");
    }
    else {
        // Firmware upgrade failed
        printf("Upgrade Firmware failed. state: %d, errMsg: %s, percent: %u \n", (int)state, message ? message : "", (uint32_t)percent);
    }
}

// Perform firmware upgrades on the target device
bool upgrade_firmware(ob_device *device, const char *firmwarePath) {
    const char *index     = strstr(firmwarePath, ".img");
    bool        isImgFile = (bool)index;
    index                 = strstr(firmwarePath, ".bin");
    bool isBinFile        = (bool)index;
    if(!(isImgFile || isBinFile)) {
        // Firmware upgrade files are usually bin or img. In actual business, it is best to use information such as file name and file MD5 to prevent errors
        printf("Upgrade Fimware failed. invalid firmware file: %s\n", firmwarePath);
        return false;
    }

    // Call the firmware upgrade interface for upgrading;
    is_upgrade_success_ = false;
    ob_error *error     = NULL;
    ob_device_upgrade(device, firmwarePath, device_upgrade_callback, false, &callback_user_data_, &error);
    check_error(error);
    return is_upgrade_success_;
}
```

After a successful firmware upgrade, the device needs to be restarted. There are two ways to restart a device: one is to unplug and unplug the device (or restart the operating system), and the other is to call the reboot interface of the OrbbecSDK. After the device goes online, the dump_device_info() function in this example can be used to query the current firmware version information of the device

The following example demonstrates restarting a device through the firmware reboot interface
```cpp
// reboot device
printf("Reboot device\n");
is_device_removed_       = false;
is_wait_reboot_complete_ = true;
ob_device_reboot(dev, &error);
check_error(error);

// Release resources to prevent memory leaks
ob_delete_device(dev, &error);
check_error(error);
```

On ob_device_changed_callback, you can listen for events related to device rebooting and going offline or online. For more details, please refer to the example of hot swapping
```cpp
// Monitoring device changes
void device_changed_callback(ob_device_list *removed, ob_device_list *added, void *user_data) {
    ob_error *error = NULL;

    // Processing online devices through added

    // Processing offline devices through removal

    // Release resources to avoid memory leaks
    ob_delete_device_list(removed, &error);
    check_error(error);

    // elease resources to avoid memory leaks
    ob_delete_device_list(added, &error);
    check_error(error);
}
```
## DepthViewer

Function description: Demonstrate using SDK to get depth data and draw display, get resolution and set, display depth images, and exit the program through ESC_KEY key

> This example is based on the C High Level API for demonstration

Firstly, you need to create a Pipeline to connect the device and open color and depth streams
```c
pipe = ob_create_pipeline( &error );
```
Create a Configuration to configure the resolution, frame rate, and format of color and depth streams
```c
ob_config* config = ob_create_config( &error );
```
Configure stream
```c
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
Start Pipeline through Configuration
```c
ob_pipeline_start_with_config(pipe, config, &error);
```
Release resources and exit the program.
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
## ColorViewer

Function description: Demonstrate using SDK to get color data and draw display, get resolution and set it, display color images, and exit the program through ESC_KEY key

> This example is based on the C High Level API for demonstration

Firstly, you need to create a Pipeline to connect the device and open color and depth streams

```c
pipe = ob_create_pipeline( &error );
```

Create a Configuration to configure the resolution, frame rate, and format of color and depth streams

```c
ob_config* config = ob_create_config( &error );
```

Configure streaming

```c
//Configure Color streaming
ob_stream_profile *     color_profile = NULL;
ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_Color, &error);
//Search for the corresponding Profile according to the specified format, and prioritize the RGB888 format
color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30, &error);
//If the RGB888 format is not found and does not match the format, search for the corresponding profile to open the stream
if(error){
	color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, OB_HEIGHT_ANY, OB_FORMAT_ANY, 30, &error);
	error = nullptr;
}
ob_config_enable_stream(config, depth_profile, &error);  // Enable configuration
```

Start Pipeline through Configuration

```c
ob_pipeline_start_with_config(pipe, config, &error);
```

Release resources and exit the program.

```c
//Release frameSet
ob_delete_frame(frameset, &error);

//Release profile
ob_delete_stream_profile(color_profile, &error);

//Release profile list
ob_delete_stream_profile_list(profiles, &error);

//Release device
ob_delete_device(device, &error);

//Release pipeline
ob_delete_pipeline(pipe, &error);
```


## HotPlugin

Function description: Device hot plug monitoring, automatically connects the device when it is online and opens deep stream, and automatically disconnects the device when it is disconnected., And exit the program through the ESC_KEY key
> This example is based on the C Low Level API for device registration and offline callback, and the High Level API for open stream frame retrieval demonstration

Create a device connection processing function, create a pipeline within the function, and call createAnd_start_with configuration to enable color and depth streams.
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
Create a device disconnection handling function that stops the pipeline and sets the exit pipeline flag isExit to true
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
The device connection and disconnection handling functions will be called within the device's online and offline callback functions
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
Within the main function, create a Context and register device online and offline callbacks
```c
 //Create Context
ob_context* ctx = ob_create_context( &error );

//Register device callback
ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );
```

After exiting the main loop, relevant resources need to be released
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
## PointCloud

Function description: Connect the device to open the stream, generate a deep point cloud or RGBD point cloud and save it as a ply format file, and exit the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Create a point cloud and save it as a ply file function. The detailed description of the ply file format can be viewed on the network
```c
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

// Save color point cloud data to ply
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
Create Pipeline and stream Configuration
```c
//Create a pipeline to open the Color and Depth streams after connecting devices
pipeline = ob_create_pipeline( &error );
//Create a config to configure the resolution, frame rate, and format of the Color and Depth streams
ob_config* config = ob_create_config( &error );


//Configure Color Stream
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

    //Configure Depth Stream
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
Turn on device D2C function
```c
// Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds
ob_config_set_align_mode(config, align_mode, &error);
```

Enable Steaming
```c
ob_pipeline_start_with_config(pipeline, config, &error);
```

Create a point cloud filter after opening the stream. Point cloud filters are used to convert depth and color frame data into point cloud data. The purpose of creating a point cloud filter after streaming is to enable the SDK to automatically set camera parameters based on the current streaming configuration. Of course, you can also set it up yourself.
```c
// Create a point cloud Filter object (device parameters will be obtained inside the Pipeline when the point cloud filter is created, so try to configure
// the device before creating the filter)
ob_filter *point_cloud = ob_create_pointcloud_filter(&error);
check_error(error);

// Get the current open-stream camera parameters from the pipeline and pass them to the point cloud filter
ob_camera_param camera_param = ob_pipeline_get_camera_param(pipeline, &error);
check_error(error);
ob_pointcloud_filter_set_camera_param(point_cloud, camera_param, &error);
check_error(error);
```

Start the main loop, which calls the point cloud filter based on user keys to generate deep point cloud or RGBD point cloud data, and save it as a ply file.
```c
// Waiting for one frame of data with a timeout of 100ms
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
After the main loop exits, stop the stream and destroy the recycled resources
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

## AlignFilterViewer

Supported devices: Gemini 330 series cameras.

Function description: Demonstrate using SDK for stream alignment demonstration, display aligned images, and exit the program with ESC_KEY key.

>This example is based on the C High Level API for demonstration.

Create Pipeline and stream Configuration
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

Configure stream alignment
```c
/* Config depth align to color or color align to depth.
ob_stream_type align_to_stream = OB_STREAM_DEPTH; */
ob_stream_type align_to_stream = OB_STREAM_COLOR;
ob_filter *    align_filter    = ob_create_align(&error, align_to_stream);
```

Start Pipeline through Configuration
```c
// Start the pipeline with config
ob_pipeline_start_with_config(pipeline, config, &error);
check_error(error);
```

Get aligned data
```c
ob_frame *new_frame_set = ob_filter_process(align_filter, frameset, &error);
check_error(error);

new_depth_frame = ob_frameset_depth_frame(new_frame_set, &error);
check_error(error);

new_color_frame = ob_frameset_color_frame(new_frame_set, &error);
check_error(error);
```

Release resources and exit the program.

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

## DoubleInfraredViewer

Function description: Demonstrate obtaining dual infrared camera images, displaying left and right infrared images, and exiting the program with the ESC_KEY key

>This example is based on the C High Level API for demonstration

Create pipeline and config
```c
// Create a pipeline to open the Infrared stream after connecting the device
pipeline = ob_create_pipeline(&error);
check_error(error);

// Create config to configure the resolution, frame rate, and format of the Infrared stream
ob_config *config = ob_create_config(&error);
check_error(error);
```

Get Left IR Stream Configuration
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

Get Right IR Stream Configuration
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

Enable pipeline through config
```c
ob_pipeline_start_with_config(pipeline, config, &error);
check_error(error);
```

Get left and right IR frame data
```c
ob_frame *ir_left_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_LEFT, &error);
check_error(error);

ob_frame *ir_right_frame = ob_frameset_get_frame(frameset, OB_FRAME_IR_RIGHT, &error);
check_error(error);
```

// stop pipeline
```c
ob_pipeline_stop(pipeline, &error);
check_error(error);
```

Release relevant resources
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

## InfraredViewer

Function description: Demonstrate obtaining infrared camera images, displaying infrared images, and exiting the program with the ESC_KEY key

>This example is based on the C High Level API for demonstration

Create a pipeline for stream configuration
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

Enable pipeline through config
```c
// Start the pipeline with config
ob_pipeline_start_with_config(pipeline, config, &error);
check_error(error);
```

Release relevant resources
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

## Post-Processing

Supported devices: Gemini 330 series cameras.

Function description: Demonstrate the use of post-processing operations, display post-processing images, and exit the program using the ESC_KEY key

>This example is based on the C High Level API for demonstration

Create a pipeline for stream configuration
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

Get sensors
```c
// Get device
ob_device *device = ob_pipeline_get_device(pipeline, &error);
check_error(error);

// Get depth sensor
ob_sensor *depthSensor = ob_device_get_sensor(device, OB_SENSOR_DEPTH, &error);
check_error(error);
```

Get post-processing list
```c
ob_filter_list *filterList = ob_sensor_get_recommended_filter_list(depthSensor, &error);
check_error(error);
```

Post processing
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

Release relevant resources
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

## HdrMerge

Supported devices: Gemini 330 series cameras.

Function description: Demonstrate using HDR operation, display HDR processed images, and exit the program with ESC_KEY key

>This example is based on the C High Level API for demonstration

Create a pipeline, configure depth
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


Get device
```c
// Get the device through the pipeline
ob_device *dev = ob_pipeline_get_device(pipeline, &error);
check_error(error);
```

Open HDR processing
```c
// Create HdrMerge post processor
ob_filter *hdr_merge_filter = ob_create_hdr_merge(&error);
check_error(error);

bool is_supported = ob_device_is_property_supported(dev, OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE, &error);
check_error(error);
// open hdr merge
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

Open pipeline
```c
ob_pipeline_start_with_config(pipeline, config, &error);
```

Get HDR processed images
```c
auto new_frame_set = ob_filter_process(hdr_merge_filter, frameset, &error);
check_error(error);

auto new_depth_frame = ob_frameset_depth_frame(new_frame_set, &error);
check_error(error);
if(new_depth_frame != nullptr) {
    frames.push_back(new_depth_frame);
}
```

Stop pipeline
```c
ob_pipeline_stop(pipeline, &error);
```

## ColorDepthViewer

Function description: Demonstrate using SDK to get color and depth 0 data, draw and display, get resolution and set, display color images, and exit the program with ESC_KEY key

>his example is based on the C High Level API for demonstration

Create a pipeline and configure stream information
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

Turn off synchronization
```c
ob_pipeline_disable_frame_sync(pipe, &error);
check_error(error);
```

Enable pipeline through configuration
```c
ob_pipeline_start_with_config(pipe, config, &error);
check_error(error);
```

Get frame data
```c
ob_frame *color_frame = ob_frameset_color_frame(frameset, &error);
ob_frame *depth_frame = ob_frameset_depth_frame(frameset, &error);
```

Stop pipeline
```c
ob_pipeline_stop(pipe, &error);
```

Release relevant resources
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

Function Description: Used to demonstrate SDK initialization, get SDK version, get device model, get device serial number, get firmware version number, SDK resource release, and exit the program through the ESC_KEY key
> This example is based on the C++ Low Level API for demonstration

Print the version number of the SDK, which is divided into main version number, minor version number, and revision version number
```cpp
std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;
```

Firstly, it is necessary to create a Context for obtaining a list of device information and creating a device
```cpp
ob::Context ctx;
```

Query the list of connected devices
```cpp
auto devList = ctx.queryDeviceList();

// Get the number of connected devices
if(devList->deviceCount() == 0) {
    std::cerr << "Device not found!" << std::endl;
    return -1;
}
```

Create device
```cpp
//Create a device, where 0 represents the index of the first device
auto dev = devList->getDevice(0);
```

Next, you can get information related to this device: device information, device name, device PID, vid, uid, device firmware version number, and device serial number
```cpp
//Get device information
auto devInfo = dev->getDeviceInfo();

//Get the name of the device
std::cout << "Device name: " << devInfo->name() << std::endl;

//Get the device's pid, vid, uid
std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

//Get the firmware version number of the device
auto fwVer = devInfo->firmwareVersion();
std::cout << "Firmware version: " << fwVer << std::endl;

//Get the serial number of the device
auto sn = devInfo->serialNumber();
std::cout << "Serial number: " << sn << std::endl;

// By getting the connection type of the device
auto connectType = devInfo->connectionType();
```

Get a list of supported sensors
```cpp
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
The console output is as follows：<br />

![image1](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage001.png)

## DepthViewer

Function description: This example mainly demonstrates the use of SDK to get depth data and draw display, get resolution and select settings, display depth images, and exit the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, it is necessary to create a pipeline, through which multiple types of streams can be easily opened and closed, and a set of frame data can be obtained
```cpp
ob::Pipeline pipe;
```
Get all stream configurations of the depth camera, including stream resolution, frame rate, and frame format
```cpp
auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
try{
	//Find the corresponding Profile according to the specified format, with priority given to Y16 format
	depthProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16, 30);
}catch(ob::Error &e){
	//If the Y16 format is not found and does not match the format, search for the corresponding profile to open the stream
	depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}
```

Configure which streams to enable or disable in the Pipeline by creating a Configuration, where deep streams will be enabled
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```

Select sequence ID
```cpp
    ob::SequenceIdFilter sequenceIdFilter;
    sequenceIdFilter.selectSequenceId(0);
```

Start the stream configured in Configuration. If no parameters are passed, the default configuration startup stream will be started
```cpp
pipe.start(config);
```

Wait for a frame of data in a blocking manner, which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```
Stop Pipeline, no more frame data will be generated
```cpp
pipe.stop();
```
The final depth map is shown below<br />![image2](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage002.png)<br />After the program exits normally, resources will be automatically released

## ColorViewer

Function description: This example mainly demonstrates the use of SDK to get color data and draw display, get resolution and select settings, display color images, and exit the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, it is necessary to create a pipeline, through which multiple types of streams can be easily opened and closed, and a set of frame data can be obtained
```cpp
ob::Pipeline pipe;
```
Get all stream configurations of the color camera, including stream resolution, frame rate, and frame format
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
Start the Stream configured in Configuration
```cpp
pipe.start(config);
```

Wait for a frame of data in a blocking manner, which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
auto frameSet = pipe.waitForFrames(100);	//Set the waiting time to 100ms
```
Print metadata every 30 frames
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
Stop Pipeline, no more frame data will be generated
```cpp
pipe.stop();
```
The final color chart is shown below<br />![image3](https://developer-orbbec-oss.oss-cn-shenzhen.aliyuncs.com/images/technical_library/orbbec_sdkimage003.png)<br />After the program exits normally, resources will be automatically released

## SyncAlignViewer

Function description: This example demonstrates the operation of aligning Sensor data stream control, displaying the aligned image, and exiting the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, it is necessary to create a pipeline, through which multiple types of streams can be easily opened and closed, and a set of frame data can be obtained
```cpp
ob::Pipeline pipe;
```
Get all stream configurations for color and depth cameras, including stream resolution, frame rate, and frame format
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
Configure which streams to enable or disable in the Pipeline by creating a Configuration, where color and depth streams will be enabled
```cpp
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(colorProfile);
config->enableStream(depthProfile);
```
Control stream alignment, turn on software alignment here
```cpp
// Configure alignment mode to software D2C alignment
config->setAlignMode(ALIGN_D2C_SW_MODE);
```
Start the stream configured in Configuration. If no parameters are passed, the default configuration startup stream will be started
```cpp
pipe.start(config);
```
Stop Pipeline, no more frame data will be generate
```cpp
pipe.stop();
```
After the program exits normally, resources will be automatically released

## FirmwareUpgrade
Function Description: This example demonstrates how to upgrade a device using firmware
> This example is based on the C++Low Level API for demonstration

Get firmware files through command parameters in the main function interface
```cpp
// The checkFirmwareFilePath() function is used to check if a file exists. In actual code, it is best to check if the suffix is bin or img, and if the firmware file matches the target device
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

    // The following business code

    return 0;
}
catch(ob::Error &e) {
    // Handling OrbbecSDK interface call exceptions. For example, in order to simplify the process of trying to catch multiple interfaces together, it is recommended to use a separate interface and try to catch each other in practical business scenarios
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
```

Create ob:: Context and get the device through ob:: Context. This example assumes that the upper computer (Windows, Ubuntu, Android platforms) has already inserted the device before running FirmwareUpgrade [. exe]. Ob:: DeviceChangedCallback is used to listen for business processing of the upgraded device after firmware upgrade and device restart
```cpp
// Create ob:: Context object
ob::Context ctx;
// Set up a device change listener, device_changed_callback is a key function for managing device declaration cycles, and developers must pay attention to this callback
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

// Query the currently connected devices
auto devList = ctx.queryDeviceList();

// Retrieve the current number of connected devices from ob:: DeviceList
if(devList->deviceCount() == 0) {
     // Firmware upgrade example: The default device has been connected to the upper computer (Windows, Ubuntu, Android platforms)
    std::cerr << "Device not found!" << std::endl;
    return -1;
}

// Get the first device, index=0
auto dev = devList->getDevice(0);
// Print device information
dumpDeviceInfo(dev);
```

Get the current firmware version information of the device
```cpp
// Print device name, SN, VID, PID, and firmware version
void dumpDeviceInfo(std::shared_ptr<ob::Device> device) {
    // Get the ob:: DeviceInfo object, through which the basic information of the target device can be obtained
    auto devInfo = device->getDeviceInfo();

    // Device Name
    std::cout << "Device name: " << devInfo->name() << std::endl;

    // VID, PID, UID of the device
    std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

    // The current firmware version number of the device
    auto fwVer = devInfo->firmwareVersion();
    std::cout << "Firmware version: " << fwVer << std::endl;

    // SN of the device
    auto sn = devInfo->serialNumber();
    std::cout << "Serial number: " << sn << std::endl;

    // The devInfo resource will be automatically released
}
```

Upgrade firmware for the target device
a. Implement firmware upgrade callback interface for C API;
b. Call the firmware upgrade interface for upgrading;
```cpp
// Perform firmware upgrades on the target device
bool upgradeFirmware(std::shared_ptr<ob::Device> device, std::string firmwarePath) {
    auto index     = firmwarePath.find_last_of(".img");
    bool isImgFile = index != std::string::npos;
    index          = firmwarePath.find_last_of(".bin");
    bool isBinFile = index != std::string::npos;
    if(!(isImgFile || isBinFile)) {
        // Firmware upgrade files are usually bin or img. In actual business, it is best to use information such as file name and file MD5 to prevent errors
        std::cout << "Upgrade Fimware failed. invalid firmware file: " << firmwarePath << std::endl;
        return false;
    }

    bool isUpgradeSuccess = false;
    try {
        // Call the firmware upgrade interface for upgrading;
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
                    // Firmware upgrade successful
                    std::cout << "Upgrade Firmware done, percent: " << (uint32_t)percent << std::endl;
                    isUpgradeSuccess = true;
                }
                else if(state == STAT_VERIFY_IMAGE) {
                    std::cout << "Upgrade Firmware verify image" << std::endl;
                }
                else {
                    // Firmware upgrade failed
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

After a successful firmware upgrade, the device needs to be restarted. There are two ways to restart a device: one is to unplug and unplug the device (or restart the operating system), and the other is to call the reboot interface of the OrbbecSDK. After the device goes online, the dump_device_info() function in this example can be used to query the current firmware version information of the device

The following example demonstrates restarting a device through the firmware reboot interface
```cpp
// reboot device
std::cout << "Reboot device" << std::endl;
isDeviceRemoved_      = false;
isWaitRebootComplete_ = true;
dev->reboot();
// Release resources. After the device restarts, the ob:: Device object becomes unusable
dev     = nullptr;
```

After the program exits normally, resources will be automatically released

## MultiStream
Function description: This example mainly demonstrates the operation of opening multiple sensor streams simultaneously using device, displaying multiple stream images, and exiting the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, you need to create a pipeline
```cpp
ob::Pipeline pipe;
```

Enumerate devices and perform stream configuratio
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

Enable pipeline through configuration
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

The IMU frame rate is much faster than the video, so it is advisable to use a separate pipeline to get IMU data.
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

Close pipeline
```cpp
pipe.stop();
```

Close IMU pipeline
```cpp
imuPipeline->stop();
```

## MultiDevice

Function description: This example mainly demonstrates the operation of multiple devices.
> This example is based on the C++High Level API for demonstration

Firstly, it is necessary to create a Context to get a list of device information for creating devices and pipelines
```cpp
ob::Context ctx;
// Query the list of connected devices
auto devList = ctx.queryDeviceList();
// Get the number of connected devices
int devCount = devList->deviceCount();
```
Create a device and create a pipeline through the device
```cpp
// Traverse device list and create devices
std::vector<std::shared_ptr<ob::Device>> devices;
for (int i = 0; i < devCount; i++)
{
    // Get the device and create a pipeline
	auto dev  = devList->getDevice( i );
	auto pipe = std::make_shared< ob::Pipeline >( dev );
	pipes.push_back( pipe );
}
```
Open depth and color streams for multiple devices
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
Stop all open streams on devices
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
        // stop pipeline
        pipe->stop();
        i++;
    }
}
```
After the program exits normally, resources will be automatically released

## HotPlugin

Function description: This example mainly demonstrates the setting of device unplugging callback, as well as the processing of the obtained stream after unplugging, and exiting the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, it is necessary to create a Context for obtaining a list of device information and creating a device
```cpp
ob::Context ctx;
```
Register device callback and execute relevant functions during device unplugging and unplugging
```cpp
ctx.setDeviceChangedCallback( []( std::shared_ptr< ob::DeviceList > removedList, std::shared_ptr< ob::DeviceList > addedList ) {
        DeviceDisconnectCallback( removedList );
        DeviceConnectCallback( addedList );
    } );
```

Enable streaming
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


Stop streaming
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
After the program exits normally, resources will be automatically released

## PointCloud

Function description: Connect the device to open the stream, generate a deep point cloud or RGBD point cloud and save it as a ply format file, and exit the program through the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Create a point cloud and save it as a ply file function. The detailed description of the ply file format can be viewed on the network<br />Firstly, create two functions to save the point cloud data obtained from the stream, which are functions for saving regular point cloud data
```cpp
// Save point cloud data to ply
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
Create another function to save color point cloud data for storing color point cloud data
```cpp
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
Set the Log level to avoid excessive Info level logs affecting the output results of point clouds
```cpp
ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_ERROR);
```
Create a pipeline that makes it easy to open and close multiple types of streams and get a set of frame data
```cpp
ob::Pipeline pipeline;
```
Configure color stream
```cpp
auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
if(colorProfiles) {
    auto profile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
    colorProfile = profile->as<ob::VideoStreamProfile>();
}
config->enableStream(colorProfile);
```

Configure deep stream
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

Enable D2C alignment to generate RGBD point clouds
```cpp
config->setAlignMode(ALIGN_D2C_HW_MODE);
```

Start Pipeline
```cpp
pipeline.start( config );
```

Create a point cloud Filter object and set camera intrinsic parameters
```cpp
// Create a point cloud filter object (when creating a point cloud filter, device parameters are obtained within the Pipeline, so it is recommended to configure the device as much as possible before creating the filter)
ob::PointCloudFilter pointCloud;

// Get camera internal parameters and input them into the point cloud filter
auto cameraParam = pipeline.getCameraParam();
pointCloud.setCameraParam(cameraParam);
```

Set some operation prompts
```cpp
 std::cout << "Press R to create rgbd pointCloud and save to ply file! " << std::endl;
 std::cout << "Press d to create depth pointCloud and save to ply file! " << std::endl;
 std::cout << "Press ESC to exit! " << std::endl;
```
Set the main process to get and save point cloud data through the point cloud Filter object created above
```cpp
if(key == 'R' || key == 'r') {
  count = 0;
  // Limit up to 10 repetitions
  while(count++ < 10) {
    // Waiting for one frame of data with a timeout of 100ms
    auto frameset = pipeline.waitForFrames(100);
    if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
      try {
        // Generate colored point clouds and save them
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
  // Limit up to 10 repetitions
  while(count++ < 10) {
    // Waiting for one frame of data with a timeout of 100ms
    auto frameset = pipeline.waitForFrames(100);
    if(frameset != nullptr && frameset->depthFrame() != nullptr) {
      try {
        // Generate point cloud and save
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
Finally, stop the stream through the Pipeline
```cpp
 pipeline.stop();
```
After the program exits normally, resources will be released

## SaveToDisk

Function description: Connect the device to open the stream, get color and depth maps, and store them in PNG format
> This example is based on the C++High Level API for demonstration

Create two functions to save the obtained image to a file
```cpp
// Save depth map in PNG format
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

// Save color images in PNG format
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
Create a pipeline that makes it easy to open and close multiple types of streams and get a set of frame data
```cpp
ob::Pipeline pipeline;
```
Then, all configurations of color and depth streams can be obtained through the Pipeline, including the resolution, frame rate, and format of the stream, as well as the required color and depth stream configurations
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
Enable Stream
```cpp
pipeline.start( config );
```
Get frame data through waiting
```cpp
auto frameset = pipeline.waitForFrames( 100 );
```
Get data for a single type of frame
```cpp
auto colorFrame = frameset->colorFrame();
auto depthFrame = frameset->depthFrame();
```
Create a format conversion filter to convert color images to RGB format and save them
```cpp
// Create Format Conversion Filter
ob::FormatConvertFilter formatConverFilter;

formatConverFilter.setFormatConvertType(FORMAT_MJPEG_TO_RGB888);
colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
formatConverFilter.setFormatConvertType(FORMAT_RGB_TO_BGR);
colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
```
Store the obtained data through the storage function edited at the beginning
```cpp
saveColor( colorFrame );
saveDepth( depthFrame );
```
Stop Pipeline
```cpp
pipeline.stop();
```
After the program exits normally, resources will be released

## Recorder

Function description: Connect the device to start streaming, record the current video stream to a file, and exit the program with the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, it is necessary to create a pipeline, through which multiple types of streams can be easily opened and closed, and a set of frame data can be obtained
```cpp
ob::Pipeline pipe;
```
Get the stream configuration of the depth camera
```cpp
depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
```
Configure which streams to enable or disable in the Pipeline by creating a Configuration, where deep streams will be enabled
```cpp
// Configure the stream types opened by Pipeline
std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
config->enableStream(depthProfile);
```
Start the stream configured in Configuration. If no parameters are passed, the default configuration start stream will be activated, and the recording switch will be activated
```cpp
pipe.start(config);
pipe.startRecord("./OrbbecPipeline.bag");
```
Wait for a frame of data in a blocking manner, which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
auto frameSet = pipe.waitForFrames(100);	// Set the waiting time to 100ms
```
Stop Pipeline, no more frame data will be generated
```cpp
pipe.stop();
```
After the program exits normally, resources will be released
## Playback

Function description: Connect the device to start streaming, load video files for playback, and exit the program with the ESC_KEY key
> This example is based on the C++High Level API for demonstration

Firstly, you need to create a Pipeline for playing back files
```cpp
// Create a pipeline object for playback
ob::Pipeline pipe("./OrbbecPipeline.bag");
```
Retrieve the playback status callback of the playback object
```cpp
// Get playback object, set playback status callback
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
Reading device information and internal parameter information from playback files
```cpp
// Reading device information from playback files
auto deviceInfo = playback->getDeviceInfo();
std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
  << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;

// Reading internal parameter information from playback files
auto cameraParam = pipe.getCameraParam();
std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
              << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;
```
Enable playback
```cpp
//Enable playback
pipe.start(NULL);
```
Wait for a frame of data in a blocking manner, which is a composite frame containing frame data for all streams enabled in the configuration, and set the waiting timeout time for the frame
```cpp
auto frameSet = pipe.waitForFrames(100);	// Set the waiting time to 100ms
```
Stop Pipeline, no more frame data will be generated
```cpp
pipe.stop();
```
After the program exits normally, resources will be released

## DepthUnitControl

Function description: Demonstrate the operation of obtaining depth accuracy, display depth images, output depth accuracy, and exit the program through the ESC_KEY key

>This example is based on the C++High Level API for demonstration

Create a pipeline and stream configuration, and get devices through the pipeline
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Get the device inside the pipeline
auto device = pipe.getDevice();
```

Check the way to get depth accuracy
```cpp
bool changeDepthUnitFlag = false;
// check the way to adjust depth unit
if(device->isPropertySupported(OB_PROP_DEPTH_PRECISION_LEVEL_INT, OB_PERMISSION_READ_WRITE)) {
    // change depth unit by adjust the depth precision level
    changeDepthUnitFlag = changeDepthUnit(device);
}
else if(device->isPropertySupported(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT, OB_PERMISSION_READ_WRITE)) {
    // for some models (Such as Gemini 330 series), user can adjust the depth unit by adjusting the depth unit flexible adjustment property
    changeDepthUnitFlag = changeDepthUnitFlexible(device);
}
else {
    std::cout << "Current device not support adjust depth unit, use the default depth unit 1mm" << std::endl;
    changeDepthUnitFlag = true;
}
```

Calculate depth value
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

Release resources and exit the program
```cpp
    pipe.stop();
```

## HdrMerge

Supported devices: Gemini 330 series cameras.

Function description: Demonstrate using HDR operation, display HDR processed images, and exit the program with ESC_KEY key

>This example is based on the C++High Level API for demonstration

Create a pipeline and configure the stream
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

Open HDR processing
```cpp
// Create Hdrmerge post processor
ob::HdrMerge hdrMerge;

// Open hdr merge
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

Open pipeline
```cpp
pipe.start(config);
```

Get HDR processed images
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

Close pipeline
```cpp
pipe.stop();
```

Turn off HDR processing
```cpp
if(pipe.getDevice()->isPropertySupported(OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE)) {
    OBHdrConfig obHdrConfig;
    uint32_t    dataSize = sizeof(OBHdrConfig);
    pipe.getDevice()->getStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, &dataSize);
    obHdrConfig.enable = false;
    pipe.getDevice()->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));
}
```

## DepthWorkMode

Supported devices: Gemini 2 series, Gemini 330 series cameras, Astra 2 series cameras

Function description: Demonstrate the operation of converting deep working mode

>This example is based on the C++High Level API for demonstration

Create a pipeline to get devic
```cpp
// Create a pipeline with default device
ob::Pipeline pipe;

// Get the device inside the pipeline
auto device = pipe.getDevice();
```

Check if work mode conversion is supported
```cpp
// Check whether the camera depth working mode is supported, currently (December 5, 2022) only the Gemini2 binocular camera supports the depth working mode
if(!device->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE)) {
    pressKeyExit("Current device not support depth work mode!");
    return -1;
}
```

Get a list of deep work modes
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

Set deep working mode
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

## DoubleInfraredViewer

Supported devices: OpenNI protocol cameras such as Dabia series, Astra/Astra 2 series, Femto series cameras

Function description: Demonstrate obtaining dual infrared camera images, displaying left and right infrared images, and exiting the program with the ESC_KEY key

>This example is based on the C++High Level API for demonstration

Create a pipeline to get devices
```cpp
    ob::Pipeline pipe;
```

Configure Left IR
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

Configure Right IR
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

Open pipeline
```cpp
pipe.start(config);
```

Get frame data
```cpp
auto leftFrame  = frameSet->getFrame(OB_FRAME_IR_LEFT);
auto rightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
```

Stop pipeline
```cpp
pipe.stop();
```

## InfraredViewer
Function description: Demonstrate obtaining infrared camera images, displaying infrared images, and exiting the program with the ESC_KEY key

>This example is based on the C++High Level API for demonstration

Create a pipeline and configure stream information
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

Enable pipeline
```cpp
pipe.start(config);
```

Get frame data
```cpp
auto frameSet = pipe.waitForFrames(100);
```

Stop pipeline
```cpp
pipe.stop();
```

## ImuReader
Function description: Demonstration of obtaining IMU data

>This example is based on the C++low level API for demonstration, outputting imu data and exiting the program using the ESC_KEY key

Get device
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

Get gyroscope data
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

Get accelerometer
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
// turn off the stream
if(gyroSensor) {
    gyroSensor->stop();
}
if(accelSensor) {
    accelSensor->stop();
}
```

## MultiDeviceSync
Function description: Demonstrate multi device synchronization operation

>This example is based on the C++low level API for demonstration

Configure program
```cpp
std::cout << "Please select options: " << std::endl;
std::cout << " 0 --> config devices" << std::endl;
std::cout << " 1 --> start stream" << std::endl;
std::cout << "input: ";
int index = -1;
std::cin >> index;
```

Configure multi device synchronization
```cpp
exitValue = configMultiDeviceSync();
```

Distinguishing master-slave devices
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

Enable slave devices
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

Turn on the main device
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

Set the synchronization interval time for multiple device
```cpp
// Start the multi-device time synchronization function
context.enableDeviceClockSync(3600000);  // update and sync every hour
```

Conduct multi device testing
```cpp
testMultiDeviceSync();
```

Close data stream
```cpp
// close data stream
for(auto itr = pipelineHolderList.begin(); itr != pipelineHolderList.end(); itr++) {
    stopStream(*itr);
}
```

## PostProcessing

Supported devices: Gemini 330 series cameras.

Function description: Demonstrate post-processing operations, display post-processed images, and exit the program using the ESC_KEY key

>This example is based on the C++High Level API for demonstration

Get the pipeline and configure the stream
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

Get a list of deep post-processing filters
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

Open pipeline
```cpp
pipe.start(config);
```


## SensorControl

Function description: Demonstrate sensor control operations

>This example is based on the C++low level API for demonstration

Get device list
```cpp
// Create a Context.
ob::Context context;

// Query the list of connected devices
auto deviceList = context.queryDeviceList();
```

Get device and output device information
```cpp
if(deviceList->deviceCount() > 0) {
    if(deviceList->deviceCount() <= 1) {
        // If a single device is plugged in, the first one is selected by default
        device = deviceList->getDevice(0);
    }
    else {
        device = selectDevice(deviceList);
    }
    auto deviceInfo = device->getDeviceInfo();
    std::cout << "\n------------------------------------------------------------------------\n";
    std::cout << "Current Device: "
                << " name: " << deviceInfo->name() << ", vid: 0x" << std::hex << deviceInfo->vid() << ", pid: 0x" << std::setw(4) << std::setfill('0')
                << deviceInfo->pid() << ", uid: 0x" << deviceInfo->uid() << std::dec << std::endl;
}
```

Follow the prompts to get all properties
```cpp
std::cout << "Input \"?\" to get all properties." << std::endl;
std::getline(std::cin, choice);
```

Input parameter prompt
```cpp
std::cout << "Property control usage: [property index] [set] [property value] or [property index] [get]" << std::endl;
```

Get property
```cpp
getPropertyValue(device, propertyItem);
```

Set property
```cpp
setPropertyValue(device, propertyItem, controlVec.at(2));
```


## AlignFilterViewer

Supported devices: Gemini 330 series cameras.

Function description: Demonstrate the synchronization and alignment of sensor data streams, display the aligned image, and exit the program using the ESC_KEY key

>This example is based on the C++high level API for demonstration

Create pipeline
```cpp
ob::Pipeline pipe;
```

Enable color stream
```cpp
auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
if(colorProfiles) {
    colorProfile = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30);
}
config->enableStream(colorProfile);
```

Enable deep stream
```cpp
auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
if(depthProfiles) {
depthProfile = depthProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
//depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
}
config->enableStream(depthProfile);
```

Set alignment mode
```cpp
/* Config depth align to color or color align to depth.
OBStreamType align_to_stream = OB_STREAM_DEPTH; */
OBStreamType align_to_stream = OB_STREAM_COLOR;
ob::Align align(align_to_stream);
```

Open pipeline
```cpp
pipe.start(config);
```

Get frame data
```cpp
auto colorFrame = frameSet->colorFrame();
auto depthFrame = frameSet->depthFrame();
```

Perform alignment processing
```cpp
aif(align_to_stream == OB_STREAM_COLOR) {
    app.resize(colorFrame->width(), colorFrame->height());
}
else {
    app.resize(depthFrame->width(), depthFrame->height());
}
```

Close pipeline
```cpp
pipe.stop();
```