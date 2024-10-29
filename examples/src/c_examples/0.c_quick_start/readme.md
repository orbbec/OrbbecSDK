# Quick Start with C

This is a quick start guide to start device streams using the Orbbec SDK C API.

## Overview

### Knowledge

**Pipeline** is a pipeline for processing data streams, providing multi-channel stream configuration, switching, frame aggregation, and frame synchronization functions.

**Frameset** is a combination of different types of Frames

## Code Overview

### 1. Create pipeline

```c
// Used to return SDK interface error information.
ob_error *error = NULL;

// Create a pipeline to manage the streams
ob_pipeline *pipe = ob_create_pipeline(&error);
```

### 2. Start pipeline

```c
// Start Pipeline with default configuration (At default, the pipeline will start with the color and depth streams)
ob_pipeline_start(pipe, &error);
```

### 3. Get frameset from pipeline

```c
// Wait for frameset from pipeline, with a timeout of 1000 milliseconds.
ob_frame *frameset = ob_pipeline_wait_for_frameset(pipe, 1000, &error);
CHECK_OB_ERROR_EXIT(&error);

// If frameset is NULL, timeout occurred, continue to next iteration.
if(frameset == NULL) {
    continue;
}

// Get the color and depth frames from the frameset and calculate their frame rate.
calculate_and_print_frame_rate(frameset);

// delete frameset
ob_delete_frame(frameset, &error);
CHECK_OB_ERROR_EXIT(&error);
```

## Run Sample

If you are on Windows, you can switch to the directory `OrbbecSDK-dev/build/win_XX/bin` to find the `ob_quick_start_c.exe`.

If you are on linux, you can switch to the directory `OrbbecSDK-dev/build/linux_XX/bin` to find the `ob_quick_start_c`.

### Key introduction

Press 'ESC' key to stop the pipeline and exit the program.

### Result

![Quick_Start_C](../../../docs/resource/quick_start_c.jpg)
