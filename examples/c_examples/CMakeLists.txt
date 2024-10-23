# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)

file(GLOB subdirectories RELATIVE ${CMAKE_CURRENT_LIST_DIR} "*")

foreach(subdir ${subdirectories})
    if(IS_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/${subdir})
        add_subdirectory(${subdir})
    endif()
endforeach()

