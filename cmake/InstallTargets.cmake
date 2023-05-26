# install
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
install(DIRECTORY ${ORBBEC_INCLUDE_DIR}/ DESTINATION include)
install(DIRECTORY ${ORBBEC_CONFIG_DIR}/ DESTINATION config)
install(DIRECTORY ${ORBBEC_EXAMPLES_DIR}/ DESTINATION examples)
install(DIRECTORY ${ORBBEC_DOCS_DIR}/ DESTINATION docs)
install(DIRECTORY ${ORBBEC_UDEV_DIR}/ DESTINATION udev)
if (UNIX)
  install(DIRECTORY
    ${ORBBEC_LIBS}/
    DESTINATION lib/
    FILES_MATCHING PATTERN "libOrbbecSDK.so"
    PATTERN "libOrbbecSDK.so.*"
    )
  if (INSTALL_UDEV)
    install(DIRECTORY
      ${ORBBEC_LIBS}/
      DESTINATION lib/
      FILES_MATCHING PATTERN "libudev.so"
      PATTERN "libudev.so.*"
      )
  endif ()
elseif (WIN32)
  install(DIRECTORY
    ${ORBBEC_LIBS}/
    DESTINATION lib/
    FILES_MATCHING PATTERN "OrbbecSDK.dll"
    PATTERN "OrbbecSDK.dll.*"
    )
endif ()

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/OrbbecSDKConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake" @ONLY)

install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake"
  DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}
)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Packing.cmake)
