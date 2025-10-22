# obsensor_metadata_win10

This document introduces the process for getting metadata from Orbbec cameras with UVC protocal on Windows system. The following process has been verified on Windows 10 and Windows 11.

## Registering Metadata Using a Script

To get device metadata through UVC protocol, users need to modify the registry and complete registration first, because Windows system has default limitations.

1. Connect the device and confirm that the device is online;
2. Open PowerShell with administrator privileges, then use the `cd` command to enter the directory where the script is located;
3. Execute the `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser` command, and enter `Y` as prompted to confirm;
   - Try to execute the `Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope CurrentUser` command if the previous command fails in some cases;
4. Execute `.\obsensor_metadata_win10.ps1 -op install_all` to complete the registration.

**Notes**:
Users need to run this script every time a new device is connected. This process can be cumbersome and easy to forget, leading to an inability to obtain the device's metadata.
