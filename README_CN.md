# OrbbecSDK
<!-- 本文档面向开发者 -->
新一代 Orbbec 3D 相机产品软件开发套件，全面支持 UVC，实现免驱即插即用，提供低阶和高阶简洁、易用的 API，帮助开发在不同场景下灵活使用。

## 目录说明

1.Doc
> API 参考文档及示例说明文档。

2.Driver
> windows device 驱动程序，只有windows的部分模组需要安装。（Dabai、Dabai DCW、Dabai DW、Astra mini Pro、Astra Pro Plus、A1 Pro、Gemini E、Gemini E Lite、Gemini 这些使用的 OpenNI 的私有协议的模组，在 windows 上需要安装私有的驱动，而使用标准 UVC 协议的模组，不需要安装驱动）。

3.Script
> Windows：时间戳注册脚本，用于解决 Windows 下获取 UVC 时间戳及 Metadata 问题；
> Linux：udev rules 安装脚本，用于解决设备访问权限问题。

4.Example
> C/C++ 示例，包括示例工程源码和已编译好的可执行文件。

5.SDK
> OrbbecSDK 头文件和库文件、OrbbecSDK运行配置文件。

## 支持的硬件产品

| **SDK 版本** | **产品列表** | **固件版本** |
| --- | --- | --- |
| v1.5.7      | Gemini2        | 1.4.60                     |
|             | FemtoMega      | 1.1.5  (支持 window10、ubuntu20.04 及以上系统)                     |
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
|  | *Femto W | 1.1.3（仅支持 Window10） |
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

## 相关链接

* [3D视觉开发者社区](https://developer.orbbec.com.cn/)
* [OrbbecSDK主页](https://developer.orbbec.com.cn/develop_details.html?id=1)

