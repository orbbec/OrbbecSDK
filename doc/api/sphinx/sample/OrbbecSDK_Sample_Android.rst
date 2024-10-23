获取所有示例都可以在工程的OrbbecSdkExamples目录中找到

+----------------+----------+----------------------------------------+
| **名称**       | **语言** | **描述**                               |
+================+==========+========================================+
| HelloOrbbec    | Java     | 演示连接到设备获取SDK版本和设备信息    |
+----------------+----------+----------------------------------------+
| 深度示例       | Java     | 通过Pipeline开启指定配置彩色流并渲染   |
+----------------+----------+----------------------------------------+
| 彩色示例       | Java     | 通过Pipeline开启指定配置深度流并渲染   |
+----------------+----------+----------------------------------------+
| 红外示例       | Java     | 通过Pipeline开启指定配置红外流并渲染   |
+----------------+----------+----------------------------------------+
| 流对齐示例     | Java     | 演示对传感器数据流对齐的操作           |
+----------------+----------+----------------------------------------+
| 热拔插示例     | Java     | 演示设备                               |
|                |          | 拔插回调的设置，并获取到插拔后处理的流 |
+----------------+----------+----------------------------------------+
| IMU示例        | Java     | 获取IMU数据并输出显示                  |
+----------------+----------+----------------------------------------+
| 多设备示例     | Java     | 演示对多设备进行操作                   |
+----------------+----------+----------------------------------------+
| 点云示例       | Java     | 演示生成                               |
|                |          | 深度点云或RGBD点云并保存成ply格式文件  |
+----------------+----------+----------------------------------------+
| 存储示例       | Java     | 获取彩色存储为png格式，深度存          |
|                |          | 储为raw格式以及通过filter进行格式转换  |
+----------------+----------+----------------------------------------+
| 传感器控制示例 | Java     | 演示对设备和传感器控制命令的操作       |
+----------------+----------+----------------------------------------+
| 录制与回放示例 | Java     | :连接设备开流 ,                        |
|                |          | 录制当                                 |
|                |          | 前视频流到文件，载入视频文件进行回放。 |
+----------------+----------+----------------------------------------+

Android
=======

HelloOrbbec
-----------

功能描述：用于演示SDK初始化、获取SDK版本、获取设备型号、获取设备序列号、获取固件版本号、SDK释放资源。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               StringBuilder builder = new StringBuilder();
               // 2.查看SDK版本
               builder.append("SDK Version = " + OBContext.getVersionName() + "\n");

               // 3.获取设备个数
               int deviceCount = deviceList.getDeviceCount();

               for (int i = 0; i < deviceCount; ++i) {
                   // 4.根据设备索引创建设备
                   Device device = deviceList.getDevice(i);

                   // 5.获取设备信息
                   DeviceInfo info = device.getInfo();

                   // 6.获取设备版本信息
                   builder.append("Name: " + info.getName() + "\n");
                   builder.append("Vid: " + LocalUtils.formatHex04(info.getVid()) + "\n");
                   builder.append("Pid: " + LocalUtils.formatHex04(info.getPid()) + "\n");
                   builder.append("Uid: " + info.getUid() + "\n");
                   builder.append("SN: " + info.getSerialNumber() + "\n");
                   String firmwareVersion = info.getFirmwareVersion();
                   builder.append("FirmwareVersion: " + firmwareVersion + "\n");
                   String hardwareVersion = info.getHardwareVersion();
                   builder.append("HardwareVersion: " + hardwareVersion + "\n");

                   // 7.遍历当前设备的传感器
                   for (Sensor sensor : device.querySensors()) {
                       // 8.查询传感器类型
                       builder.append("Sensor : type = " + sensor.getType() + "\n");
                   }
                   runOnUiThread(() -> txtInfo.setText(builder.toString()));

                   // 9.释放设备信息
                   info.close();

                   // 10.释放设备资源
                   device.close();
               }
               // 11.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

查看SDK版本号

.. code:: java

   // 获取SDK版本名称
   String version = OBContext.getVersionName();
   // 获取SDK内核版本名称(Orbbec SDK so的版本)
   String coreVersion = OBContext.getCoreVersionName();

通过回调可得到设备索引，根据设备索引创建设备，此处以创建第一个设备为例

.. code:: java

   Device device = deviceList.getDevice(0);

接下来就可以获取和这个设备相关的信息了，下面这些信息都可以通过DeviceInfo来获取

.. code:: java

   // 5.获取设备信息
   DeviceInfo info = device.getInfo();

   // 6.获取设备版本信息
   builder.append("Name: " + info.getName() + "\n");
   builder.append("Vid: " + LocalUtils.formatHex04(info.getVid()) + "\n");
   builder.append("Pid: " + LocalUtils.formatHex04(info.getPid()) + "\n");
   builder.append("Uid: " + info.getUid() + "\n");
   builder.append("SN: " + info.getSerialNumber() + "\n");
   String firmwareVersion = info.getFirmwareVersion();
   builder.append("FirmwareVersion: " + firmwareVersion + "\n");
   String hardwareVersion = info.getHardwareVersion();
   builder.append("HardwareVersion: " + hardwareVersion + "\n");

   // 7.遍历当前设备的传感器
   for (Sensor sensor : device.querySensors()) {
       // 8.查询传感器类型
       builder.append("Sensor : type = " + sensor.getType() + "\n");
   }
   runOnUiThread(() -> txtInfo.setText(builder.toString()));

   // 9.释放设备信息
   info.close();

遍历传感器类型

.. code:: java

   for (Sensor sensor : device.querySensors()) {
       // 查询传感器类型
       Log.d(TAG,"Sensor : type = " + sensor.getType() + "\n");
   }

资源释放

.. code:: java

   // 关闭设备
   device.close();

   // 释放SDK
   if (null != mOBContext) {
       mOBContext.close();
       mOBContext = null;
   }

彩色示例-ColorViewer
--------------------

功能描述：本示例主要演示了SDK的初始化、设备创建、Pipeline的初始化及配置以及通过Pipeline开启指定配置彩色流并渲染。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.创建Device，并通过Device初始化Pipeline
                   mDevice = deviceList.getDevice(0);
                   mPipeline = new Pipeline(mDevice);

                   // 3.创建Pipeline配置
                   Config config = new Config();

                   // 4.获取彩色流配置，并配置到Config，这里配置为640x480 YUYV格式或者是640x480 I420格式
                   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
                   StreamProfile streamProfile = null;
                   if (null != colorProfileList) {
                       streamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.YUYV, 0);

                       if (null == streamProfile) {
                           streamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.I420, 0);
                       }

                       colorProfileList.close();
                   }

                   if (streamProfile == null) {
                       return;
                   }

                   // 5.如果配置格式为I420,则创建格式转换filter，用于将I420格式转换为RGB888格式来进行渲染
                   if (streamProfile.getFormat() == Format.I420) {
                       mFormatConvertFilter = new FormatConvertFilter();
                       mFormatConvertFilter.setFormatType(FormatConvertType.FORMAT_I420_TO_RGB888);
                   }

                   // 6.使能彩色配置
                   config.enableStream(streamProfile);

                   // 7.设置镜像
                   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_COLOR_MIRROR_BOOL, PermissionType.OB_PERMISSION_WRITE)) {
                       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_COLOR_MIRROR_BOOL, true);
                   }

                   // 8.开流
                   mPipeline.start(config);

                   // 9.释放config
                   config.close();

                   // 10.释放streamProfile
                   streamProfile.close();

                   // 11.创建获取Pipeline数据线程
                   start();
               }

               // 12.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.创建Device，并通过Device初始化Pipeline
   mDevice = deviceList.getDevice(0);
   mPipeline = new Pipeline(mDevice);

创建Pipeline配置

.. code:: java

   // 3.创建Pipeline配置
   Config config = new Config();

获取彩色流配置，并配置到Config，这里配置为640x480 YUYV格式或者是640x480
I420格式

.. code:: java

   // 4.获取彩色流配置，并配置到Config，这里配置为640x480 YUYV格式或者是640x480 I420格式
   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
   StreamProfile streamProfile = null;
   if (null != colorProfileList) {
       streamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.YUYV, 0);

       if (null == streamProfile) {
           streamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.I420, 0);
       }

       colorProfileList.close();
   }

   if (streamProfile == null) {
       return;
   }

   // 5.如果配置格式为I420,则创建格式转换filter，用于将I420格式转换为RGB888格式来进行渲染
   if (streamProfile.getFormat() == Format.I420) {
       mFormatConvertFilter = new FormatConvertFilter();
       mFormatConvertFilter.setFormatType(FormatConvertType.FORMAT_I420_TO_RGB888);
   }

   // 6.使能彩色配置
   config.enableStream(streamProfile);

通过Device属性接口设置镜像

.. code:: java

   // 7.设置镜像
   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_COLOR_MIRROR_BOOL, PermissionType.OB_PERMISSION_WRITE)) {
       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_COLOR_MIRROR_BOOL, true);
   }

使用Config配置通过Pipeline开流，并释放局部资源

.. code:: java

   // 8.开流
   mPipeline.start(config);

   // 9.释放config
   config.close();

   // 10.释放streamProfile
   streamProfile.close();

创建获取Pipeline数据线程

.. code:: java

   private void start() {
       mIsStreamRunning = true;
       if (null == mStreamThread) {
           mStreamThread = new Thread(mStreamRunnable);
           mStreamThread.start();
       }
   }

阻塞方式获取数据集，等待100ms后如果获取不到，则超时

.. code:: java

   FrameSet frameSet = mPipeline.waitForFrameSet(100);

数据处理及渲染

.. code:: java

   // 获取彩色流数据
   ColorFrame colorFrame = frameSet.getColorFrame();

   // 数据处理及渲染
   if (null != colorFrame) {
       if (colorFrame.getFormat() == Format.I420) {
           // 通过filter将I420格式的彩色数据帧转换为RGB888格式的数据帧
           Frame frame = mFormatConvertFilter.process(colorFrame);
           if (null != frame) {
               // 获取数据并进行渲染
               byte[] frameData = new byte[colorFrame.getWidth() * colorFrame.getHeight() * 3];
               frame.getData(frameData);
               mColorView.update(frameData, colorFrame.getWidth(), colorFrame.getHeight(), StreamType.COLOR, Format.RGB888);
               frame.close();
           }
       } else {
           // 获取数据并进行渲染
           byte[] frameData = new byte[colorFrame.getDataSize()];
           colorFrame.getData(frameData);
           mColorView.update(frameData, colorFrame.getWidth(), colorFrame.getHeight(), StreamType.COLOR, colorFrame.getFormat());
       }

       // 释放彩色数据帧
       colorFrame.close();
   }
   // 释放数据集
   frameSet.close();

停止获取Pipeline数据

.. code:: java

   private void stop() {
       mIsStreamRunning = false;
       if (null != mStreamThread) {
           try {
               mStreamThread.join(300);
           } catch (InterruptedException e) {
           }
           mStreamThread = null;
       }
   }

资源释放

.. code:: java

   // 释放filter资源
   if (null != mFormatConvertFilter) {
       try {
           mFormatConvertFilter.close();
       } catch (Exception e) {
           e.printStackTrace();
       }

   }

   // 停止Pipeline，并释放
   if (null != mPipeline) {
       mPipeline.stop();
       mPipeline.close();
   }

   // 释放Device
   if (mDevice != null) {
       mDevice.close();
   }

   // 释放SDK
   if (null != mOBContext) {
       mOBContext.close();
   }

   // 释放渲染窗口资源
   if (mColorView != null) {
       mColorView.release();
   }

