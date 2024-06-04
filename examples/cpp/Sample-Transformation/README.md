# C++ Transformation sample
This example uses a utility class to obtain undistorted point clouds,Demonstrate the 2D transformation between depth and color.
    
## 1.  Invoking the utils class to generate an undistorted depth point cloud. 
```cpp
    // Invoking the utils class to generate an undistorted depth point cloud.
    depthPointCloudTransformation(device, case_number);
```

## 2. Invoking the utils class to generate an undistorted RGBD point cloud.
```cpp
    // Invoking the utils class to generate an undistorted RGBD point cloud.
    RGBPointCloudTransformation(device);
```

## 3. Demonstrate Depth 2D converted to Color 2D
```cpp
    // Demonstrate Depth 2D converted to Color 2D
    depth2DToColor2DTransformation(device);
```
## 4. Demonstrate Color 2D converted to  Depth 2D
```cpp
    // Demonstrate Color 2D converted to  Depth 2D
    color2DToDepth2DTransformation(device);
```