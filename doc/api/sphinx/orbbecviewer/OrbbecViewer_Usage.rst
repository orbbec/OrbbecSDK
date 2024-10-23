介绍
====

OrbbecViewer是基于Orbbec
SDK开发的示例工具，帮助开发者快速使用Orbbec的3D传感器产品。实现了 Orbbec
3D传感器产品的特性功能，包括读取序列号、获取设备类型、相机参数、控制等功能。

本文档对这些功能进行了描述并演示了基本使用的过程。

OrbbecViewer工具概述
====================

目标用户
--------

OrbbecViewer工具专为最终用户/开发人员设计，用于使用Orbbec SDK 3D相机。

支持设备及平台
--------------

=========== ============ =============
**SDK版本** **产品列表** **固件版本**
=========== ============ =============
v1.0.2      Astra+       1.0.19
v1.1.6      Astra+       1.0.19/1.0.20
\           Femto        1.5.1
v1.2.8      Astra+       1.0.20
\           Femto        1.6.7
=========== ============ =============

+--------------+--------------------------+--------------------------+
| **操作系统** | **要求**                 | **描述**                 |
+==============+==========================+==========================+
| Windows      | - Windows 10 的 2018 年  | vs的工程的生成依赖       |
|              | 4 月（版本               | 安装vs版本和cmake版本，  |
|              | 1803，操作系统内部版本   | 支持vs2015/vs2017/vs2019 |
|              | 17134）发行版 (x64)      |                          |
|              | 或更高版本- 4 GB 内存-   |                          |
|              | USB2.0 及以上端口        |                          |
+--------------+--------------------------+--------------------------+
| Android      | - Android 6/7/8/9/10     |                          |
+--------------+--------------------------+--------------------------+
| Linux        | - Linux Ubuntu           | 支持GCC 7.5              |
|              | 16.04/18.04/20.04 (x64)- |                          |
|              | 4 GB 内存-               |                          |
|              | USB2.0 及以上端口        |                          |
+--------------+--------------------------+--------------------------+

OrbbecViewer工具的使用
======================

.. _31-软件主界面:

3.1 软件主界面
--------------

如图所示，主界面用红框分隔标注了3个区域，作用分别是：

======== ========== ===================================================
**区域** **名称**   **功能**
======== ========== ===================================================
区域一   设备管理区 传感器列表、设备固件更新
区域二   控制面板区 数据流开关及参数配置、图像采集功能控制、D2C功能控制
区域三   图像预览区 传感器图像预览，帧率、时间戳等信息查看
======== ========== ===================================================

|image1.png|\ 其中，控制面板区左侧有六个按钮，最下方按钮\ |image2.png|\ 用于切换中英文；\ |3|\ 用于打开软件详情页面。\ |image4.png|\ 用于查看设备信息和固件升级；剩下两个按钮用于切换不同功能的控制面板TAB页，从上往下一次为“单相机模式”、“高级模式”。点击区域三下方Log信息处，会展示具体的日志信息。

.. _32-数据流:

3.2 数据流
----------

OrbbecViewer
允许用户选择和配置深度、红外和彩色数据流。本节概述了用户可在工具中自定义的参数。用户完成配置后，必须单击顶部\ |image5.png|\ 按钮开始数据流传输（Astra+无惯性测试单元)。

OrbbecViewer工具允许用户选择深度、红外和彩色数据流组合。用户可以通过从可用流列表中选中/取消选中以启用/禁用该流。查看器支持
USB2.0 和 USB3.0，因此可用的配置参数会因 USB2.0/USB3.0
功能而异。\ |image6.png|

分辨率与帧率
~~~~~~~~~~~~

摄像机为用户提供了多种分辨率可供选择，以满足他们的解决方案需求。各路数据流都提供了多种可选的帧格式，已供用户自由选择合适的图像尺寸、图像格式和帧率组合。

例如，深度流可以支持320x240和640x480两种分辨率，FPS帧率的区间范围为5到30。
\ |image7.png|\ 

彩色流可以支持640x480~1920x1080多种分辨率，FPS帧率的区间范围5到30不等，支持MJPG、RGB、I420、H265。对于彩色流的预览，MJPG、H264、H265是编码格式，需要先进行解码，解码通常耗时会比较大，会导致掉帧或渲染出“花屏”画面。在某些型号PC上，可通过硬件加速解码。
\ |image8.png|\ 

红外流可以支持320x240和640x480两种分辨率，FPS帧率5到30不等。
\ |image9.png|\ 

用户可根据实际情况选择最适合使用的分辨率。请注意，分辨率越高，精确度越高，但会增加计算强度。

.. _33-设备信息:

3.3 设备信息
------------

点击按钮“\ |image10.png|\ ”查看设备信息OrbbecViewer工具包含简单的设备信息，如固件版本、产品识别码、相机参数、温度等。\ |image11.png|

.. _34-图像预览区:

3.4 图像预览区
--------------

