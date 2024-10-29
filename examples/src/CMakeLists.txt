# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)

file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")

find_package(OpenCV QUIET)
if(NOT ${OpenCV_FOUND})
    #message(WARNING "OpenCV not found, some examples may not be built. Please install OpenCV or set OpenCV_DIR to the directory of your OpenCV installation.")
    message(WARNING "OpenCV not found in system ,used third_parth from OrbbecSDK.")

    set(OpenCV_DIR ${CMAKE_CURRENT_LIST_DIR}/../../third_party/opencv)
    set(OpenCV_INCLUDE_DIRS ${OpenCV_DIR}/include)
    set(OpenCV_LIBRARY_DIRS ${OpenCV_DIR}/lib/)
    set(OpenCV_LIBS opencv_core opencv_highgui opencv_imgcodecs opencv_imgproc opencv_video opencv_videoio )
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIBRARY_DIRS})
    set(OpenCV_FOUND TRUE)
endif()

# utils
add_subdirectory(utils)

# c examples
add_subdirectory(c_examples)

# cpp examples
add_subdirectory(0.basic.enumerate)
add_subdirectory(1.stream.imu)
add_subdirectory(2.device.control)
add_subdirectory(3.advanced.point_cloud)
add_subdirectory(3.advanced.sync_align_pure)

# cpp examples with OpenCV Requirements
if(${OpenCV_FOUND})
    add_subdirectory(0.basic.quick_start)
    add_subdirectory(1.stream.depth)
    add_subdirectory(1.stream.color)
    add_subdirectory(1.stream.infrared)
    add_subdirectory(1.stream.callback)
    add_subdirectory(1.stream.multi_streams)
    add_subdirectory(3.advanced.common_usages)
    add_subdirectory(3.advanced.sync_align)
    add_subdirectory(3.advanced.hw_d2c_align)
    add_subdirectory(3.advanced.coordinate_transform)
endif()

# InstallRequiredSystemLibraries
include(InstallRequiredSystemLibraries)
