cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDK VERSION 1.6.3 LANGUAGES C CXX)

# Set the C++ standard to C++ 11 and C standard to C11
set(CMAKE_CXX_STANDARD 11)
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

# OrbbecSDK library
set(OrbbecSDK_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/${HOST_PLATFORM})
set(OrbbecSDK_LIBRARY  ${OrbbecSDK_LIBS_DIR}/${ORBBEC_LIB_PREFIX}OrbbecSDK${ORBBEC_LIB_SUFFIX})
set(OrbbecSDK_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(OrbbecSDK_LIBS ${OrbbecSDK_LIBRARY})
set(OrbbecSDK_FOUND TRUE)

# paths
set(ORBBEC_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples/)
set(ORBBEC_DOCS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/doc/)
set(ORBBEC_MISC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/misc/)
set(ORBBEC_CONFIG_DIR ${ORBBEC_MISC_DIR}/config/)
set(ORBBEC_SCRIPT_DIR ${ORBBEC_MISC_DIR}/scripts/)

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
    INSTALL_DESTINATION lib/
    PATH_VARS OrbbecSDK_INCLUDE_DIRS OrbbecSDK_LIBS_DIR
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfigVersion.cmake"
    DESTINATION lib/
)

# Uninstall target
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/UninstallTarget.cmake)
