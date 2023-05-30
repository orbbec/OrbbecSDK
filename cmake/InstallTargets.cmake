# install
install(DIRECTORY ${ORBBEC_INCLUDE_DIR}/ DESTINATION include)
include(GNUInstallDirs)
install(DIRECTORY ${ORBBEC_CONFIG_DIR}/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/orbbec/config)
install(DIRECTORY ${ORBBEC_EXAMPLES_DIR}/ DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/orbbec/examples)
install(DIRECTORY ${ORBBEC_DOCS_DIR}/ DESTINATION ${CMAKE_INSTALL_DOCDIR})

if (UNIX)
  install(FILES ${ORBBEC_UDEV_DIR}/99-obsensor-libusb.rules DESTINATION /etc/udev/rules.d/)
  install(DIRECTORY
    ${ORBBEC_LIBS}/
    DESTINATION lib/
    FILES_MATCHING PATTERN "libOrbbecSDK.so"
    PATTERN "libOrbbecSDK.so.*"
    )
  if (INSTALL_LIBUDEV)
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
