# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)

project(ob_depth_c)
add_executable(ob_depth_c depth.c)

target_link_libraries(ob_depth_c PRIVATE ob::OrbbecSDK ob::examples::utils)

set_target_properties(ob_depth_c PROPERTIES FOLDER "examples_c")
set_target_properties(ob_depth_c PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

install(TARGETS ob_depth_c RUNTIME DESTINATION bin)

