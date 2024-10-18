# Installation and Development Guide

This is a comprehensive guide on installing the SDK, setting up the development environment, running the sample programs and developing the application.

## 1. Step-by-Step Guide

### 1.1 Download the precompiled SDK package

- Download the zip package "OrbbecSDK_v2.x.x_xxx.zip" from [https://github.com/orbbec/OrbbecSDK-dev/releases](https://github.com/orbbec/OrbbecSDK-dev/releases).
- Unzip the OrbbecSDK to the directory you want

### 1.2 Environment Setup

#### 1.2.1 windows

For windows, you need to register the metadata associated with frames (this includes things like timestamps and other information about the video frame).

- Metadata registration follow this:[/scripts/env_setup/obsensor_metadata_win10.md](../../scripts/env_setup/obsensor_metadata_win10.md)

#### 1.2.2 Linux

For Linux, we have provided a script to help you set up the environment. You can run the script as follows:

```bash
cd scripts/env_setup
./setup.sh
```

**Purpose**:

1. Install compilation tools and dependencies (e.g., OpenCV).
2. Compile the sample program.
3. Install libusb rules file.

> It's not necessary. If you don't, there will be no sample executable, and you'll need to run install libusb rules file manually

**`setup.sh` Script Requirements**:

1. **Internet Connection**: The script may need to download compilation tools and dependencies from the internet.
2. **sudo Permissions**: The installation process may require system-level permissions to install software packages or modify system files.

**Expected Outcome**:

When the script executes successfully, it will generate the sample executable in the project's bin directory. This file is the result of compiling the sample program and can be used to demonstrate the functionality.

### 1.3 Run example

Connect the device to the computer and run the sample program at your unzip directory.

```shell
cd bin
./ob_color  #  ./ob_color for linux
```

> More information about the examples of Orbbec SDK, please refer to [examples](../../examples/README.md).

## 2. Develop a New Application with the SDK

### 2.1 Create a new CMake Project

First, create a new folder for your project root directory and set up the basic CMake files and source code files within it.

Example Project Folder Structure:

```plaintext
MyOrbbecApp/
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── include/
    └── (optional custom header files)
```

### 2.2 Add the OrbbecSDK to the Project

Create a lib directory in the project and unpack the Orbbec SDK library files into this lib directory.

Example Project Folder Structure:

```plaintext
MyOrbbecApp/
├── CMakeLists.txt
├── src/
│   └── main.cpp
├── include/
│   └── (optional custom header files)
└── lib/
    └── orbbecsdk
        └── (Orbbec SDK library files)
```

### 2.3 Config the CMakeLists.txt to Link the OrbbecSDK

Add the following lines to the CMakeLists.txt file:

```cmake
add_executable(MyOrbbecApp src/main.cpp)

set(OrbbecSDK_DIR lib/orbbecsdk/lib)
find_package(OrbbecSDK REQUIRED)

target_link_libraries(MyOrbbecApp ob::OrbbecSDK)
```

### 2.4 Develop the Application

Now you can develop your application using the Orbbec SDK. You can refer to the sample programs in the Orbbec SDK to get started.

### 2.5 Compile and place the library files

As a cmake project, you can compile the project as the cmake way, depends on your build system and habit. Here is an example of using the command line to compile the project:

```shell
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd ..
```

After compiling, **it is important to copy the Orbbec SDK library and its extentions files to the target directory**.

```shell
cp -r lib/orbbecsdk/lib/extensions build/bin/
cp -r lib/orbbecsdk/lib/libOrbbecSDK.so build/bin/
```

After all these steps, you can run the application by executing the binary file in the build/bin directory.
