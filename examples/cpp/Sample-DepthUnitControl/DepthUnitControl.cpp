#include "window.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <libobsensor/ObSensor.hpp>
#include "utils.hpp"

#define ESC 27

const std::map<OBDepthPrecisionLevel, std::string> depthUnitMap = {
    { OB_PRECISION_1MM, "1.0mm" },  { OB_PRECISION_0MM8, "0.8mm" }, { OB_PRECISION_0MM4, "0.4mm" },   { OB_PRECISION_0MM1, "0.1mm" },
    { OB_PRECISION_0MM2, "0.2mm" }, { OB_PRECISION_0MM5, "0.5mm" }, { OB_PRECISION_0MM05, "0.05mm" },
};

std::vector<OBDepthPrecisionLevel> supportedPrecisionList = {
    OB_PRECISION_1MM, OB_PRECISION_0MM8, OB_PRECISION_0MM4, OB_PRECISION_0MM1,
    OB_PRECISION_0MM2, OB_PRECISION_0MM5, OB_PRECISION_0MM05
};

static void pressKeyExit(std::string msg) {
    if(msg.size() > 0) {
        std::cout << msg << std::endl;
    }

    while(true) {
        // Get the value of the pressed key, if it is the esc key, exit the program
        int key = getch();
        if(key == ESC)
            break;
    }
}

// Convert depth unit to 1mm
std::vector<uint16_t> convertDepthFrameUnitTo1mm(std::shared_ptr<ob::DepthFrame> depthFrame) {
    auto size  = depthFrame->dataSize();
    auto pixel = static_cast<uint16_t *>(depthFrame->data());
    auto resultImageData = std::vector<uint16_t>(size / sizeof(uint16_t));

    // Get depth value scale
    // after change depth unit, we can get the value scale from frame, which can be used to convert the depth unit to 1mm
    auto valueScale = depthFrame->getValueScale();

    // Convert depth unit to 1mm
    // Tips:
    //  1. In fact, for user application, don't need to convert depth unit to 1mm for all pixels, just need to convert the pixels used critical.
    //  2. User can convert depth unit to ohter unit by multiply the depth value with the other value.
    for(int i = 0; i < size / sizeof(uint16_t); i++) {
        resultImageData.data()[i] = static_cast<uint16_t>(pixel[i] * valueScale);
    }

    return resultImageData;
}

// change depth unit by adjust the depth precision level
bool changeDepthUnit(std::shared_ptr<ob::Device> &device) {
    auto currentPrecisionLevel = (OBDepthPrecisionLevel)device->getIntProperty(OB_PROP_DEPTH_PRECISION_LEVEL_INT);
    std::cout << "Current depth unit is " << depthUnitMap.find(currentPrecisionLevel)->second << "mm" << std::endl;


    // Get the current disparity to depth mod
    auto isHwD2DMode = device->getBoolProperty(OB_PROP_DISPARITY_TO_DEPTH_BOOL);
    if(isHwD2DMode) { // if is hardware-d2D mode, get the supported depth precision level list from device
        std::cout << "Current Disparity to depth is hardware-d2D" << std::endl;

        // Get the depthPrecisionList
        if(!device->isPropertySupported(OB_STRUCT_DEPTH_PRECISION_SUPPORT_LIST, OB_PERMISSION_READ)) {
            pressKeyExit("Current device not support get supported depth precision level list!");
            return false;
        }

        auto depthPrecision = device->getStructuredDataExt(OB_STRUCT_DEPTH_PRECISION_SUPPORT_LIST);
        supportedPrecisionList.clear();
        for(int i = 0; i < depthPrecision->dataSize; i += 2) {
            auto precision = *((uint16_t *)((uint8_t *)depthPrecision->data + i));
            supportedPrecisionList.push_back((OBDepthPrecisionLevel)precision);
            std::cout << "precisionList[" << i << "]: " << depthUnitMap.find((OBDepthPrecisionLevel)precision)->second << std::endl;
        }
    }
    else { // if is software-d2D mode, support all depth precision level
        std::cout << "Current Disparity to depth is software-d2D" << std::endl;
    }

    std::cout << "The supported depth unit list is: " << std::endl;
    for(int i = 0; i < supportedPrecisionList.size(); i++) {
        std::cout << i << ": " << depthUnitMap.find(supportedPrecisionList[i])->second << std::endl;
    }

    // Let the user choose a depthPrecision, then switch
    std::cout << "Please input the index from above list, newIndex = ";
    uint32_t index = 0;
    std::cin >> index;
    if(index >= supportedPrecisionList.size()) {
       pressKeyExit("Invalid index input!");
       return false;
    }
    device->setIntProperty(OB_PROP_DEPTH_PRECISION_LEVEL_INT, index);

    std::cout << "Depth unit has been changed to " << depthUnitMap.find(supportedPrecisionList[index])->second << "mm" << std::endl;

    return true;
}