开流会默认显示平均帧率、时间搓、图像帧号等信息，点击右上角\ |image12.png|\ 来切换信息显示\关闭。点击\ |image13.png|\ 可以关闭流，但不从预览区中移除。点击\ |image14.png|\ 可以重新打开已经暂停且未从预览区移除的流。数据流关闭后，图像预览窗并不会随之关闭，需要用户点击图像预览窗右上角\ |image15.png|\ 。\ |Snipaste_2022-04-21_14-45-09.png|

.. _35-控制面板区:

3.5 控制面板区
--------------

.. _351-传感器和数据流:

3.5.1 传感器和数据流
~~~~~~~~~~~~~~~~~~~~

获取相机名称、序列号及USB协议
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

设备上电并将USB与PC主机连接后，打开OrbbecViewer工具软件，软件会自动连接设备。某些设备可能初始化较长时间，请耐心等待设备连接成功。

各流的参数配置项默认隐藏，可点击设备管理区四个按钮展开查看和设置具体配置项。\ |16|\ 

连接成功后，控制面板区自动显示设备具体信息。\ |image17.png|

深度流模块
^^^^^^^^^^

包括：镜像、软件滤波（范围模式：关闭、中等、长距离）、深度有效范围获取（MinDepthValue/MaxDepthValue）、曝光等功能。

\ |image18.png|

彩色流模块
^^^^^^^^^^

包括：镜像、翻转、对齐、曝光、白平衡、增益、亮度、锐度、饱和度、对比度、色调等功能。

\ |image19.png|

红外流板块
^^^^^^^^^^

包括镜像、曝光功能。

\ |image20.png|

设备数据管理
^^^^^^^^^^^^^^^^^^^^

用户可选择对应图像和捕获帧来进行录制，采集的帧默认保存在“OrbbecViewer/output”目录中。

\ |image21.png|

.. _36-d2c功能支持硬件d2c和软件d2c:

3.6 D2C功能（支持硬件D2C和软件D2C）
-----------------------------------

在高级模式下的“D2C”功能，可控制实现深度流和彩色流同步功能：

1.点击按钮\ |image22.png|\ 即可默认同步打开深度流和彩色流；

2.可通过切换按钮\ |image23.png|\ 实现打开或关闭帧同步功能；

3.点击按钮\ |image24.png|\ 可预览深度叠加到彩色的渲染显示效果；

4.点击按钮\ |image25.png|\ 可打开或关闭深度和彩色帧同步采集（Femto特有）；

5.录制的图片默认保存在“OrbbecViewer/output/RecordFile”目录中。

\ |image26|

.. _37-点云:

3.7 点云
--------

点击\ |image26.png|\ 即可开启点云。

可通过鼠标的滑轮控制点云的缩放，鼠标的移动来控制点云的移动。

可选择导出深度点云（xyz）或RGBD点云（xyzrgb）并保存成ply格式文件，默认保存在“OrbbecViewer/output/PointCloud”目录中。

\ |image27.png|

.. _38-固件更新:

3.8 固件更新
------------

在设备已连接状态，点击\ |28|\ 可进入固件更新页面。

1. Femto设备固件包含系统固件和板载MCU固件，可在“类型”下拉框选择升级固件类型。
2. 选择好固件类型后，在“固件”输入框输入现有更新的固件镜像文件（“.img”后缀），点击“升级”按钮开始更新。
3. 设备固件更新采用A/B双分区方案，如更新意外失败，不会影响原固件运行，重新连接设备进行更新即可。设备更新时间比较长，且设备会自动重启一次，请留意提示信息并耐心等待。

Astra+界面：

\ |1650524827127_47E08781-CEC2-4d18-BC50-F8984C3F87C4.png|\ 

Femto界面：

\ |image29.png|

.. _39-日志信息:

3.9 日志信息
------------

默认情况下，日志信息区是折叠单条显示状态，可通过点击最右边按钮\ |image30.png|\ 展开查看完整log信息。展开后可通过点击按钮\ |image31.png|\ 清空log，通过点击按钮\ |image32.png|\ 恢复折叠单条显示状态。\ |Snipaste_2022-04-21_14-45-27.png|

功能介绍
========

演示了主要API的使用。

.. _41-获取序列号:

4.1 获取序列号
--------------

|33|

C++代码
~~~~~~~

.. code:: cpp

   DeviceInfo deviceInfo = device->getDeviceInfo();
   std::string serialNumber = deviceInfo.serialNumber();

Android代码
~~~~~~~~~~~

.. code:: java

   DeviceInfo deviceInfo = device.getInfo();
   String serialNumber = deviceInfo.getSerialNumber();

.. _42-获取设备名称:

4.2 获取设备名称
----------------

|34|

.. _c代码-1:

C++代码
~~~~~~~

.. code:: cpp

   std::shared_ptr< DeviceInfo > deviceInfo = device->getDeviceInfo();
   std::string deviceName = deviceInfo->name();

