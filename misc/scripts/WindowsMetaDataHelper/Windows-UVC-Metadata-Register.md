# Windows UVC Metadata Register
Since devices must be registered using the obsensor_metadata_win10.ps1 script, users need to run this script every time a new device is connected. This process can be cumbersome and easy to forget, leading to the inability to obtain Metadata. 

The purpose of this code is to demonstrate how to register Metadata on Windows programmatically.

**This sample only supports the Windows platform.**


## Background

To obtain UVC custom metadata windows the platform, you need to add special fields to the device entries in the registry. The relevant information is as follows:
https://learn.microsoft.com/en-us/windows-hardware/drivers/stream/uvc-extensions-1-5

![image](image/metadata.png)

## Solution

The entries of Orbbec's device in the registry are fixed. Traverse all entries in this path, find the pid and vid that match the current device, and add corresponding fields to the successfully matched entries to obtain UVC custom metadata.
```cpp
    "SYSTEM\CurrentControlSet\Control\DeviceClasses\e5323777-f976-4f5b-9b55-b94699c46e44"
    
    "SYSTEM\CurrentControlSet\Control\DeviceClasses\65E8773D-8F56-11D0-A3B9-00A0C9223196"
```

