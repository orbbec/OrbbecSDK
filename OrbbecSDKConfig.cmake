# Determine host platform
include(${CMAKE_CURRENT_LIST_DIR}/cmake/DetermineHostPlatform.cmake)

if(WIN32)
    set(ORBBEC_LIB_PREFIX "")
    set(ORBBEC_LIB_SUFFIX ".lib")
    set(ORBBEC_DLL_SUFFIX ".dll")
elseif(APPLE)
    set(ORBBEC_LIB_PREFIX "lib")
    set(ORBBEC_LIB_SUFFIX ".dylib")
    set(ORBBEC_DLL_SUFFIX ".dylib")
elseif(UNIX)
    set(ORBBEC_LIB_PREFIX "lib")
    set(ORBBEC_LIB_SUFFIX ".so")
    set(ORBBEC_DLL_SUFFIX ".so")
endif()

# OrbbecSDK library
set(OrbbecSDK_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include/)
set(OrbbecSDK_LIBS_DIR ${CMAKE_CURRENT_LIST_DIR}/lib/${HOST_PLATFORM})
set(OrbbecSDK_LIBRARY  ${OrbbecSDK_LIBS_DIR}/${ORBBEC_LIB_PREFIX}OrbbecSDK${ORBBEC_LIB_SUFFIX})
set(OrbbecSDK_DLL ${OrbbecSDK_LIBS_DIR}/${ORBBEC_LIB_PREFIX}OrbbecSDK${ORBBEC_DLL_SUFFIX})

add_library(OrbbecSDK::OrbbecSDK SHARED IMPORTED GLOBAL)
set_target_properties(OrbbecSDK::OrbbecSDK PROPERTIES
    IMPORTED_LOCATION ${OrbbecSDK_DLL}
    IMPORTED_IMPLIB ${OrbbecSDK_LIBRARY}
    INTERFACE_INCLUDE_DIRECTORIES ${OrbbecSDK_INCLUDE_DIRS}
)

set(OrbbecSDK_LIBS ${OrbbecSDK_LIBRARY})
file(GLOB OrbbecSDK_DLL_FILES ${OrbbecSDK_LIBS_DIR}/*${ORBBEC_DLL_SUFFIX}*)

# runtime resource file should copy to cwd.
file(GLOB OrbbecSDK_RUNTIME_RESOURCE_FILES  ${OrbbecSDK_LIBS_DIR}/*depthengine*)

# config file
file(GLOB OrbbecSDK_CONFIG_FILES  ${CMAKE_CURRENT_LIST_DIR}/misc/config/*)

# script
if(WIN32)
    file(GLOB OrbbecSDK_SCRIPT_FILES  ${CMAKE_CURRENT_LIST_DIR}/misc/scripts/obsensor_metadata_win10*)
else()
    file(GLOB OrbbecSDK_SCRIPT_FILES  ${CMAKE_CURRENT_LIST_DIR}/misc/scripts/*rules*)
endif()

set(OrbbecSDK_FOUND TRUE)