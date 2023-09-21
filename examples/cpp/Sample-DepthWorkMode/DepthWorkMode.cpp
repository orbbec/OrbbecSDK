#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <libobsensor/ObSensor.hpp>
#include "utils.hpp"

#define ESC 27

static void pressKeyExit(std::string msg) {
    if(msg.size() > 0) {
        std::cout << msg << std::endl;
    }

    while(true) {
        // Get the value of the pressed key, if it is the esc key, exit the program
        int key = getch();
        if(key == ESC)
            break;
    }
}

static std::string   toHexText(const uint8_t *data, const uint32_t dataLen);
static std::ostream &operator<<(std::ostream &os, const OBDepthWorkMode &dataType);

int main(int argc, char *argv[]) try {
    // Print the sdk version number, the sdk version number is divided into major version number, minor version number and revision number
    std::cout << "SDK version: " << ob::Version::getMajor() << "." << ob::Version::getMinor() << "." << ob::Version::getPatch() << std::endl;

    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Get the device inside the pipeline
    auto device = pipe.getDevice();
    // Check whether the camera depth working mode is supported, currently (December 5, 2022) only the Gemini2 binocular camera supports the depth working mode
    if(!device->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE, OB_PERMISSION_READ_WRITE)) {
        pressKeyExit("Current device not support depth work mode!");
        return -1;
    }

    // Query the current camera depth mode
    auto curDepthMode = device->getCurrentDepthWorkMode();
    // Get the list of camera depth modes
    auto depthModeList = device->getDepthWorkModeList();
    std::cout << "depthModeList size: " << depthModeList->count() << std::endl;
    for(uint32_t i = 0; i < depthModeList->count(); i++) {
        std::cout << "depthModeList[" << i << "]: " << (*depthModeList)[i];
        if(strcmp(curDepthMode.name, (*depthModeList)[i].name) == 0) {
            std::cout << "  (Current WorkMode)";
        }

        std::cout << std::endl;
    }

    // Let the user choose a mode, then switch
    if(depthModeList->count() > 0) {
        uint32_t index = 0;
        std::cout << "Please input the index from above depthModeList, newIndex = ";
        std::cin >> index;
        if(index >= 0 && index < depthModeList->count()) {  // legitimacy check
            device->switchDepthWorkMode((*depthModeList)[index].name);

            // Check whether the mode changes after the display is switched
            curDepthMode = device->getCurrentDepthWorkMode();
            if(strcmp(curDepthMode.name, (*depthModeList)[index].name) == 0) {
                std::cout << "Switch depth work mode success! currentDepthMode: " << curDepthMode << std::endl;
            }
            else {
                std::cout << "Switch depth work mode failed!" << std::endl;
            }
        }
        else {
            std::cout << "switchDepthMode faild. invalid index: " << index << std::endl;
        }
    }

    // This is the end of switching the camera depth mode, you can use the pipeline to open the camera to get the stream
    // Notice:
    // 1. If you need to switch the camera depth mode, you must do it berfore start camera stream; the effective resolution supported
    // by each camera depth mode is different
    // 2. If the data stream has been opened using the pipeline, the original pipeline must be released before switching camera depth mode.  otherwise, wild
    // pointer or memory leak will be caused.
    pressKeyExit("Press ESC to exit!");
    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

static std::string toHexText(const uint8_t *data, const uint32_t dataLen) {
    static char alpha[] = "0123456789abcdef";
    char       *hexStr  = new char[dataLen * 2 + 1]{ 0 };
    for(uint32_t i = 0; i < dataLen; i++) {
        uint8_t val       = *(data + i);
        *(hexStr + i)     = alpha[(val >> 4) & 0xf];
        *(hexStr + i + 1) = alpha[val & 0xf];
    }

    std::string ret(hexStr);
    delete[] hexStr;
    return ret;
}

static std::ostream &operator<<(std::ostream &os, const OBDepthWorkMode &workMode) {
    os << "{name: " << workMode.name << ", checksum: " << toHexText(workMode.checksum, sizeof(workMode.checksum)) << "}";
    return os;
}