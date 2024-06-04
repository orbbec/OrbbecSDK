# Orbbec SDK 配置文件介绍(OrbbecSDKConfig_v1.0.xml)


## Log 配置
Log 主要设置Log等级、设置Log输出到控制台的等级、输出到File的等级，配置保存Log文件的路径，设置每个Log文件的大小，设置Log文件的个数。
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


## Memory 配置

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

**注意事项**

1、默认内存池的大小为2GB(2048MB)。
```cpp
        <MaxFrameBufferSize> 2048 </MaxFrameBufferSize>
```
2、默认Pipeline缓冲的帧的个数为10个,如果上位机处理比较慢,SDK内部最多会缓冲10帧数据，这样会影响用户取帧的延迟，如果延迟比较大，可以减少缓冲区个数。
```cpp
        <PipelineFrameQueueSize>10</PipelineFrameQueueSize>
```

3、默认内部处理单元(Processing unit)的队列的个数为10个，如果上位机处理比较慢,SDK内部最多会缓冲10帧数据，这样会影响用户取帧的延迟，如果延迟比较大，可以减少队列个数。

```cpp
        <FrameProcessingBlockQueueSize>10</FrameProcessingBlockQueueSize>
```

## 全局时间戳

基于设备的时间戳，并且考虑了数据的传输延迟，通过线性回归，转换到系统时间戳维度。 可以用来同步多个不同的设备的时间戳。实现方案如下：
1. 全局时间戳拟合器：定时获取设备时间戳和当前修系统时间戳，通过一元线性回归方法计算出拟合方程参数。
2. 全局时间戳转换器：将数据帧时间戳单位转换到与设备时间戳相同的单位，然后根据设备时间戳计算出溢出次数从而转为 64bit 时间戳，然后根据全局时间戳拟合器输出的拟合参数，换算成全局时间戳。
```cpp
    <Misc>
        <GlobalTimestampFitterEnable>true</GlobalTimestampFitterEnable>
        <!--Global timestamp fitter refresh interval, unit: milliseconds, default value: 8000, minimum value: 100, it is recommended not to be greater than 10000 -->
        <GlobalTimestampFitterInterval>8000</GlobalTimestampFitterInterval>
        <!--Global timestamp fitter queue size, default value: 10, minimum value: 4 -->
        <GlobalTimestampFitterQueueSize>10</GlobalTimestampFitterQueueSize>
    </Misc>

```

1. 默认8秒获取一次设备时间，更新全局时间戳拟合器。
```cpp
        <GlobalTimestampFitterInterval>8000</GlobalTimestampFitterInterval>
```

2. 默认的全局时间戳拟合器的队列大小为10。
```cpp
        <GlobalTimestampFitterQueueSize>10</GlobalTimestampFitterQueueSize>
```

**注意事项**

1. 全局时间戳，主要支持Gemini 330系列。 Gemini 2、Gemini 2L、Femto Mega、Femto Bolt 也支持，但是未做详细测试，如果这些设备使用稳定性有问题，可以将全局时间戳功能关掉。
```cpp
        <GlobalTimestampFitterEnable>false</GlobalTimestampFitterEnable>
```

## Pipeline 配置

```cpp
    <!--Default working configuration of pipeline-->
    <Pipeline>
        <Stream>
            <Depth>
                <!--Use default stream profile, bool type, true： use default stream profile, false：use specified stream profile in the configuration file-->
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

1、Pipeline主要设置开启哪些视频流，默认只开启Depth和Color流，可以增加开启IR流、左IR、右IR，设置的方法如下。
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
2、Pipeline 可以设置D2C，如硬件D2C和软件D2C，有些设备不支持硬件D2C，如：Femto Bolt。如果要使用多种不同类型的设备，建议在用户代码中通过设备类型，进行设置。
```cpp
  <AlignMode>0</AlignMode>
```
3、Pipeline 可以设置帧同步，开启帧同步，Pipeline 内部会根据时间戳匹配，将Depth、Color等视频帧以FrameSet输出。
```cpp
        <FrameSync>false</FrameSync>
```

## Device 配置
```CPP
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

1. 设置是否枚举网络设备，Femto Mega 和 Gemini 2 XL 支持网络功能，如果需要使用这2个设备的网络功能，可以设置为true
```cpp
        <EnumerateNetDevice>false</EnumerateNetDevice>
```

2. 设置在Linux 或ARM上，使用LibUVC 还是V4L2接收数据，V4L2 不是所有的设备都支持，而且使用V4L2，我们推荐使用LibUVC。 Gemini 330系列设备支持V4L2，但是需要打内核补丁，才能获取Metadata数据。
```cpp
        <LinuxUVCBackend>LibUVC</LinuxUVCBackend>
```

3、设置分辨率、帧率、数据格式。 