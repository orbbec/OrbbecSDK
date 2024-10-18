获取所有示例都可以在工程的Examples目录中找到

+----------------+----------+----------------------------------------+
| **名称**       | **语言** | **描述**                               |
+================+==========+========================================+
| HelloOrbbec    | C        | 演示连接到设备获取SDK版本和设备信息    |
+----------------+----------+----------------------------------------+
| 传感器控制示例 | C        | 演示对设备、传感器控制命令的操作       |
+----------------+----------+----------------------------------------+
| 彩色和深度示例 | C        | 演示使用Pipeline高级接口来             |
|                |          | 操作设备，获取分辨率列表并选择进行设置 |
+----------------+----------+----------------------------------------+
| 热拔插示例     | C        | 演示设备热拔                           |
|                |          | 插监控，检测到设备上线自动连接设备打开 |
|                |          | 深度流，检测到设备掉线自动断开设备连接 |
+----------------+----------+----------------------------------------+
| 点云示例       | C        | 演示生成                               |
|                |          | 深度点云或RGBD点云并保存成ply格式文件  |
+----------------+----------+----------------------------------------+
| HelloOrbbec    | C++      | 演示连接到设备获取SDK版本和设备信息    |
+----------------+----------+----------------------------------------+
| 深度示例       | C++      | 演示使用SDK获取深度数据并绘制显        |
|                |          | 示、获取分辨率并进行设置、显示深度图像 |
+----------------+----------+----------------------------------------+
| 彩色示例       | C++      | 演示使用SDK获取彩色数据并绘制显        |
|                |          | 示、获取分辨率并进行设置、显示彩色图像 |
+----------------+----------+----------------------------------------+
| 红外示例       | C++      | 演示使用SDK获取红外数据并绘制显        |
|                |          | 示、获取分辨率并进行设置、显示红外图像 |
+----------------+----------+----------------------------------------+
| 流对齐示例     | C++      | 演示对传感器数据流对齐的操作           |
+----------------+----------+----------------------------------------+
| 传感器控制示例 | C++      | 演示对设备和传感器控制命令的操作       |
+----------------+----------+----------------------------------------+
| 多设备示例     | C++      | 演示对多设备进行操作                   |
+----------------+----------+----------------------------------------+
| 热拔插示例     | C++      | 演示设备                               |
|                |          | 拔插回调的设置，并获取到插拔后处理的流 |
+----------------+----------+----------------------------------------+
| IMU示例        | C++      | 获取IMU数据并输出显示                  |
+----------------+----------+----------------------------------------+
| 点云示例       | C++      | 演示生成                               |
|                |          | 深度点云或RGBD点云并保存成ply格式文件  |
+----------------+----------+----------------------------------------+
| 存储示例       | C++      | 获取彩色和深度图并存储为png格式        |
+----------------+----------+----------------------------------------+
| 录制示例       | C++      | 录制当前视频流到文件                   |
+----------------+----------+----------------------------------------+
| 回放示例       | C++      | 载入视频文件进行回放                   |
+----------------+----------+----------------------------------------+

C
=

HelloOrbbec
-----------

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源。

   本示例基于C Low Level API进行演示

首先获取并打印当前SDK版本

.. code:: c

   printf("SDK version: %d.%d.%d\n", ob_get_major_version(), ob_get_minor_version(), ob_get_patch_version());

创建一个Context，用于获取设备信息列表和创建设备

.. code:: c

   ob_error*   error = NULL;
   ob_context* ctx   = ob_create_context( &error );

查询已经接入设备的列表，从而获取列表内设备数量，并通过索引号0创建获取列表内第一个设备

.. code:: c

   ob_device_list* dev_list = ob_query_device_list( ctx, &error );
   int count = ob_device_list_device_count(dev_list, &error);
   ob_device* dev = ob_get_device(dev_list, 0, &error);

接下来就可以获取和这个设备相关的信息了

.. code:: c

   //获取设备信息
   ob_device_info* dev_info = ob_device_get_device_info(dev, &error);

   //获取设备名称
   const char* name = ob_device_info_name(dev_info, &error);

   //获取设备的pid, vid, uid
   int pid = ob_device_info_pid(dev_info, &error);
   int vid = ob_device_info_vid(dev_info, &error);
   int uid = ob_device_info_uid(dev_info, &error);

   //通过获取设备的固件版本号
   const char* fw_ver = ob_device_info_firmware_version(dev_info, &error);

   //通过获取设备的序列号
   const char* sn = ob_device_info_serial_number(dev_info, &error);

   //获取支持的传感器列表
   ob_sensor_list* sensor_list = ob_device_get_sensor_list(dev, &count, &error);
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

释放资源，退出程序。

.. code:: c

   ob_delete_sensor_list(sensor_list, &error); //销毁sensor list
   ob_delete_device_info(dev_info, &error); //销毁device info
   ob_delete_device(dev, &error); //销毁device
   ob_delete_device_list(dev_list, &error); //销毁device list
   ob_delete_context(ctx, &error); //销毁context

传感器控制示例-SensorControl
----------------------------

功能描述：本示例主要演示了对device控制命令的操作、对Sensor控制命令的操作、对Sensor进行流操作。

   本示例基于C Low Level API进行演示

创建一个Context，并查询已经接入设备的列表

