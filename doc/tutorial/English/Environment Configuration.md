<!-- 本文档面向开发者 -->
# introduction
This document introduces the installation of OrbbecSDK dependencies and the compilation instructions of OrbbecSDK Sample.

# install dependency
The Orbbec SDK is compatible with the standard UVC protocol, and the standard UVC used by Gemini2、Femto 、Femto W、Femto Mega、Astra+ device, so no additional drivers are required.

## 1. install driver

Dabai, Dabai DCW, Dabai DW, Astra mini Pro, Astra Pro Plus, A1 Pro, Gemini E, Gemini E Lite, Gemini These use the private protocol of openni and need to install private drivers.
The directory of the openni driver is as follows: double-click the following .exe image directory to install the driver. 

![image0](Image/orbbec_driver_000.png)

## 2. Configure OpenCV (Examples dependency)

Data rendering relies on the third-party library OpenCV. Here we take OpenCV 4.3.0 as an example to demonstrate the installation configuration<br />
Execute the OpenCV installation file, select the directory where opencv is to be installed, and click extract to execute the installation.<br />

![2](Image/orbbec_opencv_001.png)

2)Add the path of OpenCV in the environment variables of the system, enter OpenCV_DIR for the variable name, pay attention to the capitalization of the letters, and the variable value is the path to the build folder of the OpenCV installation directory；<br />

![3](Image/orbbec_opencv_002.png)

![4](Image/orbbec_opencv_003.png)


## 3. Generate your first application
Software dependency: VisualStudio2019, cmake 3.10 and above<br />1) Download/obtain our SDK software package. The storage location is assumed to be the root directory of Disk D: "D:/V1.5.7". The directory structure is as follows:<br />

![5](Image/orbbec_sample_004.png))

2)Open Cmake and set the "Examples" folder as the code path, and the "build" folder under "Examples" as the path to generate the binary file, as shown in the figure below. If there is no build under Examples, you need to create a new folder.<br />

![6](Image/orbbec_sample_005.png)

3)Click "Configure" and select the corresponding Visual Studio version and platform version, then click "Finish", as shown below:<br />

![7](Image/orbbec_sample_006.png)

4)Click "Generate", as shown below:<br />

![8](Image/orbbec_sample_007.png)

5)The Sample project can be opened in the following two ways<br />Method 1: With cmake, click the "Open Project" button to open the Visual Studio project<br />

![9](Image/orbbec_sample_008.png)

Method 2: In the folder, directly start the Visual Studio project in Examples/build, as shown in the figure below.

![10](Image/orbbec_sample_009.png)

6)Open the Examples project interface as shown below:<br />

![11](Image/orbbec_sample_010.png)

(7) Select the project you want to run, right click and "set as startup project", select release and 64-bit version at the run option；<br /><br />

![12](Image/orbbec_sample_011.png)

![13](Image/orbbec_sample_013.png)

<br />8) Connect the device to the host<br />
9) Copy the dll files from the bin directory of the Examples folder to the build folder where the generated executables located at , and then run the project.

![14](Image/orbbec_sample_014.png)



# Linux
## 1. System environment configuration
1)Install libudev library: sudo apt install libudev-dev<br />2) Install libusb library: sudo apt install libusb-dev

## 2. USB access rights configuration
By default, direct access to USB devices in Linux systems requires root privileges, which can be resolved through the rules configuration file. After the files released by OrbbecSDK are decompressed, there will be a "99-obsensor-libusb.rules" configuration file and "install.sh" installation script in the root directory. Run the "install.sh" script through the sudo command to complete the rules Installation of configuration files. In addition, if the "install.sh" installation script does not have execution permission, it can be solved by changing the command: "sudo chmod +x ./install.sh". After the installation script is successfully executed, it will take effect when the device is connected again (the connected device needs to be re-plugged).<br />![20](Image/orbbec_sample_linux_001.png)

## 3. Verify device status
1)Environment preparation: ubuntu18.04 x64 desktop<br />2) Take Astra+ as an example, use USB 3.0 Type-C data cable to connect with PC.<br />3) Use the lsusb command to check if the PC system correctly recognizes the camera<br />

![21](Image/orbbec_sample_linux_002.png)

<br />4) Judge whether the camera is recognized normally by PID&VID<br />VID: 2bc5 PID: 0532 (color camera)<br />VID: 2bc5 PID: 0636 (depth camera)


## 4、Linux Sample compile
Create a build directory in the Examples directory, as follows:
![image17](Image/orbbec_sample_linux_003.png)<br />
cd build <br/>
cmake .. <br/>
make <br/>

# Frequently Asked Question

[FAQ](https://developer.orbbec.com.cn/technical_library.html?id=59) <br/>

