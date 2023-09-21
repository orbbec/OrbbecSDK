#include "libobsensor/ObSensor.hpp"
// #include "opencv2/opencv.hpp"
#include <fstream>
#include <iostream>
#include "utils.hpp"

#define KEY_ESC 27
#define KEY_R 82
#define KEY_r 114

// Save point cloud data to ply
void savePointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "end_header\n");

    OBPoint *point = (OBPoint *)frame->data();
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

// Save colored point cloud data to ply
void saveRGBPointsToPly(std::shared_ptr<ob::Frame> frame, std::string fileName) {
    int   pointsSize = frame->dataSize() / sizeof(OBColorPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "property uchar red\n");
    fprintf(fp, "property uchar green\n");
    fprintf(fp, "property uchar blue\n");
    fprintf(fp, "end_header\n");

    OBColorPoint *point = (OBColorPoint *)frame->data();
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
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

    // Get all stream profiles of the depth camera, including stream resolution, frame rate, and frame format
    auto                                    depthProfiles = pipeline.getStreamProfileList(OB_SENSOR_DEPTH);
    std::shared_ptr<ob::VideoStreamProfile> depthProfile  = nullptr;
    if(depthProfiles) {
        // Open the default profile of Depth Sensor, which can be configured through the configuration file
        depthProfile = std::const_pointer_cast<ob::StreamProfile>(depthProfiles->getProfile(0))->as<ob::VideoStreamProfile>();
    }
    config->enableStream(depthProfile);

    // Turn on D2C alignment, which needs to be turned on when generating RGBD point clouds
    config->setAlignMode(ALIGN_D2C_HW_MODE);

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution, frame rate, and frame format
        auto colorProfiles = pipeline.getStreamProfileList(OB_SENSOR_COLOR);
        if(colorProfiles) {
            colorProfile = std::const_pointer_cast<ob::StreamProfile>(colorProfiles->getProfile(0))->as<ob::VideoStreamProfile>();
        }
        config->enableStream(colorProfile);
    }
    catch(ob::Error &e) {
        config->setAlignMode(ALIGN_DISABLE);
        std::cerr << "Current device is not support color sensor!" << std::endl;
    }

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
