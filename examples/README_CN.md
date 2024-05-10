# Orbbec 相机 示例代码
这些示例代码用来演示获取Depth、IR、Color、D2C、点云、多机同步等.  详细的接口文档和示例解释参考 (../doc)。

## 示例

| **名称** | **语言** | **描述** |**注意事项**|
| --- | --- | --- | ---|
| HelloOrbbec | C | 演示连接到设备获取SDK版本和设备信息 |
| FirmwareUpgrade | C | 演示选择固件bin或者img文件给设备升级固件版本 |
| DepthViewer | C | 演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像 |
| ColorViewer       | C        | 演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像 |
| Hotplugin | C | 演示设备热拔插监控，检测到设备上线自动连接设备打开深度流，检测到设备掉线自动断开设备连接 |
| PointCloud | C | 演示生成深度点云或RGBD点云并保存成ply格式文件 |注意：该示例同时开Depth和Color流，如相机不支持Color流 (Gemini E Lite 或 DaBai DW) 或 用户不需要开Color流，那么需要将开Color流的代码及设置D2C的代码去掉   |
| AlignFilterViewer | C | 演示传感器数据流对齐操作，支持D2C和C2D对齐  | Gemini 330系列 支持|
| DoubleInfraredViewer | C | 演示同时获取双目相机的左右IR数据 | 目前只有Gemini 2 XL、Gemini 330系列 支持  |
| InfraredViewer       | C        | 演示使用SDK获取红外数据并绘制显示、获取分辨率并进行设置、显示红外图像 | 默认配置IR Sensor获取数据，Gemini 2 XL、Gemini 330系列  通过左IR或右IR 获取数据，该Sample不支持，请参考DoubleInfraredViewer示例  |
| Post-Processing       | C        | 演示Gemini 330系列 处理功能  | Gemini 330系列 支持 |
| HdrMerge      | C        | 演示Gemini 330系列 HDR 功能 | Gemini 330系列 支持   |
| HelloOrbbec | C++ | 演示连接到设备获取SDK版本和设备信息 |
| DepthViewer | C++ | 演示使用SDK获取深度数据并绘制显示、获取分辨率并进行设置、显示深度图像 |
| ColorViewer | C++ | 演示使用SDK获取彩色数据并绘制显示、获取分辨率并进行设置、显示彩色图像 |
| InfraredViewer | C++ | 演示使用SDK获取红外数据并绘制显示、获取分辨率并进行设置、显示红外图像 | 默认配置IR Sensor获取数据，Gemini 2 XL、Gemini 330系列通过左IR或右IR 获取数据，该Sample不支持，请参考DoubleInfraredViewer示例  |
| DoubleInfraredViewer | C++ | 演示同时获取双目相机的左右IR数据 | 目前只有Gemini 2 XL、Gemini 330系列 支持  |
| IMUReader | C++ | 获取IMU数据并输出显示 | 仅支持Imu的相机支持    |
| Hotplugin | C++ | 演示设备拔插回调的设置，并获取到插拔后处理的流 |
| SensorControl | C++ | 演示对设备和传感器控制命令的操作 |
| DepthWorkMode | C++ | 演示如何获取相机深度工作模式，查询支持的深度模式列表，切换模式 | 仅部分相机支持,Gemini 2 、Gemini 2 L、Gemini 2 XL、Astra 2 支持深度模式,可以切换不同的深度模式   |
| SyncAlignViewer | C++ | 演示对传感器数据流对齐的操作 |注意：1.有些相机Depth和Color镜像默认不一致，需要调用镜像接口设置; 2. DaBai DCW、Gemini E，默认配置的Depth和Color分辨率不能同时支持硬件D2C和软件D2C，请根据产品规格书，设置能对齐的分辨率    |
| PointCloud | C++ | 演示生成深度点云或RGBD点云并保存成ply格式文件 | 注意：该示例同时开Depth和Color流，如相机不支持Color流 (Gemini E Lite 或 DaBai DW) 或 用户不需要开Color流，那么需要将开Color流的代码及设置D2C的代码去掉  |
| SaveToDisk | C++ | 获取彩色和深度图并存储为png格式 |
| Recorder | C++ | 录制当前视频流到文件 | MacOS 不支持 |
| Playback | C++ | 载入视频文件进行回放 | MacOS 不支持 |
| NetDevice | C++ | 演示通过网络获取深度数据和彩色数据、并绘制显示  | 支持网络功能的相机才可以使用,目前Femto Mega、Gemini 2 XL 支持网络    |
| MultiStream | C++ | 演示一个设备同时打开多路流的操作 |    |
| MultiDevice | C++ | 演示对多设备进行操作 |    |
| MultiDeviceSync | C++ | 演示多机同步功能 |   |
| FirmwareUpgrade | C++ | 演示选择固件bin或者img文件给设备升级固件版本 |
| Transformation | C++ |  演示CoordinateTransformHelper类的函数调用，该类用来做不同坐标系下点的转换及生成无畸变的depth点云和RGBD点云|
| HdrMerge      | C++        | 演示Gemini 330系列 HDR 功能 | Gemini 330系列 支持|
| Post-Processing       | C++        | 演示Gemini 330系列 处理功能  | Gemini 330系列 支持|
| AlignFilterViewer | C++ | 演示传感器数据流对齐操作，支持D2C和C2D对齐   |Gemini 330系列 支持|

