# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_multi_streams)

add_executable(ob_multi_streams multi_streams.cpp)

set_property(TARGET ob_multi_streams PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_multi_streams ob::OrbbecSDK ob::examples::utils)
set_target_properties(ob_multi_streams PROPERTIES FOLDER "examples")

install(TARGETS ob_multi_streams RUNTIME DESTINATION bin)

