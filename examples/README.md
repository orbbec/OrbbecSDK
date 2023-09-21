# Sample Code for Orbbec cameras
These simple examples demonstrate how to easily use the SDK to access the camera into your applications.  

For a detailed explanations and API documentation see our (../doc) section.

## List of Examples:

| Name | Language | Description | Notes |
| --- | --- | --- | ---|
| HelloOrbbec | C | Demonstrate connect to device to get SDK version and device information |  |
| DepthViewer | C | Demonstrate using SDK to get depth data and draw display, get resolution and set, display depth image |
| ColorViewer | C | Demonstrate using SDK to get color data and draw display, get resolution and set, display color image |
| InfraredViewer | C | Demonstrate using SDK to obtain infrared data and draw display, obtain resolution and set, display infrared image | Gemini 2 XL obtains data through left IR or right IR, this Sample does not support, please refer to DoubleInfraredViewer example  |
| DoubleInfraredViewer | C | Demonstrate obtain left and right IR data of binocular cameras  | Currently, only the Gemini 2 XL supports |
| SensorControl | C | Demonstrate the operation of device, sensor control commands |   |
| DepthWorkMode | C |Demonstrate get current depth work mode, obtain supported depth work mode list, switch depth work mode. |  Some cameras support, Gemini 2, Gemini 2 L, Gemini 2 XL, Astra 2 support depth mode, you can switch different depth modes  |
| Hotplugin | C | Demonstrate device hot-plug monitoring, automatically connect the device to open depth streaming when the device is online, and automatically disconnect the device when it detects that the device is offline |    |
| PointCloud | C | Demonstrate the generation of depth point cloud or RGBD point cloud and save it as ply format file |   |
| NetDevice | C | Demonstrates the acquisition of depth and color data through network mode|  Only cameras that support network functions can be used this sample, and currently the Femto Mega and Gemini 2 XL support network functions |
| FirmwareUpgrade | C | Demonstrate upgrade device firmware |  |
| HelloOrbbec | C++ | Demonstrate connect to device to get SDK version and device info |
| DepthViewer | C++ | Demonstrate using SDK to get depth data and draw display, get resolution and set, display depth image |
| ColorViewer | C++ | Demonstrate using SDK to get color data and draw display, get resolution and set, display color image |
| InfraredViewer | C++ | Demonstrate using SDK to obtain infrared data and draw display, obtain resolution and set, display infrared image | Gemini 2 XL obtains data through left IR or right IR, this Sample does not support, please refer to DoubleInfraredViewer example   |
| CommonUsages | C++ |  Demonstrate the setting and acquisition of commonly used control parameters   |    |
| DoubleInfraredViewer | C++ | Demonstrate obtain left and right IR data of binocular cameras  | Currently, only the Gemini 2 XL supports |
| IMUReader | C++ | Get IMU data and output display | The camera must have an IMU function |
| HotPlugin | C++ | Demonstrate the settings of the device plug and unplug callback, and get the stream processed after plugging and unplugging |
| SensorControl | C++ | Demonstrate manipulation of device and sensor control commands |
| DepthWorkMode | C++ |Demonstrate get current depth work mode, obtain supported depth work mode list, switch depth work mode. | Some cameras support, Gemini 2, Gemini 2 L, Gemini 2 XL, Astra 2 support depth mode, you can switch different depth modes  |
| SyncAlignViewer | C++ | Demonstrate operations on sensor data stream alignment |
| PointCloud | C++ | Demonstrate the generation of depth point cloud or RGBD point cloud and save it as ply format file | Notes: This example start both Depth and Color streams, if the camera does not support Color streams (Gemini E Lite or Dabai DW) or the user does not need to start Color streams, then the code that start the Color stream and the code that sets D2C need to be removed |
| SaveToDisk | C++ | Get color and depth maps and save as png format |
| Recorder | C++ | Record current video stream to file | MacOS not support this sample |
| Playback | C++ | Load video files for playback | MacOS not support this sample|
| NetDevice| C++ | Demonstrates the acquisition of depth and color data through network mode|  Only cameras that support network functions can be used this sample, and currently the Femto Mega and Gemini 2 XL support network functions |
| MultiStream | C++ | Demonstrate one device to start Color, Ir, Depth, Gyro, Accel Sensor stream. | On the Linux/Arm platform ,this sample requires users to compile with Opencv4.2 or above,otherwise, it cannot be rendered    |
| MultiDevice | C++ | Demonstrate operation on multiple devices |On the Linux/Arm platform ,this sample requires users to compile with Opencv4.2 or above,otherwise, it cannot be rendered    |
| MultiDeviceSync | C++ | Demostrate how to config multiple devices synchronize config and how to start stream with this config | On the Linux/Arm platform ,this sample requires users to compile with Opencv4.2 or above,otherwise, it cannot be rendered    |
| FirmwareUpgrade | C++ | Demonstrate upgrade device firmware |
