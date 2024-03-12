

#set tag
set(REPO_TAG "v1.9.5")
#set HASH_VALUE
set(HASH_VALUE 263ff1f79c2d690448d63bd381f1155a971acdf837050a1164fa4503c0cb5da9)

#set download zip URL
set(ZIP_URL "https://github.com/orbbec/OrbbecSDK-Lib/archive/refs/tags/${REPO_TAG}.tar.gz")


message("-config download orbbecsdk dependent lib & driver start...")
set(ProjectLibName OrbbecSDK-Lib)
include(ExternalProject)
ExternalProject_Add(
    ${ProjectLibName}
    URL "${ZIP_URL}"
    URL_HASH SHA256=${HASH_VALUE}
    DOWNLOAD_NO_PROGRESS 1
    DOWNLOAD_EXTRACT_TIMESTAMP 1
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads  # download directory
    #CONFIGURE_COMMAND ${CMAKE_COMMAND} -E tar xzf <DOWNLOAD_DIR>/main.zip
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/${ProjectLibName}-prefix/src/${ProjectLibName}/lib ${CMAKE_BINARY_DIR}/../lib
    BUILD_COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_BINARY_DIR}/${ProjectLibName}-prefix/src/${ProjectLibName}/driver ${CMAKE_BINARY_DIR}/../misc/driver
    INSTALL_COMMAND ""
)
message("-config download orbbecsdk dependent lib & driver finish...")
