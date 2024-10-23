# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_post_processing)

add_executable(ob_post_processing post_processing.cpp)

set_property(TARGET ob_post_processing PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_post_processing ob::OrbbecSDK ob::examples::utils)
set_target_properties(ob_post_processing PROPERTIES FOLDER "examples")

install(TARGETS ob_post_processing RUNTIME DESTINATION bin)

