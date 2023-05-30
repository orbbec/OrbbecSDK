cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDK VERSION 1.5.8 LANGUAGES C CXX)

# Set the C++ standard to 14 and C standard to 11
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

# Compiler and Linker options
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3")
set(CMAKE_BUILD_TYPE "Release")

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# Options
option(BUILD_EXAMPLES "Build examples" ON)
option(INSTALL_LIBUDEV "Install libudev" OFF)

if(WIN32)
    set(ORBBEC_LIB_PREFIX "")
    set(ORBBEC_LIB_SUFFIX ".lib")
else()
    set(ORBBEC_LIB_PREFIX "lib")
    set(ORBBEC_LIB_SUFFIX ".so")
endif()
# Determine host platform
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/DetermineHostPlatform.cmake)

# Paths
set(ORBBEC_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/lib/${HOST_PLATFORM})
set(ORBBEC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(ORBBEC_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config/)
set(ORBBEC_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
set(ORBBEC_DOCS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/)
set(ORBBEC_UDEV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/udev)
# Subdirectories
if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif ()

# Installation of headers, configs, libs
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/InstallTargets.cmake)

# Package configuration
include(CMakePackageConfigHelpers)
configure_package_config_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/OrbbecSDKConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake"
  INSTALL_DESTINATION lib/cmake/OrbbecSDK
  PATH_VARS ORBBEC_INCLUDE_DIR ORBBEC_LIBS
)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY AnyNewerVersion
)
install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfigVersion.cmake"
  DESTINATION lib/cmake/OrbbecSDK
  )

# Uninstall target
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/UninstallTarget.cmake)
