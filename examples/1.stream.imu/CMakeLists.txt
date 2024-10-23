# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_imu)

add_executable(ob_imu imu.cpp)

set_property(TARGET ob_imu PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_imu ob::OrbbecSDK ob::examples::utils)

set_target_properties(ob_imu PROPERTIES FOLDER "examples")

install(TARGETS ob_imu RUNTIME DESTINATION bin)

