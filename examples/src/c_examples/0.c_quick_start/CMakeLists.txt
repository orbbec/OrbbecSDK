# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
project(ob_quick_start_c)
add_executable(ob_quick_start_c quick_start.c)

target_link_libraries(ob_quick_start_c PRIVATE ob::OrbbecSDK ob::examples::utils)

set_target_properties(ob_quick_start_c PROPERTIES FOLDER "examples_c")
set_target_properties(ob_quick_start_c PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

install(TARGETS ob_quick_start_c RUNTIME DESTINATION bin)

