# OrbbecSDK
![stability](https://img.shields.io/badge/stability-stable-green) ![version](https://img.shields.io/badge/version-1.6.3-green)

The Orbbec 3D camera product software development kit，fully supports UVC, realizes driver-free plug-and-play, provides low-level and high-level simple and easy-to-use APIs, and helps developers use it flexibly in different scenarios.

Besides, this SDK is compatible with Orbbec's original OpenNI protocol devices through built-in code, so that developers can completely migrate to OrbbecSDK to support Orbbec's new generation of products and old products with one set of code.

## What is included in the repository

* **library** : OrbbecSDK core library files and C/C++ header files.
* **examples** : C/C++ samples project source code.
* **doc** : API reference documentation and sample documentation.
* **driver** : Windows device driver for OpenNI protocol devices (Dabai, Dabai DCW, Dabai DW, Astra mini Pro, Astra Pro Plus, A1 Pro, Gemini E, Gemini E Lite, Gemini). While modules that use the standard UVC protocol do not need to install drivers.
* **scripts** : Linux udev rules for resolving permission issues and Windows timestamp registration scripts for resolving timestamp and metadata issues.

## Platform support

Windows 10, Ubuntu 16.04/18.04/20.04, ARM Linux 32/64 bit (Raspberry Pi 4B, Jetson Nano, A311D etc.)

*Windows 11, Ubuntu 22.04 and other Linux platforms may also be supported, but have not been fully tested.*

## Product support

| **products list** | **firmware version** |
| --- | --- |
| Astra2         | 2.8.20                     |
| Gemini2 L      | 1.4.32                     |
| Gemini2        | 1.4.60                     |
| FemtoMega      | 1.1.7  (window10、ubuntu20.04 and above)|
| Astra+         | 1.0.22/1.0.21/1.0.20/1.0.19 |
| Femto          | 1.6.7                       |
| Femto W        | 1.1.8                       |
| Dabai          | 2436                        |
| Dabai DCW      | 2460                        |
| Dabai DW       | 2606                        |
| Astra Mini Pro | 1007                        |
| Astra Pro Plus | 2513                        |
| A1 Pro         | 3057                        |
| Gemini E       | 3460                        |
| Gemini E Lite  | 3606                        |
| Gemini         | 3.0.18                      |

## Getting started

* Get source code

```bash
git clone https://github.com/OrbbecDeveloper/OrbbecSDK.git
```

* Build && install

```bash
cd OrbbecSDK && mkdir build && cd build &&
cmake .. && make -j4
sudo make install # install to /usr/local
# you can run `sudo make uninstall` to uninstall
sudo ldconfig # refresh ld cache
```

* Run examples

```bash
# if you use linux, first install udev rules
sudo cp -r ./udev/* /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo udevadm trigger
cd build/examples/cpp && ./Sample-DepthViewer/OBDepthViewer
# or you can run /usr/local/bin/OBDepthViewer if you install to /usr/local
```

* Use OrbbecSDK in your CMake project

```cmake
cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDKTest)
find_package(OrbbecSDK REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} OrbbecSDK::OrbbecSDK)
```

* Write your code

Please refer to the examples in the [examples directory](examples) for usage.

## Related links

* [Orbbec 3D Club](https://3dclub.orbbec3d.com)
* [Download OrbbecSDK](https://orbbec3d.com/index/download.html)