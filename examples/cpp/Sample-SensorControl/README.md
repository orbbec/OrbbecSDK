# C++ Sample Device Control


Function description: Demonstrate Device control operations

This example is based on the C++ low level API for demonstration

## 1. Get device list
```cpp
    // Create a Context.
    ob::Context context;
    
    // Query the list of connected devices
    auto deviceList = context.queryDeviceList();
```

Get device and output device information
```cpp
    if(deviceList->deviceCount() > 0) {
        if(deviceList->deviceCount() <= 1) {
            // If a single device is plugged in, the first one is selected by default
            device = deviceList->getDevice(0);
        }
        else {
            device = selectDevice(deviceList);
        }
        auto deviceInfo = device->getDeviceInfo();
        std::cout << "\n------------------------------------------------------------------------\n";
        std::cout << "Current Device: "
                    << " name: " << deviceInfo->name() << ", vid: 0x" << std::hex << deviceInfo->vid() << ", pid: 0x" << std::setw(4) << std::setfill('0')
                    << deviceInfo->pid() << ", uid: 0x" << deviceInfo->uid() << std::dec << std::endl;
    }
```

## 2. Follow the prompts to get all properties
```cpp
    std::cout << "Input \"?\" to get all properties." << std::endl;
    std::getline(std::cin, choice);
```

## 3. Input parameter prompt
```cpp
    std::cout << "Property control usage: [property index] [set] [property value] or [property index] [get]" << std::endl;
```

## 4. Get property
```cpp
    getPropertyValue(device, propertyItem);
```

## 5. Set property
```cpp
    setPropertyValue(device, propertyItem, controlVec.at(2));
```

## 6. expected Output

![image](Image/SensorControl.png)