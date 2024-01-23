# Introduction
Orbbec Viewer is a tool developed based on Orbbec SDK to help developers quickly use Orbbec's 3D sensor products. The functional features of Orbbec 3D sensor products are realized, including functions such as reading serial numbers, obtaining device types, acquiring camera parameters, controlling cameras,etc.

This document describes these functions and demonstrates the basic usage process.
# Overview
## Target users
The OrbbecViewer tool is designed for end users/developers to use Orbbec SDK 3D cameras.
## Supported Platforms

| **Operating system** | **Requirement** | **Description** |
| --- | --- | --- |
| Windows | - Windows 10 April 2018 (version 1803, operating system build 17134) release (x64) or higher<br />- 4 GB RAM<br />- USB2.0 and above ports<br /> | The generation of the VS project depends on the installation of the VS version and the cmake version, and supports VS2015/vs2017/vs2019 |
| Android | - Android 6/7/8/9/10<br /> |  |
| Linux | - Linux Ubuntu 16.04/18.04/20.04 (x64)<br />- 4 GB RAM<br />- USB2.0 and above ports<br /> |Support GCC 7.5|
| Arm64 | <br />- Linux Ubuntu 18.04/20.04/22.04<br />- 4 GB RAM<br />- USB2.0 and above ports<br /> |  Support GCC 7.5 |

# OrbbecViewer Usage
## 3.1 Main interface of the software
As shown in the figure, the main interface is marked with three areas separated by red boxes. The functions are:

| **Area** | **Name** | **Function** |
| --- | --- | --- |
| Area 1 | Equipment management area | Sensor list,|
| Area 2 | Control panel area | Data stream switch and parameter configuration, image acquisition function control, D2C function control |
| Area 3 | Image preview area | Sensor image preview, frame rate, timestamp and other information viewing |

![1](.\images\1.png)

Among them, there are six buttons on the left side of the control panel area, the bottom button.


![2](.\images\2.png) Used to switch between Chinese and English;![3](.\images\3.png) Used to open the software details page.

![4](.\images\4.png) Used to view device information and firmware upgrade; the remaining two buttons are used to switch the control panel TAB pages of different functions, from top to bottom, it is "Single Camera Mode" and "Advanced Mode".

Click the Log information at the bottom of area 3, and the specific log information will be displayed.

## 3.2 Data stream
OrbbecViewer allows the user to select and configure depth, infrared and color data streams. This section outlines the parameters that the user can customize in the tool. After the user completes the configuration, they must click the top ![5](.\images\5.png)button to start data streaming.

The OrbbecViewer tool allows the user to select a combination of depth, infrared and color data streams. User can enable/disable the stream by checking/unchecking from the list of available streams. The viewer supports both USB2.0 and USB3.0, so the available configuration parameters will vary depending on the USB2.0/USB3.0 capabilities.


###  Resolution and Frame Rate
The cameras provide users with a choice of resolutions to suit their solution needs. Each data stream provides a variety of optional frame formats for users to freely choose the appropriate combination of image size, image format and frame rate.

For example, depth stream  can support 320x240, 640x480, 640x400, 1280x800, 320x288, 640x576, 1024x1024, 512x512 and other resolutions, 
and the range of FPS frame rate is 5 to 30, different devices support different resolutions, image formats, and frame rates, please refer to the product manual.

![8](.\images\8.png)

The color stream can support multiple resolutions from 640x480 to 3840x2160, the range of FPS frame rate ranges from 5 to 30, and supports MJPG, RGB, I420, and H265.

For the preview of the color stream, MJPG, H264, and H265 are the encoding formats, which need to be decoded first. The decoding usually takes a lot of time, resulting in dropped frames or rendering a "corrupted" picture. On some models of PC, hardware-accelerated decoding is available.

![7](.\images\7.png)

The infrared stream can support 320x240、640x480、640x400、1280x800、640x576、512x512、1024x1024 and other resolutions, and the FPS frame rate ranges from 5 to 30. different devices support different resolutions, image formats, and frame rates, please refer to the product manual<br />
![9](.\images\9.png)

Users can choose the most suitable resolution according to the actual situation. Note that higher resolutions are more accurate, but increase computational intensity.