.. code:: c

   ob_error*   error = NULL;
   ob_context* ctx   = ob_create_context( &error );
   ob_device_list* dev_list = ob_query_device_list( ctx, &error );

通过索引号打印列表内设备信息

.. code:: c

   const char *name = ob_device_list_get_device_name(device_list, i, &g_error);
   int pid = ob_device_list_get_device_pid(device_list, i, &g_error);
   int vid = ob_device_list_get_device_vid(device_list, i, &g_error);
   const char *uid = ob_device_list_get_device_uid(device_list, i, &g_error);
   const char *sn = ob_device_list_get_device_serial_number(device_list, i, &g_error);
   printf("%d. name: %s, pid: %d, vid: %d, uid: %s, sn: %s\n", i, name, pid, vid, uid, sn);

创建一个设备

.. code:: c

   if(devCount <= 1) {
       // 如果插入单个设备，默认选择第一个
        device = ob_device_list_get_device(dev_list, 0, &g_error);
   }
   else {
       // 如果有多个设备，者有用户输入选择
       device = select_device(dev_list);  // select_device 代码请参考实例源码
   }

获取并打印当前创建的设备信息

.. code:: c

    ob_device_info *deviceInfo = ob_device_get_device_info(device, &g_error);
    const char *name = ob_device_info_name(deviceInfo, &g_error);
    int pid = ob_device_info_pid(deviceInfo, &g_error);
    int vid = ob_device_info_vid(deviceInfo, &g_error);
    const char *uid = ob_device_info_uid(deviceInfo, &g_error);
    printf("Current Device: name: %s, pid: %d, vid: %d, uid: %s\n", name, pid, vid, uid);

获取支持的控制命令属性

.. code:: c

   // 获取支持控制命令属性个数
   uint32_t propertySize = ob_device_get_supported_property_count(device, &g_error);
   // 通过索引号i获取控制命令属性项
   ob_property_item property_item = ob_device_get_supported_property(device, i, &g_error);

读/写控制命令

.. code:: c

   // 读取
   bool_ret = ob_device_get_bool_property(device, property_item.id, &g_error);// bool型参数
   int_ret = ob_device_get_int_property(device, property_item.id, &g_error);/ int 型参数
   float_ret = ob_device_get_float_property(device, property_item.id, &g_error);// float型参数

   // 读取命令的值范围、默认值、步进值等信息
   ob_int_property_range   int_range;
   ob_float_property_range float_range;
   ob_bool_property_range  bool_range;
   sprintf(str, "Bool value(min:0, max:1, step:1)"); // bool 型
   int_range = ob_device_get_int_property_range(device, property_item.id, &g_error); // int 型
   float_range = ob_device_get_float_property_range(device, property_item.id, &g_error); // float型

   // 写入
   ob_device_set_bool_property(device, property_item.id, bool_value, &g_error); // bool型参数
   ob_device_set_int_property(device, property_item.id, int_value, &g_error); // int 型参数
   ob_device_set_float_property(device, property_item.id, float_value, &g_error); // float型参数

最后程序退出时需要释放相应的已创建资源。

.. code:: c

   //销毁context
   ob_delete_context(ctx, &g_error);
   //销毁device list
   ob_delete_device_list(dev_list, &g_error);
   //销毁device
   ob_delete_device(device, &g_error);

彩色和深度示例-ColorDepthViewer
-------------------------------

功能描述：本示例主要演示了使用Pipeline高级接口来操作设备，获取分辨率列表并选择进行设置。

   本示例基于C High Level API进行演示

首先需要创建Pipeline，用于连接设备后打开彩色和深度流

.. code:: c

   pipe = ob_create_pipeline( &error );

创建Config，用于配置彩色和深度流的分辨率、帧率、格式

.. code:: c

   ob_config* config = ob_create_config( &error );

配置流

.. code:: c

   // 2.1 配置Depth流
   ob_stream_profile *     depth_profile = NULL;
   ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_DEPTH, &error);
   //根据指定的格式查找对应的Profile
   depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_Y16, 30, &error);
   if(!depth_profile) {  // 如果没找到指定格式profile，使用profile列表中第一个profile
        depth_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
   }
   ob_config_enable_stream(config, depth_profile, &error);  // 使能配置

   // 2.2 配置Color流
   ob_stream_profile *color_profile = NULL;
   profiles                         = ob_pipeline_get_stream_profile_list(pipe, OB_SENSOR_COLOR, &error);
   //根据指定的格式查找对应的Profile
   color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_MJPG, 30, &error);
   if(!color_profile) {  // 如果没找到指定格式profile，使用profile列表中第一个profile
       color_profile = ob_stream_profile_list_get_profile(profiles, 0, &error);
   }
   ob_config_enable_stream(config, color_profile, &error);  // 使能配置

帧通过功能设置

.. code:: c

   ob_pipeline_disable_frame_sync(pipe, &error); // 关闭帧同步功能

通过Config，启动Pipeline

.. code:: c

   ob_pipeline_start_with_config(pipe, config, &error);

Pipeline停止与销毁

.. code:: c

   // 停止 pipeline
   ob_pipeline_stop(pipe, &error);

   // 销毁 pipeline
   ob_delete_pipeline(pipe, &error);

热拔插示例-HotPlugin
--------------------