.. _android代码-1:

Android代码
~~~~~~~~~~~

.. code:: java

   DeviceInfo deviceInfo = device.getInfo();
   String name = deviceInfo.getName();

.. _43-获取相机参数:

4.3 获取相机参数
----------------

|image38|

.. _c代码-2:

C++代码
~~~~~~~

.. code:: cpp

   //获取深度相机内参
   OBCameraIntrinsic colorCameraIntrinsic = device->getCameraIntrinsic(OB_SENSOR_DEPTH);
   //获取彩色相机内参
   OBCameraIntrinsic depthCameraIntrinsic = device->getCameraIntrinsic(OB_SENSOR_COLOR);
   //获取深度相机畸变参数
   OBCameraDistortion depthCameraDistortion = device->getCameraDistortion(OB_SENSOR_DEPTH);
   //获取彩色相机畸变参数
   OBCameraDistortion colorCameraDistortion = device->getCameraDistortion(OB_SENSOR_color);
   //获取旋转矩阵
   OBD2CTransform d2cTransform = device->getD2CTransform();

.. _android代码-2:

Android代码
~~~~~~~~~~~

.. code:: java

   CameraParams object = new CameraParams();
   boolean isSupport = device.isPropertySupported(DeviceProperty.CAMERA_PARA);
   if (!isSupport) {
       return;
   }
   device.getPropertyValueDataType(DeviceProperty.CAMERA_PARA, object);
   //获取深度相机内参
   float[] depthParams = object.getDepthInternalParams();
   //获取彩色相机内参
   float[] colorParams = object.getColorInternalParams();
   //获取深度相机畸变参数
   float[] depthCoeffs = object.getDepthCoeffs();
   //获取彩色相机畸变参数
   float[] colorCoeffs = object.getColorCoeffs();

.. _44-获取和设置红外相机曝光值:

4.4 获取和设置红外相机曝光值
----------------------------

|image39|

.. _c代码-3:

C++代码
~~~~~~~

.. code:: cpp

   std::shared_ptr<ob::Sensor> irSensor = device->getSensorList()->getSensor(OB_SENSOR_IR);
   if(!irSensor->isPropertySupported(OB_SENSOR_PROPERTY_EXPOSURE_INT))
       return;
   //获取红外相机曝光值
   int32_t exposure = irSensor->getIntProperty(OB_SENSOR_PROPERTY_EXPOSURE_INT);
   //设置红外相机曝光值
   irSensor->setIntProperty(OB_SENSOR_PROPERTY_EXPOSURE_INT, exposure / 2);

.. _android代码-3:

Android代码
~~~~~~~~~~~

.. code:: java

   Sensor irSensor = device.getSensor(SensorType.IR);
   boolean isSupport = irSensor.isPropertySupported(SensorProperty.EXPOSURE_INT);
   if (!isSupport) {
       return;
   }
   //获取红外相机曝光值
   int exposure = irSensor.getPropertyValueI(SensorProperty.EXPOSURE_INT);
   //设置红外相机曝光值
   irSensor.setPropertyValueI(SensorProperty.EXPOSURE_INT, exposure / 2);

.. _45-彩色相机自动曝光:

4.5 彩色相机自动曝光
--------------------

|image40|

.. _c代码-4:

C++代码
~~~~~~~

.. code:: cpp

   bool isOpen;
   std::shared_ptr<ob::Sensor> colorSensor = device->getSensorList()->getSensor(OB_SENSOR_COLOR);
   if(!colorSensor->isPropertySupported(OB_SENSOR_PROPERTY_ENABLE_AUTO_EXPOSURE_BOOL))
       return;

   colorSensor->setBoolProperty(OB_SENSOR_PROPERTY_ENABLE_AUTO_EXPOSURE_BOOL, isOpen);

.. _android代码-4:

Android代码
~~~~~~~~~~~

.. code:: java

   boolean isOpen;
   Sensor colorSensor = device.getSensor(SensorType.COLOR);
   boolean isSupport = colorSensor.isPropertySupported(SensorProperty.ENABLE_AUTO_EXPOSURE_BOOL);
   if (!isSupport) {
       return;
   }
   colorSensor.setPropertyValueB(SensorProperty.ENABLE_AUTO_EXPOSURE_BOOL, isOpen);

.. _46-获取和设置彩色相机曝光值:

4.6 获取和设置彩色相机曝光值
----------------------------

|image41|

.. _c代码-5:

C++代码
~~~~~~~

.. code:: cpp

   std::shared_ptr<ob::Sensor> colorSensor = device->getSensorList()->getSensor(OB_SENSOR_COLOR);
   if(!colorSensor->isPropertySupported(OB_SENSOR_PROPERTY_EXPOSURE_INT))
       return;
   //获取彩色相机曝光值
   int32_t exposure = colorSensor->getIntProperty(OB_SENSOR_PROPERTY_EXPOSURE_INT);
   //设置彩色相机曝光值
   colorSensor->setIntProperty(OB_SENSOR_PROPERTY_EXPOSURE_INT, exposure / 2);

