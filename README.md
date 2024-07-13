# Orbbec SDK

![stability](https://img.shields.io/badge/stability-stable-green) ![version](https://img.shields.io/badge/version-1.10.8-green)

The Orbbec 3D camera product software development kit fully supports UVC, enabling driver-free plug-and-play. It provides both low-level and high-level APIs that are simple and easy to use, allowing developers to use it flexibly in different scenarios.

Additionally, this SDK is compatible with Orbbec's original OpenNI protocol devices through built-in code, enabling developers to migrate to Orbbec SDK to support both new and old products with one set of code.

If you are a user in China, it is recommended to use Gitee([gitee Repo](https://gitee.com/orbbecdeveloper/OrbbecSDK)).

## What is included in the repository

* **library** : Orbbec SDK core library files and C/C++ header files.
* **examples** : C/C++ samples project source code.
* **doc** : API reference documentation and sample documentation.
* **driver** : Windows device driver for OpenNI protocol devices (Dabai, Dabai DCW, Dabai DW, Astra mini Pro, Astra Pro Plus, A1 Pro, Gemini E, Gemini E Lite, Gemini). While modules that use the standard UVC protocol do not need to install drivers.
* **scripts** : Linux udev rules for resolving permission issues and Windows timestamp registration scripts for resolving timestamp and metadata issues.

## license structure

The current software license structure is as follows

![SdkLicenseDiagram](doc/resources/sdk_licensediagram.png)

More information about the license of each module can be found in the [license file](LICENSE.txt).

## Platform support

| Operating system | Requirement                                                                                  | Description                                                                                                                             |
|------------------|----------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| Windows          | - Windows 10 April 2018 (version 1803, operating system build 17134) release (x64) or higher | The generation of the VS project depends on the installation of the VS version and the cmake version, and supports VS2015/vs2017/vs2019 |
| Linux            | - Linux Ubuntu 16.04/18.04/20.04 (x64)                                                       | Support GCC 7.5                                                                                                                         |
| Arm32            | - Linux Ubuntu 16.04/18.04/20.04                                                             | Support GCC 7.5                                                                                                                         |
| Arm64            | - Linux Ubuntu 18.04/20.04                                                                   | Support GCC 7.5                                                                                                                         |
| MacOS            | - M series chip, 11.0 and above; Intel x86 chip, 10.15 and above.                            | supported hardware products: Gemini 2, Gemini 2 L, Astra 2, Gemini 2 XL, Femto Mega, Gemini 330 series                                                   |

* Note: supported Arm platforms: jestson nano (arm64)、 AGX Orin(arm64)、Orin NX (arm64)、Orin Nano(arm64)、AGX Xavier、Xavier NX、A311D (arm64), Raspberry Pi 4 (arm64), Raspberry Pi 3 (arm32), rk3399 (arm64), other Arm systems, may need to Cross-compile.
* Windows 11, Ubuntu 22.04 and other Linux platforms may also be supported, but have not been fully tested.

## Product support

| **Products List** | **Firmware Version**        |
|-------------------|-----------------------------|
| Gemini 335        | 1.2.20                      |
| Gemini 335L        | 1.2.20                     |
| Femto Bolt        | 1.0.6/1.0.9                 |
| Femto Mega        | 1.1.7/1.2.7                 |
| Gemini 2 XL       | Obox: V1.2.5  VL:1.4.54     |
| Astra 2           | 2.8.20                      |
| Gemini 2 L        | 1.4.32                      |
| Gemini 2          | 1.4.60 /1.4.76              |
| Astra+            | 1.0.22/1.0.21/1.0.20/1.0.19 |
| Femto             | 1.6.7                       |
| Femto W           | 1.1.8                       |
| DaBai             | 2436                        |
| DaBai DCW         | 2460                        |
| DaBai DW          | 2606                        |
| Astra Mini Pro    | 1007                        |
| Gemini E          | 3460                        |
| Gemini E Lite     | 3606                        |
| Gemini            | 3.0.18                      |
| Astra Mini S Pro  | 1.0.05                      |

## OrbbecViewer

OrbbecViewer is a useful tool based on Orbbec SDK, that can be used to view the data stream from the Orbbec camera and control the camera.
![OrbbecViewer](doc/resources/OrbbecViewer.png)

**Supported platforms**: Windows x64, Linux x64 & ARM64, MacOS M series chip & Intel x86 chip

**Download link**: [Releases](https://github.com/orbbec/OrbbecSDK/releases)

**OrbbecViewer User Manual**: [OrbbecViewer User Manual](doc/OrbbecViewer/English/OrbbecViewer.md)

## Getting started

### Get source code

```bash
git clone https://github.com/orbbec/OrbbecSDK.git
```
Alternatively, you can install via binary packages, please refer to [installation guidance](doc/tutorial/English/Installation_guidance.md) for more information.

### Environment setup

* Linux:

 If you installed via a debian package, you can skip the installation of the udev rules file. If not, please install it using the following commands:

  ```bash
  cd OrbbecSDK/misc/scripts
  sudo chmod +x ./install_udev_rules.sh
  sudo ./install_udev_rules.sh
  sudo udevadm control --reload && sudo udevadm trigger
  ```
* Windows:

  Timestamp registration: [follow this: obsensor_metadata_win10](misc/scripts/obsensor_metadata_win10.md)
* *For more information, please refer to：[Environment Configuration](doc/tutorial/English/Environment_Configuration.md)*

## Examples

The sample code is located in the `./examples` directory and can be built using CMake.

### Build

```bash
cd OrbbecSDK && mkdir build && cd build && cmake .. && cmake --build . --config Release
```

### Run example

To connect your Orbbec camera to your PC, run the following steps:

```bash
cd OrbbecSDK/build/bin # build output dir
./OBMultiStream  # OBMultiStream.exe on Windows
```

Notes: On MacOS, sudo privileges are required.

```bash
# MacOS
cd OrbbecSDK/build/bin # build output dir
sudo ./OBMultiStream
```

The following image is the result of running MultiStream on the Gemini2 device. Other Devices run result maybe different.

![Multistream](doc/resources/MultiStream.png)

Notes:
On the Linux/Arm platform ,this sample requires users to compile with Opencv4.2 or above,otherwise, it cannot be rendered.

### Use Orbbec SDK in your CMake project

Find and link Orbbec SDK in your CMakeLists.txt file like this:

```cmake
cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDKTest)

add_executable(${PROJECT_NAME} main.cpp)

# find Orbbec SDK
set(OrbbecSDK_DIR "/your/path/to/OrbbecSDK")
find_package(OrbbecSDK REQUIRED)

# link Orbbec SDK
target_link_libraries(${PROJECT_NAME} OrbbecSDK::OrbbecSDK)
```

## Documents

### Overview
* Github Pages：[https://orbbec.github.io/OrbbecSDK/](https://orbbec.github.io/OrbbecSDK/)
* Overview Document：[doc/tutorial/English/OverviewDocument.md](doc/tutorial/English/OverviewDocument.md)

### Tutorials
* Quick Start：[doc/tutorial/English/Environment_Configuration.md](doc/tutorial/English/Environment_Configuration.md)
* Examples: [examples/README.md](examples/README.md)

### API Reference
* API Reference: [doc/api/English/index.html](https://orbbec.github.io/OrbbecSDK/doc/api/English/index.html)
* OrbbecSDK C++ API user guide：[doc/tutorial/English/OrbbecSDK_C++_API_user_guide-v1.0.pdf](https://orbbec.github.io/OrbbecSDK/doc/tutorial/English/OrbbecSDK_C++_API_user_guide-v1.0.pdf)

## Related links

* [Orbbec SDK Repo](https://github.com/orbbec/OrbbecSDK)
* [Orbbec Main Page](https://www.orbbec.com/)
* [Orbbec 3D Club](https://3dclub.orbbec3d.com)
