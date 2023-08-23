# OrbbecSDK
![stability](https://img.shields.io/badge/stability-stable-green) ![version](https://img.shields.io/badge/version-1.6.3-green)

Orbbec 3D相机产品软件开发套件，全面支持UVC，实现免驱动即插即用，提供低层和高层简单易用的API，帮助开发者在不同场景下灵活使用。

此外，该SDK通过内置代码兼容Orbbec原有的OpenNI协议设备，开发者可以完全迁移到OrbbecSDK，一套代码即可支持Orbbec的新一代产品和老产品。

## 本仓库包含内容

* **library** : OrbbecSDK核心库文件和C/C++头文件。
* **examples** : C/C++示例工程源码
* **doc** : API参考文档和示例文档。
* **driver** :  Windows设备驱动，用于OpenNI协议设备（Dabai、Dabai DCW、Dabai DW、Astra mini Pro、Astra Pro Plus、A1 Pro、Gemini E、Gemini E Lite、Gemini），使用标准UVC协议的模块则无需安装驱动。
* **scripts** : Linux udev规则，用于解决权限问题；Windows时间戳注册脚本，用于解决时间戳和元数据问题。

## 支持平台

Windows 10, Ubuntu 16.04/18.04/20.04, ARM Linux 32/64 bit (Raspberry Pi 4B, Jetson Nano, A311D 等.)

*Windows 11, Ubuntu 22.04 和其他一些Linux平台理论上也支持，但是未经过完整测试”

## 支持产品

| **产品列表** | **固件版本** |
| --- | --- |
| Astra2         | 2.8.20                     |
| Gemini2 L      | 1.4.32                     |
| Gemini2        | 1.4.60                     |
| FemtoMega      | 1.1.7  (window10、ubuntu20.04 及以上)|
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

## 快速开始

* 获取源码

```bash
git clone https://github.com/OrbbecDeveloper/OrbbecSDK.git
```

* 编译和安装

```bash
cd OrbbecSDK && mkdir build && cd build &&
cmake .. && make -j4
sudo make install # install to /usr/local
# you can run `sudo make uninstall` to uninstall
sudo ldconfig # refresh ld cache
```

* 运行示例

```bash
# if you use linux, first install udev rules
sudo cp -r ./udev/* /etc/udev/rules.d/
sudo udevadm control --reload-rules && sudo udevadm trigger
cd build/examples/cpp && ./Sample-DepthViewer/OBDepthViewer
# or you can run /usr/local/bin/OBDepthViewer if you install to /usr/local
```

* 在你的工程中使用OrbbecSDK

```cmake
cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDKTest)
find_package(OrbbecSDK REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${OrbbecSDK_LIBS})
target_include_directories(${PROJECT_NAME} ${OrbbecSDK_INCLUDE_DIRS})
```

* 编写代码

请参考示例代码[examples directory](examples) 完成您的应用代码编写。

## 相关链接

* [3D视觉开发者社区](https://developer.orbbec.com.cn/)
* [OrbbecSDK主页](https://developer.orbbec.com.cn/develop_details.html?id=1)

