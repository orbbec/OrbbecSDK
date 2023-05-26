#include "libobsensor/ObSensor.hpp"

#if defined(_WIN32)
#include <conio.h>
#else
#include "../conio.h"
#endif
#include <iostream>
#include <thread>

#define ESC 27
std::shared_ptr<ob::Pipeline> pipeline = NULL;

void CreateAndStartWithConfig() {
  // start video stream according to the stream profile of the configuration
  // file.If there is no configuration file, the first stream profile will be
  // used.
  try {
    pipeline->start(nullptr);
  } catch (...) {
    std::cout << "Pipeline start failed!" << std::endl;
  }
}

// Device connection callback
void DeviceConnectCallback(std::shared_ptr<ob::DeviceList> connectList) {
  std::cout << "Device connect: " << connectList->deviceCount() << std::endl;
  if (connectList->deviceCount() > 0) {
    if (pipeline == NULL) {
      pipeline = std::make_shared<ob::Pipeline>();
      CreateAndStartWithConfig();
    }
  }
}

// Device disconnect callback
void DeviceDisconnectCallback(std::shared_ptr<ob::DeviceList> disconnectList) {
  std::cout << "Device disconnect: " << disconnectList->deviceCount()
            << std::endl;
  if (disconnectList->deviceCount() > 0) {
    try {
      pipeline->stop();
    } catch (ob::Error &e) {
      std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
                << "\nmessage:" << e.getMessage()
                << "\ntype:" << e.getExceptionType() << std::endl;
    }
    pipeline.reset();
  }
}

int main(int argc, char **argv) try {
  // create context
  ob::Context ctx;

  // register device callback
  ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList,
                                  std::shared_ptr<ob::DeviceList> addedList) {
    DeviceDisconnectCallback(removedList);
    DeviceConnectCallback(addedList);
  });

  // Get the current number of devices and open the stream
  if (ctx.queryDeviceList()->deviceCount() > 0) {
    if (pipeline == NULL) {
      pipeline = std::make_shared<ob::Pipeline>();
      CreateAndStartWithConfig();
    }
  }

  while (true) {
    if (kbhit()) {
      int key = getch();

      // Press the esc key to exit
      if (key == ESC) {
        break;
      }

      // Press the r key to reboot the device to trigger the device disconnect
      // and reconnect event, or manually unplug and plugin the device.
      if (key == 'r' || key == 'R') {
        if (pipeline) {
          pipeline->getDevice()->reboot();
        }
      }
    }
    if (pipeline) {
      // Wait for up to 100ms for a frameset in blocking mode.
      auto frameSet = pipeline->waitForFrames(100);
      if (frameSet == nullptr) {
        continue;
      }

      // Get the depth data frame
      auto depthFrame = frameSet->depthFrame();
      if (depthFrame) {
        uint16_t *data = (uint16_t *)depthFrame->data();
        auto width = depthFrame->width();
        auto height = depthFrame->height();
        auto scale = depthFrame->getValueScale();
        uint16_t pixelValue = *(data + width * height / 2 + width / 2);
        std::cout << "=====Depth Frame Info======"
                  << "FrameType: " << depthFrame->type()
                  << ", index: " << depthFrame->index() << ", width: " << width
                  << ", height: " << height
                  << ", format: " << depthFrame->format()
                  << ", timeStampUs: " << depthFrame->timeStampUs()
                  << "us, centerDepth: " << (int)(pixelValue * scale) << "mm"
                  << std::endl;
      }

      // Get the color data frame
      auto colorFrame = frameSet->colorFrame();
      if (colorFrame) {
        std::cout << "=====Color Frame Info======"
                  << "FrameType: " << colorFrame->type()
                  << ", index: " << colorFrame->index()
                  << ", width: " << colorFrame->width()
                  << ", height: " << colorFrame->height()
                  << ", format: " << colorFrame->format()
                  << ", timeStampUs: " << colorFrame->timeStampUs() << "us"
                  << std::endl;
      }
    }
  }
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  exit(EXIT_FAILURE);
}