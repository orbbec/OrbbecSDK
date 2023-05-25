#include <libobsensor/ObSensor.hpp>

#include "../window.hpp"

int main(int argc, char **argv) try {
  // Create a pipeline with default device
  ob::Pipeline pipe;

  // Check if camera depth work mode is supported
  if (!pipe.getDevice()->isPropertySupported(OB_STRUCT_CURRENT_DEPTH_ALG_MODE,
                                             OB_PERMISSION_READ_WRITE)) {
    std::cerr << "Current device is not support depth work mode." << std::endl;
    exit(-1);
  }
  // Switch to self-calibration mode
  pipe.getDevice()->switchDepthWorkMode("In-scene Calibration");

  // Get all stream profiles of the infrared camera, including stream
  // resolution, frame rate, and frame format
  auto profiles = pipe.getStreamProfileList(OB_SENSOR_IR_LEFT);

  std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
  try {
    // Find the corresponding profile according to the specified format, first
    // look for the y16 format
    irProfile = profiles->getProfile(0)->as<ob::VideoStreamProfile>();
    // irProfile = profiles->getVideoStreamProfile(640, 0, OB_FORMAT_Y16,30);
    std::cout << "format: " << (int)irProfile->format() << std::endl;
  } catch (ob::Error &e) {
    // After the y16 format is not found, the format does not match and the
    // corresponding profile is searched for open stream
    irProfile = profiles->getVideoStreamProfile(640, 0, OB_FORMAT_UNKNOWN, 30);
  }

  // By creating config to configure which streams to enable or disable for the
  // pipeline, here the infrared stream will be enabled
  std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
  config->enableStream(irProfile);

  // Get all stream profiles of the infrared camera, including stream
  // resolution, frame rate, and frame format
  auto irRightProfiles = pipe.getStreamProfileList(OB_SENSOR_IR_RIGHT);

  std::shared_ptr<ob::VideoStreamProfile> irRightProfile = nullptr;
  try {
    // Find the corresponding profile according to the specified format, first
    // look for the y16 format
    irRightProfile =
        irRightProfiles->getProfile(0)->as<ob::VideoStreamProfile>();
    // irRightProfile = irRightProfiles->getVideoStreamProfile(640, 0,
    // OB_FORMAT_Y16,30);
  } catch (ob::Error &e) {
    // After the y16 format is not found, the format does not match and the
    // corresponding profile is searched for open stream
    irRightProfile =
        irRightProfiles->getVideoStreamProfile(640, 0, OB_FORMAT_UNKNOWN, 30);
  }
  config->enableStream(irRightProfile);

  // Start the pipeline with config
  pipe.start(config);

  // Create a window for rendering and set the resolution of the window
  Window app("InfraredViewer", irProfile->width(), irProfile->height() / 2);

  bool isMirror = false;
  // Get whether the mirror property has writable permission
  if (pipe.getDevice()->isPropertySupported(OB_PROP_IR_MIRROR_BOOL,
                                            OB_PERMISSION_WRITE)) {
    // Set mirroring
    pipe.getDevice()->setBoolProperty(OB_PROP_IR_MIRROR_BOOL, isMirror);
  }
  // Get whether the mirror property has writable permission
  if (pipe.getDevice()->isPropertySupported(OB_PROP_IR_RIGHT_MIRROR_BOOL,
                                            OB_PERMISSION_WRITE)) {
    // Set mirroring
    pipe.getDevice()->setBoolProperty(OB_PROP_IR_RIGHT_MIRROR_BOOL, isMirror);
  }

  while (app) {
    // Wait for up to 100ms for a frameset in blocking mode.
    auto frameSet = pipe.waitForFrames(100);
    if (frameSet == nullptr) {
      continue;
    }

    // Get the data of left and right IR
    auto leftFrame = frameSet->getFrame(OB_FRAME_IR_LEFT);
    auto rightFrame = frameSet->getFrame(OB_FRAME_IR_RIGHT);
    if (leftFrame == nullptr || rightFrame == nullptr) {
      std::cout << "left ir frame or right ir frame is null. left frame: "
                << leftFrame << ", rightFrame: " << rightFrame << std::endl;
      continue;
    }

    // Render a set of frame in the window, only the infrared frame is rendered
    // here, but it must also be passed in as an array. RENDER_SINGLE means only
    // render the first frame in the array.
    app.render({leftFrame, rightFrame}, RENDER_ONE_ROW);
  }

  // Stop the pipeline, no frame data will be generated
  pipe.stop();

  return 0;
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  exit(EXIT_FAILURE);
}