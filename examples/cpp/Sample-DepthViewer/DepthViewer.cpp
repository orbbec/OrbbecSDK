#include "../window.hpp"

#include "libobsensor/hpp/Error.hpp"
#include "libobsensor/hpp/Pipeline.hpp"

int main(int argc, char **argv) try {
  // Create a pipeline with default device
  ob::Pipeline pipe;

  // Get all stream profiles of the depth camera, including stream resolution,
  // frame rate, and frame format
  auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

  std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
  try {
    // Find the corresponding profile according to the specified format, first
    // look for the y16 format
    depthProfile = profiles->getVideoStreamProfile(640, 0, OB_FORMAT_Y16, 30);
  } catch (ob::Error &e) {
    // If the specified format is not found, search for the default profile to
    // open the stream
    depthProfile =
        std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(0))
            ->as<ob::VideoStreamProfile>();
  }

  // By creating config to configure which streams to enable or disable for the
  // pipeline, here the depth stream will be enabled
  std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
  config->enableStream(depthProfile);

  // Start the pipeline with config
  pipe.start(config);

  // Get whether the mirror property has writable permission
  if (pipe.getDevice()->isPropertySupported(OB_PROP_DEPTH_MIRROR_BOOL,
                                            OB_PERMISSION_WRITE)) {
    // Set mirroring
    pipe.getDevice()->setBoolProperty(OB_PROP_DEPTH_MIRROR_BOOL, true);
  }

  // Create a window for rendering and set the resolution of the window
  Window app("DepthViewer", depthProfile->width(), depthProfile->height());

  while (app) {
    // Wait for up to 100ms for a frameset in blocking mode.
    auto frameSet = pipe.waitForFrames(100);
    if (frameSet == nullptr) {
      continue;
    }

    // Render a set of frame in the window only the depth frame is rendered
    // here, but due to interface design reasons, it must also be passed in as
    // an array. RENDER_SINGLE means only the first frame in the array is
    // rendered
    app.render({frameSet->depthFrame()}, RENDER_SINGLE);
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
