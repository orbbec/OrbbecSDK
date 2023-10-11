#include <cstring>
#include <iomanip>
#include <iostream>
#include <libobsensor/ObSensor.hpp>
#include <string>

#include "libobsensor/hpp/Error.hpp"
#include "libobsensor/hpp/Pipeline.hpp"
#include "utils.hpp"
#include "window.hpp"

#define ESC 27

const std::map<OBDepthPrecisionLevel, std::string> depthUnitMap = {
    {OB_PRECISION_1MM, "1.0mm"},    {OB_PRECISION_0MM8, "0.8mm"},
    {OB_PRECISION_0MM4, "0.4mm"},   {OB_PRECISION_0MM1, "0.1mm"},
    {OB_PRECISION_0MM2, "0.2mm"},   {OB_PRECISION_0MM5, "0.5mm"},
    {OB_PRECISION_0MM05, "0.05mm"},
};
std::vector<uint16_t> precisionList;
OBDepthPrecisionLevel curDepthPrecisionLevel = OB_PRECISION_UNKNOWN;
float unit = -1;

static void pressKeyExit(std::string msg) {
  if (msg.size() > 0) {
    std::cout << msg << std::endl;
  }

  while (true) {
    // Get the value of the pressed key, if it is the esc key, exit the program
    int key = getch();
    if (key == ESC) break;
  }
}
// Convert depth precision unit to 1mm
void depthPrecisionConversion(std::shared_ptr<ob::DepthFrame> depthFrame) {
  auto size = depthFrame->dataSize();
  auto pixel = static_cast<uint16_t *>(depthFrame->data());

  for (int i = 0; i < size / sizeof(uint16_t); i++) {
    pixel[i] = static_cast<uint16_t>(pixel[i] * unit);
  }
}

