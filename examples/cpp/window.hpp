// Copyright(c) 2020 Orbbec Corporation. All Rights Reserved.
#pragma once

#include <libobsensor/ObSensor.hpp>
#include <opencv2/opencv.hpp>
#include <string>

#define ESC 27

// 快速取平方根的倒数
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
    RENDER_SINGLE,      // 只渲染数组里的第一个帧
    RENDER_ONE_ROW,     // 以一行的形式渲染数组里的帧
    RENDER_ONE_COLUMN,  // 以一列的形式渲染数组里的帧
    RENDER_GRID,        // 以格子的形式渲染数组里的帧
    RENDER_OVERLAY      // 以叠加的形式渲染数组里的帧
} RenderType;

class Window {
public:
    Window(std::string name, int width, int height)
        : _name(name),
          _width(width),
          _height(height),
          _windowClose(false),
          _key(-1),
          _showInfo(false),
          _averageColorFps(0),
          _averageDepthFps(0),
          _averageIrFps(0) {
        // cv::namedWindow(_name, cv::WINDOW_KEEPRATIO);
    }

    ~Window() {}

    void resize(int width, int height) {
        _width  = width;
        _height = height;
    }

    void render(std::vector<std::shared_ptr<ob::Frame>> frames, RenderType renderType) {
        _key = cv::waitKey(10);
        if(_key == ESC) {
            _windowClose = true;
        }
        else if(_key == 'I' || _key == 'i') {
            _showInfo = !_showInfo;
        }
        if(_windowClose) {
            cv::destroyAllWindows();
            return;
        }

        auto mats = processFrames(frames);
        renderMats(mats, renderType);
    }

    void render(std::vector<std::shared_ptr<ob::Frame>> frames, float alpha) {
        _key = cv::waitKey(10);
        if(_key == ESC) {
            _windowClose = true;
        }
        else if(_key == 'I' || _key == 'i') {
            _showInfo = !_showInfo;
        }
        if(_windowClose) {
            cv::destroyAllWindows();
            return;
        }

        auto mats = processFrames(frames);
        renderMats(mats, alpha);
    }

    inline int getKey() {
        return _key;
    }

    operator bool() {
        return !_windowClose;
    }

    void setShowInfo(bool show) {
        _showInfo = show;
    };

    void setColorAverageFps(int averageFps) {
        _averageColorFps = averageFps;
    }

    void setDepthAverageFps(int averageFps) {
        _averageDepthFps = averageFps;
    }

    void setIrAverageFps(int averageFps) {
        _averageIrFps = averageFps;
    }

private:
    std::string _name;
    int         _width;
    int         _height;
    bool        _windowClose;
    int         _key;
    bool        _showInfo;
    int         _averageColorFps, _averageDepthFps, _averageIrFps;

