CMake配置说明
=============

Examples CMake工程结构
----------------------

::

   Examples
   │
   ├─c
   │  ├─CMakeLists.txt
   │  ├─Sample-ColorViewer
   │  │     └── CMakeLists.txt
   │  ├─Sample-DepthViewer
   │  │     └── CMakeLists.txt
   │  ├─Sample-HelloOrbbec
   │  │     └── CMakeLists.txt
   │  ├─Sample-HotPlugin
   │  │     └── CMakeLists.txt
   │  ├─Sample-InfraredViewer
   │  │     └── CMakeLists.txt
   │  ├─Sample-PointCloud
   │  │     └── CMakeLists.txt
   │  └─Sample-SensorControl
   │        └── CMakeLists.txt
   └─cpp
       ├─CMakeLists.txt
       ├─Sample-ColorViewer
       │     └── CMakeLists.txt
       ├─Sample-DepthViewer
       │     └── CMakeLists.txt
       ├─Sample-HelloOrbbec
       │     └── CMakeLists.txt
       ├─Sample-HotPlugin
       │     └── CMakeLists.txt
       ├─Sample-ImuReader
       │     └── CMakeLists.txt
       ├─Sample-InfraredViewer
       │     └── CMakeLists.txt
       ├─Sample-MultiDevice
       │     └── CMakeLists.txt
       ├─Sample-PointCloud
       │     └── CMakeLists.txt
       ├─Sample-SaveToDisk
       │     └── CMakeLists.txt
       ├─Sample-SensorControl
       │     └── CMakeLists.txt
       ├─Sample-SyncAlignViewer
       |     └── CMakeLists.txt 
       ├─Sample-Recorder
       |     └── CMakeLists.txt 
       └─Sample-Playback
             └── CMakeLists.txt 
             

如上树型结构，整个CMake工程结构可分为三层，与程序目录结构相同。每一层都会有一个CMakeLists.txt文件，第一层根目录中CMakeLists.txt是主CMakeLists.txt。第二层的CMakeLists.txt被放在两个不同的目录下，分别对应C版本和C++版本的Example。第三层的CMakeLists.txt是放在各自Example目录下。

.. _主cmakeliststxt说明:

主CMakeLists.txt说明
--------------------

下面从根目录的CMakeLists.txt开始介绍：

1. 设置需要使用CMake的最低版本为3.1.15

::

   cmake_minimum_required(VERSION 3.1.15)

2. 生成一个名为OrbbecSDK-Examples工程，使用C++和C语言

::

   project(OrbbecSDK-Examples LANGUAGES CXX C)

3. 使用标准的C++ 11进行编译

::

   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_STANDARD_REQUIRED True)

4. 设置SDK的头文件及库路径

::

   set(OBSensor_DIR ${CMAKE_SOURCE_DIR}/../SDK)
   set(OBSensor_LIBRARY_DIRS ${OBSensor_ROOT_DIR}/lib/${CMAKE_BUILD_TYPE})
   set(OBSensor_INCLUDE_DIR ${OBSensor_ROOT_DIR}/include)
   include_directories(${OBSensor_INCLUDE_DIR})
   link_directories(${OBSensor_LIBRARY_DIRS})

5. 查找opencv库

::

   # set(OpenCV_DIR "your/path/to/opencv")  # 可通过该命令指定OpenCV_DIR模块查找路径
   find_package(OpenCV QUIET)
   if(${OpenCV_FOUND})
       include_directories(${OpenCV_INCLUDE_DIRS})
           link_directories(${OpenCV_LIBRARY_DIRS})
   ENDIF(${OpenCV_FOUND})

6. 设置ARCHIVE、LIBRARY、RUNTIME的输出目录，

::

   set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
   set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
   set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

关于这三个变量的说明参考：

======= ========== ==========
\       Windows    Unix
======= ========== ==========
RUNTIME .exe、.dll 可执行程序
LIBRARY            .so
ARCHIVE .a、.lib   .a
======= ========== ==========

7. 添加子工程目录

::

   add_subdirectory(c)
   add_subdirectory(cpp)

C语言版本Examples 主CMakeLists说明
----------------------------------

::

   cmake_minimum_required(VERSION 3.1.15)

   add_subdirectory(Sample-HelloOrbbec)
   add_subdirectory(Sample-SensorControl)
   add_subdirectory(Sample-HotPlugin)

   # opencv required
   IF(${OpenCV_FOUND})
       add_subdirectory(Sample-ColorViewer)
       add_subdirectory(Sample-DepthViewer)
       add_subdirectory(Sample-InfraredViewer)
       add_subdirectory(Sample-PointCloud)
   ENDIF(${OpenCV_FOUND})

这个文件比较简单，这里做一个简要说明。主要是根据是否有查找到opencv库及各example属性是否依赖opencv库决定是否将该example添加到工程。

C语言版本Examples 子CMakeLists说明
----------------------------------

在各Example子目录下包含各自的CMakeLists.txt, 以Sample-HelloOrbbec为例:

::

   cmake_minimum_required(VERSION 3.1.15)
   set(CMAKE_CXX_STANDARD 11)

   project(Sample-HelloOrbbec)

   add_executable(hello_orbbec HelloOrbbec.c)
   target_link_libraries(hello_orbbec obsensor)

内容比较简单，主要是指定将源文件HelloOrbbec.c生产可执行文件hello_orbbec（windows平台生成文件待.exe后缀），并指定连接obsensor库。

C++版本Examples 主CMakeLists说明
--------------------------------

::

   cmake_minimum_required(VERSION 3.1.15)

   add_subdirectory(Sample-HelloOrbbec)
   add_subdirectory(Sample-SensorControl)
   add_subdirectory(Sample-ImuReader)

   # opencv required
   IF(${OpenCV_FOUND})
       add_subdirectory(Sample-ColorViewer)
       add_subdirectory(Sample-DepthViewer)
       add_subdirectory(Sample-InfraredViewer)
       add_subdirectory(Sample-SyncAlignViewer)
       add_subdirectory(Sample-HotPlugin)
       add_subdirectory(Sample-PointCloud)
       add_subdirectory(Sample-MultiDevice)
       add_subdirectory(Sample-SaveToDisk)
       add_subdirectory(Sample-Recorder)
       add_subdirectory(Sample-Playback)
   ENDIF(${OpenCV_FOUND})

C++版本Examples
主CMakeLists.txt目标和内容和C语言版本基本一致，这里不再赘述。不同的是C++版本提供了更为丰富的例程。

C++版本Examples 子CMakeLists说明
--------------------------------

与C语言版本基本一致，这里不再复述。
