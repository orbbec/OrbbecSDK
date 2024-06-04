#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"

const char *metaDataTypes[] = { "TIMESTAMP",
                                "SENSOR_TIMESTAMP",
                                "FRAME_NUMBER",
                                "AUTO_EXPOSURE",
                                "EXPOSURE",
                                "GAIN",
                                "AUTO_WHITE_BALANCE",
                                "WHITE_BALANCE",
                                "BRIGHTNESS",
                                "CONTRAST",
                                "SATURATION",
                                "SHARPNESS",
                                "BACKLIGHT_COMPENSATION",
                                "HUE",
                                "GAMMA",
                                "POWER_LINE_FREQUENCY",
                                "LOW_LIGHT_COMPENSATION",
                                "MANUAL_WHITE_BALANCE",
                                "ACTUAL_FRAME_RATE",
                                "FRAME_RATE",
                                "AE_ROI_LEFT",
                                "AE_ROI_TOP",
                                "AE_ROI_RIGHT",
                                "AE_ROI_BOTTOM",
                                "EXPOSURE_PRIORITY",
                                "HDR_SEQUENCE_NAME",
                                "HDR_SEQUENCE_SIZE",
                                "HDR_SEQUENCE_INDEX",
                                "LASER_POWER",
                                "LASER_POWER_LEVEL",
                                "LASER_STATUS",
                                "GPIO_INPUT_DATA" };

int main(int argc, char **argv) try {
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableVideoStream(OB_STREAM_COLOR);

    // Start the pipeline with config
    pipe.start(config);
    auto currentProfile = pipe.getEnabledStreamProfileList()->getProfile(0)->as<ob::VideoStreamProfile>();
    // Create a window for rendering, and set the resolution of the window
    Window app("ColorViewer", currentProfile->width(), currentProfile->height());
    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        // get color frame from frameset
        auto colorFrame = frameSet->colorFrame();
        if(colorFrame == nullptr) {
            continue;
        }

        // print metadata every 30 frames
        auto index = colorFrame->index();
        if(index % 30 == 0) {
            std::cout << "*************************** Color Frame #" << index << " Metadata List ********************************" << std::endl;
            for(int metaDataType = 0; metaDataType < OB_FRAME_METADATA_TYPE_COUNT; metaDataType++) {
                // Check if it is supported metaDataType for current frame
                if(colorFrame->hasMetadata((OBFrameMetadataType)metaDataType)) {
                    // Get the value of the metadata
                    std::cout << metaDataTypes[metaDataType] << ": " << colorFrame->getMetadataValue((OBFrameMetadataType)metaDataType) << std::endl;
                }
                else {
                    std::cout << metaDataTypes[metaDataType] << ": "
                              << "unsupported" << std::endl;
                }
            }
            std::cout << "********************************************************************************" << std::endl << std::endl;
        }

        // Render frameset in the window, only color frames are rendered here.
        app.addToRender(colorFrame);
    }

    // Stop the Pipeline, no frame data will be generated
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
