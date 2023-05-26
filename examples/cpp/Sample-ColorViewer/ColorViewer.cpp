#include "../window.hpp"
#include "libobsensor/hpp/Error.hpp"
#include "libobsensor/hpp/Pipeline.hpp"

int main(int argc, char **argv) try {
  // Create a pipeline with default device
  ob::Pipeline pipe;

  // Configure which streams to enable or disable for the Pipeline by creating a
  // Config
  std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

  std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
  try {
    // Get all stream profiles of the color camera, including stream resolution,
    // frame rate, and frame format
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_COLOR);
    try {
      // Find the corresponding Profile according to the specified format, and
      // choose the RGB888 format first
      colorProfile =
          profiles->getVideoStreamProfile(1280, 0, OB_FORMAT_RGB, 30);
    } catch (ob::Error &e) {
      // If the specified format is not found, search for the default profile to
      // open the stream
      colorProfile =
          std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(0))
              ->as<ob::VideoStreamProfile>();
    }
    config->enableStream(colorProfile);
  } catch (ob::Error &e) {
    std::cerr << "Current device is not support color sensor!" << std::endl;
    exit(EXIT_FAILURE);
  }

  // Start the pipeline with config
  pipe.start(config);

  // Create a window for rendering and set the resolution of the window
  Window app("ColorViewer", colorProfile->width(), colorProfile->height());

  // Get whether the mirror property has writable permission
  try {
    if (pipe.getDevice()->isPropertySupported(OB_PROP_COLOR_MIRROR_BOOL,
                                              OB_PERMISSION_WRITE)) {
      // set mirroring
      pipe.getDevice()->setBoolProperty(OB_PROP_COLOR_MIRROR_BOOL, true);
    }
  } catch (const ob::Error &e) {
    std::cerr << "Failed to set mirror property: " << e.getMessage()
              << std::endl;
  }

  while (app) {
    // Wait for up to 100ms for a frameset in blocking mode.
    auto frameSet = pipe.waitForFrames(100);
    if (frameSet == nullptr) {
      continue;
    }

    // Render frameset in the window, only color frames are rendered here, but
    // also passed in as an array, RENDER_SINGLE means only render the first
    // frame in the array
    app.render({frameSet->colorFrame()}, RENDER_SINGLE);
  }

  // Stop the Pipeline, no frame data will be generated
  pipe.stop();

  return 0;
} catch (ob::Error &e) {
  std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs()
            << "\nmessage:" << e.getMessage()
            << "\ntype:" << e.getExceptionType() << std::endl;
  exit(EXIT_FAILURE);
}
