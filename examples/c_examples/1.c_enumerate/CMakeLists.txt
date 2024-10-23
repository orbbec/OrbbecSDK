# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)

project(ob_enumerate_c)
add_executable(ob_enumerate_c enumerate.c)

target_link_libraries(ob_enumerate_c PRIVATE ob::OrbbecSDK ob::examples::utils)

set_target_properties(ob_enumerate_c PROPERTIES FOLDER "examples_c")
set_target_properties(ob_enumerate_c PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

install(TARGETS ob_enumerate_c RUNTIME DESTINATION bin)

