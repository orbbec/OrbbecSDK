# Examples Code for Orbbec cameras

## Introduction

There are several examples codes for users learn how to use Orbbec cameras. Here is a brief introduction to each sample code:

| Level    | Sample                                                  | Description                                                                                                                                                                                                             |
| -------- | ------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| basic    | [quick_start](0.basic.quick_start)                      | Quick show how to use the SDK to get frames from the Orbbec RGB-D camera device.                                                                                                                                        |
| basic    | [enumerate](0.basic.enumerate)                          | Use the SDK interface to obtain camera-related information, including model, various sensors, and sensor-related configurations.                                                                                        |
| stream   | [depth](1.stream.depth)                                 | Use the SDK interface to obtain the depth stream of the camera and display it in the window.                                                                                                                            |
| stream   | [color](1.stream.color)                                 | Use the SDK interface to obtain the camera's color stream and display it in the window.                                                                                                                                 |
| stream   | [infrared](1.stream.infrared)                           | Use the SDK interface to obtain the camera IR stream and display it in the window.                                                                                                                                      |
| stream   | [imu](1.stream.imu)                                     | Use the SDK interface to obtain the camera's internal imu data and output it                                                                                                                                            |
| stream   | [multi_streams](1.stream.multi_streams)                 | Use SDK to obtain multiple camera data streams and output them.                                                                                                                                                         |
| stream   | [callback](1.stream.callback)                           | In this sample, users can obtain depth, RGB, and IR images. This sample also supports performing user-defined operations such as data acquisition, data processing, and data modification within the callback function. |
| device   | [control](2.device.control)                             | The SDK can be used to modify camera-related parameters, including laser switch, laser level intensity, white balance switch, etc.                                                                                      |
| device   | [hot_plugin](2.device.hot_plugin)                       | Use SDK to handle the settings of device unplug callback and process the acquired code stream after unplugging.                                                                                                         |
| advanced | [sync_align](3.advanced.sync_align)                     | Use the SDK interface to demonstrate the synchronization and alignment of sensor data streams, display the aligned image.                                                                                               |
| advanced | [hw_d2c_align](3.advanced.hw_d2c_align)                 | Use the SDK interface to demonstrate the application of hardware depth-to-color alignment.                                                                                                                              |
| advanced | [post_processing](3.advanced.post_processing)           | Use the SDK interface to  demonstrate post-processing operations, display post-processed images.                                                                                                                        |
| advanced | [point_cloud](3.advanced.point_cloud)                   | Connect the device to open the stream, generate a depth point cloud or RGBD point cloud and save it as a ply format file.                                                                                               |
| advanced | [preset](3.advanced.preset)                             | Use the SDK interface to set and get the preset value.                                                                                                                                                                  |
| advanced | [coordinate_transform](3.advanced.coordinate_transform) | Use the SDK interface to  transform different coordinate systems.                                                                                                                                                       |
| advanced | [hdr](3.advanced.hdr)                                   | In this sample, user can get the HDR merge image. Also supports user to toggle HDR merge and toggle alternate show origin frame.                                                                                        |
| advanced | [multi_devices](3.advanced.multi_devices)               | In this sample, users can connect multiple camera devices and get color and depth images of different cameras.                                                                                                          |
| advanced | [common_usages](3.advanced.common_usages)               | Use SDK to handle the settings of device unplug callback and process the acquired code stream after unplugging.                                                                                                         |

### C language examples

The listed examples at previous section are written in C++ language. Here is a brief introduction to c language examples:

| Level  | Sample                                      | Description                                                                                     |
| ------ | ------------------------------------------- | ----------------------------------------------------------------------------------------------- |
| basic  | [c_quick_start](c_examples/0.c_quick_start) | This is a quick start guide to start device streams using the Orbbec SDK C API.                 |
| stream | [c_enumerate](c_examples/1.c_enumerate)     | This is a enumerate guide to get device streams profile information using the Orbbec SDK C API. |
| stream | [c_depth](c_examples/2.c_depth)             | This is a depth guide to get depth stream and depth image by using the Orbbec SDK C API.        |

## Append

### The utils functions and classes used in the examples code

***utils***

It contains functions to wait for a key press with an optional timeout, get the current time in milliseconds, and parse the input option from a key press event.This is done with C++.

***utils_c***

It contains functions to get the current system timestamp and wait for keystrokes from the user, as well as a macro to check for and handle errors. These capabilities can be used in scenarios such as time measurement, user interaction, and error handling.This is done with C.

***utils_opencv***

The CVWindow class includes the following main functionalities:

The CVWindow class leverages OpenCV to create a flexible and customizable graphical interface for displaying and managing camera frames.

 *Window Creation:*

- The constructor accepts the window name, width, and height as parameters, along with an optional - arrangement mode (ArrangeMode).
- The arrangement modes include single frame display, displaying multiple frames in a row, displaying multiple frames in a column, grid display, and overlay display.

 *Image Frame Display and Processing:*

- The pushFramesToView method is used to push a set or multiple sets of image frames to the window.
- There is an internal thread processThread_ for processing image frames.
- The arrangeFrames method arranges the image frames based on the selected arrangement mode.
- The visualize and drawInfo methods are used to draw additional information on the images.

 *User Interaction:*

- The setKeyPressedCallback method sets a key press callback function.
- The setKeyPrompt method sets a prompt message.
- The addLog method adds a log message.

 *Configuration and State Management:*

- The setShowInfo method controls whether frame information should be displayed.
- The setAlpha set alpha for OVERLAY render mode
- The resize method adjusts the window size.
- The close method closes the window.
- The reset method clears the cached frames and image matrices.

### The Error Handling in the examples code

When using the Orbbec SDK, if an error occurs, the SDK reports the error by throwing an exception of type ob::Error. The ob::Error exception class typically contains detailed information about the error, which can help developers diagnose the problem.
The example uses a 'try' block to wrap the entire main function.If an exception of type ob::Error is thrown, the program will catch it and print the error message to the console.
Here is the information that can be obtained from an ob::Error:

**Function Name (getFunction()):**
Indicates the name of the function where the exception was thrown. This helps pinpoint the location of the error within the code.

**Arguments (getArgs()):**
Provides information about the arguments passed to the function when the exception occurred. This context can be useful for understanding the specific conditions under which the error happened.

**Error Message (what()):**
Returns a string describing the nature of the error. This is often the most important piece of information, as it directly explains what went wrong.

**Exception Type (getExceptionType()):**
Specifies the type of the exception. This can help categorize the error and determine appropriate handling strategies.Read the comments of the OBExceptionType enum in the libobsensor/h/ObTypes.h file for more information.

**Example Code** in C++

```cpp
catch(ob::Error &e) {
    std::cerr << "function: " << e.getFunction() << std::endl;
    std::cerr << "args: " << e.getArgs() << std::endl;
    std::cerr << "message: " << e.what() << std::endl;
    std::cerr << "type: " << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
```
