# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_point_cloud)

add_executable(ob_point_cloud point_cloud.cpp)

set_property(TARGET ob_point_cloud PROPERTY CXX_STANDARD 11)
target_link_libraries(ob_point_cloud ob::OrbbecSDK ob::examples::utils)

set_target_properties(ob_point_cloud PROPERTIES FOLDER "examples")

install(TARGETS ob_point_cloud RUNTIME DESTINATION bin)
