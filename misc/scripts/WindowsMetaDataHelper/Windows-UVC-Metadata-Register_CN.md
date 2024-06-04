# Windows UVC Metadata Register

由于设备必须使用 `obsensor_metadata_win10.ps1` 脚本进行注册，用户需要每次连接新设备时运行此脚本。这个过程可能会很繁琐，也容易忘记，导致无法获取到Metadata。

这段代码的目的是演示如何在 Windows 平台上通过编程方式注册Metadata。

**这个示例只支持 Windows 平台**
## 背景

要在 Windows 平台上获取 UVC 自定义元数据，需要向注册表中的设备条目添加特殊字段。相关信息如下：
https://learn.microsoft.com/en-us/windows-hardware/drivers/stream/uvc-extensions-1-5

![image](image/metadata.png)

## 解决方案

Orbbec 设备在注册表中的条目是固定的。遍历此路径中的所有条目，找到与当前设备匹配的 pid 和 vid，并将相应的字段添加到成功匹配的条目中，以获取 UVC 自定义元数据。
```cpp
    "SYSTEM\CurrentControlSet\Control\DeviceClasses\e5323777-f976-4f5b-9b55-b94699c46e44"
    
    "SYSTEM\CurrentControlSet\Control\DeviceClasses\65E8773D-8F56-11D0-A3B9-00A0C9223196"
```