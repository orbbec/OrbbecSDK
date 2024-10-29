# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_color)

add_executable(ob_color color.cpp)

set_property(TARGET ob_color PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_color ob::OrbbecSDK ob::examples::utils)
set_target_properties (ob_color PROPERTIES FOLDER "examples")

install(TARGETS ob_color RUNTIME DESTINATION bin)

