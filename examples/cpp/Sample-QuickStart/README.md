# C++ Sample Quick Start
This Quick Start Sample demonstrates using the minimum API to obtain data streams.

## 1. Create a pipeline with default device
```cpp
ob::Pipeline pipe;
```

## 2.  Start the pipeline with default config，more info please refer to the `OrbbecSDKConfig_v1.0.xml`, by default only enables Depth and Color streams. By modifying the configuration file, you can enable the IR stream.
```cpp
 pipe.start();
```

## 3. get frame data
```cpp
   auto frameSet = pipe.waitForFrames();
   auto colorFrame = frameSet->colorFrame();
   auto depthFrame = frameSet->depthFrame();
```

## 4. Stop the Pipeline
```cpp

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();
```