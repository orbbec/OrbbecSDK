cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDK-Samples LANGUAGES CXX C)

if(UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++11 -pthread ${CMAKE_CXX_FLAGS}")
endif()

if(NOT DEFINED OrbbecSDK_FOUND)
    set(OrbbecSDK_DIR "path/to/OrbbecSDK/lib" )
    message(STATUS "OrbbecSDK_DIR: ${OrbbecSDK_DIR}")
    find_package(OrbbecSDK REQUIRED)
endif()

# find opencv
set(OpenCV_DIR "your/path/to/opencv/lib")
find_package(OpenCV QUIET)

add_subdirectory(c)
add_subdirectory(cpp)
