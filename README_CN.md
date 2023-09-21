# OrbbecSDK
![stability](https://img.shields.io/badge/stability-stable-green) ![version](https://img.shields.io/badge/version-1.7.4-green)

Orbbec 3D 相机产品软件开发套件，全面支持 UVC，实现免驱动即插即用，提供低层和高层简单易用的 API，帮助开发者在不同场景下灵活使用。

此外，该 SDK 通过内置代码兼容 Orbbec 原有的 OpenNI 协议设备，开发者可以完全迁移到 OrbbecSDK，一套代码即可支持 Orbbec 的新一代产品和老产品。

## 本仓库包含内容

* **library** : OrbbecSDK 核心库文件和 C/C++ 头文件。
* **examples** : C/C++ 示例工程源码
* **doc** : API 参考文档和示例文档。
* **driver** :  Windows 设备驱动，用于 OpenNI 协议设备（Dabai、Dabai DCW、Dabai DW、Astra mini Pro、Astra Pro Plus、A1 Pro、Gemini E、Gemini E Lite、Gemini），使用标准 UVC 协议的模块则无需安装驱动。
* **scripts** : Linux udev 规则，用于解决权限问题；Windows 时间戳注册脚本，用于解决时间戳和元数据问题。

## 支持平台

Windows 10, Ubuntu 16.04/18.04/20.04, ARM Linux 32/64 bit (Raspberry Pi 4B, Jetson Nano, A311D 等.)

*Windows 11, Ubuntu 22.04 和其他一些 Linux 平台理论上也支持，但是未经过完整测试”

## 支持产品

| ** 产品列表 ** | ** 固件版本 ** |
| --- | --- |
| Gemini 2 XL      | Obox: V1.2.5  VL:1.4.54    |
| Astra 2          | 2.8.20                     |
| Gemini 2 L       | 1.4.32                     |
| Gemini 2         | 1.4.60 /1.4.76             |
| Femto Mega       | 1.1.7  (window10、ubuntu20.04、ubuntu22.04)  |
| Astra+           | 1.0.22/1.0.21/1.0.20/1.0.19 |
| Femto            | 1.6.7                       |
| Femto W          | 1.1.8                       |
| DaBai            | 2436                        |
| DaBai DCW        | 2460                        |
| DaBai DW         | 2606                        |
| Astra Mini Pro   | 1007                        |
| Gemini E         | 3460                        |
| Gemini E Lite    | 3606                        |
| Gemini           | 3.0.18                      |
| Astra Mini S Pro | 1.0.05                      |

## 快速开始

* 获取源码

    ```bash
    git clone https://github.com/OrbbecDeveloper/OrbbecSDK.git
    ```

* 环境配置
    Linux: 安装 udev rules 文件

    ``` bash
    cd OrbbecSDK/misc/scripts
    sudo chmod +x ./install_udev_rules.sh
    ./install_udev_rules.sh
    ```

    Windows: 时间戳注册: [follow this: obsensor_metadata_win10](misc\scripts\obsensor_metadata_win10.md)

* 编译

    ```bash
    cd OrbbecSDK && mkdir build && cd build &&
    cmake .. && cmake --build . --config Release
    ```

* 运行示例

    ```bash
    cd OrbbecSDK/build/bin # build output dir
    ./OBMultiStream  # OBMultiStream.exe on Windows
    ```

* 在你的工程中使用 OrbbecSDK

    将 OrbbecSDK 拷贝到你的 CMake 工程子目录下，再 CMake 配置中增加如下配置：

    ```cmake
    cmake_minimum_required(VERSION 3.1.15)
    project(OrbbecSDKTest)

    add_executable(${PROJECT_NAME} main.cpp)

    add_subdirectory("your/path/to/OrbbecSDK")
    target_link_libraries(${PROJECT_NAME} OrbbecSDK::OrbbecSDK)
    ```

* 编写代码

    请参考[示例代码](examples) 完成您的应用代码编写。

## 相关链接

* [3D 视觉开发者社区](https://developer.orbbec.com.cn/)
* [OrbbecSDK 主页](https://developer.orbbec.com.cn/develop_details.html?id=1)
