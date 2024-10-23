# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_preset)

add_executable(ob_preset preset.cpp)

set_property(TARGET ob_preset PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_preset ob::OrbbecSDK ob::examples::utils)
set_target_properties(ob_preset PROPERTIES FOLDER "examples")

install(TARGETS ob_preset RUNTIME DESTINATION bin)

