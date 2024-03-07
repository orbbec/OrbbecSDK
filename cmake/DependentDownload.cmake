

#set branch
set(REPO_BRANCH main)

#set download zip URL
set(ZIP_URL https://github.com/orbbec/OrbbecSDK-Lib/archive/${REPO_BRANCH}.zip)

#set dependent lib & driver directory
set(DependentLib ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(DependentDriver ${CMAKE_CURRENT_SOURCE_DIR}/misc/driver)

#check and download dependent lib & driver
if(NOT EXISTS ${DependentLib} OR NOT EXISTS ${DependentDriver} )
    message(STATUS "dependent lib & driver directory does not exist.")
    message("---config download orbbecsdk dependent lib & driver start...")
    set(ProjectLibName OrbbecSDK-Lib)
    include(ExternalProject)
    ExternalProject_Add(
        ${ProjectLibName}
        URL ${ZIP_URL}
        DOWNLOAD_NO_PROGRESS 1
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads  # download directory
        #CONFIGURE_COMMAND ${CMAKE_COMMAND} -E tar xzf <DOWNLOAD_DIR>/main.zip
        CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/${ProjectLibName}-prefix/src/${ProjectLibName}/lib ${CMAKE_BINARY_DIR}/../lib
        BUILD_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/${ProjectLibName}-prefix/src/${ProjectLibName}/driver ${CMAKE_BINARY_DIR}/../misc/driver
        INSTALL_COMMAND ""
    )
    message("---config download orbbecsdk dependent lib & driver finish...")
else()
    message(STATUS "Directory exists.")
endif()
