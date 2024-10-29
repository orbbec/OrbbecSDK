// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"

#include <thread>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>

std::shared_ptr<ob::Device> selectDevice(std::shared_ptr<ob::DeviceList> deviceList);
std::vector<OBPropertyItem> getPropertyList(std::shared_ptr<ob::Device> device);
bool                        isPrimaryTypeProperty(OBPropertyItem propertyItem);
void                        printfPropertyList(std::shared_ptr<ob::Device> device, const std::vector<OBPropertyItem> &propertyList);
void                        setPropertyValue(std::shared_ptr<ob::Device> device, OBPropertyItem item, std::string strValue);
void                        getPropertyValue(std::shared_ptr<ob::Device> device, OBPropertyItem item);
std::string                 permissionTypeToString(OBPermissionType permission);

int main(void) try {
    // Create a Context.
    ob::Context context;

    // Query the list of connected devices
    auto deviceList = context.queryDeviceList();

    bool isSelectDevice = true;
    while(isSelectDevice) {
        // select a device to operate
        std::shared_ptr<ob::Device> device = nullptr;
        if(deviceList->getCount() > 0) {
            if(deviceList->getCount() <= 1) {
                // If a single device is plugged in, the first one is selected by default
                device = deviceList->getDevice(0);
            }
            else {
                device = selectDevice(deviceList);
            }
            auto deviceInfo = device->getDeviceInfo();
            std::cout << "\n------------------------------------------------------------------------\n";
            std::cout << "Current Device: "
                      << " name: " << deviceInfo->getName() << ", vid: 0x" << std::hex << deviceInfo->getVid() << ", pid: 0x" << std::setw(4)
                      << std::setfill('0') << deviceInfo->getPid() << ", uid: 0x" << deviceInfo->getUid() << std::dec << std::endl;
        }
        else {
            std::cout << "Device Not Found" << std::endl;
            isSelectDevice = false;
            break;
        }

        std::cout << "Input \"?\" to get all properties." << std::endl;

        std::vector<OBPropertyItem> propertyList = getPropertyList(device);
        std::sort(propertyList.begin(), propertyList.end(), [](const OBPropertyItem &a, const OBPropertyItem &b) { return a.id < b.id; });

        bool isSelectProperty = true;
        while(isSelectProperty) {
            std::string choice;
            std::getline(std::cin, choice);

            if(choice != "?") {
                std::istringstream       ss(choice);
                std::string              tmp;
                std::vector<std::string> controlVec;
                while(ss >> tmp) {
                    controlVec.push_back(tmp);
                }

                if(controlVec.size() <= 0)
                    continue;

                // exit the program
                if(controlVec.at(0) == "exit") {
                    isSelectProperty = false;
                    isSelectDevice   = false;
                    break;
                }

                // Check if it matches the input format
                if(controlVec.size() <= 1 || (controlVec.at(1) != "get" && controlVec.at(1) != "set") || controlVec.size() > 3
                   || (controlVec.at(1) == "set" && controlVec.size() < 3)) {
                    std::cout << "Property control usage: [property index] [set] [property value] or [property index] [get]" << std::endl;
                    continue;
                }
                size_t size     = propertyList.size();
                size_t selectId = std::atoi(controlVec.at(0).c_str());
                if(selectId >= size) {
                    std::cout << "Your selection is out of range, please reselect: " << std::endl;
                    continue;
                }

                bool isGetValue   = controlVec.at(1) == "get" ? true : false;
                auto propertyItem = propertyList.at(selectId);

                if(isGetValue) {
                    // get property value
                    getPropertyValue(device, propertyItem);
                }
                else {
                    // set property value
                    setPropertyValue(device, propertyItem, controlVec.at(2));
                }
            }
            else {
                printfPropertyList(device, propertyList);
                std::cout << "Please select property.(Property control usage: [property number] [set/get] [property value])" << std::endl;
            }
        }
    }

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

// Select a device, the name, pid, vid, uid of the device will be printed here, and the corresponding device object will be created after selection
std::shared_ptr<ob::Device> selectDevice(std::shared_ptr<ob::DeviceList> deviceList) {
    int devCount = deviceList->getCount();
    std::cout << "Device list: " << std::endl;
    for(int i = 0; i < devCount; i++) {
        std::cout << i << ". name: " << deviceList->getName(i) << ", vid: 0x" << std::hex << deviceList->getVid(i) << ", pid: 0x" << std::setw(4)
                  << std::setfill('0') << deviceList->getPid(i) << ", uid: 0x" << deviceList->getUid(i) << ", sn: " << deviceList->getSerialNumber(i)
                  << std::dec << std::endl;
    }
    std::cout << "Select a device: ";

    int devIndex;
    std::cin >> devIndex;
    while(devIndex < 0 || devIndex >= devCount || std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Your select is out of range, please reselect: " << std::endl;
        std::cin >> devIndex;
    }

    return deviceList->getDevice(devIndex);
}

// Print a list of supported properties
void printfPropertyList(std::shared_ptr<ob::Device> device, const std::vector<OBPropertyItem> &propertyList) {
    std::cout << "size: " << propertyList.size() << std::endl;
    if(propertyList.empty()) {
        std::cout << "No supported property!" << std::endl;
    }
    std::cout << "\n------------------------------------------------------------------------\n";
    for(size_t i = 0; i < propertyList.size(); i++) {
        auto        property_item = propertyList[i];
        std::string strRange      = "";

        OBIntPropertyRange   int_range;
        OBFloatPropertyRange float_range;
        switch(property_item.type) {
        case OB_BOOL_PROPERTY:
            strRange = "Bool value(min:0, max:1, step:1)";
            break;
        case OB_INT_PROPERTY: {
            try {
                int_range = device->getIntPropertyRange(property_item.id);
                strRange  = "Int value(min:" + std::to_string(int_range.min) + ", max:" + std::to_string(int_range.max)
                           + ", step:" + std::to_string(int_range.step) + ")";
            }
            catch(...) {
                std::cout << "get int property range failed." << std::endl;
            }
        } break;
        case OB_FLOAT_PROPERTY:
            try {
                float_range = device->getFloatPropertyRange(property_item.id);
                strRange    = "Float value(min:" + std::to_string(float_range.min) + ", max:" + std::to_string(float_range.max)
                           + ", step:" + std::to_string(float_range.step) + ")";
            }
            catch(...) {
                std::cout << "get float property range failed." << std::endl;
            }
            break;
        default:
            break;
        }

        std::cout.setf(std::ios::right);
        std::cout.fill('0');
        std::cout.width(2);
        std::cout << i << ". ";
        std::cout << property_item.name << "(" << (int)property_item.id << ")";
        std::cout << ", permission=" << permissionTypeToString(property_item.permission) << ", range=" << strRange << std::endl;
    }
    std::cout << "------------------------------------------------------------------------\n";
}

bool isPrimaryTypeProperty(OBPropertyItem propertyItem) {
    return propertyItem.type == OB_INT_PROPERTY || propertyItem.type == OB_FLOAT_PROPERTY || propertyItem.type == OB_BOOL_PROPERTY;
}

// Get property list
std::vector<OBPropertyItem> getPropertyList(std::shared_ptr<ob::Device> device) {
    std::vector<OBPropertyItem> propertyVec;
    propertyVec.clear();
    uint32_t size = device->getSupportedPropertyCount();
    for(uint32_t i = 0; i < size; i++) {
        OBPropertyItem property_item = device->getSupportedProperty(i);
        if(isPrimaryTypeProperty(property_item) && property_item.permission != OB_PERMISSION_DENY) {
            propertyVec.push_back(property_item);
        }
    }
    return propertyVec;
}

// set properties
void setPropertyValue(std::shared_ptr<ob::Device> device, OBPropertyItem propertyItem, std::string strValue) {
    try {
        int   int_value   = 0;
        float float_value = 0.0f;
        int   bool_value  = 0;
        switch(propertyItem.type) {
        case OB_BOOL_PROPERTY:
            bool_value = std::atoi(strValue.c_str());
            try {
                device->setBoolProperty(propertyItem.id, bool_value);
            }
            catch(ob::Error &e) {
                std::cout << "set bool property fail: " << e.what() << std::endl;
            }
            std::cout << "property name:" << propertyItem.name << ",set bool value:" << bool_value << std::endl;
            break;
        case OB_INT_PROPERTY:
            int_value = std::atoi(strValue.c_str());
            try {
                device->setIntProperty(propertyItem.id, int_value);
            }
            catch(ob::Error &e) {
                std::cout << "set int property fail: " << e.what() << std::endl;
            }
            std::cout << "property name:" << propertyItem.name << ",set int value:" << int_value << std::endl;
            break;
        case OB_FLOAT_PROPERTY:
            float_value = static_cast<float>(std::atof(strValue.c_str()));
            try {
                device->setFloatProperty(propertyItem.id, float_value);
            }
            catch(ob::Error &e) {
                std::cout << "set floar property fail: " << e.what() << std::endl;
            }
            std::cout << "property name:" << propertyItem.name << ",set float value:" << float_value << std::endl;
            break;
        default:
            break;
        }
    }
    catch(...) {
        std::cout << "set property failed: " << propertyItem.name << std::endl;
    }
}

// get property value
void getPropertyValue(std::shared_ptr<ob::Device> device, OBPropertyItem propertyItem) {
    try {
        bool  bool_ret  = false;
        int   int_ret   = 0;
        float float_ret = 0.0f;

        switch(propertyItem.type) {
        case OB_BOOL_PROPERTY:
            try {
                bool_ret = device->getBoolProperty(propertyItem.id);
            }
            catch(ob::Error &e) {
                std::cout << "get bool property failed: " << e.what() << std::endl;
            }
            std::cout << "property name:" << propertyItem.name << ",get bool value:" << bool_ret << std::endl;
            break;
        case OB_INT_PROPERTY:
            try {
                int_ret = device->getIntProperty(propertyItem.id);
            }
            catch(ob::Error &e) {
                std::cout << "get int property failed: " << e.what() << std::endl;
            }
            std::cout << "property name:" << propertyItem.name << ",get int value:" << int_ret << std::endl;
            break;
        case OB_FLOAT_PROPERTY:
            try {
                float_ret = device->getFloatProperty(propertyItem.id);
            }
            catch(ob::Error &e) {
                std::cout << "get float property failed: " << e.what() << std::endl;
            }
            std::cout << "property name:" << propertyItem.name << ",get float value:" << float_ret << std::endl;
            break;
        default:
            break;
        }
    }
    catch(...) {
        std::cout << "get property failed: " << propertyItem.name << std::endl;
    }
}

std::string permissionTypeToString(OBPermissionType permission) {
    switch(permission) {
    case OB_PERMISSION_READ:
        return "R/_";
    case OB_PERMISSION_WRITE:
        return "_/W";
    case OB_PERMISSION_READ_WRITE:
        return "R/W";

    default:
        break;
    }
    return "_/_";
}