.. _android代码-5:

Android代码
~~~~~~~~~~~

.. code:: java

   Sensor colorSensor = device.getSensor(SensorType.COLOR);
   boolean isSupport = colorSensor.isPropertySupported(SensorProperty.EXPOSURE_INT);
   if (!isSupport) {
       return;
   }
   //获取彩色相机曝光值
   int exposure = colorSensor.getPropertyValueI(SensorProperty.EXPOSURE_INT);
   //设置彩色相机曝光值
   colorSensor.setPropertyValueI(SensorProperty.EXPOSURE_INT, exposure / 2);

.. _47-彩色相机自动白平衡:

4.7 彩色相机自动白平衡
----------------------

|image42|

.. _c代码-6:

C++代码
~~~~~~~

.. code:: cpp

   bool isOpen;
   std::shared_ptr<ob::Sensor> colorSensor = device->getSensorList()->getSensor(OB_SENSOR_COLOR);
   if(!colorSensor->isPropertySupported(OB_SENSOR_PROPERTY_ENABLE_AUTO_WHITE_BALANCE_BOOL))
       return;

   colorSensor->setBoolProperty(OB_SENSOR_PROPERTY_ENABLE_AUTO_WHITE_BALANCE_BOOL, isOpen);

.. _android代码-6:

Android代码
~~~~~~~~~~~

.. code:: java

   boolean isOpen;
   Sensor colorSensor = device.getSensor(SensorType.COLOR);
   boolean isSupport = colorSensor.isPropertySupported(SensorProperty.ENABLE_AUTO_WHITE_BALANCE_BOOL);
   if (!isSupport) {
       return;
   }
   colorSensor.setPropertyValueB(SensorProperty.ENABLE_AUTO_WHITE_BALANCE_BOOL, isOpen);

.. _48-获取和设置彩色相机增益:

4.8 获取和设置彩色相机增益
--------------------------

|image43|

.. _c代码-7:

C++代码
~~~~~~~

.. code:: cpp

   std::shared_ptr<ob::Sensor> colorSensor = device->getSensorList()->getSensor(OB_SENSOR_COLOR);
   if(!colorSensor->isPropertySupported(OB_SENSOR_PROPERTY_GAIN_INT))
       return;
   //获取彩色相机增益值
   int32_t gain = colorSensor->getIntProperty(OB_SENSOR_PROPERTY_GAIN_INT);
   //设置彩色相机增益值
   colorSensor->setIntProperty(OB_SENSOR_PROPERTY_GAIN_INT, gain / 2);

.. _android代码-7:

Android代码
~~~~~~~~~~~

.. code:: java

   boolean isExposure;
   Sensor colorSensor = device.getSensor(SensorType.COLOR);
   boolean isSupport = colorSensor.isPropertySupported(SensorProperty.GAIN_INT);
   if (!isSupport) {
       return;
   }
   //获取彩色相机增益
   int gain = colorSensor.getPropertyValueI(SensorProperty.GAIN_INT);
   //设置彩色相机增益
   colorSensor.setPropertyValueI(SensorProperty.GAIN_INT, gain / 2);

.. _49-彩色相机数据流镜像:

4.9 彩色相机数据流镜像
----------------------

|image44|

.. _c代码-8:

C++代码
~~~~~~~

.. code:: cpp

   std::shared_ptr<ob::Sensor> colorSensor = device->getSensorList()->getSensor(OB_SENSOR_COLOR);
   if(!colorSensor->isPropertySupported(OB_SENSOR_PROPERTY_ROLL_INT))
       return;
   //1 - 设置镜像； 0 - 不设置镜像
   colorSensor->setBoolProperty(OB_SENSOR_PROPERTY_ROLL_INT, 1);

.. _android代码-8:

Android代码
~~~~~~~~~~~

.. code:: java

   Sensor colorSensor = device.getSensor(SensorType.COLOR);
   boolean isSupport = colorSensor.isPropertySupported(SensorProperty.ROLL_INT);
   if (!isSupport) {
       return;
   }
   //1 - 设置镜像； 0 - 不设置镜像
   colorSensor.setPropertyValueI(SensorProperty.ROLL_INT, 1);

.. _410-深度相机数据流镜像:

4.10 深度相机数据流镜像
-----------------------

|image45|

.. _c代码-9:

C++代码
~~~~~~~

.. code:: cpp

   bool isMirror;
   std::shared_ptr<ob::Sensor> depthSensor = device->getSensorList()->getSensor(OB_SENSOR_COLOR);
   if(!depthSensor->isPropertySupported(OB_DEVICE_PROPERTY_DEPTH_MIRROR_BOOL))
       return;

   depthSensor->setBoolProperty(OB_DEVICE_PROPERTY_DEPTH_MIRROR_BOOL, isMirror);

