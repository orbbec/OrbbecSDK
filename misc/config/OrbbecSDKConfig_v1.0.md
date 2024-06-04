# Orbbec SDK Configuration File Introduction (OrbbecSDKConfig_v1.0.xml)

## Log Configuration
Log configuration mainly sets the Log level, the Log level output to the console, the Log level output to a file, configures the path for saving Log files, sets the size of each Log file, and sets the number of Log files.

```cpp
    <Log>
        <!--Log output level, int type, optional values: 0-DEBUG, 1-INFO, 2-WARN, 3-ERROR, 4-FATAL, 5-OFF-->
        <!--Log level configuration execution priority: 0. Application code configuration > 1. Detailed configuration of configuration file (file & console configuration) > 2. Global configuration of configuration file > 3. SDK code default configuration (file: 0, terminal: 1)-->
        <!--Global log output level-->
        <LogLevel>0</LogLevel>
        <!--File log output level-->
        <FileLogLevel>0</FileLogLevel>
        <!--Console log output level-->
        <ConsoleLogLevel>1</ConsoleLogLevel>
        <!--Default log output file path, string type. If this item is not configured, the default path will be used: Win/Linux: "./Log"; Android: "/sdcard/Orbbec/Log"-->
        <!--<OutputDir>./log</OutputDir>-->
        <!--Default log output file size, int type, unit: MB-->
        <MaxFileSize>100</MaxFileSize>
        <!--Default log output file number (circular overwrite), int type-->
        <MaxFileNum>3</MaxFileNum>
        <!--Log asynchronous output, changing to asynchronous output can reduce the blocking time of printing logs, but some logs may be lost when the program exits abnormally; true-enable, false-disable (default)-->
        <Async>false</Async>
    </Log>
```

## Memory Configuration

```cpp
    <Memory>
        <!--Enable memory pool to reuse frame memory, avoid frequent memory allocation and release. true-enable, false-disable)-->
        <EnableMemoryPool> true </EnableMemoryPool>
        <!--Maximum memory size of all data frames, int type, unit: MB, minimum 100MB-->
        <MaxFrameBufferSize> 2048 </MaxFrameBufferSize>
        <!--Frame buffer queue size in pipeline-->
        <PipelineFrameQueueSize>10</PipelineFrameQueueSize>
        <!--Frame buffer queue size in internal processing unit-->
        <FrameProcessingBlockQueueSize>10</FrameProcessingBlockQueueSize>
    </Memory>
```

**Notes**

1. The default size of the memory pool is 2GB (2048MB).
```cpp
        <MaxFrameBufferSize> 2048 </MaxFrameBufferSize>
```
2. The default number of frame buffere queue in the Pipeline is 10. If the host machine processes slowly, the SDK will buffer up to 10 frames internally, which may affect the delay time in retrieving frames by the user. If the delay is significant, you can reduce the number of buffers.
```cpp
        <PipelineFrameQueueSize>10</PipelineFrameQueueSize>
```

3. The default number of queues in the internal processing unit (Processing unit) is 10. If the host machine processes slowly, the SDK will buffer up to 10 frames internally, which may affect the delay time in retrieving frames by the user. If the delay is significant, you can reduce the number of queues.
```cpp
        <FrameProcessingBlockQueueSize>10</FrameProcessingBlockQueueSize>
```

## Global Timestamp

Based on the device's timestamp and considering data transmission delays, the timestamp is converted to the system timestamp dimension through linear regression. It can be used to synchronize timestamps of multiple different devices. The implementation plan is as follows:

1. Global timestamp fitter: Regularly obtain the device timestamp and the current system timestamp, and calculate the fitting equation parameters using a linear regression method.
2. Global timestamp converter: Convert the data frame timestamp unit to the same unit as the device timestamp, then calculate the overflow times according to the device timestamp to convert to a 64-bit timestamp, and then convert to a global timestamp according to the fitting parameters output by the global timestamp fitter.

```cpp
    <Misc>
        <GlobalTimestampFitterEnable>true</GlobalTimestampFitterEnable>
        <!--Global timestamp fitter refresh interval, unit: milliseconds, default value: 8000, minimum value: 100, it is recommended not to be greater than 10000 -->
        <GlobalTimestampFitterInterval>8000</GlobalTimestampFitterInterval>
        <!--Global timestamp fitter queue size, default value: 10, minimum value: 4 -->
        <GlobalTimestampFitterQueueSize>10</GlobalTimestampFitterQueueSize>
    </Misc>
```

1. By default, the device time is obtained every eight seconds to update the global timestamp fitter.
```cpp
        <GlobalTimestampFitterInterval>8000</GlobalTimestampFitterInterval>
```

2. The default queue size of the global timestamp fitter is 10.
```cpp
        <GlobalTimestampFitterQueueSize>10</GlobalTimestampFitterQueueSize>
```

**Notes**

1. The global timestamp mainly supports the Gemini 330 series. Gemini 2, Gemini 2L, Femto Mega, and Femto Bolt are also supported but not thoroughly tested. If there are stability issues with these devices, the global timestamp function can be turned off.
```cpp
        <GlobalTimestampFitterEnable>false</GlobalTimestampFitterEnable>
```

## Pipeline Configuration

