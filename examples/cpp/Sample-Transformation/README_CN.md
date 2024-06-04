# C++ Transformation 示例
该示例使用工具类获取无畸变的点云，并演示深度和彩色之间的2D转换。
    
## 1.  调用工具类生成无畸变的深度点云
```cpp
    // Invoking the utils class to generate an undistorted depth point cloud.
    depthPointCloudTransformation(device, case_number);
```

## 2. 调用工具类生成无畸变的RGBD点云
```cpp
    // Invoking the utils class to generate an undistorted RGBD point cloud.
    RGBPointCloudTransformation(device);
```

## 3. 演示深度2D转换为彩色2D
```cpp
    // Demonstrate Depth 2D converted to Color 2D
    depth2DToColor2DTransformation(device);
```
## 4. 演示彩色2D转换为深度2D
```cpp
    // Demonstrate Color 2D converted to  Depth 2D
    color2DToDepth2DTransformation(device);
```