cmake_minimum_required(VERSION 3.1.15)

add_definitions(-DEXAMPLE_CUSTOMER_MUYUAN)

add_subdirectory(Sample-HelloOrbbec)
add_subdirectory(Sample-SensorControl)
add_subdirectory(Sample-ImuReader)
add_subdirectory(Sample-DepthWorkMode)
add_subdirectory(Sample-HotPlugin)


# opencv required
if(${OpenCV_FOUND})
    add_subdirectory(Sample-ColorViewer)
    add_subdirectory(Sample-DepthViewer)
    add_subdirectory(Sample-InfraredViewer)
    add_subdirectory(Sample-DoubleIRViewer)
    add_subdirectory(Sample-SyncAlignViewer)
    add_subdirectory(Sample-PointCloud)
    add_subdirectory(Sample-MultiDevice)
    add_subdirectory(Sample-MultiDeviceSync)
    add_subdirectory(Sample-NetDevice)
    add_subdirectory(Sample-SaveToDisk)
    add_subdirectory(Sample-Recorder)
    add_subdirectory(Sample-Playback)
endif()
