# Orbbec SDK v2 for Femto Mega development Mode

This branch is contains the Orbbec SDK v2 pre-compiled library and examples code for Femto Mega development Mode.

The pre-compiled library is located in the `./lib` directory and the examples code is located in the `./examples` directory.

The pre-compiled library can only be used on Femto Mega development Mode, aarch64 architecture, and communicates with the camera server via Inter-Processor Communication (IPC) interface.


## 1. Building the examples

Execute the setup script to build the examples:

``` bash
./setup.sh
```

After the setup script is executed, the examples can be built and outputted to the `./bin` directory.

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
