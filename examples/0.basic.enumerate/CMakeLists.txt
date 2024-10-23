# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_enumerate)

add_executable(ob_enumerate enumerate.cpp)

set_property(TARGET ob_enumerate PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_enumerate ob::OrbbecSDK ob::examples::utils)
set_target_properties(ob_enumerate PROPERTIES FOLDER "examples")

install(TARGETS ob_enumerate RUNTIME DESTINATION bin)

