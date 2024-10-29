// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include "utils_opencv.hpp"
#include "utils.hpp"
#include "utils_types.h"

#if defined(__has_include)
#if __has_include(<opencv2/core/utils/logger.hpp>)
#include <opencv2/core/utils/logger.hpp>
#define TO_DISABLE_OPENCV_LOG
#endif
#endif

namespace ob_smpl {

const std::string defaultKeyMapPrompt = "'Esc': Exit Window, '?': Show Key Map";
CVWindow::CVWindow(std::string name, uint32_t width, uint32_t height, ArrangeMode arrangeMode)
    : name_(std::move(name)),
      arrangeMode_(arrangeMode),
      width_(width),
      height_(height),
      closed_(false),
      showInfo_(true),
      showSyncTimeInfo_(false),
      isWindowDestroyed_(false),
      alpha_(0.6f),
      showPrompt_(false) {

#if defined(TO_DISABLE_OPENCV_LOG)
    cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
#endif

    prompt_ = defaultKeyMapPrompt;

    cv::namedWindow(name_, cv::WINDOW_NORMAL);
    cv::resizeWindow(name_, width_, height_);

    renderMat_ = cv::Mat::zeros(height_, width_, CV_8UC3);
    cv::putText(renderMat_, "Waiting for streams...", cv::Point(8, 16), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    cv::imshow(name_, renderMat_);

    // start processing thread
    processThread_ = std::thread(&CVWindow::processFrames, this);

    winCreatedTime_ = getNowTimesMs();
}

CVWindow::~CVWindow() noexcept {
    close();
    destroyWindow();
}

void CVWindow::setKeyPressedCallback(std::function<void(int)> callback) {
    keyPressedCallback_ = callback;
}

// if window is closed
bool CVWindow::run() {

    {
        // show render mat
        std::lock_guard<std::mutex> lock(renderMatsMtx_);
        cv::imshow(name_, renderMat_);
    }

    int key = cv::waitKey(1);
    if(key != -1) {
        if(key == ESC_KEY) {
            closed_ = true;
            srcFrameGroupsCv_.notify_all();
        }
        else if(key == '1') {
            arrangeMode_ = ARRANGE_SINGLE;
            addLog("Switch to SINGLE arrange mode");
        }
        else if(key == '2') {
            arrangeMode_ = ARRANGE_ONE_ROW;
            addLog("Switch to ONE_ROW arrange mode");
        }
        else if(key == '3') {
            arrangeMode_ = ARRANGE_ONE_COLUMN;
            addLog("Switch to ONE_COLUMN arrange mode");
        }
        else if(key == '4') {
            arrangeMode_ = ARRANGE_GRID;
            addLog("Switch to GRID arrange mode");
        }
        else if(key == '5') {
            arrangeMode_ = ARRANGE_OVERLAY;
            addLog("Switch to OVERLAY arrange mode");
        }
        else if(key == '?' || key == '/') {
            showPrompt_ = !showPrompt_;
        }
        else if(key == '+' || key == '=') {
            alpha_ += 0.1f;
            if(alpha_ > 1) {
                alpha_ = 1;
            }
            addLog("Adjust alpha to " + ob_smpl::toString(alpha_, 1) + " (Only valid in OVERLAY arrange mode)");
        }
        else if(key == '-' || key == '_') {
            alpha_ -= 0.1f;
            if(alpha_ < 0) {
                alpha_ = 0;
            }
            addLog("Adjust alpha to " + ob_smpl::toString(alpha_, 1) + " (Only valid in OVERLAY arrange mode)");
        }
        if(keyPressedCallback_) {
            keyPressedCallback_(key);
        }
    }
    return !closed_;
}

// close window
void CVWindow::close() {
    {
        std::lock_guard<std::mutex> lock(renderMatsMtx_);
        closed_ = true;
        srcFrameGroupsCv_.notify_all();
    }

    if(processThread_.joinable()) {
        processThread_.join();
    }

    matGroups_.clear();
    srcFrameGroups_.clear();
}

void CVWindow::destroyWindow() {
    if(!isWindowDestroyed_){
        cv::destroyWindow(name_);
        cv::waitKey(1);
        isWindowDestroyed_ = true;
    }else{
        std::cout << "CVWindows has been destroyed!" << std::endl;
    }
}

void CVWindow::reset() {
    // close thread and clear cache
    close();

    // restart thread
    closed_        = false;
    processThread_ = std::thread(&CVWindow::processFrames, this);
}

// set the window size
void CVWindow::resize(int width, int height) {
    width_  = width;
    height_ = height;
    cv::resizeWindow(name_, width_, height_);
}

void CVWindow::setKeyPrompt(const std::string &prompt) {
    prompt_ = defaultKeyMapPrompt + ", " + prompt;
}

void CVWindow::addLog(const std::string &log) {
    log_            = log;
    logCreatedTime_ = getNowTimesMs();
}

// add frames to the show
void CVWindow::pushFramesToView(std::vector<std::shared_ptr<const ob::Frame>> frames, int groupId) {
    if(frames.empty()) {
        return;
    }

    std::vector<std::shared_ptr<const ob::Frame>> singleFrames;
    for(auto &frame: frames) {
        if(frame == nullptr) {
            continue;
        }

        if(!frame->is<ob::FrameSet>()) {
            // single frame, add to the list
            singleFrames.push_back(frame);
            continue;
        }

        // FrameSet contains multiple frames
        auto frameSet = frame->as<ob::FrameSet>();
        for(uint32_t index = 0; index < frameSet->getCount(); index++) {
            auto subFrame = frameSet->getFrameByIndex(index);
            singleFrames.push_back(subFrame);
        }
    }

    std::lock_guard<std::mutex> lk(srcFrameGroupsMtx_);
    srcFrameGroups_[groupId] = singleFrames;
    srcFrameGroupsCv_.notify_one();
}

void CVWindow::pushFramesToView(std::shared_ptr<const ob::Frame> currentFrame, int groupId) {
    pushFramesToView(std::vector<std::shared_ptr<const ob::Frame>>{ currentFrame }, groupId);
}

// set show frame info
void CVWindow::setShowInfo(bool show) {
    showInfo_ = show;
}

// set show frame synctime info
void CVWindow::setShowSyncTimeInfo(bool show) {
    showSyncTimeInfo_ = show;
}
// set alpha for OVERLAY render mode
void CVWindow::setAlpha(float alpha) {
    alpha_ = alpha;
    if(alpha_ < 0) {
        alpha_ = 0;
    }
    else if(alpha_ > 1) {
        alpha_ = 1;
    }
}

// frames processing thread
void CVWindow::processFrames() {
    std::map<int, std::vector<std::shared_ptr<const ob::Frame>>> frameGroups;
    while(!closed_) {
        if(closed_) {
            break;
        }
        {
            std::unique_lock<std::mutex> lk(srcFrameGroupsMtx_);
            srcFrameGroupsCv_.wait(lk);
            frameGroups = srcFrameGroups_;
        }

        if(frameGroups.empty()) {
            continue;
        }

        for(const auto &framesItem: frameGroups) {
            int         groupId = framesItem.first;
            const auto &frames  = framesItem.second;
            for(const auto &frame: frames) {
                auto rstMat = visualize(frame);
                if(!rstMat.empty()) {
                    int uid         = groupId * OB_FRAME_TYPE_COUNT + static_cast<int>(frame->getType());
                    matGroups_[uid] = { frame, rstMat };
                }
            }
        }

        if(matGroups_.empty()) {
            continue;
        }

        arrangeFrames();
    }
}

void CVWindow::arrangeFrames() {
    cv::Mat renderMat;
    try {
        if(arrangeMode_ == ARRANGE_SINGLE || matGroups_.size() == 1) {
            auto &mat = matGroups_.begin()->second.second;
            renderMat = resizeMatKeepAspectRatio(mat, width_, height_);
        }
        else if(arrangeMode_ == ARRANGE_ONE_ROW) {
            for(auto &item: matGroups_) {
                auto &  mat       = item.second.second;
                cv::Mat resizeMat = resizeMatKeepAspectRatio(mat, static_cast<int>(width_ / matGroups_.size()), height_);
                if(renderMat.dims > 0 && renderMat.cols > 0 && renderMat.rows > 0) {
                    cv::hconcat(renderMat, resizeMat, renderMat);
                }
                else {
                    renderMat = resizeMat;
                }
            }
        }
        else if(arrangeMode_ == ARRANGE_ONE_COLUMN) {
            for(auto &item: matGroups_) {
                auto &  mat       = item.second.second;
                cv::Mat resizeMat = resizeMatKeepAspectRatio(mat, width_, static_cast<int>(height_ / matGroups_.size()));
                if(renderMat.dims > 0 && renderMat.cols > 0 && renderMat.rows > 0) {
                    cv::vconcat(renderMat, resizeMat, renderMat);
                }
                else {
                    renderMat = resizeMat;
                }
            }
        }
        else if(arrangeMode_ == ARRANGE_GRID) {
            int count     = static_cast<int>(matGroups_.size());
            int idealSide = static_cast<int>(std::sqrt(count));
            int rows      = idealSide;
            int cols      = idealSide;
            while(rows * cols < count) {  // find the best row and column count
                cols++;
                if(rows * cols < count) {
                    rows++;
                }
            }

            std::vector<cv::Mat> gridImages;  // store all images in grid
            auto                 it = matGroups_.begin();
            for(int i = 0; i < rows; i++) {
                std::vector<cv::Mat> rowImages;  // store images in the same row
                for(int j = 0; j < cols; j++) {
                    int     index = i * cols + j;
                    cv::Mat resizeMat;
                    if(index < count) {
                        auto mat  = it->second.second;
                        resizeMat = resizeMatKeepAspectRatio(mat, width_ / cols, height_ / rows);
                        it++;
                    }
                    else {
                        resizeMat = cv::Mat::zeros(height_ / rows, width_ / cols, CV_8UC3);  // fill with black
                    }
                    rowImages.push_back(resizeMat);
                }
                cv::Mat lineMat;
                cv::hconcat(rowImages, lineMat);  // horizontal concat all images in the same row
                gridImages.push_back(lineMat);
            }

            cv::vconcat(gridImages, renderMat);  // vertical concat all images in the grid
        }
        else if(arrangeMode_ == ARRANGE_OVERLAY && matGroups_.size() >= 2) {
            cv::Mat     overlayMat;
            const auto &mat1 = matGroups_.begin()->second.second;
            const auto &mat2 = matGroups_.rbegin()->second.second;
            renderMat        = resizeMatKeepAspectRatio(mat1, width_, height_);
            overlayMat       = resizeMatKeepAspectRatio(mat2, width_, height_);

            float alpha = alpha_;
            for(int i = 0; i < renderMat.rows; i++) {
                for(int j = 0; j < renderMat.cols; j++) {
                    cv::Vec3b &outRgb     = renderMat.at<cv::Vec3b>(i, j);
                    cv::Vec3b &overlayRgb = overlayMat.at<cv::Vec3b>(i, j);

                    outRgb[0] = (uint8_t)(outRgb[0] * (1.0f - alpha) + overlayRgb[0] * alpha);
                    outRgb[1] = (uint8_t)(outRgb[1] * (1.0f - alpha) + overlayRgb[1] * alpha);
                    outRgb[2] = (uint8_t)(outRgb[2] * (1.0f - alpha) + overlayRgb[2] * alpha);
                }
            }
        }
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    if(renderMat.empty()) {
        return;
    }

    if(showPrompt_ || getNowTimesMs() - winCreatedTime_ < 5000) {
        cv::putText(renderMat, prompt_, cv::Point(8, 16), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }

    if(!log_.empty() && getNowTimesMs() - logCreatedTime_ < 3000) {
        cv::putText(renderMat, log_, cv::Point(8, height_ - 16), cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }

    std::lock_guard<std::mutex> lock(renderMatsMtx_);
    renderMat_ = renderMat;
}

cv::Mat CVWindow::visualize(std::shared_ptr<const ob::Frame> frame) {
    if(frame == nullptr) {
        return cv::Mat();
    }

    cv::Mat rstMat;
    if(frame->getType() == OB_FRAME_COLOR) {
        auto videoFrame = frame->as<const ob::VideoFrame>();
        switch(videoFrame->getFormat()) {
        case OB_FORMAT_MJPG: {
            cv::Mat rawMat(1, videoFrame->getDataSize(), CV_8UC1, videoFrame->getData());
            rstMat = cv::imdecode(rawMat, 1);
        } break;
        case OB_FORMAT_NV21: {
            cv::Mat rawMat(videoFrame->getHeight() * 3 / 2, videoFrame->getWidth(), CV_8UC1, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_NV21);
        } break;
        case OB_FORMAT_YUYV:
        case OB_FORMAT_YUY2: {
            cv::Mat rawMat(videoFrame->getHeight(), videoFrame->getWidth(), CV_8UC2, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_YUY2);
        } break;
        case OB_FORMAT_RGB: {
            cv::Mat rawMat(videoFrame->getHeight(), videoFrame->getWidth(), CV_8UC3, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_RGB2BGR);
        } break;
        case OB_FORMAT_RGBA: {
            cv::Mat rawMat(videoFrame->getHeight(), videoFrame->getWidth(), CV_8UC4, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_RGBA2BGR);
        } break;
        case OB_FORMAT_BGRA: {
            cv::Mat rawMat(videoFrame->getHeight(), videoFrame->getWidth(), CV_8UC4, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_BGRA2BGR);
        } break;
        case OB_FORMAT_UYVY: {
            cv::Mat rawMat(videoFrame->getHeight(), videoFrame->getWidth(), CV_8UC2, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_UYVY);
        } break;
        case OB_FORMAT_I420: {
            cv::Mat rawMat(videoFrame->getHeight() * 3 / 2, videoFrame->getWidth(), CV_8UC1, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_I420);
        } break;
        default:
            break;
        }
        if(showSyncTimeInfo_ && !rstMat.empty()) {
            drawInfo(rstMat, videoFrame);
        }
    }
    else if(frame->getType() == OB_FRAME_DEPTH) {
        auto videoFrame = frame->as<const ob::VideoFrame>();
        if(videoFrame->getFormat() == OB_FORMAT_Y16 || videoFrame->getFormat() == OB_FORMAT_Z16) {
            cv::Mat rawMat = cv::Mat(videoFrame->getHeight(), videoFrame->getWidth(), CV_16UC1, videoFrame->getData());
            // depth frame pixel value multiply scale to get distance in millimeter
            float scale = videoFrame->as<ob::DepthFrame>()->getValueScale();

            cv::Mat cvtMat;
            // normalization to 0-255. 0.032f is 256/8000, to limit the range of depth to 8000mm
            rawMat.convertTo(cvtMat, CV_32F, scale * 0.032f);

            // apply gamma correction to enhance the contrast for near objects
            cv::pow(cvtMat, 0.6f, cvtMat);

            //  convert to 8-bit
            cvtMat.convertTo(cvtMat, CV_8UC1, 10);  // multiplier 10 is to normalize to 0-255 (nearly) after applying gamma correction

            // apply colormap
            cv::applyColorMap(cvtMat, rstMat, cv::COLORMAP_JET);
        }
        if(showSyncTimeInfo_ && !rstMat.empty()) {
            drawInfo(rstMat, videoFrame);
        }
    }
    else if(frame->getType() == OB_FRAME_IR || frame->getType() == OB_FRAME_IR_LEFT || frame->getType() == OB_FRAME_IR_RIGHT) {
        auto videoFrame = frame->as<const ob::VideoFrame>();
        if(videoFrame->getFormat() == OB_FORMAT_Y16) {
            cv::Mat cvtMat;
            cv::Mat rawMat = cv::Mat(videoFrame->getHeight(), videoFrame->getWidth(), CV_16UC1, videoFrame->getData());
            rawMat.convertTo(cvtMat, CV_8UC1, 1.0 / 16.0f);
            cv::cvtColor(cvtMat, rstMat, cv::COLOR_GRAY2RGB);
        }
        else if(videoFrame->getFormat() == OB_FORMAT_Y8) {
            cv::Mat rawMat = cv::Mat(videoFrame->getHeight(), videoFrame->getWidth(), CV_8UC1, videoFrame->getData());
            cv::cvtColor(rawMat, rstMat, cv::COLOR_GRAY2RGB);
        }
        else if(videoFrame->getFormat() == OB_FORMAT_MJPG) {
            cv::Mat rawMat(1, videoFrame->getDataSize(), CV_8UC1, videoFrame->getData());
            rstMat = cv::imdecode(rawMat, 1);
            cv::cvtColor(rstMat, rstMat, cv::COLOR_GRAY2RGB);
        }
        if(showSyncTimeInfo_ && !rstMat.empty()) {
            drawInfo(rstMat, videoFrame);
        }
    }
    else if(frame->getType() == OB_FRAME_ACCEL) {
        rstMat                 = cv::Mat::zeros(320, 240, CV_8UC3);
        auto        accelFrame = frame->as<ob::AccelFrame>();
        auto        value      = accelFrame->getValue();
        std::string str        = "Accel:";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 60), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" timestamp=") + std::to_string(accelFrame->getTimeStampUs()) + "us";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 100), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" x=") + std::to_string(value.x) + "m/s^2";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 140), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" y=") + std::to_string(value.y) + "m/s^2";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 180), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" z=") + std::to_string(value.z) + "m/s^2";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 220), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }
    else if(frame->getType() == OB_FRAME_GYRO) {
        rstMat                = cv::Mat::zeros(320, 240, CV_8UC3);
        auto        gyroFrame = frame->as<ob::GyroFrame>();
        auto        value     = gyroFrame->getValue();
        std::string str       = "Gyro:";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 60), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" timestamp=") + std::to_string(gyroFrame->getTimeStampUs()) + "us";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 100), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" x=") + std::to_string(value.x) + "rad/s";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 140), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" y=") + std::to_string(value.y) + "rad/s";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 180), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
        str = std::string(" z=") + std::to_string(value.z) + "rad/s";
        cv::putText(rstMat, str.c_str(), cv::Point(8, 220), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 1, cv::LINE_AA);
    }
    return rstMat;
}

