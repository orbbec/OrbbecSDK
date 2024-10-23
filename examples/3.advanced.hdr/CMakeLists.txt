# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_hdr)

add_executable(ob_hdr hdr.cpp)

set_property(TARGET ob_hdr PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_hdr ob::OrbbecSDK ob::examples::utils)
set_target_properties(ob_hdr PROPERTIES FOLDER "examples")

install(TARGETS ob_hdr RUNTIME DESTINATION bin)

