# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_device_control)

add_executable(${PROJECT_NAME} device_control.cpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} ob::OrbbecSDK ob::examples::utils )

set_target_properties (${PROJECT_NAME} PROPERTIES FOLDER "examples")

install(TARGETS ob_device_control RUNTIME DESTINATION bin)

