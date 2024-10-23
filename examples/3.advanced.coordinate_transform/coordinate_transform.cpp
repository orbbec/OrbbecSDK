// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>
#include "libobsensor/hpp/Utils.hpp"
#include "libobsensor/hpp/Frame.hpp"

void printUsage();
std::string inputWatcher() ;
void transformation2dto2d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame);
void transformation2dto3d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame);
void transformation3dto2d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame);
void transformation3dto3d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame);

int main(void) try {
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    auto config = std::make_shared<ob::Config>();

    // enable depth and color streams with specified format
    config->enableVideoStream(OB_STREAM_DEPTH, OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FPS_ANY, OB_FORMAT_Y16);
    config->enableVideoStream(OB_STREAM_COLOR, OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FPS_ANY, OB_FORMAT_RGB);

    // set the frame aggregate output mode to ensure all types of frames are included in the output frameset
    config->setFrameAggregateOutputMode(OB_FRAME_AGGREGATE_OUTPUT_ALL_TYPE_FRAME_REQUIRE);

    // Create a pipeline with default device to manage stream
    auto pipe = std::make_shared<ob::Pipeline>();

    std::string testType = "1";   

    // Start the pipeline with config
    pipe->start(config);
    while(1) {
        printUsage();
        testType = inputWatcher();

        // Wait for a frameset from the pipeline
        auto frameSet = pipe->waitForFrameset(100);  
        if(frameSet == nullptr) {
            continue;
        }        

        // Get the color frame and check its validity
        auto colorFrame = frameSet->getFrame(OB_FRAME_COLOR);
        
        // Get the depth frame and check its validity
        auto depthFrame = frameSet->getFrame(OB_FRAME_DEPTH);
        
        if(testType == "1") {
            transformation2dto2d(colorFrame, depthFrame);
        } else if (testType == "2") {
            transformation2dto3d(colorFrame, depthFrame);
        } else if (testType == "3") {
            transformation3dto3d(colorFrame, depthFrame);
        } else if (testType == "4") {
            transformation3dto2d(colorFrame, depthFrame);
        } else {
            std::cout << "Invalid command" << std::endl;
        }  
     }
     
    pipe->stop();
    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

std::string inputWatcher() {
    while(true) {
        std::string cmd;
        std::cout << "\nInput command:  ";
        std::getline(std::cin, cmd);
        if(cmd == "quit" || cmd == "q") {
            exit(EXIT_SUCCESS);
        }
        return cmd;
    }
}
void printUsage() {
    std::cout << "Support commands:" << std::endl;
    std::cout << "    1 - transformation 2d to 2d" << std::endl;
    std::cout << "    2 - transformation 2d to 3d" << std::endl;
    std::cout << "    3 - transformation 3d to 3d" << std::endl;
    std::cout << "    4 - transformation 3d to 2d" << std::endl;

    std::cout << "--------------------------------" << std::endl;
    std::cout << "    quit / q- quit application" << std::endl;
}

void printRuslt(std::string msg, OBPoint2f sourcePixel, OBPoint2f targetPixel) {
    std::cout << msg << ":" << "(" << sourcePixel.x << ", " << sourcePixel.y << ") -> (" << targetPixel.x << ", " << targetPixel.y << ")" << std::endl;
}

void printRuslt(std::string msg, OBPoint2f sourcePixel, OBPoint3f targetPixel, float depthValue) {
    std::cout << msg << ":" << "depth " << depthValue <<  " (" << sourcePixel.x << ", " << sourcePixel.y << ") -> (" << targetPixel.x << ", " << targetPixel.y << ", " << targetPixel.z << ")" << std::endl;
}

void printRuslt(std::string msg, OBPoint3f sourcePixel, OBPoint2f targetPixel) {
    std::cout << msg << ":" << "(" << sourcePixel.x << ", " << sourcePixel.y << ", " << sourcePixel.z << ") -> (" << targetPixel.x << ", " << targetPixel.y << ")" << std::endl;
}

void printRuslt(std::string msg, OBPoint3f sourcePixel, OBPoint3f targetPixel) {
    std::cout << msg << ":" << "(" << sourcePixel.x << ", " << sourcePixel.y << ", " << sourcePixel.z << ") -> (" << targetPixel.x << ", " << targetPixel.y << ", " << targetPixel.z << ")" << std::endl;
}

// test the transformation from one 2D coordinate system to another
void transformation2dto2d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame) {
    // Get the width and height of the color and depth frames
    auto colorFrameWidth = colorFrame->as<ob::VideoFrame>()->getWidth();
    auto depthFrameWidth = depthFrame->as<ob::VideoFrame>()->getWidth();    
    auto colorFrameHeight = colorFrame->as<ob::VideoFrame>()->getHeight(); 
    auto depthFrameHeight = depthFrame->as<ob::VideoFrame>()->getHeight();

    // Get the stream profiles for the color and depth frames
    auto colorProfile =  colorFrame->getStreamProfile();
    auto depthProfile = depthFrame->getStreamProfile();
    auto extrinsicD2C = depthProfile->getExtrinsicTo(colorProfile);

    // Get the intrinsic and distortion parameters for the color and depth streams
    auto colorIntrinsic = colorProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
    auto colorDistortion = colorProfile->as<ob::VideoStreamProfile>()->getDistortion();
    auto depthIntrinsic = depthProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
    auto depthDistortion = depthProfile->as<ob::VideoStreamProfile>()->getDistortion();
     // Access the depth data from the frame
    uint16_t *pDepthData = (uint16_t *)depthFrame->getData();
    uint16_t convertAreaWidth = 3;
    uint16_t convertAreaHeight = 3;

    // Transform depth values to the color frame's coordinate system
    for(uint32_t i = depthFrameHeight / 2; i < (depthFrameHeight / 2 + convertAreaHeight); i++) {
        for(uint32_t j = depthFrameWidth / 2; j < (depthFrameWidth / 2 + convertAreaWidth); j++) {
            OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
            OBPoint2f targetPixel = {};
            float     depthValue  = (float)pDepthData[i * depthFrameWidth + j];
            if(depthValue == 0) {
                std::cout << "The depth value is 0, so it's recommended to point the camera at a flat surface" << std::endl;
                continue;
            }
            
            // Demonstrate Depth 2D converted to Color 2D
            bool result = ob::CoordinateTransformHelper::transformation2dto2d(sourcePixel, depthValue, depthIntrinsic, depthDistortion, colorIntrinsic,
                                                                              colorDistortion, extrinsicD2C, &targetPixel);
            
            // Check transformation result and whether the target pixel is within the color frame
            if(!result || targetPixel.y < 0 || targetPixel.x < 0 || targetPixel.y > colorFrameWidth || targetPixel.x > colorFrameWidth) {
                continue;
            }

            // Calculate the index position of the target pixel in the transformation data buffer
            auto index = (((uint32_t)targetPixel.y * colorFrameWidth) + (uint32_t)targetPixel.x);
            if(index > colorFrameWidth * colorFrameHeight) {
                continue;
            }

            printRuslt("depth to color: depth image coordinate transform to color image coordinate", sourcePixel, targetPixel);            
        }
    }
}

