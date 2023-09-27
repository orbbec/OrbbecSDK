# introduction
This document introduces the installation of OrbbecSDK dependencies and the compilation instructions of OrbbecSDK Sample.

# install dependency
The Orbbec SDK is compatible with the standard UVC protocol, and the standard UVC used by Gemini2、Femto 、Femto W、Femto Mega、Astra+、Astra2、Gemini2 L device, so no additional drivers are required.

## 1. install driver

Dabai, Dabai DCW, Dabai DW, Astra mini Pro, Astra Pro Plus, A1 Pro, Gemini E, Gemini E Lite, Gemini These use the private protocol of openni and need to install private drivers.
The directory of the openni driver is as follows: double-click the following .exe image directory to install the driver.

![image0](Image/orbbec_driver_000.png)

## 2. Configure OpenCV (Examples dependency)

Data rendering relies on the third-party library OpenCV. Here we take OpenCV 4.3.0 as an example to demonstrate the installation configuration.

* Execute the OpenCV installation file, select the directory where opencv is to be installed, and click extract to execute the installation.

![2](Image/orbbec_opencv_001.png)

* Add the path of OpenCV in the environment variables of the system, enter OpenCV_DIR for the variable name, pay attention to the capitalization of the letters, and the variable value is the path to the build folder of the OpenCV installation directory.

![3](Image/orbbec_opencv_002.png)

![4](Image/orbbec_opencv_003.png)


## 3. Generate your first application
Software dependency: VisualStudio2019, cmake 3.10 and above.
* Download OrbbecSDK software package. Take version 1.5.7 as an example to explain how Windows compiles.

* Open Cmake and set the "Examples" folder as the code path, and the "build" folder under "Examples" as the path to generate the binary file, as shown in the figure below. If there is no build under Examples, you need to create a new folder.


![6](Image/orbbec_sample_005.png)

* Click "Configure" and select the corresponding Visual Studio version and platform version, then click "Finish", as shown below:

![7](Image/orbbec_sample_006.png)

* Click "Generate", as shown below:

![8](Image/orbbec_sample_007.png)

* The Sample project can be opened in the following two ways.

Method 1: With cmake, click the "Open Project" button to open the Visual Studio project.

![9](Image/orbbec_sample_008.png)

Method 2: In the folder, directly start the Visual Studio project in Examples/build, as shown in the figure below.

![10](Image/orbbec_sample_009.png)

* Open the Examples project interface as shown below:

![11](Image/orbbec_sample_010.png)

* Select the project you want to run, right click and "set as startup project", select release and 64-bit version at the run option.


* Connect the device to the host

* Copy the dll files from the bin directory of the Examples folder to the build folder where the generated executables located at.
![13](Image/orbbec_sample_013.png)

*  run the HelloOrbbec project

![14](Image/orbbec_sample_014.png)

# Linux
## 1. System environment configuration
* Install libudev library:
``` bash
sudo apt install libudev-dev
```

* Install libusb library:
``` bash
sudo apt install libusb-dev
```

## 2. USB access rights configuration
By default, direct access to USB devices in Linux systems requires root privileges, which can be resolved through the rules configuration file. After the files released by OrbbecSDK are decompressed, there will be a "99-obsensor-libusb.rules" configuration file and "install.sh" installation script in the Script directory.

![20](Image/orbbec_sample_linux_001.png)
Run the "install.sh" script through the sudo command to complete the rules Installation of configuration files.
``` bash
sudo chmod +x ./install.sh
sudo ./install.sh
 ```

## 3. Verify device status
* Environment preparation: ubuntu18.04 x64 desktop

* Take Astra+ as an example, use USB 3.0 Type-C data cable to connect with PC.

* Use the lsusb command to check if the PC system correctly recognizes the camera.

![21](Image/orbbec_sample_linux_002.png)

* Judge whether the camera is recognized normally by PID&VID
VID: 2bc5 PID: 0536 (color camera)
VID: 2bc5 PID: 0636 (depth camera)


## 4、Linux Sample compile
Create a build directory in the Examples directory, as follows:
![image17](Image/orbbec_sample_linux_003.png)
``` bash
cd build
cmake ..
make
```