深度示例-DepthViewer
--------------------

功能描述：本示例主要演示了SDK的初始化、设备创建、Pipeline的初始化及配置以及通过Pipeline开启指定配置深度流并渲染。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.创建Device, 并通过Device初始化Pipeline
                   mDevice = deviceList.getDevice(0);
                   mPipeline = new Pipeline(mDevice);

                   // 3.创建Pipeline配置
                   Config config = new Config();

                   // 4.获取深度流配置，并配置到Config,这里配置的为640x480 Y16格式
                   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
                   StreamProfile streamProfile = null;

                   if (null != depthProfileList) {
                       streamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
                       depthProfileList.close();
                   }

                   if (streamProfile == null) {
                       return;
                   }

                   // 5.使能深度流配置
                   config.enableStream(streamProfile);

                   // 6.设置镜像
                   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, PermissionType.OB_PERMISSION_WRITE)) {
                       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, true);
                   }

                   // 7.开流
                   mPipeline.start(config);

                   // 8.释放config
                   config.close();

                   // 9.释放streamProfile
                   streamProfile.close();

                   // 10.创建获取Pipeline数据线程
                   start();
               }
               // 11.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.创建Device, 并通过Device初始化Pipeline
   mDevice = deviceList.getDevice(0);
   mPipeline = new Pipeline(mDevice);

创建Pipeline配置

.. code:: java

   Config config = new Config();

获取深度流配置，并配置到Config，这里配置的为640x480 Y16格式

.. code:: java

   // 4.获取深度流配置，并配置到Config,这里配置的为640x480 Y16格式
   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
   StreamProfile streamProfile = null;

   if (null != depthProfileList) {
       streamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
       depthProfileList.close();
   }

   if (streamProfile == null) {
       return;
   }

   // 5.使能深度流配置
   config.enableStream(streamProfile);

通过Device属性接口设置镜像

.. code:: java

   // 6.设置镜像
   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, PermissionType.OB_PERMISSION_WRITE)) {
       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, true);
   }

使用Config配置通过Pipeline开流，并释放局部资源

.. code:: java

   mPipeline.start(config);

   // 8.释放config
   config.close();

   // 9.释放streamProfile
   streamProfile.close();

创建获取Pipeline数据线程

.. code:: java

   private void start() {
       mIsStreamRunning = true;
       if (null == mStreamThread) {
           mStreamThread = new Thread(mStreamRunnable);
           mStreamThread.start();
       }
   }

阻塞方式获取数据集，等待100ms后如果获取不到，则超时

.. code:: java

   // 等待100ms后如果获取不到，则超时
   FrameSet frameSet = mPipeline.waitForFrameSet(100);

数据处理及渲染

.. code:: java

   // 获取深度流数据
   DepthFrame frame = frameSet.getDepthFrame();

   if (frame != null) {
       // 获取深度数据并进行渲染
       byte[] frameData = new byte[frame.getDataSize()];
       frame.getData(frameData);
       mDepthView.update(frameData, frame.getWidth(), frame.getHeight(), StreamType.DEPTH, frame.getFormat());

       // 释放深度数据帧
       frame.close();
   }

   // 释放数据集
   frameSet.close();

停止获取Pipeline数据

.. code:: java

   private void stop() {
       mIsStreamRunning = false;
       if (null != mStreamThread) {
           try {
               mStreamThread.join(300);
           } catch (InterruptedException e) {
           }
           mStreamThread = null;
       }
   }

资源释放

.. code:: java

   // 停止Pipeline，并释放
   if (null != mPipeline) {
       mPipeline.stop();
       mPipeline.close();
   }

   // 释放Device
   if (mDevice != null) {
       mDevice.close();
   }

   // 释放SDK
   if (null != mOBContext) {
       mOBContext.close();
   }

   // 释放渲染窗口资源
   if (mDepthView != null) {
       mDepthView.release();
   }

红外示例-InfraredViewer
-----------------------

功能描述：本示例主要演示了SDK的初始化、设备创建、Pipeline的初始化及配置以及通过Pipeline开启指定配置红外流并渲染。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.创建Device, 并通过Device初始化Pipeline
                   mDevice = deviceList.getDevice(0);
                   mPipeline = new Pipeline(mDevice);

                   // 3.创建Pipeline配置
                   Config config = new Config();

                   // 4.获取流配置，并配置到Config
                   StreamProfileList irProfileList = mPipeline.getStreamProfileList(SensorType.IR);
                   StreamProfile streamProfile = null;
                   if (null != irProfileList) {
                       streamProfile = irProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
                       irProfileList.close();
                   }

                   if (streamProfile == null) {
                       return;
                   }

                   // 5.使能红外流配置
                   config.enableStream(streamProfile);

                   // 6.设置镜像
                   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_IR_MIRROR_BOOL, PermissionType.OB_PERMISSION_READ)) {
                       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_IR_MIRROR_BOOL, true);
                   }

                   // 7.开流
                   mPipeline.start(config);

                   // 8.释放config
                   config.close();

                   // 9.释放streamProfile
                   streamProfile.close();

                   // 10.创建获取Pipeline数据线程
                   start();
               }
               // 11.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.创建Device, 并通过Device初始化Pipeline
   mDevice = deviceList.getDevice(0);
   mPipeline = new Pipeline(mDevice);

创建Pipeline配置

.. code:: java

   // 3.创建Pipeline配置
   Config config = new Config();

获取红外流配置，并配置到Config，这里配置的为640x480 Y16格式

.. code:: java

   // 4.获取流配置，并配置到Config
   StreamProfileList irProfileList = mPipeline.getStreamProfileList(SensorType.IR);
   StreamProfile streamProfile = null;
   if (null != irProfileList) {
       streamProfile = irProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
       irProfileList.close();
   }

   if (streamProfile == null) {
       return;
   }

   // 5.使能红外流配置
   config.enableStream(streamProfile);

通过Device属性接口设置镜像

.. code:: java

   // 6.设置镜像
   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_IR_MIRROR_BOOL, PermissionType.OB_PERMISSION_READ)) {
       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_IR_MIRROR_BOOL, true);
   }

使用Config配置通过Pipeline开流，并释放局部资源

.. code:: java

   // 7.开流
   mPipeline.start(config);

   // 8.释放config
   config.close();

   // 9.释放streamProfile
   streamProfile.close();

创建获取Pipeline数据线程

.. code:: java

   private void start() {
       mIsStreamRunning = true;
       if (null == mStreamThread) {
           mStreamThread = new Thread(mStreamRunnable);
           mStreamThread.start();
       }
   }

阻塞方式获取数据集，等待100ms后如果获取不到，则超时

.. code:: java

   // 等待100ms后如果获取不到，则超时
   FrameSet frameSet = mPipeline.waitForFrameSet(100);

数据处理及渲染

.. code:: java

   // 获取红外流数据
   IRFrame frame = frameSet.getIrFrame();

   if (frame != null) {
       // 获取红外数据并进行渲染
       byte[] frameData = new byte[frame.getDataSize()];
       frame.getData(frameData);
       mIrView.update(frameData, frame.getWidth(), frame.getHeight(), StreamType.IR, frame.getFormat());

       // 释放红外数据帧
       frame.close();
   }

   // 释放数据集
   frameSet.close();

停止获取Pipeline数据

.. code:: java

   private void stop() {
       mIsStreamRunning = false;
       if (null != mStreamThread) {
           try {
               mStreamThread.join(300);
           } catch (InterruptedException e) {
           }
           mStreamThread = null;
       }
   }

资源释放

.. code:: java

   // 停止Pipeline，并释放
   if (null != mPipeline) {
       mPipeline.stop();
       mPipeline.close();
   }

   if (null != mDevice) {
       mDevice.close();
   }

   // 释放SDK
   if (null != mOBContext) {
       mOBContext.close();
   }

   // 释放渲染窗口资源
   if (null != mIrView) {
       mIrView.release();
   }

流对齐示例-SyncAlignViewer
--------------------------

功能描述：本示例主要演示了对数据流控制对齐的操作。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(this, new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.获取Device并通过Device创建Pipeline
                   mDevice = deviceList.getDevice(0);
                   mPipeline = new Pipeline(mDevice);

                   // 3.创建格式转换filter，用于将I420格式的数据转换为RGB888格式
                   mFormatConvertFilter = new FormatConvertFilter();
                   mFormatConvertFilter.setFormatType(FormatConvertType.FORMAT_I420_TO_RGB888);

                   // 4.创建Pipeline配置
                   mConfig = new Config();

                   // 5.默认关闭D2C
                   mConfig.setAlignMode(AlignMode.ALIGN_D2C_DISABLE);

                   // 6.获取指定格式的彩色流配置
                   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
                   StreamProfile colorStreamProfile = null;
                   if (null != colorProfileList) {
                       colorStreamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.YUYV, 0);
                       if (null == colorStreamProfile) {
                           colorStreamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.I420, 0);
                       }
                       colorProfileList.close();
                   }

                   if (colorStreamProfile == null) {
                       return;
                   }

                   // 7.使能彩色流配置
                   mConfig.enableStream(colorStreamProfile);

                   // 8.获取深度流配置
                   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
                   StreamProfile depthStreamProfile = null;
                   if (null != depthProfileList) {
                       depthStreamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
                       depthProfileList.close();
                   }

                   if (depthStreamProfile == null) {
                       return;
                   }

                   // 9.使能深度流配置
                   mConfig.enableStream(depthStreamProfile);

                   // 10.通过config开流
                   mPipeline.start(mConfig);

                   // 11.释放colorStreamProfile
                   colorStreamProfile.close();

                   // 12.释放depthStreamProfile
                   depthStreamProfile.close();

                   // 13.创建获取Pipeline数据线程
                   start();
               }

               // 14.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.获取Device并通过Device创建Pipeline
   mDevice = deviceList.getDevice(0);
   mPipeline = new Pipeline(mDevice);

创建Pipeline配置

.. code:: java

   mConfig = new Config();

创建格式转换filter，用于将I420格式的数据转换为RGB888格式

.. code:: java

   // 3.创建格式转换filter，用于将I420格式的数据转换为RGB888格式
   mFormatConvertFilter = new FormatConvertFilter();
   mFormatConvertFilter.setFormatType(FormatConvertType.FORMAT_I420_TO_RGB888);

创建Pipeline配置

.. code:: java

   // 4.创建Pipeline配置
   mConfig = new Config();

默认关闭D2C

.. code:: java

   // 5.默认关闭D2C
   mConfig.setAlignMode(AlignMode.ALIGN_D2C_DISABLE);

获取彩色流配置，并配置到Config，这里配置为640x480 YUYV格式或者是640x480
I420格式

.. code:: java

   // 6.获取指定格式的彩色流配置
   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
   StreamProfile colorStreamProfile = null;
   if (null != colorProfileList) {
       colorStreamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.YUYV, 0);
       if (null == colorStreamProfile) {
           colorStreamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.I420, 0);
       }
       colorProfileList.close();
   }

   if (colorStreamProfile == null) {
       return;
   }

   // 7.使能彩色流配置
   mConfig.enableStream(colorStreamProfile);

