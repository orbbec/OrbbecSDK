#  minimum required cmake version: 3.1.15 support vs2019
cmake_minimum_required(VERSION 3.1.15)
project(Playback)

add_executable(${PROJECT_NAME}
  Playback.cpp
  )
target_link_libraries(${PROJECT_NAME}
  ${OpenCV_LIBS}
  ${ORBBEC_LIBS}/${ORBBEC_LIB_PREFIX}OrbbecSDK${ORBBEC_LIB_SUFFIX}
  )

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${OpenCV_INCLUDED_DIRS}
  ${ORBBEC_INCLUDE_DIR}
  )

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )
