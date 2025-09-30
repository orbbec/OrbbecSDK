#include "libobsensor/ObSensor.hpp"
#include <fstream>
#include <iostream>
#include <cstring>
using namespace std;
#include <libobsensor/hpp/Utils.hpp>
using namespace ob;

const std::map<std::string, uint16_t> gemini_330_list = { { "gemini335", 0x0800 },  { "Gemini330", 0x0801 },   { "gemini336", 0x0803 },
                                                          { "gemini335L", 0x0804 }, { "Gemini 330L", 0x0805 }, { "gemini336L", 0x0807 },
                                                          { "Gemini335Lg", 0x080B } };

bool IsGemini330Series(uint16_t pid) {
    bool find = false;
    for(auto it = gemini_330_list.begin(); it != gemini_330_list.end(); ++it) {
        if(it->second == pid) {
            // std::cout << "find gemini 330 series : " << it->first << std::endl;
            find = true;
            break;
        }
    }
    return find;
}

void savePointsDataToPly(uint8_t *pointcloudData, uint32_t pointcloudSize, std::string fileName) {
    int   pointsSize = pointcloudSize / sizeof(OBPoint);
    FILE *fp         = fopen(fileName.c_str(), "wb+");
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "element vertex %d\n", pointsSize);
    fprintf(fp, "property float x\n");
    fprintf(fp, "property float y\n");
    fprintf(fp, "property float z\n");
    fprintf(fp, "end_header\n");

    OBPoint *point = (OBPoint *)pointcloudData;
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f\n", point->x, point->y, point->z);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

void saveRGBDPointsDataToPly(uint8_t *pointcloudData, uint32_t pointcloudSize, std::string fileName) {
    int   pointsSize = pointcloudSize / sizeof(OBColorPoint);
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

    OBColorPoint *point = (OBColorPoint *)pointcloudData;
    for(int i = 0; i < pointsSize; i++) {
        fprintf(fp, "%.3f %.3f %.3f %d %d %d\n", point->x, point->y, point->z, (int)point->r, (int)point->g, (int)point->b);
        point++;
    }

    fflush(fp);
    fclose(fp);
}

int depthPointCloudTransformation(std::shared_ptr<ob::Device> device, int case_number) {
    // create pipeline
    auto pipeline = std::shared_ptr<ob::Pipeline>(new ob::Pipeline(device));

    // Configure which streams to enable or disable for the Pipeline by creating a
    // Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Get all stream profiles of the depth camera, including stream resolution,
    // frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;
    OBAlignMode                            alignMode = ALIGN_DISABLE;

    depthProfileList  = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
    auto depthProfile = depthProfileList->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_Y16, OB_FPS_ANY);
    if(depthProfile) {
        config->enableStream(depthProfile);
    }

    config->setAlignMode(alignMode);

    // start pipeline with config
    pipeline->start(config);

    // Create a point cloud Filter object (the device parameters will be obtained
    // inside the Pipeline when the point cloud filter is created, so try to
    // configure the device before creating the filter)
    ob::PointCloudFilter pointCloud;

    // get camera intrinsic and extrinsic parameters form pipeline and set to
    // point cloud filter
    auto cameraParam = pipeline->getCameraParam();
    pointCloud.setCameraParam(cameraParam);

    auto     param          = pipeline->getCalibrationParam(config);
    auto     vsp            = depthProfile->as<ob::VideoStreamProfile>();
    uint32_t depthWidth     = vsp->width();
    uint32_t depthHeight    = vsp->height();
    uint32_t pointcloudSize = depthWidth * depthHeight * sizeof(OBPoint3f);
    uint8_t *pointcloudData = new uint8_t[pointcloudSize];

    uint32_t   tableSize = depthWidth * depthHeight * 2;
    float     *data      = new float[tableSize];
    OBXYTables xyTables;
    if(!ob::CoordinateTransformHelper::transformationInitXYTables(param, OB_SENSOR_DEPTH, data, &tableSize, &xyTables)) {
        return -1;
    }

    int count = 0;
    if(case_number == 1) {
        // Limit up to 10 repetitions
        while(count++ < 10) {
            // Wait for a frame of data, the timeout is 100ms
            auto frameset = pipeline->waitForFrames(100);
            if(frameset != nullptr && frameset->depthFrame() != nullptr) {
                memset(pointcloudData, 0, pointcloudSize);
                OBPoint  *pointPixel = (OBPoint *)pointcloudData;
                auto      depthFrame = frameset->depthFrame();
                uint32_t  height     = depthFrame->height();
                uint32_t  width      = depthFrame->width();
                uint16_t *pDepthData = (uint16_t *)depthFrame->data();

                for(uint32_t i = 0; i < height; i++) {
                    for(uint32_t j = 0; j < width; j++) {
                        OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
                        OBPoint3f targetPixel = {};
                        float     depthValue  = (float)pDepthData[i * width + j];

                        ob::CoordinateTransformHelper::calibration2dTo3dUndistortion(param, sourcePixel, depthValue, OB_SENSOR_DEPTH, OB_SENSOR_DEPTH,
                                                                                     &targetPixel);

                        (*pointPixel).z = targetPixel.z;
                        (*pointPixel).x = targetPixel.x;
                        (*pointPixel).y = targetPixel.y;
                        pointPixel++;
                    }
                }

                savePointsDataToPly((uint8_t *)pointcloudData, pointcloudSize, "DepthPointsUndistortion.ply");
                std::cout << "DepthPointsUndistortion.ply Saved" << std::endl;
                break;
            }
            else {
                std::cout << "Get depth frame or depth frame failed!" << std::endl;
            }
        }
    }
    else if(case_number == 2) {
        // Limit up to 10 repetitions
        while(count++ < 10) {
            // Wait for up to 100ms for a frameset in blocking mode.
            auto frameset = pipeline->waitForFrames(100);
            if(frameset != nullptr && frameset->depthFrame() != nullptr) {
                memset(pointcloudData, 0, pointcloudSize);
                OBPoint *pointPixel = (OBPoint *)pointcloudData;

                auto      depthFrame = frameset->depthFrame();
                uint32_t  height     = depthFrame->height();
                uint32_t  width      = depthFrame->width();
                uint16_t *pDepthData = (uint16_t *)depthFrame->data();

                for(uint32_t i = 0; i < height; i++) {
                    for(uint32_t j = 0; j < width; j++) {
                        OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
                        OBPoint3f targetPixel = {};
                        float     depthValue  = (float)pDepthData[i * width + j];

                        ob::CoordinateTransformHelper::calibration2dTo3d(param, sourcePixel, depthValue, OB_SENSOR_DEPTH, OB_SENSOR_DEPTH, &targetPixel);

                        (*pointPixel).z = targetPixel.z;
                        (*pointPixel).x = targetPixel.x;
                        (*pointPixel).y = targetPixel.y;
                        pointPixel++;
                    }
                }

                savePointsDataToPly((uint8_t *)pointcloudData, pointcloudSize, "DepthPointsWithDistortion.ply");
                std::cout << "DepthPointsWithDistortion.ply Saved" << std::endl;
                break;
            }
        }
    }
    else if(case_number == 3) {
        while(count++ < 10) {
            // Wait for up to 100ms for a frameset in blocking mode.
            auto frameset = pipeline->waitForFrames(100);
            if(frameset != nullptr && frameset->depthFrame() != nullptr) {
                memset(pointcloudData, 0, pointcloudSize);
                OBPoint *pointPixel = (OBPoint *)pointcloudData;
                auto     depthFrame = frameset->depthFrame();

                ob::CoordinateTransformHelper::transformationDepthToPointCloud(&xyTables, depthFrame->data(), pointPixel);
                savePointsDataToPly((uint8_t *)pointcloudData, pointcloudSize, "DepthPointsWithTables.ply");
                std::cout << "DepthPointsWithTables.ply Saved" << std::endl;
                break;
            }
        }
    }
    // stop the pipeline
    pipeline->stop();

    if(data) {
        delete[] data;
        data = nullptr;
    }

    if(pointcloudData) {
        delete[] pointcloudData;
        pointcloudData = nullptr;
    }
    return 0;
}

int RGBPointCloudTransformation(std::shared_ptr<ob::Device> device) {

    // create pipeline
    auto pipeline = std::shared_ptr<ob::Pipeline>(new ob::Pipeline(device));
    // Configure which streams to enable or disable for the Pipeline by creating a
    // Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // Turn on D2C alignment, which needs to be turned on when generating RGBD
    // point clouds

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution,
        // frame rate, and frame format
        auto colorProfiles = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
        colorProfile       = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, OB_FPS_ANY);
        if(colorProfile) {
            config->enableStream(colorProfile);
        }
    }
    catch(ob::Error &e) {
        config->setAlignMode(ALIGN_DISABLE);
        std::cerr << "Current device is not support color sensor!" << std::endl;
        return -1;
    }

    // Get all stream profiles of the depth camera, including stream resolution,
    // frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;
    OBAlignMode                            alignMode = ALIGN_DISABLE;
    if(colorProfile) {
        // Try find supported depth to color align hardware mode profile
        depthProfileList = pipeline->getD2CDepthProfileList(colorProfile, ALIGN_D2C_HW_MODE);
        if(depthProfileList->count() > 0) {
            alignMode = ALIGN_D2C_HW_MODE;
        }
        else {
            // Try find supported depth to color align software mode profile
            depthProfileList = pipeline->getD2CDepthProfileList(colorProfile, ALIGN_D2C_SW_MODE);
            if(depthProfileList->count() > 0) {
                alignMode = ALIGN_D2C_SW_MODE;
            }
        }
    }
    else {
        depthProfileList = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);
    }

    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            // Select the profile with the same frame rate as color.
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
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
    pipeline->start(config);

    // Create a point cloud Filter object (the device parameters will be obtained
    // inside the Pipeline when the point cloud filter is created, so try to
    // configure the device before creating the filter)
    ob::PointCloudFilter pointCloud;

    // get camera intrinsic and extrinsic parameters form pipeline and set to
    // point cloud filter
    auto cameraParam = pipeline->getCameraParam();
    pointCloud.setCameraParam(cameraParam);

    //
    auto     param       = pipeline->getCalibrationParam(config);
    uint32_t colorWidth  = colorProfile->width();
    uint32_t colorHeight = colorProfile->height();
    uint32_t tableSize   = colorWidth * colorHeight * 2;
    float   *data        = new float[tableSize];

    uint32_t pointcloudSize = colorWidth * colorHeight * sizeof(OBColorPoint);
    uint8_t *pointcloudData = new uint8_t[pointcloudSize];

    OBXYTables xyTables;
    if(!ob::CoordinateTransformHelper::transformationInitXYTables(param, OB_SENSOR_COLOR, data, &tableSize, &xyTables)) {
        return -1;
    }

    bool savePoint = false;
    int  count     = 0;
    // Limit up to 10 repetitions
    while(count++ < 100) {
        // Wait for a frame of data, the timeout is 100ms
        auto frameset = pipeline->waitForFrames(100);
        if(frameset != nullptr && frameset->depthFrame() != nullptr && frameset->colorFrame() != nullptr) {
            memset(pointcloudData, 0, pointcloudSize);
            OBColorPoint *pointPixel = (OBColorPoint *)pointcloudData;
            auto          depthFrame = frameset->depthFrame();
            auto          colorFrame = frameset->colorFrame();

            uint32_t depthWidth = depthFrame->width();
            uint32_t colorWidth = colorFrame->width();

            ob::CoordinateTransformHelper::transformationDepthToRGBDPointCloud(&xyTables, depthFrame->data(), colorFrame->data(), pointPixel);

            saveRGBDPointsDataToPly((uint8_t *)pointcloudData, pointcloudSize, "RGBDDepthPointsWithTables.ply");
            std::cout << "RGBDDepthPointsWithTables.ply Saved" << std::endl;
            savePoint = true;
            break;
        }
    }

    if(!savePoint) {
        std::cout << "Failed to save RGBDDepthPointsWithTables.ply, color frame or depth frame " << std::endl;
    }

    // stop the pipeline
    pipeline->stop();

    if(data) {
        delete[] data;
        data = nullptr;
    }

    if(pointcloudData) {
        delete[] pointcloudData;
        pointcloudData = nullptr;
    }

    return 0;
}