功能描述：
设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接。

   本示例基于C Low Level API进行注册设备上下线回调，基于High Level
   API进行开流取帧演示

创建设备连接处理函数，函数内创建pipeline并调用create_and_start_with_config开启彩色流和深度流。

.. code:: c

   //设备连接处理
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

创建设备断开处理函数，函数内将pipeline停止，并将退出pipeline标记isExit设为true

.. code:: c

   //设备断开处理
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

设备连接、断开处理函数将会在设备上下线回调函数内被调用

.. code:: c

   //设备状态改变回调
   void on_device_changed_callback( ob_device_list* removed, ob_device_list* added, void* pCallback ) {
       device_disconnect_callback( removed );
       device_connect_callback( added );
   }

主函数main内，首先需要创建一个Context并注册设备上下线回调

.. code:: c

    //创建上下文
   ob_context* ctx = ob_create_context( &error );

   //注册设备回调
   ob_set_device_changed_callback( ctx, on_device_changed_callback, NULL, &error );

主循环主要任务是，当pipeline在设备上线后已创建启动，从pipeline获取数据帧集合，并将数据帧集合内的Color和Depth相关信息打印输出。

.. code:: c

   //等待一帧数据，超时时间为100ms
   ob_frame* frameset = ob_pipeline_wait_for_frameset( pipeline, 100, &error );
   if ( frameset ) {
       //获取深度数据帧
       ob_frame* depth_frame = ob_frameset_depth_frame( frameset, &error );
       if ( depth_frame ) {
           printf( "=====Depth Frame Info======Index: %lld TimeStamp: %lld\n", ob_frame_index( depth_frame, &error ), ob_frame_time_stamp( depth_frame, &error ) );
           //释放深度数据帧
           ob_delete_frame( depth_frame, &error );
       }
       //获取Color数据帧
       ob_frame* color_frame = ob_frameset_color_frame( frameset, &error );
       if ( color_frame ) {
           printf( "=====Color Frame Info======Index: %lld TimeStamp: %lld\n", ob_frame_index( color_frame, &error ), ob_frame_time_stamp( color_frame, &error ) );
           //释放Color数据帧
           ob_delete_frame( color_frame, &error );
       }
       //释放frameSet
       ob_delete_frame( frameset, &error );
   }

退出主循环后，需要将相关资源释放

.. code:: c

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

点云实例-PointCloud
-------------------

功能描述：连接设备开流，生成深度点云或RGBD点云并保存成ply格式文件。

   本示例基于C++ High Level API进行演示

创建点云保存成ply格式文件函数，ply文件格式详细描述可在网络上查看

.. code:: c

   //保存点云数据到ply
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

   //保存彩色点云数据到ply
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

创建Pipeline及流配置

.. code:: c

       //创建pipeline 用于连接设备后打开Color和Depth流
       pipeline = ob_create_pipeline( &error );
       //创建config，用于配置 Color 和 Depth 流的 分辨率、帧率、格式
       ob_config* config = ob_create_config( &error );

        //配置Depth流
       ob_stream_profile *     depth_profile = NULL;
       ob_stream_profile_list *profiles      = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_DEPTH, &error);
       //根据指定的格式查找对应的Profile
       depth_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_Y16, 30, &error);
       ob_config_enable_stream(config, depth_profile, &error);  // 使能配置

       //配置Color流
       ob_stream_profile *color_profile = NULL;
       profiles                         = ob_pipeline_get_stream_profile_list(pipeline, OB_SENSOR_COLOR, &error);
       //根据指定的格式查找对应的Profile
       color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_YUYV, 0, &error);
       //如果没找到YUYV，查找I420格式
       if(!color_profile) {
           color_profile = ob_stream_profile_list_get_video_stream_profile(profiles, 640, 480, OB_FORMAT_I420, 0, &error);
           check_error(error);
       }
       ob_config_enable_stream( config, color_profile, &error );  // 使能配置

从Pipeline上获取当前打开的设备，用以打开设备D2C功能

.. code:: c

       //获取device句柄
       ob_device *device = ob_pipeline_get_device(pipeline, &error);
       //判断设备是否支持硬件D2C对齐功能，如果支持则开启
       if(device && ob_device_is_property_supported(device, OB_PROP_DEPTH_ALIGN_HARDWARE_BOOL, OB_PERMISSION_WRITE, &error)) {
           // 开启硬件D2C对齐
           ob_device_set_bool_property(device, OB_PROP_DEPTH_ALIGN_HARDWARE_BOOL, true, &error);
       }

开流，并在开流后创建点云filter。点云filter用于将depth和color帧数据转换成点云数据。点云filter在开流后创建目的是让SDK内部自动根据当前开流配置设置好相机参数。当然也可以自行设置。

.. code:: c

       // 创建点云Filter对象（点云Filter创建时会在Pipeline内部获取设备参数, 所以尽量在Filter创建前配置好设备）
       ob_filter* point_cloud = ob_pipeline_create_pointcloud_filter( pipeline, &error );
       // 自行设置filter参数
       if ( device && ob_device_is_property_supported( device, OB_DATA_TYPE_CAMERA_PARA, &error ) ) {
           CAMERA_PARA cameraParam = {0};
           uint32_t    len;
           ob_device_get_structured_data( device, OB_DATA_TYPE_CAMERA_PARA, &cameraParam,&len, &error );
           ob_pointcloud_filter_set_camera_parameter(point_cloud,cameraParam,error);
       }

