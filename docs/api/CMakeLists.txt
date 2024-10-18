# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

#[[ minimum required cmake version: 3.1.15 support vs2019
    This file is for building html.
    Requirements for win10:
        Python3 Doxygen graphviz Sphinx breathe
        download Doxygen at https://sourceforge.net/projects/doxygen/files/latest/download
        download graphviz at http://www.graphviz.org/download/
        pip3 install Sphinx breathe
]]

cmake_minimum_required(VERSION 3.1.15)

project(ObsensorApi)

set(OB_VERSION_STRING ${CMAKE_PROJECT_VERSION})
option(BUILD_SPHINX_DOC "build Sphinx documentation" OFF)
find_package(Doxygen)
message("Doxygen_FOUND: ${Doxygen_FOUND}")

if(Doxygen_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    #configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_CN.in ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile_CN)
	file(MAKE_DIRECTORY  ${CMAKE_BINARY_DIR}/doc/)
	set(OB_SDK_VERSION ${OB_VERSION_STRING})
	add_custom_target(api_reference_en ALL
	COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Generating English version API Reference documentation with Doxygen"
	VERBATIM)

	# add_custom_target(api_reference_cn ALL
	# COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile_CN
	# WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	# COMMENT "Generating Chinese version API Reference documentation with Doxygen"
	# VERBATIM)

	IF(USE_PROJECT_FOLDERS)
		set_target_properties (api_reference_en PROPERTIES FOLDER "${CMAKE_BINARY_DIR}/Doc")
		#set_target_properties (api_reference_cn PROPERTIES FOLDER "Doc")
	ENDIF()

	#INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/api DESTINATION "${CMAKE_BINARY_DIR}/Doc")
	INSTALL(DIRECTORY ${CMAKE_BINARY_DIR}/doc DESTINATION shared)
endif()

