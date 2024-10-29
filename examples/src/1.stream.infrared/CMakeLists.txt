# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_infrared)

add_executable(ob_infrared infrared.cpp)

set_property(TARGET ob_infrared PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_infrared ob::OrbbecSDK ob::examples::utils)

set_target_properties (ob_infrared PROPERTIES FOLDER "examples")

install(TARGETS ob_infrared RUNTIME DESTINATION bin)