## 3.3 Device Information
Click the button“ ![10](.\images\10.png) ”to view device information.
The OrbbecViewer tool contains simple device information such as firmware version, product identification code, camera parameters, temperature, etc.

![11](.\images\11.png)

## 3.4 Image preview area
Open stream will display the average frame rate, time frame, image frame number and other information by default, click on the upper right corner ![12](.\images\12.png) to toggle information display\close.
Click ![13](.\images\13.png) , the stream can be paused without being removed from the preview area.
Click ![14](.\images\14.png) , streams that have been paused and not removed from the preview area can be reopened.

After the data stream is closed, the image preview window will not be closed, and the user needs to click ![15](.\images\15.png) the upper right corner of the image preview window.

![16](.\images\16.png)

## 3.5 Control Panel Area
### Sensors and Data Stream
#### Get camera name, serial number and USB protocol
After the device is powered on and the USB is connected to the PC host, open the OrbbecViewer tool software, and the software will automatically connect the device. Some devices may take a long time to initialize, please wait patiently for the device to connect successfully.

After the connection is successful, the control panel area automatically displays the specific information of the device.

![18](.\images\18.png)

#### Depth stream module
Including: mirroring, software filtering , depth effective range acquisition (MinDepthValue/MaxDepthValue), exposure and other functions.

![19](.\images\19.png)

#### Color stream module
Including: Mirror, Flip, Align, Exposure, White Balance, Gain, Brightness, Sharpness, Saturation, Contrast, Hue and other functions.

![20](.\images\20.png)

#### Infrared stream module
Including mirroring and exposure functions.

![21](.\images\21.png)

#### IMU
Including Imu param.

![21-1](.\images\21-1.png)


#### **Device data management**
Users can select corresponding images and captured frames for recording, and the captured frames are saved in the "OrbbecViewer/output" directory by default.

![22](.\images\22.png)

## 3.6 D2C and Point Cloud (support hardware D2C and software D2C)
In advanced mode, both D2C and point clouds are supported.
* Control depth and color alignment, and the alignment mode supports hardware D2C and software D2C.
* frame synchronization： Enables or disables the frame synchronization function. When frame synchronization is enabled, the SDK will combine Depth and Color into a FrameSet based on the device hardware timestamp, which can only be supported by devices that support frame synchronization, such as: Gemini2、Gemini2 L、Astra2、Femoto、Femto W、Femto Mega。
* 2D Control： Display and save 2D images。
* 3D Control： Display and save 3D PointCloud data.

1. The zoom of the point cloud can be controlled by the mouse wheel, and the movement of the point cloud can be controlled by the movement of the mouse.

2. You can choose to export the depth point cloud (xyz) or RGBD point cloud (xyzrgb) and save it as a ply format file, which is saved in the "OrbbecViewer/output/PointCloud" directory by default.

* Record and Playback: The recording file is saved in the "OrbbecViewer/output/RecordFile" directory by default.

![image23](images/23.png)

## 3.7 **Firmware update**
When the device is connected, click![30](.\images\30.png)Access the firmware update page.

* Femto device firmware includes system firmware and onboard MCU firmware. You can select the upgrade firmware type in the "Type" drop-down box.
* After selecting the firmware type, enter the existing updated firmware image file in the “Firmware” input box, and click the “Upgrade” button to start the update.
* The device firmware update adopts the A/B dual partition scheme. If the update fails unexpectedly, it will not affect the operation of the original firmware, just reconnect the device to update. The device update time is relatively long, and the device will automatically restart once, please pay attention to the prompt information and wait patiently.
* Astra+、gemini2、Gemini2 L、Astra2、Dabai、Dabai DCW、Dabai DW、Astra Mini Pro、Astra Pro Plus、Gemini、GeminiE、GeminiE Lite Upgrade interface:

![31](.\images\31.png)

* Femto、Femto W interface：

![32](.\images\32.png)

* Femto Mega firmware upgrade does not support Windows upgrade, you need to call OrbbecViewer with administrator privileges under linux.

## 3.8 **Log Information**
By default, the log information area is displayed in a single-folded state. By clicking the button on the far right.![33](.\images\33.png)Expand to view full log information.  By clicking the button![34](.\images\34.png)will empty the log. By clicking the button![35](.\images\35.png)restores the collapsed single bar display state.
![36](.\images\36.png)