// add frame info to mat
void CVWindow::drawInfo(cv::Mat &imageMat, std::shared_ptr<const ob::VideoFrame> &frame) {
    int      baseline = 0;  // Used to calculate text size and baseline
    cv::Size textSize;      // Size of the text to be drawn
    int      padding = 5;   // Padding around the text for the background

    // Helper lambda function to draw text with background
    auto putTextWithBackground = [&](const std::string &text, cv::Point origin) {
        // Getting text size for background
        textSize = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.6, 1, &baseline);

        // Drawing the white background
        cv::rectangle(imageMat, origin + cv::Point(0, baseline), origin + cv::Point(textSize.width, -textSize.height) - cv::Point(0, padding),
                      cv::Scalar(255, 255, 255), cv::FILLED);

        // Putting black text on the white background
        cv::putText(imageMat, text, origin, cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0), 1);
    };

    // Drawing text with background based on frame type
    if(frame->getType() == OB_FRAME_COLOR && frame->getFormat() == OB_FORMAT_NV21) {
        putTextWithBackground("Color-NV21", cv::Point(8, 16));
    }
    else if(frame->getType() == OB_FRAME_COLOR && frame->getFormat() == OB_FORMAT_MJPG) {
        putTextWithBackground("Color-MJPG", cv::Point(8, 16));
    }
    else if(frame->getType() == OB_FRAME_COLOR && ((frame->getFormat() == OB_FORMAT_YUYV) || (frame->getFormat() == OB_FORMAT_YUY2))) {
        putTextWithBackground("Color-YUYV", cv::Point(8, 16));
    }
    else if(frame->getType() == OB_FRAME_DEPTH) {
        putTextWithBackground("Depth", cv::Point(8, 16));
    }
    else if(frame->getType() == OB_FRAME_IR) {
        putTextWithBackground("IR", cv::Point(8, 16));
    }
    else if(frame->getType() == OB_FRAME_IR_LEFT) {
        putTextWithBackground("LeftIR", cv::Point(8, 16));
    }
    else if(frame->getType() == OB_FRAME_IR_RIGHT) {
        putTextWithBackground("RightIR", cv::Point(8, 16));
    }

    // Timestamp information with background
    putTextWithBackground("frame timestamp(ms):  " + std::to_string(frame->getTimeStampUs()), cv::Point(8, 40));
    putTextWithBackground("system timestamp(ms): " + std::to_string(frame->getSystemTimeStampUs()), cv::Point(8, 64));
}