启动主循环，循环内根据用户按键调用点云filter生成深度点云或RGBD点云数据，并保存成ply文件。

.. code:: c

   //等待一帧数据，超时时间为100ms
   ob_frame* frameset = ob_pipeline_wait_for_frames( pipeline, 100, &error );
   if ( frameset != NULL )
   {
       //按R键保存ply数据
       if ( ( key == 'R' || key == 'r' ) && frameset != NULL ) {
           ob_pointcloud_filter_set_point_format( point_cloud, OB_FORMAT_RGB_POINT, &error );
           ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
           save_rgb_points_to_ply( pointsFrame, "rgb_points.ply" );
           ob_delete_frame( pointsFrame, &error );
       }
       else if ( ( key == 'D' || key == 'd' ) && frameset != NULL ) {
           //生成点云并保存
           ob_pointcloud_filter_set_point_format( point_cloud, OB_FORMAT_POINT, &error );
           ob_frame *pointsFrame = ob_filter_process(point_cloud, frameset, &error);
           save_points_to_ply( pointsFrame, "points.ply" );
           ob_delete_frame( pointsFrame, &error );
       }
       ob_delete_frame( frameset, &error );  // 销毁frameSet 回收内存
   }

主循环退出后，停流并销毁回收资源

.. code:: c

       // 停止pipeline
       ob_pipeline_stop( pipeline, &error );

       // 销毁pipeline
       ob_delete_pipeline( pipeline, &error );

       ob_delete_context( ctx, &error );

.. _c-1:

C++
===

.. _helloorbbec-1:

HelloOrbbec
-----------

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源。

   本示例基于C++ Low Level API进行演示

打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号

.. code:: cpp

   std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: cpp

   ob::Context ctx;

查询已经接入设备的列表

.. code:: cpp

   auto devList = ctx.queryDeviceList();

   //获取接入设备的数量
   devList->deviceCount()

创建设备

.. code:: cpp

   //创建设备，0表示第一个设备的索引
   auto dev = devList->getDevice(0);

接下来就可以获取和这个设备相关的信息：设备信息、设备的名称、获取设备的pid,
vid, uid、设备的固件版本号、设备的序列号

.. code:: cpp

   //获取设备信息
   auto devInfo = dev->getDeviceInfo();

   //获取设备的名称
   std::cout << "Device name: " << devInfo->name() << std::endl;

   //获取设备的pid, vid, uid
   std::cout << "Device pid: " << devInfo->pid() << " vid: " << devInfo->vid() << " uid: " << devInfo->uid() << std::endl;

   //获取设备的固件版本号
   auto fwVer = devInfo->firmwareVersion();
   std::cout << "Firmware version: " << fwVer << std::endl;

   //获取设备的序列号
   auto sn = devInfo->serialNumber();
   std::cout << "Serial number: " << sn << std::endl;

获取支持的传感器列表

::

   //获取支持的传感器列表
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

控制台结果输出如下：\ |image.png|

深度示例-DepthViewer
--------------------

功能描述：本示例主要演示了使用SDK获取深度数据并绘制显示、获取分辨率并选择进行设置、显示深度图像，并通过ESC键退出。

   本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

.. code:: cpp

   ob::Pipeline pipe;

获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式

