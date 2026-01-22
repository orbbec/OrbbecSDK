# Orbbec SDK

![stability](https://img.shields.io/badge/stability-stable-green) ![version](https://img.shields.io/badge/version-1.10.27-green)

> [!重要]
>
> 欢迎使用 Orbbec SDK,在开始使用此版本的 Orbbec SDK 之前,请务必查看以下设备支持列表,以确认兼容性。
>

main分支基于 Orbbec SDK v1,兼容 Orbbec 原有的 OpenNI 协议设备,使开发者能够迁移到 Orbbec SDK 并用一套代码同时支持新旧产品。

随着 2024 年 10 月的重大更新,Orbbec SDK 变为开源,具有更高的灵活性和可扩展性。此次更新确保兼容所有遵循 UVC 标准的新款 Orbbec USB 产品。然而[Orbbec SDK v2](https://gitee.com/orbbecdeveloper/OrbbecSDK_v2)不再支持 Orbbec 的传统 OpenNI 协议设备,这些设备将在 Orbbec SDK v1 分支继续获得 bug 修复支持。我们建议先确认你的设备是否被 Orbbec SDK v2 支持,如果支持请使用新版本。

以下是main分支 (v1.x) 和 Orbbec SDK v2 (v2.x) 的设备支持列表：

<table border="1" style="border-collapse: collapse; text-align: left; width: 100%;">
  <thead>
    <tr style="background-color: #1f4e78; color: white; text-align: center;">
      <th>Product Series</th>
      <th>Product</th>
      <th><a href="https://gitee.com/orbbecdeveloper/OrbbecSDK/tree/main/" style="color: black; text-decoration: none;">Orbbec SDK v1</a></th>
      <th><a href="https://gitee.com/orbbecdeveloper/OrbbecSDK_v2/tree/main/" style="color: black; text-decoration: none;">Orbbec SDK v2</a></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td style="text-align: center; font-weight: bold;">Gemini 305</td>
      <td>Gemini 305</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td rowspan="2" style="text-align: center; font-weight: bold;">Gemini 340</td>
      <td>Gemini 345</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 345Lg</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
      <tr>
      <td style="text-align: center; font-weight: bold;">Gemini 435Le</td>
      <td>Gemini 435Le</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td rowspan="8" style="text-align: center; font-weight: bold;">Gemini 330</td>
      <td>Gemini 335Le</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 335</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 336</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 330</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 335L</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 336L</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 330L</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 335Lg</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td rowspan="5" style="text-align: center; font-weight: bold;">Gemini 2</td>
      <td>Gemini 2</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 2 L</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 2 XL</td>
      <td>recommended for new designs</td>
      <td>to be supported</td>
    </tr>
    <tr>
      <td>Gemini 215</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Gemini 210</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td rowspan="3" style="text-align: center; font-weight: bold;">Femto</td>
      <td>Femto Bolt</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Femto Mega</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Femto Mega I</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td rowspan="3" style="text-align: center; font-weight: bold;">Astra</td>
      <td>Astra 2</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Astra+</td>
      <td>limited maintenance</td>
      <td>not supported</td>
    </tr>
    <tr>
      <td>Astra Pro Plus</td>
      <td>limited maintenance</td>
      <td>not supported</td>
    </tr>
    <tr>
      <td rowspan="2" style="text-align: center; font-weight: bold;">Astra Mini</td>
      <td>Astra Mini Pro</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    <tr>
      <td>Astra Mini S Pro</td>
      <td>full maintenance</td>
      <td>recommended for new designs</td>
    </tr>
    </tr>
        <tr>
      <td rowspan="2" style="text-align: center; font-weight: bold;">LiDAR</td>
      <td>Pulsar ME450</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
    <tr>
      <td>Pulsar SL450</td>
      <td>not supported</td>
      <td>recommended for new designs</td>
    </tr>
  </tbody>
</table>

**注意**: 
1. 如果未找到您的设备,请联系我们的 FAE 或销售代表寻求帮助。
2. 如果设备同时支持 Orbbec SDK v1 与 Orbbec SDK v2，两个SDK所支持的最低固件版本可能不同。支持的最低固件版本请查看支持的设备列表。特别需要注意的是，Astra Mini Pro 与 Astra Mini S Pro 在 v1.x.x 与 v2.x.x 固件中采用了不同的通信协议：Orbbec SDK v1 仅支持 v1.x.x 固件，而 Orbbec SDK v2 仅支持 v2.x.x 固件。若需从 v1.x.x 升级至 v2.x.x，请参考[链接](https://gitee.com/orbbecdeveloper/OrbbecSDK_v2#12-upgrading-from-openni-protocol-to-uvc-protocol)。

**定义**:

1. recommended for new designs: 我们将提供完整支持,包括新功能、bug 修复和性能优化;
2. full maintenance: 我们将提供 bug 修复支持;
3. limited maintenance: 我们将提供关键 bug 修复支持;
4. not supported: 本版本不支持该设备;
5. to be supported: 我们在未来增加对该设备的支持。


## 支持的设备

| **Products List** | **Minimal Firmware Version** |
|-------------------|-----------------------------|
| Gemini 330        | 1.2.20                      |    
| Gemini 330L        | 1.2.20                     |  
| Gemini 335        | 1.2.20                      | 
| Gemini 335L        | 1.2.20                     | 
| Gemini 336        | 1.2.20                      |
| Gemini 336L        | 1.2.20                     |    
| Femto Bolt        | 1.0.6                       |         
| Femto Mega        | 1.1.7                       |        
| Femto Mega I      | 2.0.2                       |  
| Gemini 2 XL       | Obox: V1.2.5  VL:1.4.54     |
| Astra 2           | 2.8.20                      |
| Gemini 2 L        | 1.4.32                      |
| Gemini 2          | 1.4.60                      |       
| Astra+            | 1.0.19                      |                      
| Femto             | 1.6.7                       |
| Femto W           | 1.1.8                       | 
| DaBai             | 2436                        | 
| DaBai DCW         | 2460                        | 
| DaBai DW          | 2606                        | 
| Gemini E          | 3460                        |
| Gemini E Lite     | 3606                        | 
| Gemini            | 3018                        |
| Astra Mini Pro    | 1007                        | 
| Astra Mini S Pro  | 1005                        |

## 本仓库包含内容

* **library** : Orbbec SDK 核心库文件及 C/C++ 头文件。
* **examples** : C/C++ 示例项目源代码。
* **doc** : API 参考文档及示例文档。
* **driver** : 用于 OpenNI 协议设备（Dabai、Dabai DCW、Dabai DW、Astra mini Pro、Astra Pro Plus、A1 Pro、Gemini E、Gemini E Lite、Gemini）的 Windows 设备驱动。使用标准 UVC 协议的模块无需安装驱动。
* **scripts** : Linux 下用于解决权限问题的 udev 规则,以及 Windows 下用于解决时间戳和Metadata问题的注册脚本。



## 软件license结构说明

当前遵循软件license结构如下:
![SdkLicenseDiagram](doc/resources/sdk_licensediagram.png)

每个模块的许可信息可以在[license file](LICENSE.txt)中找到。

## 平台支持

| Operating system | Requirement                                                                                  | Description                                                                                                                             |
|------------------|----------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------|
| Windows          | - Windows 10 April 2018 (version 1803, operating system build 17134) release (x64) or higher ,windows 11 | The generation of the VS project depends on the installation of the VS version and the cmake version, and supports VS2015/vs2017/vs2019 |
| Linux            | - Linux Ubuntu 16.04/18.04/20.04/22.04 (x64)                                                       | Support GCC 7.5                                                                                                                         |
| Arm32            | - Linux Ubuntu 16.04/18.04/20.04                                                             | Support GCC 7.5                                                                                                                         |
| Arm64            | - Linux Ubuntu 18.04/20.04/22.04                                                                   | Support GCC 7.5                                                                                                                         |
| MacOS            | - M series chip, 11.0 and above; Intel x86 chip, 10.15 and above.                            | supported hardware products: Gemini 2, Gemini 2 L, Astra 2, Gemini 2 XL, Femto Mega, Gemini 330 series                                                   |

* 注意: 当前版本支持的Arm平台: NVIDIA Jetson AGX Orin (arm64), NVIDIA Jetson Orin NX (arm64), NVIDIA Jetson Orin Nano (arm64), NVIDIA Jetson AGX Xavier (arm64), NVIDIA Jetson Xavier NX (arm64), NVIDIA Jetson Nano (arm64), A311D (arm64), Raspberry Pi 4 (arm64), Raspberry Pi 3 (arm32), RK3399 (arm64), 其它Arm系统,可能需要重新交叉编译。



## OrbbecViewer

OrbbecViewer 是一个基于 Orbbec SDK 的实用工具,可用于查看 Orbbec 相机的数据流并控制相机。
![OrbbecViewer](doc/resources/OrbbecViewer.png)

**支持的平台**: Windows x64, Linux x64 & ARM64, MacOS M series chip & Intel x86 chip

**下载链接**: [Releases](https://gitee.com/orbbecdeveloper/OrbbecSDK/releases/)

**OrbbecViewer使用说明**: [OrbbecViewer User Manual](doc/OrbbecViewer/English/OrbbecViewer.md)

## 入门指南

### 获取源码

```bash
git clone https://gitee.com/orbbecdeveloper/OrbbecSDK.git
```

你也可以通过二进制包安装,更多信息请参考[installation guidance](doc/tutorial/English/Installation_guidance.md)
### 环境配置

* Linux:

如果你通过 Debian 包安装,可以跳过 udev 规则文件的安装。如果没有,请使用以下命令安装:

  ```bash
  cd OrbbecSDK/misc/scripts
  sudo chmod +x ./install_udev_rules.sh
  sudo ./install_udev_rules.sh
  sudo udevadm control --reload && sudo udevadm trigger
  ```
* Windows:

  时间戳注册: [follow this: obsensor_metadata_win10](misc/scripts/obsensor_metadata_win10.md)
* *更多信息：[Environment Configuration](doc/tutorial/English/Environment_Configuration.md)*

## 示例

示例代码位于 `./examples` 目录,可使用 CMake 构建。

### 编译

```bash
cd OrbbecSDK && mkdir build && cd build && cmake .. && cmake --build . --config Release
```

### 运行示例

将 Orbbec 相机连接到电脑后,按以下步骤运行:

```bash
cd OrbbecSDK/build/bin # build output dir
./OBMultiStream  # OBMultiStream.exe on Windows
```

注意：在 MacOS 上需要 sudo 权限。

```bash
# MacOS
cd OrbbecSDK/build/bin # build output dir
cp ../../lib/macOS/* . # copy lib to output dir
sudo ./OBMultiStream
```

下图是 Gemini2 设备运行 MultiStream 的结果,其他设备的运行结果可能有所不同。

![Multistream](doc/resources/MultiStream.png)

注意：
在 Linux/Arm 平台上,此示例要求用户使用 OpenCV 4.2 或更高版本编译,否则无法渲染。

### 在 CMake 项目中使用 Orbbec SDK

在 CMakeLists.txt 文件中查找并链接 Orbbec SDK,例如:

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

## 文档

### 概览
* Overview Document：[doc/tutorial/English/OverviewDocument.md](doc/tutorial/English/OverviewDocument.md)

### 教程
* Quick Start：[doc/tutorial/English/Environment_Configuration.md](doc/tutorial/English/Environment_Configuration.md)
* Examples: [examples/README.md](examples/README.md)

### API 参考
* API Reference: [doc/api/English/index.html](https://orbbec.github.io/OrbbecSDK/doc/api/English/index.html)
* OrbbecSDK C++ API user guide：[doc/tutorial/English/OrbbecSDK_C++_API_user_guide-v1.0.pdf](https://orbbec.github.io/OrbbecSDK/doc/tutorial/English/OrbbecSDK_C++_API_user_guide-v1.0.pdf)

### 多相机同步
* Multi Camera Sync Document: [Multi Camera Sync](https://www.orbbec.com/docs/set-up-cameras-for-external-synchronization_v1-2/)

## 常见问题(FAQ)

### 多相机无数据流

**供电不足**:
  - 确保所有相机不连接在同一个集线器上。
  - 使用带电源的集线器为每个相机提供足够电力。

**高分辨率**:
  - 尝试降低分辨率,以解决数据流问题。

**增加 usbfs_memory_mb 值**:
  - 将 `usbfs_memory_mb` 值增加到 128MB,执行以下命令：
    ```bash
    echo 128 | sudo tee /sys/module/usbcore/parameters/usbfs_memory_mb
    ```
  - 若要永久生效,请参考 [this link](https://github.com/OpenKinect/libfreenect2/issues/807)。

## 相关链接

* [Orbbec SDK Repo](https://github.com/orbbec/OrbbecSDK)
* [Orbbec Main Page](https://www.orbbec.com/)
* [Orbbec 3D Club](https://3dclub.orbbec3d.com)
