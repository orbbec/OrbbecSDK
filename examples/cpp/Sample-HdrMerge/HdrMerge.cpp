/**
 *HDR merge example
 *
 */

#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"

int main(int argc, char **argv) try {
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Get the device from the pipeline
    auto device = pipe.getDevice();

    // Check if the device supports HDR merge
    if(!device->isPropertySupported(OB_STRUCT_DEPTH_HDR_CONFIG, OB_PERMISSION_READ_WRITE)) {
        std::cerr << "Current default device does not support HDR merge" << std::endl;
        return -1;
    }

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto depthProfiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);
    auto depthProfile  = depthProfiles->getProfile(OB_PROFILE_DEFAULT);
    config->enableStream(depthProfile);

    // Create HdrMerge post processor to merge depth frames betweens different hdr sequence ids.
    // The HdrMerge also supports processing of infrared frames.
    ob::HdrMerge hdrMerge;

    // configure and enable Hdr stream
    OBHdrConfig obHdrConfig;
    obHdrConfig.enable     = true;  // enable HDR merge
    obHdrConfig.exposure_1 = 7500;
    obHdrConfig.gain_1     = 24;
    obHdrConfig.exposure_2 = 100;
    obHdrConfig.gain_2     = 16;
    device->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));

    // Start the pipeline with config
    pipe.start(config);

    // Create a window for rendering and set the resolution of the window
    bool   resizeWindows = true;
    Window app("HDR-Merge", 1280, 720);
    bool   mergeRequired = true;

    std::cout << "Press 'M' to toggle HDR merge." << std::endl;
    while(app) {
        auto key = app.waitKey(10);
        if(key == 'M' || key == 'm') {
            mergeRequired = !mergeRequired;
            if(mergeRequired) {
                std::cout << "HDR merge enabled." << std::endl;
            }
            else {
                std::cout << "HDR merge disabled." << std::endl;
            }
        }

        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        auto depthFrame = frameSet->depthFrame();
        if(depthFrame == nullptr) {
            continue;
        }

        if(mergeRequired) {
            // Using HdrMerge post processor to merge depth frames
            auto mergedDepthFrame = hdrMerge.process(depthFrame);
            if(mergedDepthFrame == nullptr) {
                continue;
            }

            // add merged depth frame to render queue
            app.addToRender(mergedDepthFrame);
        }
        else {
            // add original depth frame to render queue
            app.addToRender(depthFrame);
        }
    }

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();

    // close hdr merge
    obHdrConfig.enable = false;
    device->setStructuredData(OB_STRUCT_DEPTH_HDR_CONFIG, &obHdrConfig, sizeof(OBHdrConfig));

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