.. code:: cpp

   auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

   //通过遍历深度流的配置来获取要使用的帧格式，这里将使用Y16格式来打开流
   std::shared_ptr<ob::StreamProfile> depthProfile;
       for (int i = 0; i < profiles->count(); i++)
       {
           auto profile = profiles->getProfile(i);
           if(profile->format() == OB_FORMAT_Y16){
               depthProfile = profile;
               break;
           }
       }

   //也可以通过设置感兴趣项，返回对应Profile列表的首个Profile
   // auto depthProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);

   //如果没找到合适的流配置，打开列表中的首个配置
   if(!depthProfile) {
       depthProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用深度流

.. code:: cpp

   std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
   config->enableStream(depthProfile);

启动在Config中配置的流，如果不传参数，将启动默认配置启动流

.. code:: cpp

   pipe.start(config);

设置镜像模式，先判断设备是否有可读可写的权限，再进行设置

.. code:: cpp

   //获取镜像属性是否有可写的权限
   if(pipe.getDevice()->isPropertySupported(OB_PROP_DEPTH_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
       //设置镜像
       pipe.getDevice()->setBoolProperty(OB_PROP_DEPTH_MIRROR_BOOL, true);
   }

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间

.. code:: cpp

   auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms

停止Pipeline，将不再产生帧数据

.. code:: cpp

   pipe.stop();

最终的深度图显示如下\ |image.png|\ 程序正常退出之后资源将会自动释放

彩色示例-ColorViewer
--------------------

功能描述：本示例主要演示了使用SDK获取彩色数据并绘制显示、获取分辨率并选择进行设置、显示彩色图像，并通过ESC键退出。

   本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

.. code:: cpp

   ob::Pipeline pipe;

获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式

.. code:: cpp

   auto profiles = pipe.getStreamProfiles(OB_SENSOR_COLOR);

   //通过遍历彩色流的配置来获取要使用的帧格式，这里将使用MJPG格式来打开流
   std::shared_ptr< ob::StreamProfile > colorProfile;
   for (int i = 0; i < profiles->count(); i++) {
       auto profile = profiles->getProfile(i);
       if (profile->format() == OB_FORMAT_MJPG) {
           colorProfile = profile;
           break;
       }
   }

   //也可以通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   // auto colorProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_MJPG);

   //如果没找到合适的流配置，打开列表中的首个配置
   if(!colorProfile) {
       colorProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流

.. code:: cpp

   std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
   config->enableStream(colorProfile);

启动在Config中配置的流

.. code:: cpp

   pipe.start(config);

设置镜像模式，先判断设备是否有可读可写的权限，再进行设置

.. code:: cpp

   //获取镜像属性是否有可写的权限
   if(pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
       //设置镜像
       pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
   }

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间

.. code:: cpp

   auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms

停止Pipeline，将不再产生帧数据

.. code:: cpp

   pipe.stop();

最终的彩色图显示如下\ |1640769142832_A167CC70-8856-4af1-978F-FC28D9CAB32F.png|\ 程序正常退出之后资源将会自动释放

红外示例-InfraredViewer
-----------------------

功能描述：本示例主要演示了使用SDK获取红外数据并绘制显示、获取分辨率并选择进行设置、显示红外图像，并通过ESC键退出。

   本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

.. code:: cpp

   ob::Pipeline pipe;

通过输入的分辨率，格式，帧率等感兴趣项来获取红外相机的流配置

.. code:: cpp

   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto irProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);
   if(!irProfile) {
       irProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用红外流

.. code:: cpp

   std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
   config->enableStream(irProfile);

启动在Config中配置的流，如果不传参数，将启动默认配置启动流

.. code:: cpp

   pipe.start(config);

设置镜像模式，先判断设备是否有可读可写的权限，再进行设置

.. code:: cpp

   //获取镜像属性是否有可写的权限
   if(pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL, OB_PERMISSION_WRITE)) {
       //设置镜像
       pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
   }

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间

::

   auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms

停止Pipeline，将不再产生帧数据

.. code:: cpp

   pipe.stop();

程序正常退出之后资源将会自动释放

流对齐示例-SyncAlignViewer
--------------------------

功能描述：本示例主要演示了对Sensor数据流控制对齐的操作。

   本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

.. code:: cpp

   ob::Pipeline pipe;

获取彩色相机和深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式

.. code:: cpp

   //获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
   auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);

   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto colorProfile = colorProfiles->getVideoStreamProfile(colorWidth, colorHeight, OB_FORMAT_MJPG, 30);
   if(!colorProfile) {
       colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

   //获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
   auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto depthProfile = depthProfiles->getVideoStreamProfile(depthWidth, depthHeight, OB_FORMAT_Y16, 30);
   if(!depthProfile) {
       depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流和深度流

::

   std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
   config->enableStream(colorProfile);
   config->enableStream(depthProfile);

控制流对齐，此处开启软件对齐

::

   // 配置对齐模式为软件D2C对齐
   config->setAlignMode(ALIGN_D2C_SW_MODE);

启动在Config中配置的流，如果不传参数，将启动默认配置启动流

::

   pipe.start(config);

停止Pipeline，将不再产生帧数据

::

   pipe.stop();

程序正常退出之后资源将会自动释放

.. _传感器控制示例-sensorcontrol-1:

传感器控制示例-SensorControl
----------------------------

功能描述：本示例主要演示了对device控制命令的操作、对Sensor控制命令的操作、对Sensor进行流操作。

   本示例基于C++ Low Level API进行演示

首先需要创建一个Context，用于获取设备信息列表和创建设备

::

   ob::Context ctx;

查询设备信息列表

::

   auto devList = ctx.queryDeviceList();

选择一个设备进行操作，如果插入单个设备默认选择并打开，如果存在多个设备提供选择

::

   //选择一个设备进行操作
   std::shared_ptr<ob::Device> device = nullptr;
   if(deviceList->deviceCount() > 0) {
       if(deviceList->deviceCount() <= 1) {
       //如果插入单个设备，默认选择第一个
           device = deviceList->getDevice(0);
       }
       else {
           device = selectDevice(deviceList);
       }
   }

控制命令控制，获取Property的范围，设置属性，获取属性

::

   //获取Property的范围
   OBBoolPropertyRange  bool_range = device->getBoolPropertyRange(property_item.id)
   OBIntPropertyRange   int_range = device->getIntPropertyRange(property_item.id)
   OBFloatPropertyRange float_range = device->getFloatPropertyRange(property_item.id)

   //设置属性
   device->setBoolProperty(propertyItem.id, bool_value);
   device->setIntProperty(propertyItem.id, int_value);
   device->setFloatProperty(propertyItem.id, float_value);

   //获取属性
   bool bool_ret = device->getBoolProperty(propertyItem.id);
   int int_ret = device->getIntProperty(propertyItem.id);
   float float_ret = device->getFloatProperty(propertyItem.id);

程序正常退出之后资源将会自动释放

多设备示例-MultiDevice
----------------------

功能描述：本示例主要演示了对多设备进行操作。

   本示例基于C++ High Level API进行演示

首先需要创建一个Context，获取设备信息列表用于创建设备及Pipeline

::

   ob::Context ctx;
   //查询已经接入设备的列表
   auto devList = ctx.queryDeviceList();
   //获取接入设备的数量
   int devCount = devList->deviceCount();

创建设备，并通过设备创建Pipeline

.. code:: cpp

   //遍历设备列表并创建设备
   std::vector<std::shared_ptr<ob::Device>> devices;
   for (int i = 0; i < devCount; i++)
   {
       //获取设备并创建pipeline
       auto dev  = devList->getDevice( i );
       auto pipe = std::make_shared< ob::Pipeline >( dev );
       pipes.push_back( pipe );
   }

打开多个设备的深度和彩色流

.. code:: cpp

   for(auto &&pipe: pipes) {
       //获取深度相机配置列表
       auto depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);

       //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
       auto depthProfile = depthProfileList->getVideoStreamProfile(640, 480, OB_FORMAT_Y16)->as<ob::VideoStreamProfile>();
       if(!depthProfile) {
           depthProfile = depthProfileList->getProfile(0)->as<ob::VideoStreamProfile>();
       }
       config->enableStream(depthProfile);

       //获取彩色相机配置列表
       auto colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);

       //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
       auto colorProfile = colorProfileList->getVideoStreamProfile(640, 480, OB_FORMAT_MJPG)->as<ob::VideoStreamProfile>();
       if(!colorProfile) {
          colorProfile = colorProfileList->getProfile(0)->as<ob::VideoStreamProfile>();
       }
       config->enableStream(colorProfile);

       //启动pipeline，并传入配置
       pipe->start(config, [i](std::shared_ptr<ob::FrameSet> frameSet) {
           std::lock_guard<std::mutex> lock(frameMutex);
           if(frameSet->colorFrame() && frameSet->depthFrame()) {
               colorFrames[i] = frameSet->colorFrame();
               depthFrames[i] = frameSet->depthFrame();
           }
       });
       i++;
   }

停止所有设备已打开的流

.. code:: cpp


   void StopStream( std::vector< std::shared_ptr< ob::Pipeline > > pipes) {
       int i = 0;
       for ( auto&& pipe : pipes ) {
           if(colorFrames[i])
               colorFrames->reset();
           if(depthFrames[i])
               depthFrames->reset();
           if(irFrames[i])
               irFrames->reset();
           //停止pipeline
           pipe->stop();
           i++;
       }
   }

程序正常退出之后资源将会自动释放

.. _热拔插示例-hotplugin-1:

热拔插示例-HotPlugin
--------------------

功能描述：本示例主要演示设备拔插回调的设置，以及拔插之后处理获取到的流

   本示例基于C++ High Level API进行演示

首先需要创建一个Context，用于获取设备信息列表和创建设备

::

   ob::Context ctx;

注册设备回调，分别在设备拔插的时候执行相关函数

::

   ctx.setDeviceChangedCallback( []( std::shared_ptr< ob::DeviceList > removedList, std::shared_ptr< ob::DeviceList > addedList ) {
           DeviceDisconnectCallback( removedList );
           DeviceConnectCallback( addedList );
       } );

按照配置文件中配置的分辨率和格式进行开流

::

   //按配置文件的流配置启动流，如果没有配置文件，将使用第0个流配置启动流
   try{
     pipeline->start(nullptr);
   }catch(...){
     std::cout<<"Pipeline start failed!"<<std::endl;
   }
   //获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
   auto depthProfiles = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
   //获取对应流配置列表的第0个Profile，如果有配置文件，配置文件里面流配置是流配置列表的第0个
   auto depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   //获取彩色相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
   auto colorProfiles = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
   //获取对应流配置列表的第0个Profile，如果有配置文件，配置文件里面流配置是流配置列表的第0个
   auto colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   //获取帧率
   colorFps = colorProfile->fps();
   depthFps = depthProfile->fps();

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间

::

   auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms

停止Pipeline，将不再产生帧数据

::

   pipe.stop();

程序正常退出之后资源将会自动释放

IMU示例-ImuReader
-----------------

功能描述：本示例主要演示了使用SDK获取IMU数据并输出显示，并通过ESC键退出。

   本示例基于C++ Low Level API进行演示

打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号

.. code:: cpp

   //打印SDK的版本号，SDK版本号分为主版本号，副版本号和修订版本号
   std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: cpp

   ob::Context ctx;

查询已经接入设备的列表

.. code:: cpp

   auto devList = ctx.queryDeviceList();

   //获取接入设备的数量
   devList->deviceCount()

创建设备

.. code:: cpp

   //创建设备，0表示第一个设备的索引
   auto dev = devList->getDevice(0);

获取陀螺仪传感器和加速度传感器

.. code:: cpp

   auto gyroSensor = dev->getSensorList()->getSensor( OB_SENSOR_GYRO );

   auto accelSensor = dev->getSensorList()->getSensor( OB_SENSOR_ACCEL );

获取陀螺仪传感器的配置列表并选择第一个配置开流，在开流的回调里获取帧的数据，加速度传感器同理

.. code:: cpp

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

关流

.. code:: cpp

   gyroSensor->stop();
   accelSensor->stop();

程序正常退出之后资源将会自动释放

点云示例-PointCloud
-------------------

功能描述：连接设备开流 ,生成深度点云或RGBD点云并保存成ply格式文件

   本示例基于C++ High Level API进行演示

创建点云保存成ply格式文件函数，ply文件格式详细描述可在网络上查看首先创建两个函数来保存从流里面获取到的点云数据，这是保存普通点云数据的函数

.. code:: cpp

   //保存点云数据到ply
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

再创建一个保存彩色点云数据的函数用于存储彩色点云数据

.. code:: cpp

   //保存彩色点云数据到ply
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

设置Log等级，避免过多Info等级的Log影响点云输出的结果

.. code:: cpp

   ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_ERROR);

