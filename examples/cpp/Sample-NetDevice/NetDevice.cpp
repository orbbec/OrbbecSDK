#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "../window.hpp"
#include "libobsensor/ObSensor.hpp"
#include "libobsensor/hpp/Error.hpp"

std::shared_ptr<ob::FrameSet> currentFrameSet;
std::mutex frameSetMutex;

int main(int argc, char **argv) try {
  // Create a Context
  ob::Context ctx;

  // Enter the device ip address (currently only FemtoMega devices support
  // network connection, and its default ip address is 192.168.1.10)
  std::string ip;
  std::cout << "Input your device ip(default: 192.168.1.10):";
  std::getline(std::cin, ip);
  if (ip.empty()) {
    ip = "192.168.1.10";
  }

  // Create a network device through ip (the default port number is: 8090,
  // devices that currently support network mode do not support modifying the
  // port number)
  auto device = ctx.createNetDevice(ip.c_str(), 8090);

  // pass in device to create pipeline
  auto pipe = std::make_shared<ob::Pipeline>(device);

  // Create Config for configuring Pipeline work
  std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

  // Get the depth camera configuration list
  auto depthProfileList = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
  // use default configuration
  auto depthProfile = depthProfileList->getProfile(0);
  // enable depth stream
  config->enableStream(depthProfile);

  // Get the color camera configuration list
  auto colorProfileList = pipe->getStreamProfileList(OB_SENSOR_COLOR);
  // use default configuration
  auto colorProfile = colorProfileList->getProfile(0);
  // enable depth stream
  config->enableStream(colorProfile);

  // Pass in the configuration and start the pipeline
  pipe->start(config, [&](std::shared_ptr<ob::FrameSet> frameSet) {
    std::lock_guard<std::mutex> lock(frameSetMutex);
    currentFrameSet = frameSet;
  });

  // Create a window for rendering and set the resolution of the window
  Window app("MultiDeviceViewer", 1280, 720);

  while (app) {
    std::shared_ptr<ob::FrameSet> frameSet;
    {  // Define the scope through curly braces, and the lock will be
       // automatically unlocked after exiting the scope. Unlocking in time can
       // prevent the frameset output thread of the pipeline from being blocked
       // for too long, resulting in an increase in internal cache and an
       // increase in data frame delay
      std::lock_guard<std::mutex> lock(frameSetMutex);
      frameSet = currentFrameSet;
    }
    if (frameSet) {
      auto colorFrame = frameSet->getFrame(OB_FRAME_COLOR);
      auto depthFrame = frameSet->getFrame(OB_FRAME_DEPTH);
      if (colorFrame && depthFrame) {
        if (colorFrame->format() == OB_FORMAT_H264 ||
            colorFrame->format() == OB_FORMAT_H265) {
          // For H264 and H265 format image frames, users can refer to FFmpeg
          // and other decoding libraries and their routines to complete
          // decoding and rendering display. This example is to keep the project
          // and code concise, so it will not be shown.
          if (colorFrame->index() % 30 == 0) {
            // Print the Color data frame information every 30 frames
            std::cout << "Color Frame: index=" << colorFrame->index()
                      << ", timestamp=" << colorFrame->timeStamp();
          }
          app.render({depthFrame}, RENDER_SINGLE);
        } else {
          app.render({colorFrame, depthFrame}, RENDER_ONE_ROW);
        }
      }
    }

    // Sleep for 30ms, control the rendering display refresh rate
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  // stop the pipeline
  pipe->stop();
  return 0;
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  exit(EXIT_FAILURE);
}