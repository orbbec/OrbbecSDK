常见问题
========

1. **在Windows下编译C/C++ Example，运行时找不到dll库。**

运行所需要的dll都在Examples的bin目录下，将目录下的dll文件拷贝至执行文件处。

2. **在Linux下编译C/C++时，提示缺少libudev?**

udev在支持系统中是默认集成，如果缺失可以通过apt-get install
libudev0安装后再重新编译。

3. **Cmake编译C++程序报错 Found OpenCV Windows Pack but it has no
   binaries compatible with your configuration. You should manually
   point CMake variable OpenCV_DIR to your build of OpenCV library？**

配置OpenCV_DIR为OpenCVConfig.cmake文件所在位置，不同版本有差异，常见于opencv\build目录，配置后依然报错"but
it set OpenCV_FOUND to FALSE so package "OpenCV" is considered to beNOT
FOUND."可以尝试设置到opencv/build/x64/vc15/lib目录。

4. **Astra+设备切换D2C开关后深度图不再刷新。**

在当前版本中，
D2C开关需要在深度流关闭时进行控制，遇到此情景重启深度流即可恢复正常。后续版本更新中会对D2C的使用场景进行拓宽，使D2C在开流状态时也可使用。

5. **Femto设备的深度图像和红外图像不再刷新。**

为确保使用体验，Femto只有在深度、红外图像分辨率一致时（即均为640\ *480或320*\ 240）才可运行。

6. **在Windows下编译C/C++
   Example，运行时报错，提示“由于找不到opencv_world.dll，无法继续执行代码，重新安装程序可能会解决问题”。**

该问题是由于运行环境中的opencv版本与示例版本（opencv_world430.dll）不一致，需手动将opencv/build下bin目录中对应版本的dll（如opencv_world435.dll）拷贝到Examples/build/bin/Release文件夹中，再次运行即可解决。

7. **在Ubuntu下热插拔，设备无法正常使用，提示“failed to open usb device:
   error: OB_USB_STATUS_ACCESS”。**

如在使用时遇到此情景，可以尝试以下方法：1）使用终端：热插拔后需要重新对USB设备进行授权（sudo
chmod 777
/dev/bus/usb/00x/*）。2）使用OrbbecViewer工具：热插拔后对工具进行授权（sudo
./OrbbecViewer）即可恢复正常。

8. **Linux下点云显示异常。**

请检查您的显卡驱动是否安装或处于正常状态。点云渲染依赖OpenGL，而OpenGL需要调用GPU进行渲染，重新安装显卡驱动即可解决。

9. **PC正常连接设备后，打开设备失败。**

可以尝试以下方法：1）尝试在设备管理器里边将DataChannel卸载后重新拔插设备以解决该问题
2）尝试在SDK工作目录下放置该文件，然后重启SDK，以规避该问题

更新记录
========

.. _12版本更新说明:

1.2版本更新说明
---------------

1、新增支持Femto产品

2、新增数据录制及播放

3、新增RGB888格式数据流

4、新增获取彩色和深度共视FOV

5、新增时间戳单位支持us

6、新增固件升级

7、新增英文注释

8、优化控制指令

9、优化D2C功能及接口

10、优化稳定性及接口，修复bug

版权声明和许可链接
==================

`OpenCV <https://github.com/opencv/opencv/blob/master/LICENSE>`__\ \

`OpenGL <https://www.khronos.org/legal/>`__\ \ 

`ImGui <https://github.com/ocornut/imgui/blob/master/LICENSE.txt>`__
