# C++ Sample: 3.advanced.sync_align

## Overview

Use the SDK interface to demonstrate the synchronization and alignment of sensor data streams,display the aligned image,and exit the program using the ESC_KEY key.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions

Frameset is a combination of different types of Frames

win is used to display the frame data.

C2D（Color to Depth）is the transformation from the color image coordinate system to the depth image coordinate system.To map the pixel positions in the color image to the corresponding positions in the depth image. This is commonly used to align color and depth images so that both types of information can be used in the same coordinate system.

D2C（Depth to Color）is the transformation from the depth image coordinate system to the color image coordinate system.To map the pixel positions in the depth image to the corresponding positions in the color image. This transformation allows depth data to be applied to the color image, facilitating the annotation or analysis of depth information within the color image.

## Code overview

1. Set alignment mode

    ```cpp
    // Create a filter to align depth frame to color frame
    auto depth2colorAlign = std::make_shared<ob::Align>(OB_STREAM_COLOR);

    // create a filter to align color frame to depth frame
    auto color2depthAlign = std::make_shared<ob::Align>(OB_STREAM_DEPTH);
    ```

2. Set the callback function for the Align Filter to display the aligned frames in the window

    ```cpp
    depth2colorAlign->setCallBack([&win](std::shared_ptr<ob::Frame> frame) { win.pushFramesToView(frame); });
    color2depthAlign->setCallBack([&win](std::shared_ptr<ob::Frame> frame) { win.pushFramesToView(frame); });
   ```

3. Perform alignment processing

    ```cpp
     // Get filter according to the align mode
        std::shared_ptr<ob::Filter> alignFilter = depth2colorAlign;
        if(align_mode % 2 == 1) {
            alignFilter = color2depthAlign;
        }

        // push the frameset to the Align Filter to align the frames.
        // The frameset will be processed in an internal thread, and the resulting frames will be asynchronously output via the callback function.
        alignFilter->pushFrame(frameSet);
    ```

## Run Sample

Press the Esc key in the window to exit the program.
'T': Switch Align Mode.
'F': Toggle Synchronization.
'+/-': Adjust Transparency

### Result

Sync
![image](../../docs/resource/sync.jpg)
D2C
![image](../../docs/resource/d2c.jpg)
C2D
![image](../../docs/resource/c2d.jpg)
