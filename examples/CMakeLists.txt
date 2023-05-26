cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDK-Samples LANGUAGES CXX C)

if (UNIX)
  set(CMAKE_CXX_FLAGS "-std=c++11 -pthread ${CMAKE_CXX_FLAGS}")
endif ()

# obsensor sdk
set(OrbbecSDK_ROOT_DIR ${CMAKE_SOURCE_DIR}/../SDK)
set(OrbbecSDK_LIBRARY_DIRS ${OrbbecSDK_ROOT_DIR}/lib)
set(OrbbecSDK_INCLUDE_DIR ${OrbbecSDK_ROOT_DIR}/include)
include_directories(${OrbbecSDK_INCLUDE_DIR})
link_directories(${OrbbecSDK_LIBRARY_DIRS})

# find opencv
# set(OpenCV_DIR "your/path/to/opencv")
find_package(OpenCV QUIET)
if (OpenCV_FOUND)
  include_directories(${OpenCV_INCLUDE_DIR})
  link_directories(${OpenCV_LIBRARY_DIRS})
endif ()

add_subdirectory(c)
add_subdirectory(cpp)
