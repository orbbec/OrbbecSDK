# OrbbecSDK 1.5.x
![stability](https://img.shields.io/badge/stability-unstable-orange)
![stability](https://img.shields.io/badge/branch-1.5.x-green)
---
The Orbbec 3D camera product software development kit embodies seamless utility with 
its Universal Video Class (UVC) support, enabling plug-and-play functionality without 
the need for a driver. Offering both low-level and high-level APIs that are straightforward
and user-friendly, this toolkit empowers developers with the flexibility to tailor 
solutions to a diverse array of scenarios. Equipped with this kit, the power to create
and optimize 3D camera applications is at your fingertips, making it an indispensable
tool in any developer's arsenal.

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
cd build/examples/cpp && ./Sample-ColorViewer/OBColorViewer
# or you can run /usr/local/bin/OBColorViewer if you install to /usr/local
```
* Use OrbbecSDK in your cmake project
```cmake
cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDKTest)
find_package(OrbbecSDK REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} OrbbecSDK::OrbbecSDK)
```
* Write your code

Please refer to the examples in the [examples directory](examples) for usage.
## What include in this repository

* **doc** : API reference documentation and sample documentation.

* **lib** : Windows driver (Dabai, Dabai DCW, Dabai DW, Astra mini Pro, Astra Pro Plus, A1 Pro, Gemini E, Gemini E Lite, Gemini This device uses OpenNI's  protocol, and need to install driver on Windows, while devices using standard UVC protocols do not need to install driver)

* **udev** : Linux udev rules for resolving permission issues

* **examples** : C/C++ samples, including sample project source code and compiled executables.

## Supported hardware products

| **SDK version** | **products list** | **firmware version** |
| --- | --- | --- |
| v1.5.7      | Gemini2        | 1.4.60                     |
|             | FemtoMega      | 1.1.5  (window10、ubuntu20.04、ubuntu22.04)                     |
|             | Astra+         | 1.0.22/1.0.21/1.0.20/1.0.19 |
|             | Femto          | 1.6.7                       |
|             | Femto W       | 1.1.8          |
|             | Dabai          | 2436                        |
|             | Dabai DCW      | 2460                        |
|             | Dabai DW       | 2606                        |
|             | Astra Mini Pro | 1007                        |
|             | Astra Pro Plus | 2513                        |
|             | A1 Pro         | 3057                        |
|             | Gemini E       | 3460                        |
|             | Gemini E Lite  | 3606                  |
|             | Gemini         | 3.0.18                      |
| v1.4.3 | Astra+ | 1.0.22/1.0.21/1.0.20/1.0.19 |
|  | Femto | 1.6.7 |
|  | *Femto W | 1.1.3（ Window10） |
|  | Dabai | 2436 |
|  | Dabai DCW | 2460 |
|  | Dabai DW | 2606 |
|  | Astra Mini Pro | 1007 |
| | Astra Pro Plus | 2513 |
| | Gemini | 3.0.18 |
| v1.3.1 | Astra+ | 1.0.21/1.0.20/1.0.19 |
| | Femto | 1.6.7 |
| v1.2.8 | Astra+ | 1.0.20 |
| | Femto | 1.6.7 |
| v1.1.6 | Astra+ | 1.0.20/1.0.19 |
| | *Femto | 1.5.1 |
| v1.0.2 | Astra+ | 1.0.19 |

## Related links

* [Orbbec 3D Club](https://3dclub.orbbec3d.com)
* [Download OrbbecSDK](https://orbbec3d.com/index/download.html)
