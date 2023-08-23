# install
install(DIRECTORY ${OrbbecSDK_INCLUDE_DIRS}/ DESTINATION include)
install(DIRECTORY ${ORBBEC_CONFIG_DIR}/ DESTINATION lib)
install(DIRECTORY ${ORBBEC_CONFIG_DIR}/ DESTINATION bin)
install(DIRECTORY ${ORBBEC_EXAMPLES_DIR}/ DESTINATION examples)
install(DIRECTORY ${ORBBEC_DOCS_DIR}/ DESTINATION doc)

if (UNIX)
    install(FILES ${ORBBEC_SCRIPT_DIR}/99-obsensor-libusb.rules DESTINATION /etc/udev/rules.d/)
    install(FILES ${ORBBEC_SCRIPT_DIR}/99-obsensor-libusb.rules DESTINATION scripts/)
    install(FILES ${ORBBEC_SCRIPT_DIR}/install_udev_rules.sh DESTINATION scripts/)
    install(DIRECTORY
        ${OrbbecSDK_LIBS_DIR}/
        DESTINATION lib/
        FILES_MATCHING PATTERN "libOrbbecSDK.so"
        PATTERN "libOrbbecSDK.so.*"
    )

    if (INSTALL_LIBUDEV)
        install(DIRECTORY
            ${OrbbecSDK_LIBS_DIR}/
            DESTINATION lib/
            FILES_MATCHING PATTERN "libudev.so"
            PATTERN "libudev.so.*"
        )
    endif ()
elseif (WIN32)
    install(DIRECTORY
        ${ORBBEC_SCRIPT_DIR}/
        DESTINATION scripts/
        FILES_MATCHING PATTERN "obsensor_metadata_win10*"
    )
    install(DIRECTORY
        ${OrbbecSDK_LIBS_DIR}/
        DESTINATION bin/
        FILES_MATCHING PATTERN "OrbbecSDK.dll"
    )
    install(DIRECTORY
        ${OrbbecSDK_LIBS_DIR}/
        DESTINATION lib/
        FILES_MATCHING PATTERN "OrbbecSDK*"
    )
endif ()

# configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/OrbbecSDKConfig.cmake.in "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake" @ONLY)

# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/OrbbecSDKConfig.cmake" DESTINATION lib/)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Packing.cmake)