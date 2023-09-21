// Copyright(c) 2020 Orbbec Corporation. All Rights Reserved.
#pragma once

#include <libobsensor/ObSensor.hpp>
#include <opencv2/opencv.hpp>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

#define ESC_KEY 27

// Quickly take the reciprocal of the square root
float Q_rsqrt(float number) {
    long        i;
    float       x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = *(long *)&y;
    i  = 0x5f3759df - (i >> 1);
    y  = *(float *)&i;
    y  = y * (threehalfs - (x2 * y * y));

    return y;
}

typedef enum {
    RENDER_SINGLE,      // only render the first frame in the array
    RENDER_ONE_ROW,     // Render the frames in the array as a row
    RENDER_ONE_COLUMN,  // render the frames in the array as a column
    RENDER_GRID,        // Render the frames in the array as a grid
    RENDER_OVERLAY      // Render the frames in the array as an overlay
} RenderType;

class Window {
public:
    // create a window with the specified name, width and height
    Window(std::string name, int width, int height, RenderType renderType_ = RENDER_SINGLE)
        : name_(name), width_(width), height_(height), windowClose_(false), key_(-1), showInfo_(false), renderType_(renderType_), alpha_(0.6) {
        processThread_ = std::thread(&Window::processFrames, this);
#ifndef __APPLE__
        renderThread_ = std::thread(&Window::renderMats, this);
#else
        cv::namedWindow(name_, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
        cv::resizeWindow(name_, width_, height_);
#endif
    }

    ~Window() {
        close();
    }

    // set the window size
    void resize(int width, int height) {
        width_  = width;
        height_ = height;
        cv::resizeWindow(name_, width_, height_);
    }

    void addToRender(std::shared_ptr<ob::Frame> frame) {
        std::vector<std::shared_ptr<ob::Frame>> frames = { frame };
        addToRender(frames);
    }

    // add frames to the rendering
    inline void addToRender(std::vector<std::shared_ptr<ob::Frame>> frames) {
        std::lock_guard<std::mutex> lk(srcFramesMtx_);
        srcFrames_ = frames;
        srcFramesCv_.notify_one();
    }

    // close window
    void close() {
        windowClose_ = true;
        srcFramesCv_.notify_all();
        processThread_.join();
#ifndef __APPLE__
        renderThread_.join();
#endif
    }

    // wait for the key to be pressed
    int waitKey(uint32_t timeoutMsec = 30) {
        std::unique_lock<std::mutex> lk(keyMtx_);
        keyCv_.wait_for(lk, std::chrono::milliseconds(timeoutMsec), [&] { return key_ != -1; });
        int key = key_;
        key_    = -1;
        return key;
    }

    // if window is closed
    operator bool() {
#ifdef __APPLE__
        render();
#endif
        return !windowClose_;
    }

    // set show frame info
    void setShowInfo(bool show) {
        showInfo_ = show;
    };

    // set alpha for OVERLAY render mode
    void setAlpha(float alpha) {
        alpha_ = alpha;
        if(alpha_ < 0) {
            alpha_ = 0;
        }
        else if(alpha_ > 1) {
            alpha_ = 1;
        }
    }

private:
    std::string name_;
    RenderType  renderType_;
    int         width_;
    int         height_;
    bool        windowClose_;
    bool        showInfo_;
    float       alpha_;

    int                     key_;
    std::mutex              keyMtx_;
    std::condition_variable keyCv_;

    std::thread                             processThread_;
    std::vector<std::shared_ptr<ob::Frame>> srcFrames_;
    std::mutex                              srcFramesMtx_;
    std::condition_variable                 srcFramesCv_;

#ifndef __APPLE__
    std::thread renderThread_;
#endif
    std::vector<cv::Mat> renderMats_;
    std::mutex           renderMatsMtx_;
    cv::Mat              renderMat_;

    // frames processing thread
    void processFrames() {
        std::vector<cv::Mat>                    mats;
        cv::Mat                                 imuMat;
        cv::Mat                                 rstMat;
        std::vector<std::shared_ptr<ob::Frame>> frames;
        while(!windowClose_) {
            {
                std::unique_lock<std::mutex> lk(srcFramesMtx_);
                srcFramesCv_.wait(lk, [this] { return !srcFrames_.empty() || windowClose_; });
                if(windowClose_) {
                    break;
                }
                frames = srcFrames_;
                srcFrames_.clear();
            }
            for(auto frame: frames) {
                rstMat.release();
                if(frame == nullptr) {
                    mats.clear();
                    imuMat.release();
                    break;
                }
                if(frame->type() == OB_FRAME_COLOR) {
                    auto videoFrame = frame->as<ob::VideoFrame>();
                    switch(videoFrame->format()) {
                    case OB_FORMAT_MJPG: {
                        cv::Mat rawMat(1, videoFrame->dataSize(), CV_8UC1, videoFrame->data());
                        rstMat = cv::imdecode(rawMat, 1);
                    } break;
                    case OB_FORMAT_NV21: {
                        cv::Mat rawMat(videoFrame->height() * 3 / 2, videoFrame->width(), CV_8UC1, videoFrame->data());
                        cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_NV21);
                    } break;
                    case OB_FORMAT_YUYV:
                    case OB_FORMAT_YUY2: {
                        cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC2, videoFrame->data());
                        cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_YUY2);
                    } break;
                    case OB_FORMAT_RGB: {
                        cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC3, videoFrame->data());
                        cv::cvtColor(rawMat, rstMat, cv::COLOR_RGB2BGR);
                    } break;
                    case OB_FORMAT_UYVY: {
                        cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC2, videoFrame->data());
                        cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_UYVY);
                    } break;
                    default:
                        break;
                    }
                    if(showInfo_ && !rstMat.empty()) {
                        drawInfo(rstMat, videoFrame);
                    }
                }
                else if(frame->type() == OB_FRAME_DEPTH) {
                    auto videoFrame = frame->as<ob::VideoFrame>();
                    if(videoFrame->format() == OB_FORMAT_Y16) {
                        cv::Mat cvtMat;
                        cv::Mat rawMat = cv::Mat(videoFrame->height(), videoFrame->width(), CV_16UC1, videoFrame->data());
                        // depth frame pixel value multiply scale to get distance in millimeter
                        float scale = videoFrame->as<ob::DepthFrame>()->getValueScale();

                        // threshold to 5.12m
                        cv::threshold(rawMat, cvtMat, 5120.0f / scale, 0, cv::THRESH_TRUNC);
                        cvtMat.convertTo(cvtMat, CV_8UC1, scale * 0.05);
                        cv::applyColorMap(cvtMat, rstMat, cv::COLORMAP_JET);
                    }
                    if(showInfo_ && !rstMat.empty()) {
                        drawInfo(rstMat, videoFrame);
                    }
                }
                else if(frame->type() == OB_FRAME_IR || frame->type() == OB_FRAME_IR_LEFT || frame->type() == OB_FRAME_IR_RIGHT) {
                    auto videoFrame = frame->as<ob::VideoFrame>();
                    if(videoFrame->format() == OB_FORMAT_Y16) {
                        cv::Mat cvtMat;
                        cv::Mat rawMat = cv::Mat(videoFrame->height(), videoFrame->width(), CV_16UC1, videoFrame->data());
                        rawMat.convertTo(cvtMat, CV_8UC1, 1.0 / 16.0f);
                        cv::cvtColor(cvtMat, rstMat, cv::COLOR_GRAY2RGB);
                    }
                    else if(videoFrame->format() == OB_FORMAT_Y8) {
                        cv::Mat rawMat = cv::Mat(videoFrame->height(), videoFrame->width(), CV_8UC1, videoFrame->data());
                        cv::cvtColor(rawMat * 2, rstMat, cv::COLOR_GRAY2RGB);
                    }
                    else if(videoFrame->format() == OB_FORMAT_MJPG) {
                        cv::Mat rawMat(1, videoFrame->dataSize(), CV_8UC1, videoFrame->data());
                        rstMat = cv::imdecode(rawMat, 1);
                        cv::cvtColor(rstMat * 2, rstMat, cv::COLOR_GRAY2RGB);
                    }
                    if(showInfo_ && !rstMat.empty()) {
                        drawInfo(rstMat, videoFrame);
                    }
                }
                else if(frame->type() == OB_FRAME_ACCEL) {
                    if(imuMat.empty()) {
                        imuMat = cv::Mat::zeros(640, 360, CV_8UC3);
                    }
                    auto        accelFrame = frame->as<ob::AccelFrame>();
                    auto        value      = accelFrame->value();
                    std::string str        = "Accel:";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 60), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    str = std::string(" x=") + std::to_string(value.x) + "m/s^2";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 120), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    str = std::string(" y=") + std::to_string(value.y) + "m/s^2";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 180), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    str = std::string(" z=") + std::to_string(value.z) + "m/s^2";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 240), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    continue;
                }
                else if(frame->type() == OB_FRAME_GYRO) {
                    if(imuMat.empty()) {
                        imuMat = cv::Mat(640, 360, CV_8UC3);
                    }
                    auto        gyroFrame = frame->as<ob::GyroFrame>();
                    auto        value     = gyroFrame->value();
                    std::string str       = "Gyro:";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 300), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    str = std::string(" x=") + std::to_string(value.x) + "rad/s";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 360), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    str = std::string(" y=") + std::to_string(value.y) + "rad/s";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 420), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    str = std::string(" z=") + std::to_string(value.z) + "rad/s";
                    cv::putText(imuMat, str.c_str(), cv::Point(8, 480), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(255, 255, 255), 0.5, cv::LINE_AA);
                    continue;
                }
                if(!rstMat.empty()) {
                    mats.push_back(rstMat);
                }
            }

            if(!imuMat.empty()) {
                mats.push_back(imuMat);
                imuMat.release();
            }

            if(!mats.empty()) {
                std::lock_guard<std::mutex> lock(renderMatsMtx_);
                renderMats_ = mats;
                mats.clear();
            }
        }
    }

    // add info to mat
    static void drawInfo(cv::Mat &imageMat, std::shared_ptr<ob::VideoFrame> &frame) {
        if(frame->type() == OB_FRAME_COLOR && frame->format() == OB_FORMAT_NV21) {
            cv::putText(imageMat, "Color-NV21", cv::Point(8, 16), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 255), 1);
        }
        else if(frame->type() == OB_FRAME_COLOR && frame->format() == OB_FORMAT_MJPG) {
            cv::putText(imageMat, "Color-MJPG", cv::Point(8, 16), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 255), 1);
        }
        else if(frame->type() == OB_FRAME_DEPTH) {
            cv::putText(imageMat, "Depth", cv::Point(8, 16), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 255), 1);
        }
        else if(frame->type() == OB_FRAME_IR) {
            cv::putText(imageMat, "IR", cv::Point(8, 16), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 255), 1, 4, false);
        }
        else if(frame->type() == OB_FRAME_IR_LEFT) {
            cv::putText(imageMat, "IR_LEFT", cv::Point(8, 16), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 255), 1, 4, false);
        }
        else if(frame->type() == OB_FRAME_IR_RIGHT) {
            cv::putText(imageMat, "IR_RIGHT", cv::Point(8, 16), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 200, 255), 1, 4, false);
        }

        cv::putText(imageMat, (std::string("timeStamp: ") + std::to_string(frame->timeStamp())).c_str(), cv::Point(8, 40), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                    cv::Scalar(0, 200, 255), 1);

        cv::putText(imageMat, (std::string("systemTimeStamp: ") + std::to_string(frame->systemTimeStamp())).c_str(), cv::Point(8, 64), cv::FONT_HERSHEY_SIMPLEX,
                    0.6, cv::Scalar(0, 200, 255), 1);
    };

    void render() {
        // wait key and control render frequency
        std::vector<cv::Mat> mats;
        int                  key = cv::waitKey(30);
        if(key != -1) {
            std::unique_lock<std::mutex> lk(keyMtx_);
            key_ = key;
            keyCv_.notify_all();

            if(key == ESC_KEY) {
                windowClose_ = true;
                srcFramesCv_.notify_all();
            }
        }

        mats.clear();
        {
            std::lock_guard<std::mutex> lock(renderMatsMtx_);
            mats = renderMats_;
        }

        if(!mats.empty()) {
            renderMat_.release();
            try {
                if(renderType_ == RENDER_SINGLE) {
                    cv::resize(mats[0], renderMat_, cv::Size(width_, height_));
                }
                else if(renderType_ == RENDER_ONE_ROW) {
                    for(auto mat: mats) {
                        cv::Mat resizeMat;
                        cv::resize(mat, resizeMat, cv::Size(width_ / mats.size(), height_));
                        if(renderMat_.dims > 0 && renderMat_.cols > 0 && renderMat_.rows > 0) {
                            cv::hconcat(renderMat_, resizeMat, renderMat_);
                        }
                        else {
                            renderMat_ = resizeMat;
                        }
                    }
                    cv::imshow(name_, renderMat_);
                }
                else if(renderType_ == RENDER_ONE_COLUMN) {
                    for(auto mat: mats) {
                        cv::Mat resizeMat;
                        cv::resize(mat, resizeMat, cv::Size(width_, height_ / mats.size()));
                        if(renderMat_.dims > 0 && renderMat_.cols > 0 && renderMat_.rows > 0) {
                            cv::vconcat(renderMat_, resizeMat, renderMat_);
                        }
                        else {
                            renderMat_ = resizeMat;
                        }
                    }
                    cv::imshow(name_, renderMat_);
                }
                else if(renderType_ == RENDER_GRID) {
                    int   count = mats.size();
                    float sq    = 1.0f / Q_rsqrt(count);
                    int   isq   = (int)sq;
                    int   cols  = (sq - isq < 0.01f) ? isq : isq + 1;
                    float div   = (float)count / (float)cols;
                    int   idiv  = (int)div;
                    int   rows  = (div - idiv < 0.01f) ? idiv : idiv + 1;
                    for(int i = 0; i < rows; i++) {
                        cv::Mat lineMat;
                        for(int j = 0; j < cols; j++) {
                            cv::Mat mat;
                            cv::Mat resizeMat;
                            int     index = i * cols + j;
                            if(index < count) {
                                mat = mats[index];
                            }
                            else {
                                mat = cv::Mat::zeros(width_ / cols, height_ / rows, CV_8UC3);
                            }

                            cv::resize(mat, resizeMat, cv::Size(width_ / cols, height_ / rows));
                            if(lineMat.dims > 0 && lineMat.cols > 0 && lineMat.rows > 0) {
                                cv::hconcat(lineMat, resizeMat, lineMat);
                            }
                            else {
                                lineMat = resizeMat;
                            }
                        }

                        if(renderMat_.dims > 0 && renderMat_.rows > 0 && renderMat_.cols > 0) {
                            cv::vconcat(renderMat_, lineMat, renderMat_);
                        }
                        else {
                            renderMat_ = lineMat;
                        }
                    }
                }
                else if(renderType_ == RENDER_OVERLAY && mats.size() >= 2) {
                    cv::Mat overlayMat;
                    cv::resize(mats[0], renderMat_, cv::Size(width_, height_));
                    cv::resize(mats[1], overlayMat, cv::Size(width_, height_));
                    float alpha = alpha_;
                    for(int i = 0; i < renderMat_.rows; i++) {
                        for(int j = 0; j < renderMat_.cols; j++) {
                            cv::Vec3b &outRgb     = renderMat_.at<cv::Vec3b>(i, j);
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
        }

        // show render mat
        if(renderMat_.size().width > 0 && renderMat_.size().height > 0) {
            cv::imshow(name_, renderMat_);
        }
    }

#ifndef __APPLE__
    // render thread
    void renderMats() {
        // must create window in render thread
        cv::namedWindow(name_, cv::WINDOW_NORMAL | cv::WINDOW_KEEPRATIO);
        cv::resizeWindow(name_, width_, height_);

        while(!windowClose_) {
            render();
        }
    }
#endif
};