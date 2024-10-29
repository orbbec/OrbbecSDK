# C++ Sample: 3.advanced.coordinate_transform

## Overview

Use the SDK interface to  transform different coordinate systems.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions

Frameset is a combination of different types of Frames

## Code overview

1. Enable color stream

    ```cpp
        auto colorProfiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            colorProfile = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, 30);
        }
        config->enableStream(colorProfile);
    ```

2. Enable depth stream

    ```cpp
        auto                                    depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
        std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
        if(depthProfiles) {
        depthProfile = depthProfiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);

        }
        config->enableStream(depthProfile);
    ```

3. Get frame data

    ```cpp
        auto colorFrame = frameSet->colorFrame();
        auto depthFrame = frameSet->depthFrame();
    ```

4. Get get stream profile

    ```cpp
        auto colorProfile =  colorFrame->getStreamProfile();
        auto depthProfile = depthFrame->getStreamProfile();
    ```

5. Get the extrinsic parameters

    ```cpp
        auto extrinsicD2C = depthProfile->getExtrinsicTo(colorProfile);
        auto extrinsicC2D = colorProfile->getExtrinsicTo(depthProfile);
    ```

6. Get the intrinsic parameters

    ```cpp
        auto colorIntrinsic = colorProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
        auto colorDistortion = colorProfile->as<ob::VideoStreamProfile>()->getDistortion();
    ```

7. Get the distortion parameters

    ```cpp
        auto depthIntrinsic = depthProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
        auto depthDistortion = depthProfile->as<ob::VideoStreamProfile>()->getDistortion();
    ```

8. Processing

    ```cpp
        if(testType == "1") {
                transformation2dto2d(colorFrame, depthFrame);
            } else if (testType == "2") {
                transformation2dto3d(colorFrame, depthFrame);
            } else if (testType == "3") {
                transformation3dto3d(colorFrame, depthFrame);
            } else if (testType == "4") {
                transformation3dto2d(colorFrame, depthFrame);
            } else {
                std::cout << "Invalid command" << std::endl;
            }
    ```

## Run Sample

Press the Esc key to exit the program.
Press the 1 key - transformation 2d to 2d
Press the 2 key - transformation 2d to 3d
Press the 3 key - transformation 3d to 3d
Press the 4 key - transformation 3d to 2d

### Result

![image](../../docs/resource/coordinate_transform.jpg)
