cmake_minimum_required(VERSION 3.1.15)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Sample-HelloOrbbec)
add_subdirectory(Sample-SensorControl)
add_subdirectory(Sample-HotPlugin)
add_subdirectory(Sample-DepthWorkMode)
add_subdirectory(Sample-PointCloud)
add_subdirectory(Sample-FirmwareUpgrade)

# opencv required
if (OpenCV_FOUND)
  add_subdirectory(Sample-ColorViewer)
  add_subdirectory(Sample-DepthViewer)
  add_subdirectory(Sample-InfraredViewer)
  add_subdirectory(Sample-NetDevice)
  add_subdirectory(Sample-DoubleInfraredViewer)
endif ()
