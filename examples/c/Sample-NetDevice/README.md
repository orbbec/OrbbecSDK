# C net_device sample
Femto Mega and Gemini 2 XL support network functionality. Both devices support device discovery. As long as the PC and the device are on the same network, using the network functionality is the same as using USB, with no difference in the interface.

This example demonstrates obtaining frame data over the network by connecting to the device via IP address and port number.

**Notes:**

1. Femto Mega supports network DHCP, while Gemini 2 XL does not support DHCP.
2. First, use the Orbbec Viewer tool to view the device's IP address while connected via USB. Then, set the PC's IP address to be in the same network as the device, ensuring that you can ping the device successfully.