cv::Mat CVWindow::resizeMatKeepAspectRatio(const cv::Mat &mat, int width, int height) {
    auto    hScale    = static_cast<double>(width) / mat.cols;
    auto    vScale    = static_cast<double>(height) / mat.rows;
    auto    scale     = std::min(hScale, vScale);
    auto    newWidth  = static_cast<int>(mat.cols * scale);
    auto    newHeight = static_cast<int>(mat.rows * scale);
    cv::Mat resizeMat;
    cv::resize(mat, resizeMat, cv::Size(newWidth, newHeight));

    if(newWidth == width && newHeight == height) {
        return resizeMat;
    }

    // padding the resized mat to target width and height
    cv::Mat paddedMat;
    if(newWidth < width) {
        auto paddingLeft  = (width - newWidth) / 2;
        auto paddingRight = width - newWidth - paddingLeft;
        cv::copyMakeBorder(resizeMat, paddedMat, 0, 0, paddingLeft, paddingRight, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    }

    if(newHeight < height) {
        auto paddingTop    = (height - newHeight) / 2;
        auto paddingBottom = height - newHeight - paddingTop;
        cv::copyMakeBorder(resizeMat, paddedMat, paddingTop, paddingBottom, 0, 0, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    }
    return paddedMat;
}

}  // namespace ob_smpl
