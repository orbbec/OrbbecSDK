# C++ Sample: 3.advanced.point_clout

## Overview

Connect the device to open the stream, generate a depth point cloud or RGBD point cloud and save it as a ply format file, and exit the program through the ESC\_KEY key

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions

## Code overview

1. Create Align Filter and point cloud Filter.

    ```cpp
        // Create a point cloud Filter, which will be used to generate pointcloud frame from depth and color frames.
        auto pointCloud = std::make_shared<ob::PointCloudFilter>();
        auto align = std::make_shared<ob::Align>(OB_STREAM_COLOR); // align depth frame to color frame
    ```

2. Create RGBD Point Cloud.Note that the AlignFilter processing is needed before the PointCloud processing.

    ```cpp
        // align depth frame to color frame
        auto alignedFrameset = align->process(frameset);

        // set to create RGBD point cloud format (will be effective only if color frame and depth frame are contained in the frameset)
        pointCloud->setCreatePointFormat(OB_FORMAT_RGB_POINT);

        // process the frameset to generate point cloud frame
        std::shared_ptr<ob::Frame> frame = pointCloud->process(alignedFrameset);
    ```

3. create Depth PointCloud

    ```cpp
            // set to create depth point cloud format
        auto alignedFrameset = align->process(frameset);

        // set to create point cloud format
        pointCloud->setCreatePointFormat(OB_FORMAT_POINT);

        // process the frameset to generate point cloud frame (pass into a single depth frame to process is also valid)
        std::shared_ptr<ob::Frame> frame = pointCloud->process(alignedFrameset);
    ```

## Run Sample

Press R or r to create RGBD PointCloud and save to ply file!
Press D or d to create Depth PointCloud and save to ply file!

Press ESC to exit!

### Result

![image](../../docs/resource/point_cloud.jpg)
