# C++ Sample：2.device.control

## Overview

The SDK can be used to modify camera-related parameters, including laser switch, laser level intensity, white balance switch, etc.

### Knowledge

Pipeline is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

Device is a class that can be used to get device information, parameters, and a list of contained sensors.

Sensor can be used to obtain different components of the camera and the stream of the component, for example, RGB, IR, Depth stream can be obtained through the RGB, IR, Depth sensor.

## code overview

1. Get camera related information and output

    ```cpp
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
    ```

2. Get the relevant parameters stored in the container and reorder them by id

    ```cpp
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
    ```

    ```cpp
        std::vector<OBPropertyItem> propertyList = getPropertyList(device);
        std::sort(propertyList.begin(), propertyList.end(), [](const OBPropertyItem &a, const OBPropertyItem &b) { return a.id < b.id; });
    ```

3. Use the get command to obtain camera-related property values

    ```cpp
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
                    catch(...) {
                        std::cout << "get bool property failed." << std::endl;
                    }
                    std::cout << "property name:" << propertyItem.name << ",get bool value:" << bool_ret << std::endl;
                    break;
                case OB_INT_PROPERTY:
                    try {
                        int_ret = device->getIntProperty(propertyItem.id);
                    }
                    catch(...) {
                        std::cout << "get int property failed." << std::endl;
                    }
                    std::cout << "property name:" << propertyItem.name << ",get int value:" << int_ret << std::endl;
                    break;
                case OB_FLOAT_PROPERTY:
                    try {
                        float_ret = device->getFloatProperty(propertyItem.id);
                    }
                    catch(...) {
                        std::cout << "get float property failed." << std::endl;
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
    ```

4. Use the set command to set camera-related property values

    ```cpp
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
                    catch(...) {
                        std::cout << "set bool property fail." << std::endl;
                    }
                    std::cout << "property name:" << propertyItem.name << ",set bool value:" << bool_value << std::endl;
                    break;
                case OB_INT_PROPERTY:
                    int_value = std::atoi(strValue.c_str());
                    try {
                        device->setIntProperty(propertyItem.id, int_value);
                    }
                    catch(...) {
                        std::cout << "set int property fail." << std::endl;
                    }
                    std::cout << "property name:" << propertyItem.name << ",set int value:" << int_value << std::endl;
                    break;
                case OB_FLOAT_PROPERTY:
                    float_value = static_cast<float>(std::atof(strValue.c_str())) ;
                    try {
                        device->setFloatProperty(propertyItem.id, float_value);
                    }
                    catch(...) {
                        std::cout << "set float property fail." << std::endl;
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
    ```

## Run Sample

Select the camera you want to operate. If it is a single device, skip the selection.
You can enter the command ? to get all the properties of the camera, including setting the maximum and minimum values, etc.
You can enter set to set command to setto set parameters, for example 6 set 0 (note the space)
You can enter the get command to set parameters, for example, 6 get (note the space)
Press the Esc key in the window to exit the program.

### Result

![image](../../docs/resource/control1.jpg)

![image](../../docs/resource/control2.jpg)
