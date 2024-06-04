# net_device 示例
Femto Mega 和 Gemini 2 XL 支持网络功能，这两个设备都支持设备发现，只要确保PC和设备在同一个网络，使用网络功能就像使用USB 一样，接口没有区别。 

这个示例用来演示通过网络获取帧数据，通过IP地址和端口号连接设备。

**注意事项：** 

1、Femto Mega 支持网络DHCP, Gemini 2 XL 不支持DHCP。  
2、首先通过Orbbec Viewer工具，在USB连接下，可以查看设备的IP地址，然后将PC 的IP 设置和 设备同一个网络内，确保能ping通。