获取深度流配置，并配置到Config，这里配置为640x480 Y16格式

.. code:: java

   // 8.获取深度流配置
   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
   StreamProfile depthStreamProfile = null;
   if (null != depthProfileList) {
       depthStreamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
       depthProfileList.close();
   }

   if (depthStreamProfile == null) {
       return;
   }

   // 9.使能深度流配置
   mConfig.enableStream(depthStreamProfile);

使用Config配置通过Pipeline开流，并释放局部资源

.. code:: java

   // 10.通过config开流
   mPipeline.start(mConfig);

   // 11.释放colorStreamProfile
   colorStreamProfile.close();

   // 12.释放depthStreamProfile
   depthStreamProfile.close();

创建获取Pipeline数据线程

.. code:: java

   private void start() {
       mIsStreamRunning = true;
       if (null == mStreamThread) {
           mStreamThread = new Thread(mStreamRunnable);
           mStreamThread.start();
       }
   }

阻塞方式获取数据集，等待100ms后如果获取不到，则超时

.. code:: java

   // 等待100ms后如果获取不到，则超时
   FrameSet frameSet = mPipeline.waitForFrameSet(100);

获取彩色数据及深度数据

.. code:: java

   // 获取深度流数据
   DepthFrame depthFrame = frameSet.getDepthFrame();

   // 获取彩色流数据
   ColorFrame colorFrame = frameSet.getColorFrame();

数据处理及渲染，并释放局部资源

.. code:: java

   if (null != depthFrame && null != colorFrame) {
       //深度转RGB888
       ByteBuffer depthSrcBytebuffer = ByteBuffer.allocateDirect(depthFrame.getDataSize());
       depthSrcBytebuffer.rewind();
       byte[] depthSrcBytes = new byte[depthFrame.getDataSize()];
       depthFrame.getData(depthSrcBytes);
       depthSrcBytebuffer.put(depthSrcBytes);

       ByteBuffer depthDstBytebuffer = ByteBuffer.allocateDirect(depthFrame.getWidth() * depthFrame.getHeight() * 3);
       YuvUtil.depth2RGB888(depthSrcBytebuffer, depthDstBytebuffer, depthFrame.getWidth(), depthFrame.getHeight(), 1);
       depthData = new byte[depthFrame.getWidth() * depthFrame.getHeight() * 3];
       depthDstBytebuffer.get(depthData, 0, depthData.length);

       if (colorFrame.getFormat() == Format.I420) {
           // I420格式数据转RGB888
           Frame frame = mFormatConvertFilter.process(colorFrame);
           if (null != frame) {
               colorData = new byte[colorFrame.getWidth() * colorFrame.getHeight() * 3];
               frame.getData(colorData);

               // 释放数据帧
               frame.close();
           }
       } else {
           // YUYV格式数据转RGB888
           ByteBuffer colorSrcBytebuffer = ByteBuffer.allocateDirect(colorFrame.getDataSize());
           colorSrcBytebuffer.rewind();
           byte[] colorSrcBytes = new byte[colorFrame.getDataSize()];
           colorFrame.getData(colorSrcBytes);
           colorSrcBytebuffer.put(colorSrcBytes);

           ByteBuffer colorDstBytebuffer = ByteBuffer.allocateDirect(colorFrame.getWidth() * colorFrame.getHeight() * 3);
           YuvUtil.yuyv2Rgb888(colorSrcBytebuffer, colorDstBytebuffer, colorFrame.getWidth() * colorFrame.getHeight() * 3);
           colorData = new byte[colorFrame.getWidth() * colorFrame.getHeight() * 3];
           colorDstBytebuffer.get(colorData, 0, colorData.length);
       }
   }

   if (depthData != null && colorData != null) {
       byte[] depthColorData = depthToColor(depthData, colorData, 640, 480, mAlpha);
       mColorView.update(depthColorData, 640, 480, StreamType.COLOR, Format.RGB888);
   }

   // 释放深度帧
   if (null != depthFrame) {
       depthFrame.close();
   }

   // 释放彩色帧
   if (null != colorFrame) {
       colorFrame.close();
   }

   // 释放数据集
   frameSet.close();

设置帧同步

