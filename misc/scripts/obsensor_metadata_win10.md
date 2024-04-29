# obsensor_metadata_win10

## English

To get device timestamps through UVC protocol, users need to modify the registry and complete registration first, because Windows system has default limitations.

1. Connect the device and confirm that the device is online;
2. Open powershell with administrator privileges, then `cd` command to enter the `scripts` directory;
3. Execute the `Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser` command, and enter `Y` as prompted to confirm;
4. Execute `.\obsensor_metadata_win10.ps1 -op install_all` to complete the registration.

## Chinese

要通过UVC协议获取设备时间戳，用户需要先修改注册表并完成注册，因为Windows系统有默认的限制。

连接设备并确认设备在线；
以管理员权限打开powershell，然后使用cd命令进入scripts目录；
执行`Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser`命令，并按提示输入Y确认；
执行`.\obsensor_metadata_win10.ps1 -op install_all`来完成注册。
