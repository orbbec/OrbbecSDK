# C++ Sample: 0.basic.enumerate

## Overview

Use the SDK interface to obtain camera-related information, including model, various sensors, and sensor-related configurations.

### Knowledge

Context is the environment context, the first object created during initialization, which can be used to perform some settings, including but not limited to device status change callbacks, log level settings, etc. Context can access multiple Devices.

Device is the device object, which can be used to obtain the device information, such as the model, serial number, and various sensors.One actual hardware device corresponds to one Device object.

## Code overview

1. Create a context

    ```cpp
        // Create a context.
        ob::Context context;
    ```

2. Check if there is a camera connected

    ```cpp
        // Query the list of connected devices.
        auto deviceList = context.queryDeviceList();
        if(deviceList->getCount() < 1) {
            std::cout << "No device found! Please connect a supported device and retry this program." << std::endl;
            return -1;
        }
    ```

3. Obtain and output relevant information of the access device

    ```cpp
        std::cout << "enumerated devices: " << std::endl;

        std::shared_ptr<ob::Device>     device     = nullptr;
        std::shared_ptr<ob::DeviceInfo> deviceInfo = nullptr;
        for(uint32_t index = 0; index < deviceList->getCount(); index++) {
            // Get device from deviceList.
            device = deviceList->getDevice(index);
            // Get device information from device
            deviceInfo = device->getDeviceInfo();
            std::cout << " - " << index << ". name: " << deviceInfo->getName() << " pid: " << deviceInfo->getPid() << " SN: " << deviceInfo->getSerialNumber()
                        << std::endl;
        }
    ```

4. Wait for keyboard input to select device

    ```cpp
        // select a device.
        int deviceSelected = ob_smpl::getInputOption();
        if(deviceSelected == -1) {
            break;
        }
    ```

5. Output device sensors and wait for keyboard input

     ```cpp
        // Enumerate sensors.
        void enumerateSensors(std::shared_ptr<ob::Device> device) {
            while(true) {
                std::cout << "Sensor list: " << std::endl;
                // Get the list of sensors.
                auto sensorList = device->getSensorList();
                for(uint32_t index = 0; index < sensorList->getCount(); index++) {
                    // Get the sensor type.
                    auto sensorType = sensorList->getSensorType(index);
                    std::cout << " - " << index << "."
                            << "sensor type: " << ob::TypeHelper::convertOBSensorTypeToString(sensorType) << std::endl;
                }

                std::cout << "Select a sensor to enumerate its streams(input sensor index or \'ESC\' to enumerate device): " << std::endl;

                // Select a sensor.
                int sensorSelected = ob_smpl::getInputOption();
                if(sensorSelected == -1) {
                    break;
                }

                // Get sensor from sensorList.
                auto sensor = sensorList->getSensor(sensorSelected);
                enumerateStreamProfiles(sensor);
            }
        }
    ```

6. Output information about the selected sensor

    ```cpp
    // Enumerate stream profiles.
    void enumerateStreamProfiles(std::shared_ptr<ob::Sensor> sensor) {
        // Get the list of stream profiles.
        auto streamProfileList = sensor->getStreamProfileList();
        // Get the sensor type.
        auto sensorType = sensor->getType();
        for(uint32_t index = 0; index < streamProfileList->getCount(); index++) {
            // Get the stream profile.
            auto profile = streamProfileList->getProfile(index);
            if(sensorType == OB_SENSOR_IR || sensorType == OB_SENSOR_COLOR || sensorType == OB_SENSOR_DEPTH || sensorType == OB_SENSOR_IR_LEFT
            || sensorType == OB_SENSOR_IR_RIGHT) {
                printStreamProfile(profile, index);
            }
            else if(sensorType == OB_SENSOR_ACCEL) {
                printAccelProfile(profile, index);
            }
            else if(sensorType == OB_SENSOR_GYRO) {
                printGyroProfile(profile, index);
            }
            else {
                break;
            }
        }
    }
    ```

## Run Sample

In the window, enter the relevant information of the device sensor you want to view according to the prompts.
Press the Esc key in the window to exit the program.

### Result

![image](../../docs/resource/enumerate.jpg)