// test the transformation from 2D to 3D coordinates
void transformation2dto3d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame) {
     // Get the width and height of the color and depth frames
    auto depthFrameWidth = depthFrame->as<ob::VideoFrame>()->getWidth();    
    auto depthFrameHeight = depthFrame->as<ob::VideoFrame>()->getHeight();

    // Get the stream profiles for the color and depth frames
    auto colorProfile =  colorFrame->getStreamProfile();
    auto depthProfile = depthFrame->getStreamProfile();
    auto extrinsicD2C = depthProfile->getExtrinsicTo(colorProfile);

    // Get the intrinsic and distortion parameters for the color and depth streams
    auto depthIntrinsic = depthProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
    // Access the depth data from the frame
    uint16_t *pDepthData = (uint16_t *)depthFrame->getData();
    uint16_t convertAreaWidth = 3;
    uint16_t convertAreaHeight = 3;
    
    // Transform depth values to the color frame's coordinate system
    for(uint32_t i = depthFrameHeight / 2; i < (depthFrameHeight / 2 + convertAreaHeight); i++) {
        for(uint32_t j = depthFrameWidth / 2; j < (depthFrameWidth / 2 + convertAreaWidth); j++) {
            // Get the coordinates of the current pixel
            OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
            OBPoint3f targetPixel = {};
            // Get the depth value of the current pixel
            float     depthValue  = (float)pDepthData[i * depthFrameWidth + j];
            if(depthValue == 0) {
                std::cout << "The depth value is 0, so it's recommended to point the camera at a flat surface" << std::endl;
                continue;
            }

            // Perform the 2D to 3D transformation
            bool result = ob::CoordinateTransformHelper::transformation2dto3d(sourcePixel, depthValue, depthIntrinsic,  extrinsicD2C, &targetPixel);
            if(!result ) {
                continue;
            }
            
            printRuslt("2d to 3D: pixel coordinates and depth transform to point in 3D space", sourcePixel, targetPixel, depthValue);
        }
    }
}