```cpp
    <!--Default working configuration of pipeline-->
    <Pipeline>
        <Stream>
            <Depth>
                <!--Use default stream profile, bool type, true: use default stream profile, false: use specified stream profile in the configuration file-->
                <UseDefaultStreamProfile>true</UseDefaultStreamProfile>
            </Depth>
            <Color>
                <UseDefaultStreamProfile>true</UseDefaultStreamProfile>
            </Color>
            <!--If you need to open other streams, you can refer to the above format to add configuration-->
        </Stream>
        <!--Image alignment, int type, optional values: 0 disable, 1 hardware d2c alignment, 2 software d2c alignment-->
        <AlignMode>0</AlignMode>
        <!--Frame synchronization output between different data streams, bool type, true-enable frame synchronization, false-disable frame synchronization-->
        <FrameSync>false</FrameSync>
    </Pipeline>
```

1. Pipeline primarily sets which video streams to enable. By default, only Depth and Color streams are enabled. You can add to enable IR streams, left IR, and right IR as follows.
```cpp
<!--If you need to open other streams, you can refer to the above format to add configuration-->
            <IR>
                <UseDefaultStreamProfile>true</UseDefaultStreamProfile>
            </IR>
            <IR_LEFT>
                <UseDefaultStreamProfile>true</UseDefaultStreamProfile>
            </IR_LEFT>
            <IR_RIGHT>
                <UseDefaultStreamProfile>true</UseDefaultStreamProfile>
            </IR_RIGHT>
```
2. Pipeline can set D2C, such as hardware D2C and software D2C. Some devices do not support hardware D2C, such as Femto Bolt. If you need to use various different types of devices, it is recommended to set this in the user code based on the device type.
```cpp
  <AlignMode>0</AlignMode>
```
3. Pipeline can set frame synchronization. Enabling frame synchronization will match Depth, Color, and other video frames based on the timestamp and output them as FrameSet within the Pipeline.
```cpp
        <FrameSync>false</FrameSync>
```

## Device Configuration

```cpp
    <!--Default configuration of data streams for different types of devices-->
    <Device>
        <EnumerateNetDevice>false</EnumerateNetDevice>
        <!--UVC Backend select on Linux; optional values: Auto, V4L2, LibUVC; Auto is the default value-->
        <LinuxUVCBackend>LibUVC</LinuxUVCBackend>

            <!--Gemini 335 config-->
        <OrbbecGemini335>
	        <!--Whether to enable heartbeat by default-->
	        <DefaultHeartBeat>0</DefaultHeartBeat>

			<Depth>
			    <!--Number of retries for open stream failures, 0 means no retries-->
				<StreamFailedRetry>0</StreamFailedRetry>
				<!--Open flow waits for the timeout period of the first frame of data, after which the open flow will fail-->
				<MaxStartStreamDelayMs>5000</MaxStartStreamDelayMs>
				<!--The number of data stream restarts due to abnormal interruption, 0 means no restart-->
				<StreamInterruptedRestart>0</StreamInterruptedRestart>
				<!--The maximum frame interval time, if this value is exceeded, it will be judged that the stream is interrupted-->
				<MaxFrameIntervalMs>2000</MaxFrameIntervalMs>
                <!--The resolution width is enabled by default, int type-->
                <Width>848</Width>
                <!--High resolution is enabled by default, int type-->
                <Height>480</Height>
                <!--The frame rate of the resolution enabled by default, int type-->
                <FPS>30</FPS>
                <Format>Y16</Format>
            </Depth>
            <Color>
                <!--The resolution width is enabled by default, int type-->
                <Width>1280</Width>
                <!--High resolution is enabled by default, int type-->
                <Height>720</Height>
                <!--The frame rate of the resolution enabled by default, int type-->
                <FPS>30</FPS>
                <Format>MJPG</Format>
            </Color>
            <IR>
                <!--The resolution width is enabled by default, int type-->
                <Width>848</Width>
                <!--High resolution is enabled by default, int type-->
                <Height>480</Height>
                <!--The frame rate of the resolution enabled by default, int type-->
                <FPS>30</FPS>
                <Format>Y8</Format>
            </IR>
			<IR_LEFT>
                <!--The resolution width is enabled by default, int type-->
                <Width>848</Width>
                <!--High resolution is enabled by default, int type-->
                <Height>480</Height>
                <!--The frame rate of the resolution enabled by default, int type-->
                <FPS>30</FPS>
                <Format>Y8</Format>
            </IR_LEFT>
			<IR_RIGHT>
                <!--The resolution width is enabled by default, int type-->
                <Width>848</Width>
                <!--High resolution is enabled by default, int type-->
                <Height>480</Height>
                <!--The frame rate of the resolution enabled by default, int type-->
                <FPS>30</FPS>
                <Format>Y8</Format>
            </IR_RIGHT>
        </OrbbecGemini335>     
    </Device>
```

1. Set whether to enumerate network devices. Femto Mega and Gemini 2 XL support network functions. If you need to use the network functions of these two devices, you can set this to true.
```cpp
        <EnumerateNetDevice>false</EnumerateNetDevice>
```

2. Set whether to use LibUVC or V4L2 to receive data on Linux or ARM. V4L2 is not supported by all devices, and we recommend using LibUVC. The Gemini 330 series devices support V4L2, but kernel patches are needed to obtain Metadata data.
```cpp
        <LinuxUVCBackend>LibUVC</LinuxUVCBackend>
```

3. Set the resolution, frame rate, and data format.