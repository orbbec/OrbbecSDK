# ColorViewer 示例

功能描述：此示例主要演示使用SDK获取彩色数据并进行绘制显示，并通过ESC_KEY键退出程序。

此示例基于C++高级API进行演示。

## 1. 创建一个Pipeline，通过Pipeline 可以打开和关闭多种类型的数据流，并获取一组帧数据
```cpp
    ob::Pipeline pipe;
```

## 2. 通过创建配置来配置要启用哪些数据流
```cpp
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableVideoStream(OB_STREAM_COLOR);
```

## 3. 启动Pipeline
```cpp
    // Start the pipeline with config
    pipe.start(config);
```

## 4. 获取彩色帧数据

以阻塞方式等待一帧数据，该帧数据是一个复合帧，包含了配置中启用的所有数据流的帧数据，并设置等待超时时间
```cpp
    auto frameSet = pipe.waitForFrames(100); // 将等待时间设置为100ms
```

```cpp
// 每30帧打印一次元数据，目前仅支持Gemini 330系列设备
auto index = colorFrame->index(); 
if(index % 30 == 0) {

    std::cout << "*************************** Color Frame #" << index << " Metadata List ********************************" << std::endl;
    for(int metaDataType = 0; metaDataType < OB_FRAME_METADATA_TYPE_COUNT; metaDataType++) {
        // Check if it is supported metaDataType for current frame
        if(colorFrame->hasMetadata((OBFrameMetadataType)metaDataType)) {
            // Get the value of the metadata
            std::cout << metaDataTypes[metaDataType] << ": " << colorFrame->getMetadataValue((OBFrameMetadataType)metaDataType) << std::endl;
        }
        else {
            std::cout << metaDataTypes[metaDataType] << ": " << "unsupported" << std::endl;
        }
    }
    std::cout << "********************************************************************************" << std::endl << std::endl;

}
```

## 5. 停止Pipeline，不再生成帧数据
```cpp
    pipe.stop();
```

## 6. 期望输出 
![image](Image/ColorViewer.png)