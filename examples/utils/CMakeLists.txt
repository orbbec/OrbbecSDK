# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

cmake_minimum_required(VERSION 3.5)
project(ob_examples_utils)

add_library(ob_examples_utils STATIC
    ${CMAKE_CURRENT_LIST_DIR}/utils_c.c
    ${CMAKE_CURRENT_LIST_DIR}/utils_c.h
    ${CMAKE_CURRENT_LIST_DIR}/utils.cpp
    ${CMAKE_CURRENT_LIST_DIR}/utils.hpp
)

if(${OpenCV_FOUND})
    target_sources(ob_examples_utils PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/utils_opencv.cpp
        ${CMAKE_CURRENT_LIST_DIR}/utils_opencv.hpp
    )
    target_link_libraries(ob_examples_utils PUBLIC ${OpenCV_LIBS} ob::OrbbecSDK)
    target_include_directories(ob_examples_utils PUBLIC ${OpenCV_INCLUDE_DIRS})
endif()

find_package(Threads REQUIRED)
target_link_libraries(ob_examples_utils PUBLIC Threads::Threads)

target_include_directories(ob_examples_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(ob::examples::utils ALIAS ob_examples_utils)
set_target_properties(ob_examples_utils PROPERTIES FOLDER "examples")
