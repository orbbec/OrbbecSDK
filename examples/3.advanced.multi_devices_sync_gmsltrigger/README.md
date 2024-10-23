# C++ Sample: 3.advanced.multi_devices_sync_gmsltrigger

## Overview

GMSL Multi-device Hardware Trigger Configuration

support Connection of GMSL device (Nvidia Xavier/Orin platform)

### Knowledge

Send a hardware synchronization signal with a set frequency to all connected GMSL devices for multi-machine hardware synchronization through the /dev/camsync device node.

## Code overview

1.open /dev/camsync device.

2.set trigger frequency

```cpp
         int startTrigger(uint16_t triggerFps) {
           if(!isDeviceOpen) {
               if(openDevice() < 0) {
                   error("open device Failed!");
                   return -1;
               }
           }
           cs_param_t wt_param = { WRITE_MODE, triggerFps };
           int        ret      = writeTriggerParam(wt_param);
           if(ret < 0) {
               error("write trigger parameter Failed!");
               return ret;
           }
           info("write param: ", wt_param);

           cs_param_t rd_param = { READ_MODE, 0 };
           ret                 = readTriggerParam(rd_param);
           if(ret < 0) {
               error("read trigger parameter Failed!");
           }
           info("read param: ", rd_param);

           return ret;
       }
```

## Run Sample

### 1.**Setup of Trigger Source and Device Node Permissions**

```
sudo  chmod  777  /dev/camsync
```

### 2.**Running Configuration and Triggering Program**

The program is located in the Example/bin directory of the orbbecsdk.

Sample Configuration: 30FPS and Trigger Source Frequency of 3000

```
orbbec@agx:~/SensorSDK/build/install/Example/bin$   ./MultiDeviceSyncGmslTrigger
Please select options: 
------------------------------------------------------------
 0 --> config GMSL SOC hardware trigger Source. Set trigger fps: 
 1 --> start Trigger 
 2 --> stop Trigger 
 3 --> exit 
------------------------------------------------------------
input select item: 0

Enter FPS (frames per second) (for example: 3000): 3000
Setting FPS to 3000...
Please select options: 
------------------------------------------------------------
 0 --> config GMSL SOC hardware trigger Source. Set trigger fps: 
 1 --> start Trigger 
 2 --> stop Trigger 
 3 --> exit 
------------------------------------------------------------
input select item: 1

write param:  mode=1, fps=3000
read param:  mode=1, fps=3000
start pwm source TriggerSync... 

Please select options: 
------------------------------------------------------------
 0 --> config GMSL SOC hardware trigger Source. Set trigger fps: 
 1 --> start Trigger 
 2 --> stop Trigger 
 3 --> exit 
------------------------------------------------------------
input select item: 

```

**Brief description of configuring the trigger program**

Please select options:

 0 --> config GMSL SOC hardware trigger Source. Set trigger fps:
 1 --> start Trigger
 2 --> stop Trigger
 3 --> exit

0: Configure the frequency of the SOC hardware trigger source.

(config trigger frequency as 3000 (Note: It is recommended to configure the trigger frequency to be equal to or less than the set video frame frequency. For example, if the set video frame frequency is 30FPS, the trigger frequency should be 3000 or less.)

1: Start hardware tigger.  (start send hardware signal triggering at the configured trigger frequency)

2: Stop triggering

3: Exit the program
