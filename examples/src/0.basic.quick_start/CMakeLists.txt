# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_quick_start)

add_executable(ob_quick_start quick_start.cpp)

set_property(TARGET ob_quick_start PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_quick_start ob::OrbbecSDK ob::examples::utils)
set_target_properties (ob_quick_start PROPERTIES FOLDER "examples")

install(TARGETS ob_quick_start RUNTIME DESTINATION bin)