.. _android代码-9:

Android代码
~~~~~~~~~~~

.. code:: java

   boolean isMirror;
   boolean isSupport = device.isPropertySupported(DeviceProperty.DEPTH_MIRROR_BOOL);
   if (!isSupport) {
       return;
   }
   device.setPropertyValueB(DeviceProperty.DEPTH_MIRROR_BOOL, isMirror);

.. _411-红外相机数据流镜像:

4.11 红外相机数据流镜像
-----------------------

|image46|

.. _c代码-10:

C++代码
~~~~~~~

.. code:: cpp

   bool isMirror;
   std::shared_ptr<ob::Sensor> irSensor = device->getSensorList()->getSensor(OB_SENSOR_IR);
   if(!irSensor->isPropertySupported(OB_SDK_PROPERTY_IR_MIRROR_BOOL))
       return;

   irSensor->setBoolProperty(OB_SDK_PROPERTY_IR_MIRROR_BOOL, isMirror);

.. _android代码-10:

Android代码
~~~~~~~~~~~

.. code:: java

   boolean isMirror;
   boolean isSupport = device.isPropertySupported(DeviceProperty.IR_MIRROR_BOOL);
   if (!isSupport) {
       return;
   }
   device.setPropertyValueB(DeviceProperty.IR_MIRROR_BOOL, isMirror);