int depth2DToColor2DTransformation(std::shared_ptr<ob::Device> device) {
    // create pipeline
    auto pipeline = std::shared_ptr<ob::Pipeline>(new ob::Pipeline(device));
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution,frame rate, and frame format
        auto colorProfiles = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
        colorProfile       = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, OB_FPS_ANY);
        if(colorProfile) {
            config->enableStream(colorProfile);
        }
    }
    catch(ob::Error &e) {
        std::cerr << "Current device is not support color sensor!" << std::endl;
        return -1;
    }

    // Get all stream profiles of the depth camera, including stream resolution,frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;

    depthProfileList = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);

    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            // Select the profile with the same frame rate as color.
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
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

    OBAlignMode alignMode = ALIGN_DISABLE;
    config->setAlignMode(alignMode);

    // start pipeline with config
    pipeline->start(config);

    auto param = pipeline->getCalibrationParam(config);
    // uint32_t colorWidth  = colorProfile->width();
    // uint32_t colorHeight = colorProfile->height();

    int count = 0;
    // Limit up to 10 repetitions
    while(count++ < 20) {
        // Wait for a frame of data, the timeout is 100ms
        auto frameset = pipeline->waitForFrames(100);
        if(frameset == nullptr) {
            continue;
        }

        auto colorFrame = frameset->colorFrame();
        auto depthFrame = frameset->depthFrame();
        if(depthFrame != nullptr && colorFrame != nullptr) {

            auto      transformColorFrame = ob::FrameHelper::createFrame(OB_FRAME_DEPTH, depthFrame->format(), colorFrame->width(), colorFrame->height(), 0);
            uint16_t *transData           = (uint16_t *)transformColorFrame->data();
            memset(transData, 0, colorFrame->width() * colorFrame->height() * 2);
            // The purpose of converting each point of Depth into the coordinate system of Color is to demonstrate how Depth coordinate points are transformed
            // into Color coordinate points. Due to the coordinate transformation, no hole filling has been performed. Consequently, the converted Depth image
            // may contain hole.
            for(int i = 0; i < depthFrame->height(); i++) {
                for(int j = 0; j < depthFrame->width(); j++) {
                    OBPoint2f sourcePixel = { static_cast<float>(j), static_cast<float>(i) };
                    OBPoint2f targetPixel = {};
                    float     depthValue  = *(((uint16_t *)depthFrame->data()) + (i * depthFrame->width()) + j);

                    // Demonstrate Depth 2D converted to Color 2D
                    ob::CoordinateTransformHelper::calibration2dTo2d(param, sourcePixel, depthValue, OB_SENSOR_DEPTH, OB_SENSOR_COLOR, &targetPixel);
                    if(targetPixel.y < 0 || targetPixel.x < 0 || targetPixel.y > colorFrame->height() || targetPixel.x > colorFrame->width()) {
                        continue;
                    }

                    auto index = (((uint32_t)targetPixel.y * colorFrame->width()) + (uint32_t)targetPixel.x);
                    if(index > colorFrame->width() * colorFrame->height()) {
                        continue;
                    }

                    transData[index] = depthValue;
                }
            }

            break;
        }
    }

    // stop the pipeline
    pipeline->stop();

    return 0;
}

