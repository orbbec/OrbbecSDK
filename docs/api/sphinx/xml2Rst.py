#!/usr/bin/python
# -*- coding: UTF-8 -*-

# This script is used to automatically generate the C API XML file as ./reference/c_ref.rst when the C API header files are updated, and update the version number in ./index.rst.
# The C++ RST file is manually written. If the C++ header files are updated, please modify ./reference/cxx_ref.rst.
# First, build the project with Doxygen, which will generate XML files in the build path.
# Execute python.exe xml2Rst.py in the current directory with the XML absolute path to generate index.rst in the current directory and c_ref.rst in the reference directory.
# Example: python.exe xml2Rst.py D:\obcode\SensorSDK_202111ref\build\Win_X64\release\xmlCN\
# If there are HTML files in the build directory, delete them first and then execute the build Doc command.

import os
import sys
from fileinput import filename
from xml.dom.minidom import parse
import xml.dom.minidom

keyWordsList = []
functionList = []
typedefList = []
enumList = []
defineList = []
structList = []

# C header files
fileNameList = ["Context.h","Device.h","Error.h","Filter.h","Frame.h","ObTypes.h","Pipeline.h","Property.h",
    "RecordPlayback.h","Sensor.h","StreamProfile.h","Version.h"]

# Interface deprecated
excludeFunctions =["ob_frame_width","ob_frame_height","ob_delete_frame_set",
    "ob_stream_profile_fps","ob_stream_profile_width","ob_stream_profile_height"]

# Get information about the index.xml node
def getInfoFromXml(dirName):
    fileName= dirName + "\index.xml"
    domTree = xml.dom.minidom.parse(fileName)
    collection = domTree.documentElement
    keyWords = collection.getElementsByTagName("compound")     
    for keyWord in keyWords:
        if keyWord.getAttribute('kind')=="file":
            fileName = keyWord.getElementsByTagName('name')[0].childNodes[0].data
            if fileName in fileNameList:
                members = keyWord.getElementsByTagName('member')
                for member in members:
                    kind = member.getAttribute('kind')
                    if kind == "function":
                        name = member.getElementsByTagName('name')[0].childNodes[0].data
                        functionList.append(name)
                    elif kind == "typedef":
                        name = member.getElementsByTagName('name')[0].childNodes[0].data
                        typedefList.append(name)
                    elif kind == "enum":
                        name = member.getElementsByTagName('name')[0].childNodes[0].data
                        enumList.append(name)
                    elif kind == "define":
                        name = member.getElementsByTagName('name')[0].childNodes[0].data
                        defineList.append(name)
        elif keyWord.getAttribute('kind')=="struct":
            name = keyWord.getElementsByTagName('name')[0].childNodes[0].data
            structList.append(name)

# Doxyfile.xml node information
def getVersionFromXml(dirName):
    fileName= dirName + "\Doxyfile.xml"
    domTree = xml.dom.minidom.parse(fileName)
    collection = domTree.documentElement
    keyWords = collection.getElementsByTagName("option")    
    for keyWord in keyWords:
        if keyWord.getAttribute('id')=="PROJECT_NUMBER":
            global version
            version = keyWord.getElementsByTagName('value')[0].childNodes[0].data
            
            
# Generate rst file from xml information
def writeToRst():
    indexRst = open("./index.rst", "w")
    indexRst.write("OrbbecSdk v%s\n================\n\n" %version)
    indexRst.write(".. toctree::\n")
    indexRst.write("   :maxdepth: 3\n\n")
    indexRst.write("   summary/index\n")
    indexRst.write("   env/index\n")
    indexRst.write("   sample/index\n")    
    indexRst.write("   knowledge/index\n")    
    indexRst.write("   support/index\n")    
    indexRst.write("   reference/index\n")    
    indexRst.write("   orbbecviewer/index\n")    
    indexRst.close()

    crefRst = open("./reference/c_ref.rst", "w")
    crefRst.write("C Reference                                \n============================================\n")
    crefRst.write("Macros                                     \n--------------------------------------------\n")
    for item in defineList:
        crefRst.write(".. doxygendefine:: %s\n   :project: OrbbecSdk\n\n" %(item))
    crefRst.write("Structures                                 \n--------------------------------------------\n")
    for item in structList:
        crefRst.write(".. doxygenstruct:: %s\n   :project: OrbbecSdk\n\n" %(item))
    crefRst.write("Enumerations                               \n--------------------------------------------\n")
    for item in enumList:
        crefRst.write(".. doxygenenum:: %s\n   :project: OrbbecSdk\n\n" %(item))
    crefRst.write("Typedefs                                   \n--------------------------------------------\n")
    for item in typedefList:
        crefRst.write(".. doxygentypedef:: %s\n   :project: OrbbecSdk\n\n" %(item))
    crefRst.write("Functions                                  \n--------------------------------------------\n")
    for item in functionList:
        if item not in excludeFunctions:
            crefRst.write(".. doxygenfunction:: %s\n   :project: OrbbecSdk\n\n" %(item))
    crefRst.close()

try:
    xmlDir = sys.argv[1]
except:
    print("Please pass the path to the xml, e.g. python.exe xml2Rst.py path")
else:
    getInfoFromXml(xmlDir)
    getVersionFromXml(xmlDir)
    writeToRst()
    print("Generate . /index.rst and . /reference/c_ref.rst files")