// test the transformation from 3D coordinates to 3D coordinates
void transformation3dto3d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame) {
     // Get the width and height of the color and depth frames
    auto depthFrameWidth = depthFrame->as<ob::VideoFrame>()->getWidth();   
    auto depthFrameHeight = depthFrame->as<ob::VideoFrame>()->getHeight();

    // Get the stream profiles for the color and depth frames
    auto colorProfile =  colorFrame->getStreamProfile();
    auto depthProfile = depthFrame->getStreamProfile();
    auto extrinsicD2C = depthProfile->getExtrinsicTo(colorProfile);
    auto extrinsicC2D = colorProfile->getExtrinsicTo(depthProfile);

    // Get the intrinsic and distortion parameters for the color and depth streams
    auto depthIntrinsic = depthProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
    // Access the depth data from the frame
    uint16_t *pDepthData = (uint16_t *)depthFrame->getData();
    uint16_t convertAreaWidth = 3;
    uint16_t convertAreaHeight = 3;
    
    // Transform depth values to the color frame's coordinate system
    for(uint32_t i = depthFrameHeight / 2; i < (depthFrameHeight / 2 + convertAreaHeight); i++) {
        for(uint32_t j = depthFrameWidth / 2; j < (depthFrameWidth / 2 + convertAreaWidth); j++) {
            // Get the coordinates of the current pixel
            OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
            OBPoint3f tmpTargetPixel = {};
            OBPoint3f targetPixel = {};
            // Get the depth value of the current pixel
            float     depthValue  = (float)pDepthData[i * depthFrameWidth + j];
            if(depthValue == 0) {
                std::cout << "The depth value is 0, so it's recommended to point the camera at a flat surface" << std::endl;
                continue;
            }
            
            // Perform the 2D to 3D transformation
            bool result = ob::CoordinateTransformHelper::transformation2dto3d(sourcePixel, depthValue, depthIntrinsic, extrinsicD2C, &tmpTargetPixel);
            if(!result ) {
                continue;
            }
            printRuslt("2d to 3D: pixel coordinates and depth transform to point in 3D space", sourcePixel, tmpTargetPixel, depthValue);
            
            // Perform the 3D to 3D transformation
            result = ob::CoordinateTransformHelper::transformation3dto3d(tmpTargetPixel, extrinsicC2D, &targetPixel);
            if(!result ) {
                continue;
            }
            printRuslt("3d to 3D: transform 3D coordinates relative to one sensor to 3D coordinates relative to another viewpoint", tmpTargetPixel, targetPixel);
        }
    }
}

// test the transformation from 3D coordinates back to 2D coordinates
void transformation3dto2d(std::shared_ptr<ob::Frame> colorFrame, std::shared_ptr<ob::Frame> depthFrame) {
     // Get the width and height of the color and depth frames
    auto depthFrameWidth = depthFrame->as<ob::VideoFrame>()->getWidth();   
    auto depthFrameHeight = depthFrame->as<ob::VideoFrame>()->getHeight();

    // Get the stream profiles for the color and depth frames
    auto colorProfile =  colorFrame->getStreamProfile();
    auto depthProfile = depthFrame->getStreamProfile();
    auto extrinsicD2C = depthProfile->getExtrinsicTo(colorProfile);
    auto extrinsicC2D = colorProfile->getExtrinsicTo(depthProfile);

    // Get the intrinsic and distortion parameters for the color and depth streams
    auto depthIntrinsic = depthProfile->as<ob::VideoStreamProfile>()->getIntrinsic();
    auto depthDistortion = depthProfile->as<ob::VideoStreamProfile>()->getDistortion();
    // Access the depth data from the frame
    uint16_t *pDepthData = (uint16_t *)depthFrame->getData();
    uint16_t convertAreaWidth = 3;
    uint16_t convertAreaHeight = 3;
    
    // Transform depth values to the color frame's coordinate system
    for(uint32_t i = depthFrameHeight / 2; i < (depthFrameHeight / 2 + convertAreaHeight); i++) {
        for(uint32_t j = depthFrameWidth / 2; j < (depthFrameWidth / 2 + convertAreaWidth); j++) {
            // Get the coordinates of the current pixel
            OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
            OBPoint3f tmpTargetPixel = {};
            OBPoint2f targetPixel = {};
            // Get the depth value of the current pixel
            float     depthValue  = (float)pDepthData[i * depthFrameWidth + j];
            if(depthValue == 0) {
                std::cout << "The depth value is 0, so it's recommended to point the camera at a flat surface" << std::endl;
                continue;
            }
            
            // Perform the 2D to 3D transformation
            bool result = ob::CoordinateTransformHelper::transformation2dto3d(sourcePixel, depthValue, depthIntrinsic, 
                                            extrinsicD2C, &tmpTargetPixel);
            if(!result ) {
                continue;
            }
            printRuslt("depth 2d to 3D: pixel coordinates and depth transform to point in 3D space", sourcePixel, tmpTargetPixel, depthValue);
            
            // Perform the 3D to 2D transformation
            result = ob::CoordinateTransformHelper::transformation3dto2d(tmpTargetPixel, depthIntrinsic, depthDistortion, extrinsicC2D, &targetPixel);
            if(!result ) {
                continue;
            }
            printRuslt("3d to depth 2d : point in 3D space transform to the corresponding pixel coordinates in an image", tmpTargetPixel, targetPixel);
        }
    }
}