int color2DToDepth2DTransformation(std::shared_ptr<ob::Device> device) {
    // create pipeline
    auto pipeline = std::shared_ptr<ob::Pipeline>(new ob::Pipeline(device));
    // Configure which streams to enable or disable for the Pipeline by creating a Config
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    try {
        // Get all stream profiles of the color camera, including stream resolution,frame rate, and frame format
        auto colorProfiles = pipeline->getStreamProfileList(OB_SENSOR_COLOR);
        colorProfile       = colorProfiles->getVideoStreamProfile(1280, OB_HEIGHT_ANY, OB_FORMAT_RGB, OB_FPS_ANY);
        if(colorProfile) {
            config->enableStream(colorProfile);
        }
    }
    catch(ob::Error &e) {
        std::cerr << "Current device is not support color sensor!" << std::endl;
        return -1;
    }

    // Get all stream profiles of the depth camera, including stream resolution,frame rate, and frame format
    std::shared_ptr<ob::StreamProfileList> depthProfileList;

    depthProfileList = pipeline->getStreamProfileList(OB_SENSOR_DEPTH);

    if(depthProfileList->count() > 0) {
        std::shared_ptr<ob::StreamProfile> depthProfile;
        try {
            // Select the profile with the same frame rate as color.
            if(colorProfile) {
                depthProfile = depthProfileList->getVideoStreamProfile(640, OB_HEIGHT_ANY, OB_FORMAT_ANY, colorProfile->fps());
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

    OBAlignMode alignMode = ALIGN_DISABLE;
    config->setAlignMode(alignMode);

    // start pipeline with config
    pipeline->start(config);

    auto         param           = pipeline->getCalibrationParam(config);
    OBStreamType align_to_stream = OB_STREAM_COLOR;
    ob::Align    align(align_to_stream);
    int          count = 0;
    // Limit up to 10 repetitions
    while(count++ < 20) {
        // Wait for a frame of data, the timeout is 100ms
        auto frameset = pipeline->waitForFrames(100);
        if(frameset == nullptr) {
            continue;
        }

        auto colorFrame = frameset->colorFrame();
        auto depthFrame = frameset->depthFrame();
        if(depthFrame != nullptr && colorFrame != nullptr) {

            uint32_t                        colorWidth    = colorFrame->width();
            uint32_t                        colorHeight   = colorFrame->height();
            auto                            deviceInfo    = device->getDeviceInfo();
            auto                            pid           = deviceInfo->pid();
            uint16_t                       *depthAfterD2C = nullptr;
            float                           valueScale    = 1;
            std::shared_ptr<ob::DepthFrame> depthD2CFrame = nullptr;
            // Because of the conversion from Color 2D to Depth 2D, the Depth values of Color points are required, so D2C conversion is needed
            if(IsGemini330Series(pid)) {
                auto newFrame = align.process(frameset);
                if(newFrame == nullptr) {
                    continue;
                }
                auto newFrameSet = newFrame->as<ob::FrameSet>();
                depthD2CFrame    = newFrameSet->depthFrame();
            }
            else {
                auto frame = ob::CoordinateTransformHelper::transformationDepthFrameToColorCamera(device, depthFrame, colorWidth, colorHeight);
                if(frame == nullptr) {
                    continue;
                }
                depthD2CFrame = frame->as<ob::DepthFrame>();
            }

            if(depthD2CFrame == nullptr) {
                continue;
            }

            valueScale    = depthD2CFrame->as<ob::DepthFrame>()->getValueScale();
            depthAfterD2C = (uint16_t *)depthD2CFrame->data();

            // Convert the coordinates of the center point of Color to Depth coordinates
            OBPoint2f sourcePoint2f;
            sourcePoint2f.x = colorWidth / 2;
            sourcePoint2f.y = colorHeight / 2;

            uint16_t centerDepth = depthAfterD2C[(colorHeight / 2) * colorWidth + colorWidth / 2] * valueScale;
            if(centerDepth == 0) {
                // The Color points must have corresponding Depth values in order to be converted
                continue;
            }

            OBPoint2f targetPoint2f;
            bool bRet = ob::CoordinateTransformHelper::calibration2dTo2d(param, sourcePoint2f, centerDepth, OB_SENSOR_COLOR, OB_SENSOR_DEPTH, &targetPoint2f);
            if(bRet) {
                break;
            }
        }
    }

    // stop the pipeline
    pipeline->stop();

    return 0;
}

int main(int argc, char **argv) try {

    int case_number = 3;

    ob::Context ctx;
    ctx.setLoggerSeverity(OB_LOG_SEVERITY_WARN);
    std::shared_ptr<DeviceList> devices = ctx.queryDeviceList();
    auto                        device  = devices->getDevice(0);

    if(device == nullptr) {
        std::cout << "device is null" << std::endl;
        return -1;
    }

    // Invoking the utils class to generate an undistorted depth point cloud.
    depthPointCloudTransformation(device, case_number);

    // Invoking the utils class to generate an undistorted RGBD point cloud.
    RGBPointCloudTransformation(device);

    // Demonstrate Depth 2D converted to Color 2D
    depth2DToColor2DTransformation(device);

    // Demonstrate Color 2D converted to  Depth 2D
    color2DToDepth2DTransformation(device);
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getName() << "\nargs:" << e.getArgs() << "\nmessage:" << e.getMessage() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}