int main(int argc, char **argv) try {
  // Create a pipeline with default device
  ob::Pipeline pipe;

  // Get the device inside the pipeline
  auto device = pipe.getDevice();

  // Get the current D2D mode
  if (!device->isPropertySupported(OB_PROP_DISPARITY_TO_DEPTH_BOOL,
                                   OB_PERMISSION_READ_WRITE)) {
    pressKeyExit("Current device not support disparity to depth !");
    return -1;
  }
  // Get the current D2D mode
  auto curD2dMode = device->getBoolProperty(OB_PROP_DISPARITY_TO_DEPTH_BOOL);
  if (!curD2dMode) {
    std::cout << "Current Disparity to depth is software-D2d" << std::endl;
  } else {
    std::cout << "Current Disparity to depth is hardware-D2d" << std::endl;
  }

  // Get the current depthPrecisionLevel
  if (!device->isPropertySupported(OB_PROP_DEPTH_PRECISION_LEVEL_INT,
                                   OB_PERMISSION_READ_WRITE)) {
    pressKeyExit("Current device not support depthPrecision !");
    return -1;
  }
  curDepthPrecisionLevel = (OBDepthPrecisionLevel)device->getIntProperty(
      OB_PROP_DEPTH_PRECISION_LEVEL_INT);
  std::cout << "Current depthPrecision is "
            << depthUnitMap.find(curDepthPrecisionLevel)->second << std::endl;

  // Get the depthPrecisionList
  if (!device->isPropertySupported(OB_STRUCT_DEPTH_PRECISION_SUPPORT_LIST,
                                   OB_PERMISSION_READ) &&
      curD2dMode) {
    pressKeyExit("Current device not support depthPrecisionList !");
    return -1;
  }
  auto depthPrecision =
      device->getStructuredDataExt(OB_STRUCT_DEPTH_PRECISION_SUPPORT_LIST);
  precisionList.clear();
  // Some of the products do not have the same precision list for hardware and
  // software
  if (curD2dMode) {
    int count = 0;
    for (int i = 0; i < depthPrecision->dataSize; i += 2) {
      auto precision = *((uint16_t *)((uint8_t *)depthPrecision->data + i));
      precisionList.push_back(precision);
      std::cout << "precisionList[" << count << "]: "
                << depthUnitMap
                       .find((OBDepthPrecisionLevel)precisionList[count])
                       ->second
                << std::endl;
      count++;
    }
  } else {
    for (int i = 0; i < 6; i++) {
      precisionList.push_back(i);
      std::cout
          << "precisionList[" << i << "]: "
          << depthUnitMap.find((OBDepthPrecisionLevel)precisionList[i])->second
          << std::endl;
    }
  }

  // Let the user choose a depthPrecision, then switch
  if (precisionList.size() > 0) {
    uint32_t index = 0;
    std::cout
        << "Please input the index from above depthPrecisionList, newIndex = ";
    std::cin >> index;
    if (index >= 0 && index < precisionList.size()) {  // legitimacy check
      device->setIntProperty(OB_PROP_DEPTH_PRECISION_LEVEL_INT, index);

      // Checking depth accuracy after switching
      curDepthPrecisionLevel = (OBDepthPrecisionLevel)device->getIntProperty(
          OB_PROP_DEPTH_PRECISION_LEVEL_INT);
      std::cout << "Current depthPrecision is "
                << depthUnitMap.find(curDepthPrecisionLevel)->second
                << std::endl;
    } else {
      std::cout << "switchDepthPrecision faild. invalid index: " << index
                << std::endl;
    }
  }
  if (curDepthPrecisionLevel != OB_PRECISION_UNKNOWN) {
    switch (curDepthPrecisionLevel) {
      case OB_PRECISION_1MM:
        unit = 1;
        break;
      case OB_PRECISION_0MM8:
        unit = 0.8f;
        break;
      case OB_PRECISION_0MM4:
        unit = 0.4f;
        break;
      case OB_PRECISION_0MM1:
        unit = 0.1f;
        break;
      case OB_PRECISION_0MM2:
        unit = 0.2f;
        break;
      case OB_PRECISION_0MM5:
        unit = 0.5f;
        break;
      case OB_PRECISION_0MM05:
        unit = 0.05f;
        break;
      default:
        unit = -1;
        break;
    }
  }

  // Get all stream profiles of the depth camera, including stream resolution,
  // frame rate, and frame format
  auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

  std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
  try {
    // Find the corresponding profile according to the specified format, first
    // look for the y16 format
    depthProfile =
        profiles->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, 30);
  } catch (ob::Error &e) {
    // If the specified format is not found, search for the default profile to
    // open the stream
    depthProfile = std::const_pointer_cast<ob::StreamProfile>(
                       profiles->getProfile(OB_PROFILE_DEFAULT))
                       ->as<ob::VideoStreamProfile>();
  }

  // By creating config to configure which streams to enable or disable for the
  // pipeline, here the depth stream will be enabled
  std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
  config->enableStream(depthProfile);

  // Start the pipeline with config
  pipe.start(config);

  device->setBoolProperty(OB_PROP_LDP_BOOL, false);
  device->setBoolProperty(OB_PROP_LASER_BOOL, true);

  // Create a window for rendering, and set the resolution of the window
  Window app("DepthPrecisionViewer", depthProfile->width(),
             depthProfile->height());

  while (app) {
    // Wait for up to 100ms for a frameset in blocking mode.
    auto frameSet = pipe.waitForFrames(100);
    if (frameSet == nullptr) {
      continue;
    }

    auto depthFrame = frameSet->depthFrame();

    // Convert depth values to 1mm precision
    depthPrecisionConversion(depthFrame);

    // For Y16 format depth frame, print the distance of the center pixel every
    // 30 frames
    if (depthFrame->index() % 30 == 0 &&
        depthFrame->format() == OB_FORMAT_Y16) {
      uint32_t width = depthFrame->width();
      uint32_t height = depthFrame->height();
      uint16_t *data = (uint16_t *)depthFrame->data();

      // Pixel value multiplied by scale is the actual distance value in
      // millimeters
      uint16_t centerDistance = data[width * height / 2 + width / 2];

      // Attention: if the distance is 0, it means that the depth camera cannot
      // detect the object（may be out of detection range）
      std::cout << "The image center depth value is " << centerDistance
                << " mm. " << std::endl;
    }
    // Render frame in the window
    app.addToRender(depthFrame);
  }

  // Stop the pipeline
  pipe.stop();

  return 0;
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  exit(EXIT_FAILURE);
}