.. code:: java

   // 设置帧同步
   private void setSync(boolean isChecked) {
       try {
           if (isChecked) {
               mPipeline.enableFrameSync();
           } else {
               mPipeline.disableFrameSync();
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

设置D2C

.. code:: java

   // 设置D2C
   private void setAlignToColor(boolean isChecked, boolean isHardware) {
       try {
           if (mPipeline == null || mDevice == null) {
               return;
           }
           try {
               if (isChecked) {
                   mConfig.setAlignMode((isHardware ? AlignMode.ALIGN_D2C_HW_ENABLE : AlignMode.ALIGN_D2C_SW_ENABLE));
               } else {
                   mConfig.setAlignMode(AlignMode.ALIGN_D2C_DISABLE);
               }
               mPipeline.switchConfig(mConfig);
           } catch (Exception e) {
               Log.w(TAG, "setAlignToColor: " + e.getMessage());
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

停止获取Pipeline数据

.. code:: java

   private void stop() {
       mIsStreamRunning = false;
       if (null != mStreamThread) {
           try {
               mStreamThread.join(300);
           } catch (InterruptedException e) {
           }
       }
   }

资源释放

.. code:: java

   // 停止Pipeline，并关闭
   if (null != mPipeline) {
       mPipeline.stop();
       mPipeline.close();
   }

   // 释放格式转换filter
   if (null != mFormatConvertFilter) {
       try {
           mFormatConvertFilter.close();
       } catch (Exception e) {
       }
       mFormatConvertFilter = null;
   }

   // 释放Config
   if (null != mConfig) {
       mConfig.close();
   }

   // 释放Device
   if (null != mDevice) {
       mDevice.close();
   }

   // 释放SDK
   if (null != mOBContext) {
       mOBContext.close();
   }

   // 释放渲染窗口资源
   if (null != mColorView) {
       mColorView.release();
   }

热拔插示例-HotPlugin
--------------------

功能描述：本示例主要演示设备拔插回调的设置，以及拔插之后处理获取到的流

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

定义Color的FrameCallback

.. code:: java

   private FrameCallback mColorFrameCallback = frame -> {
       printFrameInfo(frame.as(FrameType.COLOR), mColorFps);

       // 释放frame资源
       frame.close();
   };

定义Depth的FrameCallback

.. code:: java

   private FrameCallback mColorFrameCallback = frame -> {
       printFrameInfo(frame.as(FrameType.COLOR), mColorFps);

       // 释放frame资源
       frame.close();
   };

定义IR的FrameCallback

.. code:: java

   private FrameCallback mIrFrameCallback = frame -> {
       printFrameInfo(frame.as(FrameType.IR), mIrFps);

       // 释放frame资源
       frame.close();
   };

打印数据帧信息

.. code:: java

   private void printFrameInfo(VideoFrame frame, int fps) {
       try {
           String frameInfo = "FrameType:" + frame.getStreamType()
                   + ", index:" + frame.getFrameIndex()
                   + ", width:" + frame.getWidth()
                   + ", height:" + frame.getHeight()
                   + ", format:" + frame.getFormat()
                   + ", fps:" + fps
                   + ", timeStampUs:" + frame.getTimeStampUs();
           if (frame.getStreamType() == FrameType.DEPTH) {
               frameInfo += ", middlePixelValue:" + getMiddlePixelValue(frame);
           }
           Log.i(TAG, frameInfo);
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (deviceList == null || deviceList.getDeviceCount() <= 0) {
                   setText(mNameTv, "No device connected !");
               }

               // 2.创建设备，并获取设备名称
               mDevice = deviceList.getDevice(0);
               DeviceInfo devInfo = mDevice.getInfo();
               String deviceName = devInfo.getName();
               setText(mNameTv, deviceName);
               devInfo.close();

               // 3.获取深度传感器
               mDepthSensor = mDevice.getSensor(SensorType.DEPTH);

               // 4.打开深度流，profile传入null，表示使用配置文件中配置的参数开流，
               // 如果设备中没有该配置，或不存在配置文件，则表示使用Profile列表中的第一个配置
               if (null != mDepthSensor) {
                   mDepthSensor.start(null, mDepthFrameCallback);
               }

               // 5.获取彩色传感器
               mColorSensor = mDevice.getSensor(SensorType.COLOR);

               // 6.打开彩色流，profile传入null，表示使用配置文件中配置的参数开流，
               // 如果设备中没有该配置，或不存在配置文件，则表示使用Profile列表中的第一个配置
               if (null != mColorSensor) {
                   mColorSensor.start(null, mColorFrameCallback);
               }

               // 7.获取红外传感器
               mIrSensor = mDevice.getSensor(SensorType.IR);

               // 8.打开红外流，profile传入null，表示使用配置文件中配置的参数开流，
               // 如果设备中没有该配置，或不存在配置文件，则表示使用Profile列表中的第一个配置
               if (null != mIrSensor) {
                   mIrSensor.start(null, mIrFrameCallback);
               }

               // 9.更新开流配置信息
               setText(mProfileInfoTv, formatProfileInfo());

               // 10.释放deviceList资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               setText(mNameTv, "No device connected !");
               setText(mProfileInfoTv, "");

               mDepthFps = 0;
               mColorFps = 0;
               mIrFps = 0;

               // 停止深度流
               if (null != mDepthSensor) {
                   mDepthSensor.stop();
               }

               // 停止彩色流
               if (null != mColorSensor) {
                   mColorSensor.stop();
               }

               // 停止红外流
               if (null != mIrSensor) {
                   mIrSensor.stop();
               }

               // 释放Device
               if (null != mDevice) {
                   mDevice.close();
                   mDevice = null;
               }

               // 释放deviceList
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

设备连接回调（onDeviceAttach）中的处理

.. code:: java

   try {
       if (deviceList == null || deviceList.getDeviceCount() <= 0) {
           setText(mNameTv, "No device connected !");
       }

       // 2.创建设备，并获取设备名称
       mDevice = deviceList.getDevice(0);
       DeviceInfo devInfo = mDevice.getInfo();
       String deviceName = devInfo.getName();
       setText(mNameTv, deviceName);
       devInfo.close();

       // 3.获取深度传感器
       mDepthSensor = mDevice.getSensor(SensorType.DEPTH);

       // 4.打开深度流，profile传入null，表示使用配置文件中配置的参数开流，
       // 如果设备中没有该配置，或不存在配置文件，则表示使用Profile列表中的第一个配置
       if (null != mDepthSensor) {
           mDepthSensor.start(null, mDepthFrameCallback);
       }

       // 5.获取彩色传感器
       mColorSensor = mDevice.getSensor(SensorType.COLOR);

       // 6.打开彩色流，profile传入null，表示使用配置文件中配置的参数开流，
       // 如果设备中没有该配置，或不存在配置文件，则表示使用Profile列表中的第一个配置
       if (null != mColorSensor) {
           mColorSensor.start(null, mColorFrameCallback);
       }

       // 7.获取红外传感器
       mIrSensor = mDevice.getSensor(SensorType.IR);

       // 8.打开红外流，profile传入null，表示使用配置文件中配置的参数开流，
       // 如果设备中没有该配置，或不存在配置文件，则表示使用Profile列表中的第一个配置
       if (null != mIrSensor) {
           mIrSensor.start(null, mIrFrameCallback);
       }

       // 9.更新开流配置信息
       setText(mProfileInfoTv, formatProfileInfo());

       // 10.释放deviceList资源
       deviceList.close();
   } catch (Exception e) {
       e.printStackTrace();
   }

设备断开连接回调（onDeviceDetach）中的处理

.. code:: java

   try {
       setText(mNameTv, "No device connected !");
       setText(mProfileInfoTv, "");

       mDepthFps = 0;
       mColorFps = 0;
       mIrFps = 0;

       // 停止深度流
       if (null != mDepthSensor) {
           mDepthSensor.stop();
       }

       // 停止彩色流
       if (null != mColorSensor) {
           mColorSensor.stop();
       }

       // 停止红外流
       if (null != mIrSensor) {
           mIrSensor.stop();
       }

       // 释放Device
       if (null != mDevice) {
           mDevice.close();
           mDevice = null;
       }

       // 释放deviceList
       deviceList.close();
   } catch (Exception e) {
       e.printStackTrace();
   }

资源释放

.. code:: java

   try {
       // 停止深度流
       if (null != mDepthSensor) {
           mDepthSensor.stop();
       }

       // 停止彩色流
       if (null != mColorSensor) {
           mColorSensor.stop();
       }

       // 停止红外流
       if (null != mIrSensor) {
           mIrSensor.stop();
       }

       // 释放Device
       if (null != mDevice) {
           mDevice.close();
       }

       // 释放SDK
       if (null != mOBContext) {
           mOBContext.close();
       }
   } catch (Exception e) {
       e.printStackTrace();
   }

IMU示例-IMU
-----------

功能描述：本示例主要演示了使用SDK获取IMU数据并输出显示。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

定义IMU相关的sensor

.. code:: java

   // 加速度计传感器
   private AccelFrame mAccelFrame;
   // 陀螺仪传感器
   private GyroFrame mGyroFrame;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (deviceList == null || deviceList.getDeviceCount() == 0) {
                   showToast("请接入设备");
               } else {
                   // 2.创建Device
                   mDevice = deviceList.getDevice(0);

                   // 3.通过Device获取加速度Sensor
                   mSensorAccel = mDevice.getSensor(SensorType.ACCEL);

                   // 4.通过Device获取陀螺仪Sensor
                   mSensorGyro = mDevice.getSensor(SensorType.GYRO);

                   if (mSensorAccel == null || mSensorGyro == null) {
                       showToast("本设备不支持IMU");
                       deviceList.close();
                       return;
                   } else {
                       runOnUiThread(() -> {
                           mSurfaceViewImu.setVisibility(View.VISIBLE);
                       });
                   }

                   if (mSensorAccel != null && mSensorGyro != null) {
                       // 5.获取加速度计配置
                       StreamProfileList accelProfileList = mSensorAccel.getStreamProfileList();
                       if (null != accelProfileList) {
                           mAccelStreamProfile = accelProfileList.getStreamProfile(0).as(StreamType.ACCEL);
                           accelProfileList.close();
                       }

                       // 6.获取陀螺仪配置
                       StreamProfileList gyroProfileList = mSensorGyro.getStreamProfileList();
                       if (null != gyroProfileList) {
                           mGyroStreamProfile = gyroProfileList.getStreamProfile(0).as(StreamType.GYRO);
                           gyroProfileList.close();
                       }
                   }
               }
               // 8.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               showToast("请接入设备");
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备，并通过设备获取加速度计传感器和陀螺仪传感器

.. code:: java

   // 2.创建Device
   mDevice = deviceList.getDevice(0);

   // 3.通过Device获取加速度Sensor
   mSensorAccel = mDevice.getSensor(SensorType.ACCEL);

   // 4.通过Device获取陀螺仪Sensor
   mSensorGyro = mDevice.getSensor(SensorType.GYRO);

   if (mSensorAccel == null || mSensorGyro == null) {
       showToast("本设备不支持IMU");
       deviceList.close();
       return;
   } else {
       runOnUiThread(() -> {
           mSurfaceViewImu.setVisibility(View.VISIBLE);
       });
   }

获取加速度计开流配置以及陀螺仪开流配置

.. code:: java

   if (mSensorAccel != null && mSensorGyro != null) {
       // 5.获取加速度计配置
       StreamProfileList accelProfileList = mSensorAccel.getStreamProfileList();
       if (null != accelProfileList) {
           mAccelStreamProfile = accelProfileList.getStreamProfile(0).as(StreamType.ACCEL);
           accelProfileList.close();
       }

       // 6.获取陀螺仪配置
       StreamProfileList gyroProfileList = mSensorGyro.getStreamProfileList();
       if (null != gyroProfileList) {
           mGyroStreamProfile = gyroProfileList.getStreamProfile(0).as(StreamType.GYRO);
           gyroProfileList.close();
       }
   }

通过指定配置开流

.. code:: java


   private void startIMU() {
       // 7.1.初始化IMU数据刷新线程
       mIsRefreshIMUDataRunning = true;
       mRefreshIMUDataThread = new Thread(mRefreshIMUDataRunnable);
       mRefreshIMUDataThread.setName("RefreshIMUDataThread");

       // 7.2.开启IMU数据刷新线程
       mRefreshIMUDataThread.start();

       // 7.3.开始陀螺仪采样
       startGyroStream();

       // 7.4.开始加速度计采样
       startAccelStream();
   }

开启加速度计采样

.. code:: java

   private void startAccelStream() {
       try {
           // 开启加速度计采样
           if (null != mAccelStreamProfile) {
               mSensorAccel.start(mAccelStreamProfile, new FrameCallback() {
                   @Override
                   public void onFrame(Frame frame) {
                       AccelFrame accelFrame = frame.as(FrameType.ACCEL);

                       if (null == mAccelFrame) {
                           mAccelFrame = accelFrame;
                           return;
                       }

                       frame.close();
                   }
               });
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

开启陀螺仪采样

.. code:: java

   private void startGyroStream() {
       try {
           // 开启陀螺仪采样
           if (null != mGyroStreamProfile) {
               mSensorGyro.start(mGyroStreamProfile, new FrameCallback() {
                   @Override
                   public void onFrame(Frame frame) {
                       GyroFrame gyroFrame = frame.as(FrameType.GYRO);

                       if (null == mGyroFrame) {
                           mGyroFrame = gyroFrame;
                           return;
                       }

                       frame.close();
                   }
               });
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

创建AccelFrame和GyroFrame数据绘制方法，并在数据刷新线程实时绘制

.. code:: java

   private void drawImuInfo() {
       try {
           Canvas canvas = mSurfaceViewImu.getHolder().lockCanvas();
           if (null != canvas) {
               mPaintIMU.setTextSize(25.0f);
               Paint.FontMetrics fm = mPaintIMU.getFontMetrics();
               float offsetY = fm.descent - fm.ascent;
               float x = canvas.getWidth() / 8;
               float y = canvas.getHeight() / 8;
               canvas.drawColor(Color.WHITE);
               if (mAccelFrame != null) {
                   canvas.drawText("AccelTimestamp:" + mAccelFrame.getTimeStamp(), x, y, mPaintIMU);
                   canvas.drawText("AccelTemperature:" + mAccelFrame.getTemperature() + "dC", x, (y += offsetY), mPaintIMU);

                   canvas.drawText("Accel.x:" + mAccelFrame.getAccelData()[0] + "g", x, (y += 2 * offsetY), mPaintIMU);
                   canvas.drawText("Accel.y:" + mAccelFrame.getAccelData()[1] + "g", x, (y += offsetY), mPaintIMU);
                   canvas.drawText("Accel.z:" + mAccelFrame.getAccelData()[2] + "g", x, (y += offsetY), mPaintIMU);

                   // 释放AccelFrame资源
                   mAccelFrame.close();
                   mAccelFrame = null;
               }

               if (mGyroFrame != null) {
                   canvas.drawText("GyroTimestamp:" + mGyroFrame.getTimeStamp(), x, (y += 2 * offsetY), mPaintIMU);
                   canvas.drawText("GyroTemperature:" + mGyroFrame.getTemperature() + "dC", x, (y += offsetY), mPaintIMU);

                   canvas.drawText("Gyro.x:" + mGyroFrame.getGyroData()[0] + "dps", x, (y += 2 * offsetY), mPaintIMU);
                   canvas.drawText("Gyro.y:" + mGyroFrame.getGyroData()[1] + "dps", x, (y += offsetY), mPaintIMU);
                   canvas.drawText("Gyro.z:" + mGyroFrame.getGyroData()[2] + "dps", x, (y += offsetY), mPaintIMU);

                   // 释放GyroFrame资源
                   mGyroFrame.close();
                   mGyroFrame = null;
               }
               mSurfaceViewImu.getHolder().unlockCanvasAndPost(canvas);
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

停止数据采样

.. code:: java

   private void stopIMU() {
       try {
           // 停止加速度计采样
           if (null != mSensorAccel) {
               mSensorAccel.stop();
           }

           // 停止陀螺仪采样
           if (null != mSensorGyro) {
               mSensorGyro.stop();
           }

           // 停止IMU数据刷新线程并释放
           mIsRefreshIMUDataRunning = false;
           if (null != mRefreshIMUDataThread) {
               try {
                   mRefreshIMUDataThread.join(300);
               } catch (InterruptedException e) {
                   e.printStackTrace();
               }
               mRefreshIMUDataThread = null;
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

资源释放

::

   try {
       // 释放加速度计配置
       if (null != mAccelStreamProfile) {
           mAccelStreamProfile.close();
           mAccelStreamProfile = null;
       }

       // 释放陀螺仪配置
       if (null != mGyroStreamProfile) {
           mGyroStreamProfile.close();
           mGyroStreamProfile = null;
       }

       // 释放Device
       if (null != mDevice) {
           mDevice.close();
           mDevice = null;
       }

       // 释放SDK
       if (null != mOBContext) {
           mOBContext.close();
           mOBContext = null;
       }
   } catch (Exception e) {
       e.printStackTrace();
   }

多设备示例-MultiDevice
----------------------

功能描述：本示例主要演示了对多设备进行操作。
首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 初始化SDK
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               int count = deviceList.getDeviceCount();
               for (int i = 0; i < count; i++) {
                   // 创建设备
                   Device device = deviceList.getDevice(i);
                   // 获取DeviceInfo
                   DeviceInfo devInfo = device.getInfo();
                   // 获取设备名称
                   String name = devInfo.getName();
                   // 获取设备uid
                   String uid = devInfo.getUid();
                   // 获取设备usb接口类型
                   String usbType = devInfo.getUsbType();
                   // 释放DeviceInfo资源
                   devInfo.close();
                   runOnUiThread(() -> {
                       mDeviceControllerAdapter.addItem(new DeviceBean(name, uid, usbType, device));
                   });

               }

               // 释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               for (DeviceBean deviceBean : mDeviceBeanList) {
                   // 通过uid判断下线设备
                   if (deviceBean.getDeviceUid().equals(deviceList.getUid(0))) {
                       // 释放下线设备资源
                       deviceBean.getDevice().close();
                       runOnUiThread(() -> {
                           mDeviceControllerAdapter.deleteItem(deviceBean);
                       });
                   }
               }

               // 释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               Log.w(TAG, "onDeviceDetach: " + e.getMessage());
           }
       }
   });

在设备连接回调方法中创建设备列表

.. code:: java

   int count = deviceList.getDeviceCount();
   for (int i = 0; i < count; i++) {
       // 创建设备
       Device device = deviceList.getDevice(i);
       // 获取DeviceInfo
       DeviceInfo devInfo = device.getInfo();
       // 获取设备名称
       String name = devInfo.getName();
       // 获取设备uid
       String uid = devInfo.getUid();
       // 获取设备usb接口类型
       String usbType = devInfo.getUsbType();
       // 释放DeviceInfo资源
       devInfo.close();
       runOnUiThread(() -> {
           mDeviceControllerAdapter.addItem(new DeviceBean(name, uid, usbType, device));
       });
   }

选择对应的设备开流

.. code:: java

   private void startStream(Sensor sensor, GLView glView) {
       try {
           // 获取传感器的流配置列表
           StreamProfileList profileList = sensor.getStreamProfileList();
           if (null == profileList) {
               Log.w(TAG, "start stream failed, profileList is null !");
               return;
           }
           switch (sensor.getType()) {
               case DEPTH:
                   GLView depthGLView = glView;
                   // 通过StreamProfileList获取开流配置
                   StreamProfile depthProfile = profileList.getVideoStreamProfile(640, 480, Format.Y16, 0);
                   if (null != depthProfile) {
                       // 通过指定配置开流
                       sensor.start(depthProfile, frame -> {
                           DepthFrame depthFrame = frame.as(FrameType.DEPTH);
                           byte[] bytes = new byte[depthFrame.getDataSize()];
                           depthFrame.getData(bytes);
                           // 渲染数据
                           depthGLView.update(bytes, depthFrame.getWidth(), depthFrame.getHeight(),
                                   StreamType.DEPTH, depthFrame.getFormat());
                           // 释放frame资源
                           frame.close();
                       });
                       // 释放profile资源
                       depthProfile.close();
                   } else {
                       Log.w(TAG, "start depth stream failed, depthProfile is null!");
                   }
                   break;
               case COLOR:
                   GLView colorGLView = glView;
                   // 通过StreamProfileList获取开流配置
                   StreamProfile colorProfile = profileList.getVideoStreamProfile(640, 480, Format.RGB888, 0);

                   if (null != colorProfile) {
                       // 通过指定配置开流
                       sensor.start(colorProfile, frame -> {
                           ColorFrame colorFrame = frame.as(FrameType.COLOR);
                           byte[] bytes = new byte[colorFrame.getDataSize()];
                           // 获取frame数据
                           colorFrame.getData(bytes);
                           // 渲染数据
                           colorGLView.update(bytes, colorFrame.getWidth(), colorFrame.getHeight(), StreamType.COLOR, Format.RGB888);
                           // 释放frame资源
                           frame.close();
                       });
                       // 释放profile资源
                       colorProfile.close();
                   } else {
                       Log.w(TAG, "start color stream failed, colorProfile is null!");
                   }
                   break;
               case IR:
                   GLView irGLView = glView;
                   // 通过StreamProfileList获取开流配置
                   StreamProfile irProfile = profileList.getVideoStreamProfile(640, 480, Format.Y16, 0);
                   if (null != irProfile) {
                       // 通过指定配置开流
                       sensor.start(irProfile, frame -> {
                           IRFrame irFrame = frame.as(FrameType.IR);
                           byte[] bytes = new byte[irFrame.getDataSize()];
                           // 获取frame数据
                           irFrame.getData(bytes);
                           // 渲染数据
                           irGLView.update(bytes, irFrame.getWidth(), irFrame.getHeight(),
                                   StreamType.IR, irFrame.getFormat());
                           // frame资源
                           frame.close();
                       });
                       // 释放profile资源
                       irProfile.close();
                   } else {
                       Log.w(TAG, "start ir stream failed, irProfile is null!");
                   }
                   break;
           }

           // 释放profileList资源
           profileList.close();
       } catch (Exception e) {
           Log.w(TAG, "startStream: " + e.getMessage());
       }
   }

对指定的设备关流指定sensor流

.. code:: java

   private void stopStream(Sensor sensor) {
       try {
           sensor.stop();
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

在设备断开连接回调中做对应的资源释放并刷新设备列表

.. code:: java

   try {
       for (DeviceBean deviceBean : mDeviceBeanList) {
           // 通过uid判断下线设备
           if (deviceBean.getDeviceUid().equals(deviceList.getUid(0))) {
               // 释放下线设备资源
               deviceBean.getDevice().close();
               runOnUiThread(() -> {
                   mDeviceControllerAdapter.deleteItem(deviceBean);
               });
           }
       }
   } catch (Exception e) {
       Log.w(TAG, "onDeviceDetach: " + e.getMessage());
   }

资源释放

.. code:: java

   try {
       // 释放资源
       for (DeviceBean deviceBean : mDeviceBeanList) {
           try {
               // 释放设备资源
               deviceBean.getDevice().close();
           } catch (Exception e) {
               Log.w(TAG, "onDestroy: " + e.getMessage());
           }
       }
       mDeviceBeanList.clear();

       // 释放SDK
       if (null != mOBContext) {
           mOBContext.close();
       }
   } catch (Exception e) {
       e.printStackTrace();
     }

点云示例-PointCloud
-------------------

功能描述：本示例主要演示了连接设备开流
,生成深度点云或RGBD点云并保存成ply格式文件。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.通过deviceList获取Device
                   mDevice = deviceList.getDevice(0);

                   // 3.通过Device创建Pipeline
                   mPipeline = new Pipeline(mDevice);

                   // 4.创建Config，用于配置pipeline开流
                   Config config = new Config();

                   // 5.获取彩色配置列表，并获取640x480 YUYV或者I420格式配置开流
                   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
                   VideoStreamProfile colorProfileTarget = null;
                   if (null != colorProfileList) {
                       colorProfileTarget = colorProfileList.getVideoStreamProfile(640, 480, Format.YUYV, 0);
                       if (null == colorProfileTarget) {
                           colorProfileTarget = colorProfileList.getVideoStreamProfile(640, 480, Format.I420, 0);
                       }
                       colorProfileList.close();
                   }

                   // 6.获取到指定彩色配置后使能配置
                   if (null != colorProfileTarget) {
                       config.enableStream(colorProfileTarget);
                       colorProfileTarget.close();
                   }

                   // 7.获取深度配置列表，并获取640x480 YUYV或者Y16格式配置开流
                   VideoStreamProfile depthProfileTarget = null;
                   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
                   if (null != depthProfileList) {
                       depthProfileTarget = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 0);
                       depthProfileList.close();
                   }

                   // 8.获取到指定深度配置后使能配置
                   if (null != depthProfileTarget) {
                       config.enableStream(depthProfileTarget);
                       depthProfileTarget.close();
                   }


                   // 9.开启硬件D2C
                   config.setAlignMode(AlignMode.ALIGN_D2C_HW_ENABLE);

                   // 10.使用回调方式开流
                   mPipeline.start(config, mPointCloudFrameSetCallback);

                   // 11.启动点云异步处理线程
                   start();

                   // 12.通过Pipeline创建点云filter
                   mPointCloudFilter = new PointCloudFilter();

                   // 13.设置点云filter的格式
                   mPointCloudFilter.setPointFormat(mPointFormat);

                   // 14.获取相机内参并且将参数设置到点云filter中
                   CameraParamList cameraParamList = mDevice.getCalibrationCameraParamList();
                   CameraParam cameraParam = null;
                   if (null != cameraParamList) {
                       int count = cameraParamList.getCameraParamCount();
                       for (int i = 0; i < count; i++) {
                           cameraParam = cameraParamList.getCameraParam(i);
                           int depthW = cameraParam.getDepthIntrinsic().getWidth();
                           int depthH = cameraParam.getDepthIntrinsic().getHeight();
                           int colorW = cameraParam.getColorIntrinsic().getWidth();
                           int colorH = cameraParam.getColorIntrinsic().getHeight();
                           // 14.1.需要获取标定的分辨率比例与开流分辨率比例一致的内参
                           if ((depthW / depthH == 640 / 480) && (colorW / colorH == 640 / 480)) {
                               break;
                           }
                       }
                       cameraParamList.close();
                   }
                   mPointCloudFilter.setCameraParam(cameraParam);

                   // 15.释放config资源
                   config.close();
               }

               // 16.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.通过deviceList获取Device
   mDevice = deviceList.getDevice(0);

   // 3.通过Device创建Pipeline
   mPipeline = new Pipeline(mDevice);

创建Pipeline配置

.. code:: java

   // 4.创建Config，用于配置pipeline开流
   Config config = new Config();

获取彩色流配置，并配置到Config，这里配置为640x480 YUYV格式或者是640x480
I420格式

.. code:: java

   // 5.获取彩色配置列表，并获取640x480 YUYV或者I420格式配置开流
   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
   VideoStreamProfile colorProfileTarget = null;
   if (null != colorProfileList) {
       colorProfileTarget = colorProfileList.getVideoStreamProfile(640, 480, Format.YUYV, 0);
       if (null == colorProfileTarget) {
           colorProfileTarget = colorProfileList.getVideoStreamProfile(640, 480, Format.I420, 0);
       }
       colorProfileList.close();
   }

   // 6.获取到指定彩色配置后使能配置
   if (null != colorProfileTarget) {
       config.enableStream(colorProfileTarget);
       colorProfileTarget.close();
   }

获取深度流配置，并配置到Config，这里配置为640x480 YUYV格式或者是Y16格式

.. code:: java

   // 7.获取深度配置列表，并获取640x480 YUYV或者Y16格式配置开流
   VideoStreamProfile depthProfileTarget = null;
   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
   if (null != depthProfileList) {
       depthProfileTarget = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 0);
       depthProfileList.close();
   }

   // 8.获取到指定深度配置后使能配置
   if (null != depthProfileTarget) {
       config.enableStream(depthProfileTarget);
       depthProfileTarget.close();
   }

判断设备是否支持硬件D2C并开启

::

   // 9.开启硬件D2C
   config.setAlignMode(AlignMode.ALIGN_D2C_HW_ENABLE);

创建Pipeline开流的回调方法

.. code:: java

   // Pipeline开流的回调
   private FrameSetCallback mPointCloudFrameSetCallback = frameSet -> {
       if (null != frameSet) {
           if (mIsPointCloudRunning) {
               if (null == mPointFrameSet) {
                   mPointFrameSet = frameSet;
                   return;
               }
           }

           frameSet.close();
       }
   };

使用Config配置通过Pipeline回调方式开流

.. code:: java

   // 10.使用回调方式开流
   mPipeline.start(config, mPointCloudFrameSetCallback);

启动点云异步处理线程

.. code:: java

   private void start() {
       mIsPointCloudRunning = true;
       if (null == mPointFilterThread) {
           mPointFilterThread = new Thread(mPointFilterRunnable);
           mPointFilterThread.start();
       }
   }

通过Pipeline创建点云filter，并设置点云filter的格式（如果需要保存深度点云，则需要将格式设置为Format.\ *POINT*\ ，如果需要保存RGBD点云则需要将格式设置为Format.\ *RGB_POINT*\ ）

.. code:: java

   // 12.通过Pipeline创建点云filter
   mPointCloudFilter = new PointCloudFilter();

   // 13.设置点云filter的格式
   mPointCloudFilter.setPointFormat(mPointFormat);

获取相机内参并将参数设置到点云filter中

.. code:: java

   // 14.获取相机内参并且将参数设置到点云filter中
   CameraParamList cameraParamList = mDevice.getCalibrationCameraParamList();
   CameraParam cameraParam = null;
   if (null != cameraParamList) {
       int count = cameraParamList.getCameraParamCount();
       for (int i = 0; i < count; i++) {
           cameraParam = cameraParamList.getCameraParam(i);
           int depthW = cameraParam.getDepthIntrinsic().getWidth();
           int depthH = cameraParam.getDepthIntrinsic().getHeight();
           int colorW = cameraParam.getColorIntrinsic().getWidth();
           int colorH = cameraParam.getColorIntrinsic().getHeight();
           // 14.1.需要获取标定的分辨率比例与开流分辨率比例一致的内参
           if ((depthW / depthH == 640 / 480) && (colorW / colorH == 640 / 480)) {
               break;
           }
       }
       cameraParamList.close();
   }
   mPointCloudFilter.setCameraParam(cameraParam);

点云filter线程数据处理

.. code:: java

   while (mIsPointCloudRunning) {
       try {
           if (null != mPointFrameSet) {
               Frame frame = null;
               if (mPointFormat == Format.POINT) {
                   // 设置保存格式为深度点云
                   mPointCloudFilter.setPointFormat(Format.POINT);
               } else {
                   // 设置保存格式为彩色点云
                   mPointCloudFilter.setPointFormat(Format.RGB_POINT);
               }
               // 点云filter处理生成对应的点云数据
               frame = mPointCloudFilter.process(mPointFrameSet);

               if (null != frame) {
                   // 获取点云帧
                   PointFrame pointFrame = frame.as(FrameType.POINTS);

                   if (mIsSavePoints) {
                       if (mPointFormat == Format.POINT) {
                           // 获取深度点云数据并保存，深度点云的数据大小为w * h * 3
                           float[] depthPoints = new float[pointFrame.getDataSize() / Float.BYTES];
                           pointFrame.getPointCloudData(depthPoints);
                           String depthPointsPath = mSdcardDir.toString() + "/points.ply";
                           FileUtils.savePointCloud(depthPointsPath, depthPoints);
                           runOnUiThread(new Runnable() {
                               @Override
                               public void run() {
                                   mInfoTv.append("Save Path:" + depthPointsPath + "\n");
                               }
                           });
                       } else {
                           // 获取彩色点云数据并保存，彩色点云的数据大小为w * h * 6
                           float[] colorPoints = new float[pointFrame.getDataSize() / Float.BYTES];
                           pointFrame.getPointCloudData(colorPoints);
                           String colorPointsPath = mSdcardDir.toString() + "/rgb_points.ply";
                           FileUtils.saveRGBPointCloud(colorPointsPath, colorPoints);
                           runOnUiThread(new Runnable() {
                               @Override
                               public void run() {
                                   mInfoTv.append("Save Path:" + colorPointsPath + "\n");
                               }
                           });
                       }

                       mIsSavePoints = false;
                   }

                   // 释放新生成的frame
                   frame.close();
               }

               // 释放原始数据frameSet
               mPointFrameSet.close();
               mPointFrameSet = null;
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

保存深度点云数据

.. code:: java

   public static void savePointCloud(String fileName, float[] data) {
       try {
           File file = new File(fileName);
           FileOutputStream fos = new FileOutputStream(file);
           PrintWriter writer = new PrintWriter(fos);
           writer.write("ply\n");
           writer.write("format ascii 1.0\n");
           writer.write("element vertex 307200\n");
           writer.write("property float x\n");
           writer.write("property float y\n");
           writer.write("property float z\n");
           writer.write("end_header\n");
           writer.flush();

           for (int i = 0; i < data.length; i += 3) {
               writer.print(data[i]);
               writer.print(" ");
               writer.print(data[i + 1]);
               writer.print(" ");
               writer.print(data[i + 2]);
               writer.print("\n");
           }
           writer.close();
           fos.close();
       } catch (Exception e) {
           Log.e(TAG, "exception: " + e.getMessage());
       }
   }

保存彩色点云数据

.. code:: java

   public static void saveRGBPointCloud(String fileName, float[] data) {
       try {
           File file = new File(fileName);
           FileOutputStream fos = new FileOutputStream(file);
           PrintWriter writer = new PrintWriter(fos);
           writer.write("ply\n");
           writer.write("format ascii 1.0\n");
           writer.write("element vertex 307200\n");
           writer.write("property float x\n");
           writer.write("property float y\n");
           writer.write("property float z\n");
           writer.write("property uchar red\n");
           writer.write("property uchar green\n");
           writer.write("property uchar blue\n");
           writer.write("end_header\n");
           writer.flush();

           for (int i = 0; i < data.length; i += 6) {
               writer.print(data[i]);
               writer.print(" ");
               writer.print(data[i + 1]);
               writer.print(" ");
               writer.print(data[i + 2]);
               writer.print(" ");
               writer.print((int) data[i + 3]);
               writer.print(" ");
               writer.print((int) data[i + 4]);
               writer.print(" ");
               writer.print((int) data[i + 5]);
               writer.print("\n");
           }
           writer.close();
           fos.close();
       } catch (Exception e) {
           Log.e(TAG, "exception: " + e.getMessage());
       }
   }

退出点云filter处理线程

.. code:: java

   private void stop() {
       mIsPointCloudRunning = false;
       if (null != mPointFilterThread) {
           try {
               mPointFilterThread.join(300);
           } catch (InterruptedException e) {
           }
           mPointFilterThread = null;
       }
   }

资源释放

.. code:: java

   try {
       // 停止Pipeline，并关闭
       if (null != mPipeline) {
           mPipeline.stop();
           mPipeline.close();
           mPipeline = null;
       }

       // 释放点云filter
       if (null != mPointCloudFilter) {
           try {
               mPointCloudFilter.close();
           } catch (Exception e) {
           }
           mPointCloudFilter = null;
       }

       // 释放Device
       if (mDevice != null) {
           mDevice.close();
           mDevice = null;
       }

       // 释放SDK
       if (null != mOBContext) {
           mOBContext.close();
           mOBContext = null;
       }
   } catch (Exception e) {
       e.printStackTrace();
   }

存储示例-SaveToDisk
-------------------

功能描述：本示例用于演示连接设备开流 ,
获取彩色存储为png格式，深度存储为raw格式以及通过filter进行格式转换。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.创建Device, 并通过Device初始化Pipeline
                   mDevice = deviceList.getDevice(0);
                   mPipeline = new Pipeline(mDevice);

                   // 3.初始化格式转换filter
                   mFormatConvertFilter = new FormatConvertFilter();

                   // 4.创建Pipeline配置
                   Config config = new Config();

                   // 5.获取彩色流配置，并配置到Config
                   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
                   StreamProfile colorStreamProfile = null;
                   if (null != colorProfileList) {
                       colorStreamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.MJPG, 0);
                       colorProfileList.close();
                   }

                   if (null == colorStreamProfile) {
                       Log.e(TAG, "onDeviceAttach: get color stream profile failed !");
                       deviceList.close();
                       return;
                   }

                   // 6.通过获取到的彩色流配置使能彩色流
                   config.enableStream(colorStreamProfile);

                   // 7.获取深度流配置，并配置到Config
                   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
                   StreamProfile depthStreamProfile = null;
                   if (null != depthProfileList) {
                       depthStreamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 0);
                       depthProfileList.close();
                   }

                   if (null == depthStreamProfile) {
                       Log.e(TAG, "onDeviceAttach: get depth stream profile failed !");
                       deviceList.close();
                       return;
                   }

                   // 8.通过获取到的深度流配置使能深度流
                   config.enableStream(depthStreamProfile);

                   // 9.使用Config开启Pipeline
                   mPipeline.start(config);

                   // 10.释放colorStreamProfile
                   colorStreamProfile.close();

                   // 11.释放depthStreamProfile
                   depthStreamProfile.close();

                   // 12.释放config资源
                   config.close();

                   // 13.创建获取Pipeline数据线程以及图片保存线程
                   start();
               }

               // 14.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.创建Device, 并通过Device初始化Pipeline
   mDevice = deviceList.getDevice(0);
   mPipeline = new Pipeline(mDevice);

通过Pipeline初始化格式转换filter

.. code:: java

   // 3.初始化格式转换filter
   mFormatConvertFilter = new FormatConvertFilter();

创建Pipeline配置

.. code:: java

   // 4.创建Pipeline配置
   Config config = new Config();

获取彩色流配置，并配置到Config，这里配置为640x480 MJPG格式

.. code:: java

   // 5.获取彩色流配置，并配置到Config
   StreamProfileList colorProfileList = mPipeline.getStreamProfileList(SensorType.COLOR);
   StreamProfile colorStreamProfile = null;
   if (null != colorProfileList) {
       colorStreamProfile = colorProfileList.getVideoStreamProfile(640, 480, Format.MJPG, 0);
       colorProfileList.close();
   }

   if (null == colorStreamProfile) {
       Log.e(TAG, "onDeviceAttach: get color stream profile failed !");
       deviceList.close();
       return;
   }

   // 6.通过获取到的彩色流配置使能彩色流
   config.enableStream(colorStreamProfile);

获取深度流配置，并配置到Config，这里配置为640x480 Y16格式

.. code:: java

   // 7.获取深度流配置，并配置到Config
   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
   StreamProfile depthStreamProfile = null;
   if (null != depthProfileList) {
       depthStreamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 0);
       depthProfileList.close();
   }

   if (null == depthStreamProfile) {
       Log.e(TAG, "onDeviceAttach: get depth stream profile failed !");
       deviceList.close();
       return;
   }

   // 8.通过获取到的深度流配置使能深度流
   config.enableStream(depthStreamProfile);

使用Config配置通过Pipeline开流

.. code:: java

   // 9.使用Config开启Pipeline
   mPipeline.start(config);

创建获取Pipeline数据线程以及图片保存线程

.. code:: java

   private void start() {
       colorCount = 0;
       depthCount = 0;
       mIsStreamRunning = true;
       mIsPicSavingRunning = true;
       if (null == mStreamThread) {
           mStreamThread = new Thread(mStreamRunnable);
           mStreamThread.start();
       }

       if (null == mPicSavingThread) {
           mPicSavingThread = new Thread(mPicSavingRunnable);
           mPicSavingThread.start();
       }
   }

数据流处理

.. code:: java

   int count = 0;
   while (mIsStreamRunning) {
       try {
           // 等待100ms后如果获取不到，则超时
           FrameSet frameSet = mPipeline.waitForFrameSet(100);
           if (null == frameSet) {
               continue;
           }
           if (count < 5) {
               frameSet.close();
               count++;
               continue;
           }

           // 获取彩色流数据
           ColorFrame colorFrame = frameSet.getColorFrame();
           if (null != colorFrame) {
               mFormatConvertFilter.setFormatType(FormatConvertType.FORMAT_MJPG_TO_RGB888);
               Frame rgbFrame = mFormatConvertFilter.process(colorFrame);

               FrameCopy frameT = copyToFrameT(rgbFrame.as(FrameType.VIDEO));

               mFrameSaveQueue.offer(frameT);
               colorFrame.close();
               rgbFrame.close();
           }

           // 获取深度流数据
           DepthFrame depthFrame = frameSet.getDepthFrame();
           if (null != depthFrame) {
               FrameCopy frameT = copyToFrameT(depthFrame);
               mFrameSaveQueue.offer(frameT);
               depthFrame.close();
           }

           // 释放数据集
           frameSet.close();
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

数据保存

.. code:: java

   while (mIsPicSavingRunning) {
       try {
           FrameCopy frameT = mFrameSaveQueue.poll(300, TimeUnit.MILLISECONDS);
           if (null != frameT) {
               Log.d(TAG, "colorCount :" + colorCount);
               if (frameT.getStreamType() == FrameType.COLOR && colorCount < 5) {
                   FileUtils.saveImage(frameT);
                   colorCount++;
               }

               Log.d(TAG, "depthCount :" + depthCount);
               if (frameT.getStreamType() == FrameType.DEPTH && depthCount < 5) {
                   FileUtils.saveImage(frameT);
                   depthCount++;
               }
           }
       } catch (Exception e) {
       }

       if (colorCount == 5 && depthCount == 5) {
           mIsPicSavingRunning = false;
           break;
       }
   }

   mFrameSaveQueue.clear();

退出数据处理线程和存图线程

.. code:: java

   private void stop() {
       mIsStreamRunning = false;
       mIsPicSavingRunning = false;
       if (null != mStreamThread) {
           try {
               mStreamThread.join(1000);
           } catch (InterruptedException e) {
           }
           mStreamThread = null;
       }

       if (null != mPicSavingThread) {
           try {
               mPicSavingThread.join(1000);
           } catch (InterruptedException e) {
           }
           mPicSavingThread = null;
       }
   }

资源释放

::

   // 释放filter资源
   if (null != mFormatConvertFilter) {
       try {
           mFormatConvertFilter.close();
       } catch (Exception e) {
           e.printStackTrace();
       }
   }
   // 停止Pipeline，并关闭
   if (null != mPipeline) {
       mPipeline.close();
       mPipeline = null;
   }
   // 释放Device资源
   if (null != mDevice) {
       mDevice.close();
       mDevice = null;
   }
   // 释放SDK
   if (null != mOBContext) {
       mOBContext.close();
       mOBContext = null;
   }

传感器控制示例-SensorControl
----------------------------

功能描述：本示例主要演示了对device控制命令的操作、对Sensor控制命令的操作。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 2.初始化SDK
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               // 3.将获取到的设备添加到设备列表
               mDeviceList.add(deviceList.getDevice(0));

               // 4.更新设备列表
               updateDeviceSpinnerList();

               // 5.释放设备列表资源
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               // 有设备断开连接，释放设备列表资源
               for (Device device : mDeviceList) {
                   device.close();
               }
               mDeviceList.clear();

               // 重新获取设备并更新设备列表
               DeviceList curDeviceList = mOBContext.queryDevices();
               for (int i = 0; i < curDeviceList.getDeviceCount(); i++) {
                   mDeviceList.add(curDeviceList.getDevice(i));
               }
               curDeviceList.close();
               updateDeviceSpinnerList();

               // 没有设备连接，清空传感器列表和属性列表
               if (mDeviceList.size() <= 0) {
                   clearPropertySpinnerList();
               }

               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

设备连接回调方法（onDeviceAttach）处理

.. code:: java

   try {
       // 3.将获取到的设备添加到设备列表
       mDeviceList.add(deviceList.getDevice(0));

       // 4.更新设备列表
       updateDeviceSpinnerList();

       // 5.释放设备列表资源
       deviceList.close();
   } catch (Exception e) {
       e.printStackTrace();
   }

设备断开连接回调方法（onDeviceDetach）处理

.. code:: java

   try {
       // 有设备断开连接，释放设备列表资源
       for (Device device : mDeviceList) {
           device.close();
       }
       mDeviceList.clear();

       // 重新获取设备并更新设备列表
       DeviceList curDeviceList = mOBContext.queryDevices();
       for (int i = 0; i < curDeviceList.getDeviceCount(); i++) {
           mDeviceList.add(curDeviceList.getDevice(i));
       }
       curDeviceList.close();
       updateDeviceSpinnerList();

       // 没有设备连接，清空传感器列表和属性列表
       if (mDeviceList.size() <= 0) {
           clearPropertySpinnerList();
       }

       deviceList.close();
   } catch (Exception e) {
       e.printStackTrace();
   }

根据选择的设备更新属性列表

.. code:: java

   @Override
   public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
       switch (parent.getId()) {
           case R.id.spi_device:
               // 设备切换，更新传属性列表
               mSelectDevice = mDeviceList.get(position);
               updatePropertySpinnerList();

               String deviceName = mDeviceSp.getSelectedItem().toString();
               addNewMessage("选择设备名：" + deviceName);
               addNewMessage(getVersionInfo(mDeviceList.get(position)));
               // 其他case
       }
   }

   // 更新属性列表
   private void updatePropertySpinnerList() {
       runOnUiThread(new Runnable() {
           @Override
           public void run() {
               mDevicePropertyMap.clear();
               mPropertyNameList.clear();
               try {
                   List<DevicePropertyInfo> devicePropertyList = mSelectDevice.getSupportedPropertyList();
                   for (DevicePropertyInfo deviceProperty : devicePropertyList) {
                       if (deviceProperty.getPropertyType() != PropertyType.STRUCT_PROPERTY
                           && deviceProperty.getPermissionType() != PermissionType.OB_PERMISSION_DENY) {
                           mPropertyNameList.add(deviceProperty.getPropertyName());
                           mDevicePropertyMap.put(deviceProperty.getPropertyName(), deviceProperty);
                       }
                   }
                   addNewMessage("选择设备指令");
                   mPropertyAdapter.clear();
                   mPropertyAdapter.addAll(mPropertyNameList);
                   mPropertyAdapter.notifyDataSetChanged();

                   mPropertySp.setSelection(0, true);
               } catch (Exception e) {
                   e.printStackTrace();
               }
           }
       });
   }

根据选择的属性更新设置范围提示

.. code:: java

   @Override
   public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
       switch (parent.getId()) {
           case R.id.spi_instructions:
               // 指令切换
               String instructionTypeName = mPropertySp.getSelectedItem().toString();
               PermissionType permissionType = mDevicePropertyMap.get(instructionTypeName).getPermissionType();
               addNewMessage("选择操作指令：" + instructionTypeName + " 读写权限：" + permissionType);
               updateControlPanel(permissionType);
               updateSetEditTextHint(instructionTypeName);
               break;
               // 其他case
       }
   }

根据选择的设备更新属性列表的方法

.. code:: java

   private void updatePropertySpinnerList() {
       runOnUiThread(new Runnable() {
           @Override
           public void run() {
               mDevicePropertyMap.clear();
               mPropertyNameList.clear();
               try {
                   List<DevicePropertyInfo> devicePropertyList = mSelectDevice.getSupportedPropertyList();
                   for (DevicePropertyInfo deviceProperty : devicePropertyList) {
                       if (deviceProperty.getPropertyType() != PropertyType.STRUCT_PROPERTY
                           && deviceProperty.getPermissionType() != PermissionType.OB_PERMISSION_DENY) {
                           mPropertyNameList.add(deviceProperty.getPropertyName());
                           mDevicePropertyMap.put(deviceProperty.getPropertyName(), deviceProperty);
                       }
                   }
                   addNewMessage("选择设备指令");
                   mPropertyAdapter.clear();
                   mPropertyAdapter.addAll(mPropertyNameList);
                   mPropertyAdapter.notifyDataSetChanged();

                   mPropertySp.setSelection(0, true);
               } catch (Exception e) {
                   e.printStackTrace();
               }
           }
       });
   }

根据选择的属性更新属性的设置范围提示

.. code:: java

   private void updateSetEditTextHint(String instructionTypeName) {
       mSetEt.setText("");
       mGetTv.setText("");
       try {
           switch (mDevicePropertyMap.get(instructionTypeName).getPropertyType()) {
               case INT_PROPERTY:
                   int minI = mSelectDevice.getMinRangeI(mDevicePropertyMap.get(instructionTypeName).getProperty());
                   int maxI = mSelectDevice.getMaxRangeI(mDevicePropertyMap.get(instructionTypeName).getProperty());
                   mSetEt.setHint("[" + minI + "-" + maxI + "]");
                   break;
               case BOOL_PROPERTY:
                   mSetEt.setHint("[" + 0 + "-" + 1 + "]");
                   break;
               case FLOAT_PROPERTY:
                   float minF = mSelectDevice.getMinRangeF(mDevicePropertyMap.get(instructionTypeName).getProperty());
                   float maxF = mSelectDevice.getMaxRangeF(mDevicePropertyMap.get(instructionTypeName).getProperty());
                   mSetEt.setHint("[" + minF + "-" + maxF + "]");
                   break;
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

设置属性

.. code:: java

   try {
       String setValue = mSetEt.getText().toString();
       // 设置设备指令
       DevicePropertyInfo devProperty = mDevicePropertyMap.get(mPropertySp.getSelectedItem().toString());
       switch (devProperty.getPropertyType()) {
           case INT_PROPERTY:
               //曝光和增益需要关闭AE才能调节， 亮度需要打开AE才能调节， 白平衡（色温）需要关闭自动白平衡才能调节
               if (devProperty.getProperty() == DeviceProperty.OB_PROP_COLOR_EXPOSURE_INT
                   || devProperty.getProperty() == DeviceProperty.OB_PROP_COLOR_GAIN_INT) { //曝光或增益
                   //获取自动曝光状态
                   boolean propertyExposureBool = false;
                   try {
                       propertyExposureBool = mSelectDevice.getPropertyValueB(DeviceProperty.OB_PROP_COLOR_AUTO_EXPOSURE_BOOL);
                   } catch (Exception e) {

                   }
                   Log.d(TAG, "propertyExposureBool:" + propertyExposureBool);
                   if (propertyExposureBool) {
                       showToast("自动曝光未关闭，不能设置曝光或增益");
                       return;
                   }
               }

               if (devProperty.getProperty() == DeviceProperty.OB_PROP_COLOR_BRIGHTNESS_INT) { //亮度
                   //获取自动曝光状态
                   boolean propertyExposureBool = mSelectDevice.getPropertyValueB(DeviceProperty.OB_PROP_COLOR_AUTO_EXPOSURE_BOOL);
                   Log.d(TAG, "propertyExposureBool:" + propertyExposureBool);
                   if (!propertyExposureBool) {
                       showToast("自动曝光未开启，不能设置亮度");
                       return;
                   }
               }

               if (devProperty.getProperty() == DeviceProperty.OB_PROP_COLOR_WHITE_BALANCE_INT) { //白平衡
                   //获取自动白平衡状态
                   boolean propertyWhiteBool = mSelectDevice.getPropertyValueB(DeviceProperty.OB_PROP_COLOR_AUTO_WHITE_BALANCE_BOOL);
                   Log.d(TAG, "propertyWhiteBool:" + propertyWhiteBool);
                   if (propertyWhiteBool) {
                       showToast("自动白平衡未关闭，不能设置白平衡");
                       return;
                   }
               }
               mSelectDevice.setPropertyValueI(devProperty.getProperty(), Integer.parseInt(setValue));
               break;
           case BOOL_PROPERTY:
               // 0:false 1:true
               mSelectDevice.setPropertyValueB(devProperty.getProperty(), ("1".equals(setValue) ? true : false));
               break;
           case FLOAT_PROPERTY:
               mSelectDevice.setPropertyValueF(devProperty.getProperty(), Float.parseFloat(setValue));
               break;
       }
   } catch (Exception e) {
       e.printStackTrace();
   }

获取属性

.. code:: java

   try {
       // 获取设备指令
       DevicePropertyInfo devProperty = mDevicePropertyMap.get(mPropertySp.getSelectedItem().toString());
       switch (devProperty.getPropertyType()) {
           case INT_PROPERTY:
               int valueI = mSelectDevice.getPropertyValueI(devProperty.getProperty());
               mGetTv.setText(Integer.toString(valueI));
               break;
           case BOOL_PROPERTY:
               boolean valueB = mSelectDevice.getPropertyValueB(devProperty.getProperty());
               mGetTv.setText(Boolean.toString(valueB));
               break;
           case FLOAT_PROPERTY:
               float valueF = mSelectDevice.getPropertyValueF(devProperty.getProperty());
               mGetTv.setText(Float.toString(valueF));
               break;
       }
   } catch (Exception e) {
       e.printStackTrace();
   }

资源释放

.. code:: java

   private void release() {
       try {
           // 设备列表资源释放
           for (Device device : mDeviceList) {
               device.close();
           }
           mDeviceList.clear();

           // OBContext资源释放
           if (null != mOBContext) {
               mOBContext.close();
               mOBContext = null;
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }


录制与回放示例-Recorder & Playback
----------------------------------

功能描述: :连接设备开流 , 录制当前视频流到文件，载入视频文件进行回放。

首先需要创建一个Context，用于获取设备信息列表和创建设备

.. code:: java

   private OBContext mOBContext;

初始化SDK，并监听设备变化

.. code:: java

   // 1.初始化SDK, 并监听设备变化
   mOBContext = new OBContext(getApplicationContext(), new DeviceChangedCallback() {
       @Override
       public void onDeviceAttach(DeviceList deviceList) {
           try {
               if (null == mPipeline) {
                   // 2.创建Device, 并通过Device初始化Pipeline
                   mDevice = deviceList.getDevice(0);
                   mPipeline = new Pipeline(mDevice);

                   // 3.更新设备信息ui
                   updateDeviceInfoView(false);

                   // 4.创建Pipeline配置
                   mConfig = new Config();

                   // 5.获取深度流配置，并配置到Config,这里配置的为640x480 Y16格式
                   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
                   StreamProfile streamProfile = null;

                   if (null != depthProfileList) {
                       streamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
                       depthProfileList.close();
                   }

                   if (streamProfile == null) {
                       return;
                   }

                   // 6.使能深度流配置
                   mConfig.enableStream(streamProfile);

                   // 7.设置镜像
                   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, PermissionType.OB_PERMISSION_WRITE)) {
                       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, true);
                   }

                   // 8.开流
                   mPipeline.start(mConfig);

                   // 9.释放streamProfile
                   streamProfile.close();

                   // 10.创建获取Pipeline数据线程
                   start();
               }

               // 11.释放deviceList
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }

       @Override
       public void onDeviceDetach(DeviceList deviceList) {
           try {
               release();
               updateCtlPanel(mRecordCtlPanelLL, View.VISIBLE);
               updateCtlPanel(mPlaybackCtlPanelLL, View.VISIBLE);
               deviceList.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   });

创建设备并通过设备创建Pipeline

.. code:: java

   // 2.创建Device, 并通过Device初始化Pipeline
   mDevice = deviceList.getDevice(0);
   mPipeline = new Pipeline(mDevice);

创建Pipeline配置

.. code:: java

   // 4.创建Pipeline配置
   mConfig = new Config();

获取深度流配置，并配置到Config，这里配置的为640x480 Y16格式

.. code:: java

   // 5.获取深度流配置，并配置到Config,这里配置的为640x480 Y16格式
   StreamProfileList depthProfileList = mPipeline.getStreamProfileList(SensorType.DEPTH);
   StreamProfile streamProfile = null;

   if (null != depthProfileList) {
       streamProfile = depthProfileList.getVideoStreamProfile(640, 480, Format.Y16, 30);
       depthProfileList.close();
   }

   if (streamProfile == null) {
       return;
   }

   // 6.使能深度流配置
   mConfig.enableStream(streamProfile);

通过Device属性接口设置镜像

.. code:: java

   // 7.设置镜像
   if (mDevice.isPropertySupported(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, PermissionType.OB_PERMISSION_WRITE)) {
       mDevice.setPropertyValueB(DeviceProperty.OB_PROP_DEPTH_MIRROR_BOOL, true);
   }

使用Config配置通过Pipeline开流，并释放局部资源

.. code:: java

   // 8.开流
   mPipeline.start(mConfig);

   // 9.释放streamProfile
   streamProfile.close();

开启数据流预览线程

.. code:: java

   private void start() {
       mIsStreamRunning = true;
       if (null == mStreamThread) {
           mStreamThread = new Thread(mStreamPrevRunnable);
           mStreamThread.start();
       }
   }

预览线程

.. code:: java

   // 预览线程
   private Runnable mStreamPrevRunnable = () -> {
       while (mIsStreamRunning) {
           try {
               // 等待100ms后如果获取不到，则超时
               FrameSet frameSet = mPipeline.waitForFrameSet(100);

               if (null == frameSet) {
                   continue;
               }

               // 获取深度流数据
               if (!mIsPlaying) {
                   DepthFrame frame = frameSet.getDepthFrame();

                   if (frame != null) {
                       // 获取深度数据并进行渲染
                       byte[] frameData = new byte[frame.getDataSize()];
                       frame.getData(frameData);
                       synchronized (mSync) {
                           mDepthGLView.update(frameData, frame.getWidth(), frame.getHeight(), StreamType.DEPTH, frame.getFormat());
                       }

                       // 释放深度数据帧
                       frame.close();
                   }
               }

               // 释放数据集
               frameSet.close();
           } catch (Exception e) {
               e.printStackTrace();
           }
       }
   };

定义录制文件保存路径

.. code:: java

   private static final String BAG_FILE_PATH = "/sdcard/Orbbec/recorder.bag";

开始录制

.. code:: java

   private void startRecord() {
       try {
           if (!mIsRecording) {
               if (null != mPipeline) {
                   // 开始录制
                   mPipeline.startRecord(BAG_FILE_PATH);
                   mIsRecording = true;
                   updateCtlPanel(mPlaybackCtlPanelLL, View.INVISIBLE);
               } else {
                   Log.w(TAG, "mPipeline is null !");
               }
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

停止录制

.. code:: java

   private void stopRecord() {
       try {
           if (mIsRecording) {
               mIsRecording = false;
               if (null != mPipeline) {
                   // 停止录制
                   mPipeline.stopRecord();
               }
               updateCtlPanel(mPlaybackCtlPanelLL, View.VISIBLE);
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

停止数据流预览线程

.. code:: java

   private void stop() {
       mIsStreamRunning = false;
       if (null != mStreamThread) {
           try {
               mStreamThread.join(300);
           } catch (InterruptedException e) {
           }
           mStreamThread = null;
       }
   }

定义回放状态回调

.. code:: java

   private MediaStateCallback mMediaStateCallback = new MediaStateCallback() {
       @Override
       public void onState(MediaState state) {
           if (state == MediaState.OB_MEDIA_END) {
               stopPlayback();
           }
       }
   };

定义回放线程的Runnable

.. code:: java

   while (mIsPlaying) {
       try {
           // 等待100ms后如果获取不到，则超时
           FrameSet frameSet = mPlaybackPipe.waitForFrameSet(100);
           if (null == frameSet) {
               continue;
           }

           // 获取深度流数据
           DepthFrame depthFrame = frameSet.getDepthFrame();
           if (null != depthFrame) {
               // 获取深度数据并进行渲染
               byte[] frameData = new byte[depthFrame.getDataSize()];
               depthFrame.getData(frameData);
               synchronized (mSync) {
                   mDepthGLView.update(frameData, depthFrame.getWidth(), depthFrame.getHeight(), StreamType.DEPTH, depthFrame.getFormat());
               }

               // 释放深度数据帧
               depthFrame.close();
           }

           // 释放数据集
           frameSet.close();
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

开始回放

.. code:: java

   private void startPlayback() {
       try {
           if (!FileUtils.isFileExists(BAG_FILE_PATH)) {
               Toast.makeText(RecordPlaybackActivity.this, "File not found!", Toast.LENGTH_LONG).show();
               return;
           }
           if (!mIsPlaying) {
               mIsPlaying = true;

               // 释放Playback资源
               if (null != mPlayback) {
                   mPlayback.close();
                   mPlayback = null;
               }

               // 创建回放pipeline
               if (null != mPlaybackPipe) {
                   mPlaybackPipe.close();
                   mPlaybackPipe = null;
               }
               mPlaybackPipe = new Pipeline(BAG_FILE_PATH);

               // 通过回放Pipeline获取回放器
               mPlayback = mPlaybackPipe.getPlayback();

               // 设置回放状态回调
               mPlayback.setMediaStateCallback(mMediaStateCallback);

               // 开始回放
               mPlaybackPipe.start(null);

               // 创建回放线程
               if (null == mPlaybackThread) {
                   mPlaybackThread = new Thread(mPlaybackRunnable);
                   mPlaybackThread.start();
               }

               // 更新UI
               updateDeviceInfoView(true);
               updateCtlPanel(mRecordCtlPanelLL, View.INVISIBLE);
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

停止回放

.. code:: java

   private void stopPlayback() {
       try {
           if (mIsPlaying) {
               mIsPlaying = false;
               // 停止回放线程
               if (null != mPlaybackThread) {
                   try {
                       mPlaybackThread.join(300);
                   } catch (InterruptedException e) {
                   }
                   mPlaybackThread = null;
               }

               // 停止回放
               if (null != mPlaybackPipe) {
                   mPlaybackPipe.stop();
               }

               // 更新UI
               updateDeviceInfoView(false);
               updateCtlPanel(mRecordCtlPanelLL, View.VISIBLE);
           }
       } catch (Exception e) {
           e.printStackTrace();
       }
   }

释放资源

.. code:: java

   try {
       // 释放回放器
       if (null != mPlayback) {
           mPlayback.close();
           mPlayback = null;
       }

       // 释放回放pipeline
       if (null != mPlaybackPipe) {
           mPlaybackPipe.close();
           mPlaybackPipe = null;
       }

       // 释放config
       if (null != mConfig) {
           mConfig.close();
           mConfig = null;
       }

       // 释放预览pipeline
       if (null != mPipeline) {
           mPipeline.close();
           mPipeline = null;
       }

       // 释放Device
       if (null != mDevice) {
           mDevice.close();
           mDevice = null;
       }

       // 释放SDK
       if (null != mOBContext) {
           mOBContext.close();
       }

       // 释放渲染窗口资源
       if (mDepthGLView != null) {
           mDepthGLView.release();
       }
   } catch (Exception e) {
       e.printStackTrace();
   }
