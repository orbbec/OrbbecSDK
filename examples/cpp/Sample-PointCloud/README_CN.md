# 点云示例

功能描述：连接设备开流 ,生成深度点云或RGBD点云并保存成ply格式文件，并通过ESC_KEY键退出程序

本示例基于C++ High Level API进行演示

## 1. 创建点云保存成ply格式文件函数
ply文件格式详细描述可在网络上查看<br />首先创建两个函数来保存从流里面获取到的点云数据，这是保存普通点云数据的函数
```cpp
    //保存点云数据到ply
    void savePointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
        int   pointsSize = frame->dataSize() / sizeof(OBPoint);
        FILE *fp         = fopen(fileName.c_str(), "wb+");
        fprintf(fp, "ply\n");
        fprintf(fp, "format ascii 1.0\n");
        fprintf(fp, "element vertex %d\n", pointsSize);
        fprintf(fp, "property float x\n");
        fprintf(fp, "property float y\n");
        fprintf(fp, "property float z\n");
        fprintf(fp, "end_header\n");
    
        OBPoint *point = (OBPoint *)frame->data();
        for(int i = 0; i < pointsSize; i++) {
            fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
            point++;
        }
    
        fflush(fp);
        fclose(fp);
    }
```

再创建一个保存彩色点云数据的函数用于存储彩色点云数据
```cpp
    //保存彩色点云数据到ply
    void saveRGBPointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
        int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
        FILE *fp         = fopen(fileName.c_str(), "wb+");
        fprintf(fp, "ply\n");
        fprintf(fp, "format ascii 1.0\n");
        fprintf(fp, "element vertex %d\n", pointsSize);
        fprintf(fp, "property float x\n");
        fprintf(fp, "property float y\n");
        fprintf(fp, "property float z\n");
        fprintf(fp, "property uchar red\n");
        fprintf(fp, "property uchar green\n");
        fprintf(fp, "property uchar blue\n");
        fprintf(fp, "end_header\n");
    
        OBColorPoint *point = (OBColorPoint *)frame->data();
        for(int i = 0; i < pointsSize; i++) {
            fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
            point++;
        }
    
        fflush(fp);
        fclose(fp);
    }
```
设置Log等级，避免过多Info等级的Log影响点云输出的结果

    ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_ERROR);

## 2. 创建一个Pipeline，通过Pipeline可以很容易的打开和关闭多种类型的流并获取一组帧数据
```cpp
    ob::Pipeline pipeline;
```

## 3. 配置color流
```cpp
    auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
    if(colorProfiles) {
        auto profile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
        colorProfile = profile->as<ob::VideoStreamProfile>();
    }
    config->enableStream(colorProfile);
```

## 4. 配置深度流
```cpp
    std::shared_ptr<ob::StreamProfileList> depthProfileList;
    OBAlignMode                            alignMode = ALIGN_DISABLE;
    if(colorProfile) {
        // Try find supported depth to color align hardware mode profile
        depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
        if(depthProfileList->count() > 0) {
            alignMode = ALIGN_D2C_HW_MODE;
        }
        else {
            // Try find supported depth to color align software mode profile
            depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
            if(depthProfileList->count() > 0) {
                alignMode = ALIGN_D2C_SW_MODE;
            }
        }
    }
    else {
        depthProfileList = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
    }
    
    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            // Select the profile with the same frame rate as color.
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
            }
        }
        catch(...) {
            depthProfile = nullptr;
        }
    
        if(!depthProfile) {
            // If no matching profile is found, select the default profile.
            depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
        }
        config->enableStream(depthProfile);
```    

## 5. 开启D2C对齐, 生成RGBD点云时需要开启
```cpp
    // 开启D2C对齐, 生成RGBD点云时需要开启
    config->setAlignMode(ALIGN_D2C_HW_MODE);
```

## 6. 启动Pipeline
```cpp
    pipeline.start( config );
```

## 7. 创建点云Filter对象，并且设置相机内参
```cpp
    // 创建点云Filter对象（点云Filter创建时会在Pipeline内部获取设备参数, 所以尽量在Filter创建前配置好设备）
    ob::PointCloudFilter pointCloud;
    
    //获取相机内参传入点云Filter中
    auto cameraParam = pipeline.getCameraParam();
    pointCloud.setCameraParam(cameraParam)
```

设置些操作提示
```cpp
     std::cout << "Press R to create rgbd pointCloud and save to ply file! " << std::endl;
     std::cout << "Press d to create depth pointCloud and save to ply file! " << std::endl;
     std::cout << "Press ESC to exit! " << std::endl;
```

## 8. 设置主流程通过上面创建的点云Filter对象获取并保存点云数据
```cpp
    if(key == 'R' || key == 'r') {
      count = 0;
      //限制最多重复10次
      while(count++ < 10) {
        //等待一帧数据，超时时间为100ms
        auto frameset = pipeline.waitForFrames(100);
        if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
          try {
            //生成彩色点云并保存
            std::cout << "Save RGBD PointCloud ply file..." << std::endl;
            pointCloud.setCreatePointFormat(OB_FORMAT_RGB_POINT);
            std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
            saveRGBPointsToPly(frame, "RGBPoints.ply");
            std::cout << "RGBPoints.ply Saved" << std::endl;
          }
          catch(std::exception &e) {
            std::cout << "Get point cloud failed" << std::endl;
          }
          break;
        }
      }
    }
    else if(key == 'D' || key == 'd') {
      count = 0;
      //限制最多重复10次
      while(count++ < 10) {
        //等待一帧数据，超时时间为100ms
        auto frameset = pipeline.waitForFrames(100);
        if(frameset != nullptr && frameset->depthFrame() != nullptr) {
          try {
            //生成点云并保存
            std::cout << "Save Depth PointCloud to ply file..." << std::endl;
            pointCloud.setCreatePointFormat(OB_FORMAT_POINT);
            std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
            savePointsToPly(frame, "DepthPoints.ply");
            std::cout << "DepthPoints.ply Saved" << std::endl;
          }
          catch(std::exception &e) {
            std::cout << "Get point cloud failed" << std::endl;
          }
          break;
        }
      }
    }
```

## 9. 最后通过Pipeline来停止流
```cpp
     pipeline.stop();
```

程序正常退出后会释放资源

## 10. 预期输出

![image](Image/PointCloud.png)