创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

.. code:: cpp

   ob::Pipeline pipeline;

然后可以通过Pipeline来获取彩色流的所有配置, 包括流的分辨率 ,帧率
,以及流的格式

.. code:: cpp

   auto colorProfiles = pipeline.getStreamProfileList( OB_SENSOR_COLOR );

之后根据上面获取的彩色流的配置列表来选取需要的流的配置

.. code:: cpp

   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto colorProfile = colorProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_YUYV);
   if(!colorProfile) {
     colorProfile = colorProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_I420);
     if(!colorProfile)
         colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

根据上面设置彩色流的配置的方法 ,同理来设置深度流的配置

::

   //获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
   auto depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto depthProfile = depthProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);
   if(!depthProfile) {
       depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用彩色流和深度流

.. code:: cpp

   std::shared_ptr< ob::Config > config = std::make_shared< ob::Config >();
   config->enableStream( colorProfile );
   config->enableStream( depthProfile );

开启D2C对齐, 生成RGBD点云时需要开启

.. code:: cpp

   // 开启D2C对齐, 生成RGBD点云时需要开启
   config->setAlignMode(ALIGN_D2C_HW_MODE);

创建点云Filter对象，并且设置相机内参

.. code:: cpp

   // 创建点云Filter对象（点云Filter创建时会在Pipeline内部获取设备参数, 所以尽量在Filter创建前配置好设备）
   ob::PointCloudFilter pointCloud;

   //获取相机内参传入点云Filter中
   auto cameraParam = pipeline.getCameraParam();
   pointCloud.setCameraParam(cameraParam);

启动Pipeline

.. code:: cpp

   pipeline.start( config );

设置些操作提示

.. code:: cpp

    std::cout << "Press R to create rgbd pointCloud and save to ply file! " << std::endl;
    std::cout << "Press d to create depth pointCloud and save to ply file! " << std::endl;
    std::cout << "Press ESC to exit! " << std::endl;

设置主流程通过上面创建的点云Filter对象获取并保存点云数据

.. code:: cpp

   if(key == 'R' || key == 'r') {
     count = 0;
     //限制最多重复10次
     while(count++ < 10) {
       //等待一帧数据，超时时间为100ms
       auto frameset = pipeline.waitForFrames(100);
       if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
         try {
           //生成彩色点云并保存
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
     //限制最多重复10次
     while(count++ < 10) {
       //等待一帧数据，超时时间为100ms
       auto frameset = pipeline.waitForFrames(100);
       if(frameset != nullptr && frameset->depthFrame() != nullptr) {
         try {
           //生成点云并保存
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


最后通过Pipeline来停止流

.. code:: cpp

    pipeline.stop();

程序正常退出后会释放资源

存储示例-SaveToDisk
-------------------

功能描述：连接设备开流 , 获取彩色和深度图并存储为png格式

   本示例基于C++ High Level API进行演示

创建两个函数来用于将获取的图片保存到文件中

.. code:: cpp

   //保存深度图为png格式
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

   //保存彩色图为png格式
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

创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

::

   ob::Pipeline pipeline;

然后可以通过Pipeline来获取彩色流的所有配置, 包括流的分辨率 ,帧率
,以及流的格式

.. code:: cpp

   auto colorProfiles = pipeline.getStreamProfileList( OB_SENSOR_COLOR );

之后根据上面获取的彩色流的配置列表来选取需要的流的配置

.. code:: cpp

   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto colorProfile = colorProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_MJPG, 30);
   if(!colorProfile){
     colorProfile = colorProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

使用和获取彩色流配置相同的方法来配置深度流

.. code:: cpp

   //获取深度相机的所有流配置，包括流的分辨率，帧率，以及帧的格式
   auto depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);

   //通过接口设置感兴趣项，返回对应Profile列表的首个Profile
   auto depthProfile = depthProfiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16, 30);
   if(!depthProfile){
       depthProfile = depthProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
   }

通过创建Config来配置想要开启的流

::

   std::shared_ptr< ob::Config > config = std::make_shared< ob::Config >();
   config->enableStream( colorProfile );
   config->enableStream( depthProfile )

启动流

.. code:: cpp

   pipeline.start( config );

通过等待的方式来获取帧的数据

.. code:: cpp

   auto frameset = pipeline.waitForFrames( 100 );

获取单种类型的帧的数据

.. code:: cpp

   auto colorFrame = frameset->colorFrame();
   auto depthFrame = frameset->depthFrame();

创建格式转换Filter对彩色图像进行转换成RGB格式后保存

.. code:: cpp

   //创建格式转换Filter
   ob::FormatConvertFilter formatConverFilter;

   formatConverFilter.setFormatConvertType(FORMAT_MJPG_TO_RGB888);
   colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();
   formatConverFilter.setFormatConvertType(FORMAT_RGB888_TO_BGR);
   colorFrame = formatConverFilter.process(colorFrame)->as<ob::ColorFrame>();

通过开头的编辑的存储函数来存储获得的数据

.. code:: cpp

   saveColor( colorFrame );
   saveDepth( depthFrame );

停止Pipeline

.. code:: cpp

   pipeline.stop();

程序正常退出后会释放资源

录制示例-Recorder
-----------------

功能描述：连接设备开流 , 录制当前视频流到文件

   本示例基于C++ High Level API进行演示

首先需要创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据

.. code:: cpp

   ob::Pipeline pipe;

通过输入的分辨率，格式，帧率等感兴趣项来获取深度相机的流配置

.. code:: cpp

   //获取深度流配置
   auto profiles     = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
   auto depthProfile = profiles->getVideoStreamProfile(640, 480, OB_FORMAT_Y16);

通过创建Config来配置Pipeline要启用或者禁用哪些流，这里将启用深度流

.. code:: cpp

   //配置Pipeline打开的流类型
   std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
   config->enableStream(depthProfile);

启动在Config中配置的流，如果不传参数，将启动默认配置启动流，启动录制开关

.. code:: cpp

   pipe.start(config);
   pipe.startRecord("./OrbbecPipeline.bag");

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间

::

   auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms

停止Pipeline，将不再产生帧数据

.. code:: cpp

   pipe.stop();

程序正常退出后会释放资源

回放示例-Playback
-----------------

功能描述：连接设备开流 , 载入视频文件进行回放。

   本示例基于C++ High Level API进行演示

首先需要创建一个用于回放文件的Pipeline

.. code:: cpp

   //创建用于回放的pipeline对象
   ob::Pipeline pipe("./OrbbecPipeline.bag");

获取回放对象的回放状态回调

.. code:: cpp

   //获取回放对象设置回放状态回调
   auto playback = pipe.getPlayback();
   playback->setPlaybackStateCallback([&](OBMediaState state) {
       if(state == OB_MEDIA_BEGIN) {
           std::cout << "Playback file begin." << std::endl;
       }
       else if(state == OB_MEDIA_END) {
           std::cout << "Playback file end." << std::endl;
       }
   });

从回放文件中读取设备信息与内参信息

.. code:: cpp

   //从回放文件中读取设备信息
   auto deviceInfo = playback->getDeviceInfo();
   std::cout << "======================DeviceInfo: name : " << deviceInfo->name() << " sn: " << deviceInfo->serialNumber()
     << " firmware: " << deviceInfo->firmwareVersion() << " vid: " << deviceInfo->vid() << " pid: " << deviceInfo->pid() << std::endl;

   //从回放文件中读取内参信息
   auto cameraParam = pipe.getCameraParam();
   std::cout << "======================Camera params : rgb width:" << cameraParam.rgbIntrinsic.width << " rgb height: " << cameraParam.rgbIntrinsic.height
                 << " depth width: " << cameraParam.depthIntrinsic.width << " depth height: " << cameraParam.rgbIntrinsic.height << std::endl;

开启回放

.. code:: cpp

   //开启回放
   pipe.start(NULL);

以阻塞的方式等待一帧数据，该帧是一个复合帧，里面包含配置里启用的所有流的帧数据，并设置帧的等待超时时间

::

   auto frameSet = pipe.waitForFrames(100);    //设置等待时间为100ms

停止Pipeline，将不再产生帧数据

.. code:: cpp

   pipe.stop();

程序正常退出后会释放资源

.. |image.png| image:: https://cdn.nlark.com/yuque/0/2021/png/12768372/1622789513593-6b35c059-f983-4646-959a-d9d5a36c0bcc.png#clientId=uc2495e87-ebf6-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=512&id=ube24af63&margin=%5Bobject%20Object%5D&name=image.png&originHeight=512&originWidth=979&originalType=binary&ratio=1&rotation=0&showTitle=false&size=138109&status=done&style=none&taskId=uf0abb55c-bc30-4fd7-a2eb-02c09dd85af&title=&width=979
.. |image.png| image:: https://cdn.nlark.com/yuque/0/2021/png/12768372/1622789760730-818706d5-02e2-41ad-848e-8d3cfb9512c6.png#clientId=uc2495e87-ebf6-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=513&id=uba259650&margin=%5Bobject%20Object%5D&name=image.png&originHeight=513&originWidth=642&originalType=binary&ratio=1&rotation=0&showTitle=false&size=38473&status=done&style=none&taskId=u8806e674-94b6-46b6-ba5e-bf2a9f0acc3&title=&width=642
.. |1640769142832_A167CC70-8856-4af1-978F-FC28D9CAB32F.png| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1640769409815-c53c88f9-70bf-4c33-9110-6a5317d29c81.png#clientId=u9c8a15cf-1c3a-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=511&id=ufb689f30&margin=%5Bobject%20Object%5D&name=1640769142832_A167CC70-8856-4af1-978F-FC28D9CAB32F.png&originHeight=511&originWidth=642&originalType=binary&ratio=1&rotation=0&showTitle=false&size=357675&status=done&style=none&taskId=udadfafbb-754b-41f7-884a-b0cc54aef8b&title=&width=642
