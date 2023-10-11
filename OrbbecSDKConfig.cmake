# Determine host platform
include(${CMAKE_CURRENT_LIST_DIR}/cmake/DetermineHostPlatform.cmake)

if(WIN32)
    set(ORBBEC_LIB_PREFIX "")
    set(ORBBEC_LIB_SUFFIX ".lib")
    set(ORBBEC_DLL_SUFFIX ".dll")
else()
    set(ORBBEC_LIB_PREFIX "lib")
    set(ORBBEC_LIB_SUFFIX ".so")
    set(ORBBEC_DLL_SUFFIX ".so")
endif()

# OrbbecSDK library
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
set(OrbbecSDK_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include/)
file(GLOB OrbbecSDK_DLL_FILES ${OrbbecSDK_LIBS_DIR}/*${ORBBEC_DLL_SUFFIX})
set(OrbbecSDK_FOUND TRUE)