    std::vector<cv::Mat> processFrames(std::vector<std::shared_ptr<ob::Frame>> frames) {
        std::vector<cv::Mat> mats;
        for(auto frame: frames) {
            if(frame == nullptr || frame->dataSize() < 1024) {
                continue;
                // return mats;
            }
            auto videoFrame = frame->as<ob::VideoFrame>();

            int     averageFps = _averageColorFps;
            cv::Mat rstMat;

            if(videoFrame->type() == OB_FRAME_COLOR && videoFrame->format() == OB_FORMAT_MJPG) {
                cv::Mat rawMat(1, videoFrame->dataSize(), CV_8UC1, videoFrame->data());
                rstMat = cv::imdecode(rawMat, 1);
            }
            else if(videoFrame->type() == OB_FRAME_COLOR && videoFrame->format() == OB_FORMAT_NV21) {
                cv::Mat rawMat(videoFrame->height() * 3 / 2, videoFrame->width(), CV_8UC1, videoFrame->data());
                cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_NV21);
            }
            else if(videoFrame->type() == OB_FRAME_COLOR && (videoFrame->format() == OB_FORMAT_YUYV || videoFrame->format() == OB_FORMAT_YUY2)) {
                cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC2, videoFrame->data());
                cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_YUY2);
            }
            else if(videoFrame->type() == OB_FRAME_COLOR && videoFrame->format() == OB_FORMAT_RGB) {
                cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC3, videoFrame->data());
                cv::cvtColor(rawMat, rstMat, cv::COLOR_RGB2BGR);
            }
            else if(videoFrame->type() == OB_FRAME_COLOR && videoFrame->format() == OB_FORMAT_UYVY) {
                cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC2, videoFrame->data());
                cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_UYVY);
            }
            else if(videoFrame->format() == OB_FORMAT_Y16 || videoFrame->format() == OB_FORMAT_YUYV || videoFrame->format() == OB_FORMAT_YUY2) {
                if(videoFrame->type() == OB_FRAME_DEPTH) {
                    averageFps = _averageDepthFps;
                }
                else if(videoFrame->type() == OB_FRAME_IR) {
                    averageFps = _averageIrFps;
                }
                // IR or Depth Frame
                cv::Mat cvtMat;
                cv::Mat rawMat = cv::Mat(videoFrame->height(), videoFrame->width(), CV_16UC1, videoFrame->data());
                float   scale;
                if(videoFrame->type() == OB_FRAME_DEPTH) {
                    scale = 1.0f / pow(2, videoFrame->pixelAvailableBitSize() - 10);
                }
                else {
                    scale = 1.0f / pow(2, videoFrame->pixelAvailableBitSize() - 8);
                }
                cv::convertScaleAbs(rawMat, cvtMat, scale);
                cv::cvtColor(cvtMat, rstMat, cv::COLOR_GRAY2RGB);
            }
            else if(videoFrame->type() == OB_FRAME_COLOR && videoFrame->format() == OB_FORMAT_UYVY) {
                cv::Mat rawMat(videoFrame->height(), videoFrame->width(), CV_8UC2, videoFrame->data());
                cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_UYVY);
            }

            if(videoFrame->type() == OB_FRAME_DEPTH) {
                averageFps = _averageDepthFps;
            }
            else if(is_ir_frame(videoFrame->type()) && videoFrame->format() == OB_FORMAT_Y8) {
                averageFps     = _averageIrFps;
                cv::Mat rawMat = cv::Mat(videoFrame->height(), videoFrame->width(), CV_8UC1, videoFrame->data());

                cv::cvtColor(rawMat, rstMat, cv::COLOR_GRAY2RGB);
            }
            else if(is_ir_frame(videoFrame->type()) && videoFrame->format() == OB_FORMAT_MJPG) {
                averageFps = _averageIrFps;
                cv::Mat rawMat(1, videoFrame->dataSize(), CV_8UC1, videoFrame->data());
                rstMat = cv::imdecode(rawMat, 1);
            }
            else if(is_ir_frame(videoFrame->type())) {
                averageFps = _averageIrFps;
            }

            if(_showInfo) {
                drawInfo(rstMat, videoFrame, averageFps);
            }
            mats.push_back(rstMat);
        }
        return mats;
    }
    static void drawInfo(cv::Mat &imageMat, std::shared_ptr<ob::VideoFrame> &frame, int averageFps) {
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

        if(averageFps != 0) {
            cv::putText(imageMat, (std::string("Average frame rate: ") + std::to_string(averageFps)).c_str(), cv::Point(8, 88), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                        cv::Scalar(0, 200, 255), 1);
        }
    };
    void renderMats(std::vector<cv::Mat> mats, RenderType renderType) {
        if(mats.size() == 0) {
            return;
        }
        try {
            if(renderType == RENDER_SINGLE) {
                cv::Mat outMat;
                cv::resize(mats[0], outMat, cv::Size(_width, _height));
                cv::imshow(_name, outMat);
            }
            else if(renderType == RENDER_ONE_ROW) {
                cv::Mat outMat;
                for(auto mat: mats) {
                    cv::Mat resizeMat;
                    cv::resize(mat, resizeMat, cv::Size(_width / mats.size(), _height));
                    if(outMat.dims > 0) {
                        cv::hconcat(outMat, resizeMat, outMat);
                    }
                    else {
                        outMat = resizeMat;
                    }
                }
                cv::imshow(_name, outMat);
            }
            else if(renderType == RENDER_ONE_COLUMN) {
                cv::Mat outMat;
                for(auto mat: mats) {
                    cv::Mat resizeMat;
                    cv::resize(mat, resizeMat, cv::Size(_width, _height / mats.size()));
                    if(outMat.dims > 0) {
                        cv::vconcat(outMat, resizeMat, outMat);
                    }
                    else {
                        outMat = resizeMat;
                    }
                }
                cv::imshow(_name, outMat);
            }
            else if(renderType == RENDER_GRID) {
                int     count = mats.size();
                float   sq    = 1.0f / Q_rsqrt(count);
                int     isq   = (int)sq;
                int     cols  = (sq - isq < 0.01f) ? isq : isq + 1;
                float   div   = (float)count / (float)cols;
                int     idiv  = (int)div;
                int     rows  = (div - idiv < 0.01f) ? idiv : idiv + 1;
                cv::Mat outMat;
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
                            mat = cv::Mat::zeros(_width / cols, _height / rows, CV_8UC3);
                        }

                        cv::resize(mat, resizeMat, cv::Size(_width / cols, _height / rows));
                        if(lineMat.dims > 0) {
                            cv::hconcat(lineMat, resizeMat, lineMat);
                        }
                        else {
                            lineMat = resizeMat;
                        }
                    }
                    if(outMat.dims > 0) {
                        cv::vconcat(outMat, lineMat, outMat);
                    }
                    else {
                        outMat = lineMat;
                    }
                }
                if(outMat.size().width > 0 && outMat.size().height > 0) {
                    cv::imshow(_name, outMat);
                }
            }
            else if(renderType == RENDER_OVERLAY) {
                cv::Mat outMat, resizeMat;
                cv::resize(mats[0], outMat, cv::Size(_width, _height));
                for(int i = 1; i < mats.size(); i++) {
                    cv::resize(mats[i], resizeMat, cv::Size(_width, _height));
                    cv::bitwise_or(outMat, resizeMat, outMat);
                }
                cv::imshow(_name, outMat);
            }
        }
        catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    void renderMats(std::vector<cv::Mat> mats, float alpha) {
        int size = mats.size();
        if(size != 2) {
            std::cout << "only support the satuation with two mats!" << std::endl;
            return;
        }
        try {
            cv::Mat outMat, resizeMat;
            cv::resize(mats[0], outMat, cv::Size(_width, _height));
            cv::resize(mats[1], resizeMat, cv::Size(_width, _height));

            for(int i = 0; i < outMat.rows; i++) {
                for(int j = 0; j < outMat.cols; j++) {
                    cv::Vec3b &outRgb    = outMat.at<cv::Vec3b>(i, j);
                    cv::Vec3b &resizeRgb = resizeMat.at<cv::Vec3b>(i, j);

                    outRgb[0] = (uint8_t)(outRgb[0] * (1 - alpha) + resizeRgb[0] * alpha);
                    outRgb[1] = (uint8_t)(outRgb[1] * (1 - alpha) + resizeRgb[1] * alpha);
                    outRgb[2] = (uint8_t)(outRgb[2] * (1 - alpha) + resizeRgb[2] * alpha);
                }
            }
            cv::imshow(_name, outMat);
        }
        catch(std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }
};