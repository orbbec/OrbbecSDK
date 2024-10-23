// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#pragma once
#include <libobsensor/ObSensor.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cmath>
#include <map>

#include "utils_types.h"
#include "utils.hpp"

namespace ob_smpl {

// arrange type
typedef enum {
    ARRANGE_SINGLE,      // Only show the first frame
    ARRANGE_ONE_ROW,     // Arrange the frames in the array as a row
    ARRANGE_ONE_COLUMN,  // Arrange the frames in the array as a column
    ARRANGE_GRID,        // Arrange the frames in the array as a grid
    ARRANGE_OVERLAY      // Overlay the first two frames in the array
} ArrangeMode;

class CVWindow {
public:
    // create a window with the specified name, width and height
    CVWindow(std::string name, uint32_t width = 1280, uint32_t height = 720, ArrangeMode arrangeMode = ARRANGE_SINGLE);
    ~CVWindow() noexcept;

    // run the window loop
    bool run();

    // close window
    void close();

    // clear cached frames and mats
    void reset();

    // add frames to view
    void pushFramesToView(std::vector<std::shared_ptr<const ob::Frame>> frames, int groupId = 0);
    void pushFramesToView(std::shared_ptr<const ob::Frame> currentFrame, int groupId = 0);

    // set show frame info
    void setShowInfo(bool show);

    // set show frame syncTime info
    void setShowSyncTimeInfo(bool show);

    // set alpha, only valid when arrangeMode_ is ARRANGE_OVERLAY
    void setAlpha(float alpha);

    // set the window size
    void resize(int width, int height);

    // set the key pressed callback
    void setKeyPressedCallback(std::function<void(int)> callback);

    // set the key prompt
    void setKeyPrompt(const std::string &prompt);

    // set the log message
    void addLog(const std::string &log);

    // destroyWindow
    void destroyWindow();

private:
    // frames processing thread function
    void processFrames();

    // arrange frames in the renderMat_ according to the arrangeMode_
    void arrangeFrames();

    // add info to mat
    cv::Mat visualize(std::shared_ptr<const ob::Frame> frame);

    // draw info to mat
    void drawInfo(cv::Mat &imageMat, std::shared_ptr<const ob::VideoFrame> &frame);

    cv::Mat resizeMatKeepAspectRatio(const cv::Mat &mat, int width, int height);

private:
    std::string name_;
    ArrangeMode arrangeMode_;
    uint32_t    width_;
    uint32_t    height_;
    bool        closed_;
    bool        showInfo_;
    bool        showSyncTimeInfo_;
    bool        isWindowDestroyed_;
    float       alpha_;

    std::thread                                                  processThread_;
    std::map<int, std::vector<std::shared_ptr<const ob::Frame>>> srcFrameGroups_;
    std::mutex                                                   srcFrameGroupsMtx_;
    std::condition_variable                                      srcFrameGroupsCv_;

    using StreamsMatMap = std::map<int, std::pair<std::shared_ptr<const ob::Frame>, cv::Mat>>;
    StreamsMatMap matGroups_;
    std::mutex    renderMatsMtx_;
    cv::Mat       renderMat_;

    std::string prompt_;
    bool        showPrompt_;
    uint64      winCreatedTime_;

    std::string log_;
    uint64      logCreatedTime_;

    std::function<void(int)> keyPressedCallback_;
};

}  // namespace ob_smpl
