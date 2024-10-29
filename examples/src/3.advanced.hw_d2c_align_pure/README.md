# C++ Sample: 3.advanced.hw_d2c_align_pure

## Overview

This sample demonstrates how to use the SDK to enable depth-to-color alignment on hardware devices (alse known as hardware D2C or HwD2C).

### Knowledge

- The HwD2C feature allows you to align the depth image to the color image captured by the device. This is useful for applications that require depth frame to be aligned with color frame and do want to use increase the resource usage of the host.

### Attention

- The HwD2C feature is only available on devices that support it. Please check the documentation of your device to see if it supports HwD2C.
- Is not all profile of depth stream are supported the HwD2C feature. Please call the `getD2CDepthProfileList` function of `ob::Pipeline` class to check the supported depth profile.