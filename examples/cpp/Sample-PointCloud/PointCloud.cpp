#include "libobsensor/ObSensor.hpp"
// #include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>
#include "utils.hpp"
#include <cmath>
using namespace std;

#define KEY_ESC 27
#define KEY_R 82
#define KEY_r 114

// Save point cloud data to ply
void savePointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");

    if(!fp) {
        throw std::runtime_error("Failed to open file for writing");
    }

    OBPoint          *point            = (OBPoint *)frame->data();
    int               validPointsCount = 0;
    static const auto min_distance     = 1e-6;

    // First pass: Count valid points (non-zero points)
    for(int i = 0; i < pointsSize; i++) {
        if(fabs(point->x) >= min_distance || fabs(point->y) >= min_distance || fabs(point->z) >= min_distance) {
            validPointsCount++;
        }
        point++;
    }

    // Reset pointer to the start of the data
    point = (OBPoint *)frame->data();

    // Write PLY header
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", validPointsCount);  // Use valid points count
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "end_header\n");

    // Second pass: Write valid points to the file
    for(int i = 0; i < pointsSize; i++) {
        if(fabs(point->x) >= min_distance || fabs(point->y) >= min_distance || fabs(point->z) >= min_distance) {
            fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
        }
        point++;
    }

    fflush(fp);
    fclose(fp);
}

// Save colored point cloud data to ply
void saveRGBPointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");

    if(!fp) {
        throw std::runtime_error("Failed to open file for writing");
    }

    OBColorPoint     *point            = (OBColorPoint *)frame->data();
    int               validPointsCount = 0;
    static const auto min_distance     = 1e-6;
    // First pass: Count valid points (non-zero points)
    for(int i = 0; i < pointsSize; i++) {
        if(fabs(point->x) >= min_distance || fabs(point->y) >= min_distance || fabs(point->z) >= min_distance) {
            validPointsCount++;
        }
        point++;
    }

    // Reset pointer to the start of the data
    point = (OBColorPoint *)frame->data();

    // Write PLY header
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", validPointsCount);  // Use valid points count
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "property uchar red\n");
    fprintf(fp, "property uchar green\n");
    fprintf(fp, "property uchar blue\n");
    fprintf(fp, "end_header\n");

    // Second pass: Write valid points to the file
    for(int i = 0; i < pointsSize; i++) {
        if(fabs(point->x) >= min_distance || fabs(point->y) >= min_distance || fabs(point->z) >= min_distance) {
            fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
        }
        point++;
    }

    fflush(fp);
    fclose(fp);
}

int main(int argc, char **argv) try {
    ob::Context::setLoggerSeverity(OB_LOG_SEVERITY_WARN);
    // create pipeline
    ob::Pipeline pipeline;

    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            auto profile = colorProfiles->getProfile(OB_PROFILE_DEFAULT);
            colorProfile = profile->as<ob::VideoStreamProfile>();
        }
        config->enableStream(colorProfile);
    }
    catch(ob::Error &e) {
        config->setAlignMode(ALIGN_DISABLE);
        std::cerr << "Current device is not support color sensor!" << std::endl;
    }

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;
    OBAlignMode                            alignMode = ALIGN_DISABLE;
    if(colorProfile) {
        // Try find supported depth to color align hardware mode profile
        depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
        if(depthProfileList->count() > 0) {
            alignMode = ALIGN_D2C_HW_MODE;
        }
        else {
            // Try find supported depth to color align software mode profile
            depthProfileList = pipeline.getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
            if(depthProfileList->count() > 0) {
                alignMode = ALIGN_D2C_SW_MODE;
            }
        }

        try {
            // Enable frame synchronization
            pipeline.enableFrameSync();
        }
        catch(ob::Error &e) {
            std::cerr << "Current device is not support frame sync!" << std::endl;
        }
    }
    else {
        depthProfileList = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
    }

    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            // Select the profile with the same frame rate as color.
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(OB_WIDTH_ANY, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
            }
        }
        catch(...) {
            depthProfile = nullptr;
        }

        if(!depthProfile) {
            // If no matching profile is found, select the default profile.
            depthProfile = depthProfileList->getProfile(OB_PROFILE_DEFAULT);
        }
        config->enableStream(depthProfile);
    }
    config->setAlignMode(alignMode);

    // start pipeline with config
    pipeline.start(config);

    // Create a point cloud Filter object (the device parameters will be obtained inside the Pipeline when the point cloud filter is created, so try to
    // configure the device before creating the filter)
    ob::PointCloudFilter pointCloud;

    // get camera intrinsic and extrinsic parameters form pipeline and set to point cloud filter
    auto cameraParam = pipeline.getCameraParam();
    pointCloud.setCameraParam(cameraParam);

    // operation prompt
    std::cout << "Press R or r to create RGBD PointCloud and save to ply file! " << std::endl;
    std::cout << "Press D or d to create Depth PointCloud and save to ply file! " << std::endl;
    std::cout << "Press ESC to exit! " << std::endl;

    int count = 0;
    while(true) {
        auto frameset = pipeline.waitForFrames(100);
        if(kbhit()) {
            int key = getch();
            // Press the ESC key to exit
            if(key == KEY_ESC) {
                break;
            }
            if(key == 'R' || key == 'r') {
                count = 0;
                // Limit up to 10 repetitions
                while(count++ < 10) {
                    // Wait for a frame of data, the timeout is 100ms
                    auto frameset = pipeline.waitForFrames(100);
                    if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
                        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
                        // millimeter)
                        auto depthValueScale = frameset->depthFrame()->getValueScale();
                        pointCloud.setPositionDataScaled(depthValueScale);
                        try {
                            // Generate a colored point cloud and save it
                            std::cout << "Save RGBD PointCloud ply file..." << std::endl;
                            pointCloud.setCreatePointFormat(OB_FORMAT_RGB_POINT);
                            std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
                            saveRGBPointsToPly(frame, "RGBPoints.ply");
                            std::cout << "RGBPoints.ply Saved" << std::endl;
                        }
                        catch(std::exception &e) {
                            std::cout << "Get point cloud failed" << std::endl;
                        }
                        break;
                    }
                    else {
                        std::cout << "Get color frame or depth frame failed!" << std::endl;
                    }
                }
            }
            else if(key == 'D' || key == 'd') {
                count = 0;
                // Limit up to 10 repetitions
                while(count++ < 10) {
                    // Wait for up to 100ms for a frameset in blocking mode.
                    auto frameset = pipeline.waitForFrames(100);
                    if(frameset != nullptr && frameset->depthFrame() != nullptr) {
                        // point position value multiply depth value scale to convert uint to millimeter (for some devices, the default depth value uint is not
                        // millimeter)
                        auto depthValueScale = frameset->depthFrame()->getValueScale();
                        pointCloud.setPositionDataScaled(depthValueScale);
                        try {
                            // generate point cloud and save
                            std::cout << "Save Depth PointCloud to ply file..." << std::endl;
                            pointCloud.setCreatePointFormat(OB_FORMAT_POINT);
                            std::shared_ptr<ob::Frame> frame = pointCloud.process(frameset);
                            savePointsToPly(frame, "DepthPoints.ply");
                            std::cout << "DepthPoints.ply Saved" << std::endl;
                        }
                        catch(std::exception &e) {
                            std::cout << "Get point cloud failed" << std::endl;
                        }
                        break;
                    }
                }
            }
        }
    }
    // stop the pipeline
    pipeline.stop();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
