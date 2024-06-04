# obsensor_metadata_win10
This document introduces two different methods for setting Metadata, explaining their advantages, disadvantages, and precautions.

## Registering Metadata Using a Script

To get device timestamps through UVC protocol, users need to modify the registry and complete registration first, because Windows system has default limitations.

1. Connect the device and confirm that the device is online;
2. Open powershell with administrator privileges, then `cd` command to enter the `scripts` directory;
3. Execute the `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser` command, and enter `Y` as prompted to confirm;
4. Execute `.\obsensor_metadata_win10.ps1 -op install_all` to complete the registration.

**Notes**

Users need to run this script every time a new device is connected. This process can be cumbersome and easy to forget, leading to an inability to obtain the device's Metadata.


## Registering Metadata Using Code

The purpose of the WindowsMetaDataHelper code(Script\WindowsMetaDataHelper) is to demonstrate how to programmatically register Metadata on the Windows platform.

**Notes**

1. The code must be run with administrator privileges, and the PID and VID passed into the code must be in decimal format.
2. If multiple different types of device(different PID) are used, the registration code must be called for the PID of each device.


**Advantages**

Devices of the same type (same PID) only need to be registered once, and new devices can be recognized for Metadata without additional registration.