// For some models (G300 series), user can adjust the depth unit by adjusting the depth unit flexible adjustment property
bool changeDepthUnitFlexible(std::shared_ptr<ob::Device> &device) {
    auto currentValue = device->getFloatProperty(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT);
    auto valueRange   = device->getFloatPropertyRange(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT);

    std::cout << "Current depth unit is " << currentValue << "mm" << std::endl;
    std::cout << "The available depth unit range is from " << valueRange.min << " to " << valueRange.max << "mm" << std::endl;

    std::cout << "Please input a value to adjust the depth unit, value(in mm) = ";
    float unit = 0;
    std::cin >> unit;
    if(unit < valueRange.min || unit > valueRange.max) {
        pressKeyExit("Invalid input value!");
        return false;
    }

    device->setFloatProperty(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT, unit);
    std::cout << "Depth unit has been changed to " << unit << "mm" << std::endl;

    return true;
}

int main(int argc, char **argv) try {
    // Create a pipeline with default device
    ob::Pipeline pipe;

    // Get the device inside the pipeline
    auto device = pipe.getDevice();


    bool changeDepthUnitFlag = false;
    // check the way to adjust depth unit
    if(device->isPropertySupported(OB_PROP_DEPTH_PRECISION_LEVEL_INT, OB_PERMISSION_READ_WRITE)) {
        // change depth unit by adjust the depth precision level
        changeDepthUnitFlag = changeDepthUnit(device);
    }
    else if(device->isPropertySupported(OB_PROP_DEPTH_UNIT_FLEXIBLE_ADJUSTMENT_FLOAT, OB_PERMISSION_READ_WRITE)) {
        // for some models (Such as G300 series), user can adjust the depth unit by adjusting the depth unit flexible adjustment property
        changeDepthUnitFlag = changeDepthUnitFlexible(device);
    }
    else {
        std::cout << "Current device not support adjust depth unit, use the default depth unit 1mm" << std::endl;
        changeDepthUnitFlag = true;
    }

    if(!changeDepthUnitFlag){ // error in change depth unit
        return -1;
    }

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto profiles = pipe.getStreamProfileList(OB_SENSOR_DEPTH);

    std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
    try {
        // Find the corresponding profile according to the specified format, first look for the y16 format
        depthProfile = profiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_Y16, 15);
        // OB_HEIGHT_ANY
    }
    catch(ob::Error &e) {
        // If the specified format is not found, search for the default profile to open the stream
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(profiles->getProfile(OB_PROFILE_DEFAULT))->as<ob::VideoStreamProfile>();
    }

    // By creating config to configure which streams to enable or disable for the pipeline, here the depth stream will be enabled
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();
    config->enableStream(depthProfile);

    // Start the pipeline with config
    pipe.start(config);

    // Create a window for rendering, and set the resolution of the window
    Window app("DepthUnitControl", depthProfile->width(), depthProfile->height());

    while(app) {
        // Wait for up to 100ms for a frameset in blocking mode.
        auto frameSet = pipe.waitForFrames(100);
        if(frameSet == nullptr) {
            continue;
        }

        auto depthFrame = frameSet->depthFrame();

        // For Y16 format depth frame, print the distance of the center pixel every 30 frames
        if(depthFrame->index() % 30 == 0 && depthFrame->format() == OB_FORMAT_Y16) {
            // Convert depth values to 1mm precision
            auto rstImageData = convertDepthFrameUnitTo1mm(depthFrame);

            uint32_t  width          = depthFrame->width();
            uint32_t  height         = depthFrame->height();
            uint16_t  centerDistance = rstImageData.data()[width * height / 2 + width / 2];

            // Attention: if the distance is 0, it means that the depth camera cannot detect the object（may be out of detection range）
            std::cout << "The depth frame center value is " << centerDistance << " mm. " << std::endl;
        }

        // Render frame in the window
        app.addToRender(depthFrame);
    }

    // Stop the pipeline
    pipe.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
