# C++ Sample: 0.basic.quick_start

## Overview

Use the SDK interface to quickly obtain the camera video stream and display it in the window.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions

Frameset is a combination of different types of Frames

win is used to display the frame data.

## Code overview

1. Instantiate the pipeline using the default configuration file and quickly open the video stream

    ```cpp
        // Create a pipeline.
        ob::Pipeline pipe;

        // Start the pipeline with default config.
        // Modify the default configuration by the configuration file: "OrbbecSDKConfig.xml"
        pipe.start();
    ```

2. Create a window for showing the frames, and set the size of the window

    ```cpp
        // Create a window for showing the frames, and set the size of the window.
        ob_smpl::CVWindow win("QuickStart", 1280, 720, ob_smpl::ARRANGE_ONE_ROW);
    ```

3. Open the window and display the video stream. The video stream waits for a frame of data in a blocking manner. The frame is a composite frame containing the frame data of all streams enabled in the configuration, and the waiting timeout of the frame is set

    ```cpp
        while(win.run()) {
            // Wait for frameSet from the pipeline, the default timeout is 1000ms.
            auto frameSet = pipe.waitForFrameset();

            // Push the frames to the window for showing.
            win.pushFramesToView(frameSet);
        }
    ```

4. Use pipeline to close the video stream

    ```cpp
        // Stop the Pipeline, no frame data will be generated
        pipe.stop();
    ```

## Run Sample

Press the Esc key in the window to exit the program.

### Result

![image](../../docs/resource/quick_start.jpg)