.. |image1.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1650529831703-89abdcec-93b1-48e3-a0db-8a1fc1770bf0.png#clientId=udcd67ee1-6fad-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=1015&id=u455ba7e9&margin=%5Bobject%20Object%5D&name=image.png&originHeight=1015&originWidth=1915&originalType=binary&ratio=1&rotation=0&showTitle=false&size=569548&status=done&style=none&taskId=ud67471c9-f7ed-4fe3-9a0a-32881c80a74&title=&width=1915
.. |image2.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648178647906-90083d0a-3986-4f8c-9405-ad9fc1c8da91.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=21&id=u4b760b1e&margin=%5Bobject%20Object%5D&name=image.png&originHeight=21&originWidth=53&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1060&status=done&style=none&taskId=u91d67062-7ba2-4bfd-9c29-c56e4b0751a&title=&width=53
.. |3| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648189264304-bbb1914a-33b2-4ca9-9025-6bbb18ade1f4.png#crop=0&crop=0&crop=1&crop=1&from=url&height=30&id=EFhtA&margin=%5Bobject%20Object%5D&originHeight=35&originWidth=41&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=&width=35
.. |image4.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648178706807-9aa991b7-1b9a-4e5a-b9c3-37cf5f5824fe.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=28&id=u5638a2e4&margin=%5Bobject%20Object%5D&name=image.png&originHeight=42&originWidth=46&originalType=binary&ratio=1&rotation=0&showTitle=false&size=296&status=done&style=none&taskId=u81d4f17a-31c4-456e-959f-03c56005d2f&title=&width=31
.. |image5.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648108931520-6ae90374-6134-4450-9c3b-eb3ce58d118e.png#clientId=u601826ea-9d84-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=30&id=u3d7c78ea&margin=%5Bobject%20Object%5D&name=image.png&originHeight=40&originWidth=474&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4424&status=done&style=none&taskId=ufb3300ee-ea56-45f7-a3f3-4f4564252f6&title=&width=354
.. |image6.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1650530397142-bcd91fdf-a102-4a1e-8300-fa3dea236aa9.png#clientId=ua3abc564-fea4-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=1015&id=u029ec269&margin=%5Bobject%20Object%5D&name=image.png&originHeight=1015&originWidth=1916&originalType=binary&ratio=1&rotation=0&showTitle=false&size=620470&status=done&style=none&taskId=u20dd4230-8883-472d-a3a4-2112c2ee718&title=&width=1916
.. |image7.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648109849447-e7b61e94-25d6-41af-bdcf-7c62b1172694.png#clientId=u601826ea-9d84-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=215&id=u6bccac68&margin=%5Bobject%20Object%5D&name=image.png&originHeight=215&originWidth=275&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21596&status=done&style=none&taskId=u2f10449f-1522-46d6-b4bf-9a4a7c6b7b8&title=&width=275
.. |image8.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648110409212-94740f94-3119-4a10-8842-e256687e85cb.png#clientId=u601826ea-9d84-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=212&id=uf20b9a8b&margin=%5Bobject%20Object%5D&name=image.png&originHeight=212&originWidth=290&originalType=binary&ratio=1&rotation=0&showTitle=false&size=21733&status=done&style=none&taskId=u9a12c10f-737a-4044-bb70-780c8e7c721&title=&width=290
.. |image9.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648110478875-e7f50d30-1bd6-4f4e-9f19-7f4f2d08384e.png#clientId=u601826ea-9d84-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=213&id=ucf7b3d75&margin=%5Bobject%20Object%5D&name=image.png&originHeight=213&originWidth=274&originalType=binary&ratio=1&rotation=0&showTitle=false&size=20096&status=done&style=none&taskId=u39398300-3fb7-41e6-965b-5579f9e56c2&title=&width=274
.. |image10.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648113681441-740ad3ed-3400-464d-8555-41531e3fb0ba.png#clientId=u601826ea-9d84-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=25&id=u8e0ac9b9&margin=%5Bobject%20Object%5D&name=image.png&originHeight=45&originWidth=48&originalType=binary&ratio=1&rotation=0&showTitle=false&size=307&status=done&style=none&taskId=ud9ed30dd-0136-4d6c-b364-1c80ad490a3&title=&width=27
.. |image11.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648112431902-9d6d6b40-514c-4a47-8198-dfac8fb5fce1.png#clientId=u601826ea-9d84-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=723&id=ufa888028&margin=%5Bobject%20Object%5D&name=image.png&originHeight=723&originWidth=1365&originalType=binary&ratio=1&rotation=0&showTitle=false&size=190921&status=done&style=none&taskId=ud8f6fa22-6986-4a9b-8000-aaebb452997&title=&width=1365
.. |image12.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648190245362-2a1f0154-cbed-443a-b7ef-cfa2592cb771.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=21&id=ue83562d7&margin=%5Bobject%20Object%5D&name=image.png&originHeight=21&originWidth=22&originalType=binary&ratio=1&rotation=0&showTitle=false&size=335&status=done&style=none&taskId=ud54a877a-c68b-4522-ab27-150d5506dcd&title=&width=22
.. |image13.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648190351836-2f8aded6-b17d-4feb-90fd-88612874bab4.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=15&id=u87630091&margin=%5Bobject%20Object%5D&name=image.png&originHeight=15&originWidth=22&originalType=binary&ratio=1&rotation=0&showTitle=false&size=165&status=done&style=none&taskId=u7e56d43f-a686-4d39-8225-c056bd4d47a&title=&width=22
.. |image14.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648190386660-828cbb60-595b-4810-8f9c-56605eb3e6c0.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=21&id=u2995be4e&margin=%5Bobject%20Object%5D&name=image.png&originHeight=21&originWidth=21&originalType=binary&ratio=1&rotation=0&showTitle=false&size=334&status=done&style=none&taskId=ubd664e72-f449-41ee-bffb-71cd194fe29&title=&width=21
.. |image15.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648190510639-199d8e60-34cf-4227-b422-c0d1bf227b91.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=18&id=ubb05a3cc&margin=%5Bobject%20Object%5D&name=image.png&originHeight=18&originWidth=22&originalType=binary&ratio=1&rotation=0&showTitle=false&size=437&status=done&style=none&taskId=u6ced94a0-924b-4429-b77e-4e625dca5b6&title=&width=22
.. |Snipaste_2022-04-21_14-45-09.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1650529063373-5a0d9fd1-03eb-401d-89e0-5554a4c27dfb.png#clientId=ub2a59962-d9fb-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=1018&id=u1756b4b4&margin=%5Bobject%20Object%5D&name=Snipaste_2022-04-21_14-45-09.png&originHeight=1018&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5874293&status=done&style=none&taskId=ua7e26dc4-3572-446f-ad9e-bb31fdf0799&title=&width=1920
.. |16| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648193922814-b9bf475c-ea05-4f9a-9211-63cdfe7069c9.png?x-oss-process=image%2Fresize%2Cw_750%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=avz7e&margin=%5Bobject%20Object%5D&originHeight=395&originWidth=750&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image17.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648193828014-fe829127-8a93-43f2-94b2-3bd7349b6928.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=723&id=uf0df08f4&margin=%5Bobject%20Object%5D&name=image.png&originHeight=723&originWidth=1365&originalType=binary&ratio=1&rotation=0&showTitle=false&size=216456&status=done&style=none&taskId=ucefc34e3-a08d-42a5-a05f-999721ba440&title=&width=1365
.. |image18.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648194253076-b2193c01-b1d6-4195-8a93-294556809bb1.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=320&id=u45a00cc0&margin=%5Bobject%20Object%5D&name=image.png&originHeight=320&originWidth=291&originalType=binary&ratio=1&rotation=0&showTitle=false&size=29940&status=done&style=none&taskId=udbee5558-d00d-4dbf-84df-5ad44e8fc9c&title=&width=291
.. |image19.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648196474053-2ede75f7-1212-401e-ac5d-017cddf9095d.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=655&id=u771507d8&margin=%5Bobject%20Object%5D&name=image.png&originHeight=655&originWidth=286&originalType=binary&ratio=1&rotation=0&showTitle=false&size=40466&status=done&style=none&taskId=u6066ed62-8028-45c0-9946-9d2572e94ad&title=&width=286
.. |image20.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648196726206-8e8a269e-b6b7-4324-a88e-8b3c1dee56de.png#clientId=u5e660bfe-7179-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=188&id=u58745c50&margin=%5Bobject%20Object%5D&name=image.png&originHeight=188&originWidth=288&originalType=binary&ratio=1&rotation=0&showTitle=false&size=12860&status=done&style=none&taskId=uc2641b52-d193-40bf-b041-54659492c02&title=&width=288
.. |image21.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648201290774-0ef8a066-b934-4870-8f7d-ba9c88cc4f0a.png#clientId=ua2ab74ba-4f33-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=114&id=ue5f910ef&margin=%5Bobject%20Object%5D&name=image.png&originHeight=114&originWidth=289&originalType=binary&ratio=1&rotation=0&showTitle=false&size=9754&status=done&style=none&taskId=u7affa078-45a1-4837-b37d-f030850889e&title=&width=289
.. |image22.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537831215-51a4bf19-c347-4f91-ad6f-59f53a5a936c.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=22&id=u7feabba6&margin=%5Bobject%20Object%5D&name=image.png&originHeight=22&originWidth=121&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2233&status=done&style=none&taskId=u19d96ea0-b777-4809-bed2-0337c42f077&title=&width=121
.. |image23.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537851887-47e51c96-9e33-40ef-9b27-b322575d9e50.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=21&id=u2c7a6cb9&margin=%5Bobject%20Object%5D&name=image.png&originHeight=21&originWidth=123&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1774&status=done&style=none&taskId=u3c6832ce-180a-4d59-82e5-58551ce8e7a&title=&width=123
.. |image24.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537888729-045234a1-fe61-4ca2-acd1-f28bbc475e25.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=21&id=u181c72d5&margin=%5Bobject%20Object%5D&name=image.png&originHeight=21&originWidth=137&originalType=binary&ratio=1&rotation=0&showTitle=false&size=2430&status=done&style=none&taskId=u42e5a199-dcd8-44e8-a445-b8f5688466b&title=&width=137
.. |image25.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537967163-1f3dc227-ff5b-49ff-b670-1ea24d3dafbf.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=30&id=u149017b8&margin=%5Bobject%20Object%5D&name=image.png&originHeight=30&originWidth=255&originalType=binary&ratio=1&rotation=0&showTitle=false&size=4577&status=done&style=none&taskId=ub6329e0b-57fe-476b-bfb9-7eec8807ec4&title=&width=255
.. |image26| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1650527511641-4df3f94d-7d51-432a-95e3-82e1949448d1.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10#crop=0&crop=0&crop=1&crop=1&from=url&id=z4UGW&margin=%5Bobject%20Object%5D&originHeight=443&originWidth=294&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image26.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537611246-d479b980-bce9-40dd-910d-bae6fdd75d65.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=36&id=ubd23b188&margin=%5Bobject%20Object%5D&name=image.png&originHeight=36&originWidth=106&originalType=binary&ratio=1&rotation=0&showTitle=false&size=1058&status=done&style=none&taskId=uda3bb580-5d1a-463c-b53e-8b29907db8f&title=&width=106
.. |image27.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537481182-e94bb95d-819b-4544-b0ea-463904705bf0.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=1038&id=u6c95bcbe&margin=%5Bobject%20Object%5D&name=image.png&originHeight=1038&originWidth=1919&originalType=binary&ratio=1&rotation=0&showTitle=false&size=208043&status=done&style=none&taskId=u8bd423fe-4bc3-4f7b-9501-bc5528e4ffb&title=&width=1919
.. |28| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648113681441-740ad3ed-3400-464d-8555-41531e3fb0ba.png?x-oss-process=image%2Fresize%2Cw_24%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=KcJfI&margin=%5Bobject%20Object%5D&originHeight=23&originWidth=24&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |1650524827127_47E08781-CEC2-4d18-BC50-F8984C3F87C4.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1650527852473-ace0065f-f18c-41e3-a340-aeee9a2feebd.png#clientId=u2f82a3fc-3972-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=591&id=uf6488c2b&margin=%5Bobject%20Object%5D&name=1650524827127_47E08781-CEC2-4d18-BC50-F8984C3F87C4.png&originHeight=591&originWidth=601&originalType=binary&ratio=1&rotation=0&showTitle=false&size=14615&status=done&style=none&taskId=ue0413d1b-9aed-4599-a66b-8518037158b&title=&width=601
.. |image29.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648201389847-593c48f1-6916-4240-9c49-ffb806f3c3bb.png#clientId=ua2ab74ba-4f33-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=586&id=u11e5bee2&margin=%5Bobject%20Object%5D&name=image.png&originHeight=586&originWidth=598&originalType=binary&ratio=1&rotation=0&showTitle=false&size=19272&status=done&style=none&taskId=u9083052a-102a-4165-9b80-d1ff7a692c8&title=&width=598
.. |image30.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537675935-cb8f82c6-7b05-4ef5-9cbd-06c8d06137e6.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=15&id=ue81c5c89&margin=%5Bobject%20Object%5D&name=image.png&originHeight=15&originWidth=23&originalType=binary&ratio=1&rotation=0&showTitle=false&size=402&status=done&style=none&taskId=u152ae45c-ec0a-4f86-8c19-61e53b72ddd&title=&width=23
.. |image31.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537742263-b78992c0-69c2-4ec3-9743-fb9e2766dd9d.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=18&id=u92ac4152&margin=%5Bobject%20Object%5D&name=image.png&originHeight=18&originWidth=23&originalType=binary&ratio=1&rotation=0&showTitle=false&size=436&status=done&style=none&taskId=ua740cc90-787e-4189-b958-f33926d6b49&title=&width=23
.. |image32.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1648537760887-cd7b8e8a-3a28-491f-8cdf-87c726377bd8.png#clientId=uef6b8d0d-eb1c-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=19&id=u2449ada5&margin=%5Bobject%20Object%5D&name=image.png&originHeight=19&originWidth=20&originalType=binary&ratio=1&rotation=0&showTitle=false&size=399&status=done&style=none&taskId=ued2c6d61-1b64-4cef-9900-0bac1193afe&title=&width=20
.. |Snipaste_2022-04-21_14-45-27.png| image:: https://cdn.nlark.com/yuque/0/2022/png/25457865/1650530106635-5e4f42e3-66e0-43be-9bac-83c257f554dd.png#clientId=udcd67ee1-6fad-4&crop=0&crop=0&crop=1&crop=1&from=paste&height=1014&id=u4f661c7e&margin=%5Bobject%20Object%5D&name=Snipaste_2022-04-21_14-45-27.png&originHeight=1014&originWidth=1920&originalType=binary&ratio=1&rotation=0&showTitle=false&size=5851213&status=done&style=none&taskId=u055ce2b0-6eb5-4d59-87a6-3782bf4fb91&title=&width=1920
.. |33| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639382292960-231341f8-fb2a-4fbd-bc69-566a7740eac9.png#crop=0&crop=0&crop=1&crop=1&from=url&id=AOezl&margin=%5Bobject%20Object%5D&originHeight=33&originWidth=294&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |34| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639382254638-12a76403-1467-4eb2-a07d-1f50cc227428.png#crop=0&crop=0&crop=1&crop=1&from=url&id=lWU7p&margin=%5Bobject%20Object%5D&originHeight=36&originWidth=294&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image38| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639386018191-457ff14b-6485-4bbb-89e1-0e43368fe7e0.png#crop=0&crop=0&crop=1&crop=1&from=url&id=xQR8F&margin=%5Bobject%20Object%5D&originHeight=586&originWidth=586&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image39| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639383736038-0817fe8c-29ef-4efe-aa5c-146cd342d25c.png#crop=0&crop=0&crop=1&crop=1&from=url&id=iyBmh&margin=%5Bobject%20Object%5D&originHeight=185&originWidth=294&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image40| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639380531623-f0daa0d1-726b-422b-af85-253394861176.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_281%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=yjzLs&margin=%5Bobject%20Object%5D&originHeight=215&originWidth=281&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image41| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639379927390-9eebc82d-6298-4276-9f89-8dc99331e9bb.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_284%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=yS14w&margin=%5Bobject%20Object%5D&originHeight=209&originWidth=284&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image42| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639380563812-f091c743-71a3-4be6-9cbf-6930ebe7e695.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_290%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=eCKZy&margin=%5Bobject%20Object%5D&originHeight=294&originWidth=290&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image43| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639380066566-230e5e9b-c0af-4093-b97d-6eb094eaa568.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_290%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=XGTCF&margin=%5Bobject%20Object%5D&originHeight=352&originWidth=290&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image44| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639380253712-c2b4bcde-823e-4180-9157-2707db4f3cb0.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_292%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=CNHbp&margin=%5Bobject%20Object%5D&originHeight=296&originWidth=292&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image45| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639380620773-ce0b4211-1837-40a2-9b24-0dc57c73fceb.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_281%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=GjkZ5&margin=%5Bobject%20Object%5D&originHeight=255&originWidth=281&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
.. |image46| image:: https://cdn.nlark.com/yuque/0/2021/png/25457865/1639380651007-b54e8b8f-a75f-4351-9780-e473ad5d1f0e.png?x-oss-process=image%2Fwatermark%2Ctype_d3F5LW1pY3JvaGVp%2Csize_13%2Ctext_T3JiYmVj%2Ccolor_FFFFFF%2Cshadow_50%2Ct_80%2Cg_se%2Cx_10%2Cy_10%2Fresize%2Cw_285%2Climit_0#crop=0&crop=0&crop=1&crop=1&from=url&id=Px9Ww&margin=%5Bobject%20Object%5D&originHeight=183&originWidth=285&originalType=binary&ratio=1&rotation=0&showTitle=false&status=done&style=none&title=
