// Copyright(c) 2020 Orbbec Corporation. All Rights Reserved.
#pragma once

#include <libobsensor/ObSensor.hpp>
#include <opencv2/opencv.hpp>
#include <string>

extern "C" {
#include <libobsensor/h/Error.h>
#include <libobsensor/h/Frame.h>
#include <libobsensor/h/ObTypes.h>
#include <libobsensor/h/Pipeline.h>
#include <libobsensor/h/StreamProfile.h>
}
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
    Window(std::string name, int width, int height) : _name(name), _width(width), _height(height), _windowClose(false), _key(-1) {}

    ~Window() {}

    void resize(int width, int height) {
        _width  = width;
        _height = height;
    }

    void render(std::vector<ob_frame *> frames, RenderType renderType) {
        _key = cv::waitKey(10);
        if(_key == ESC) {
            _windowClose = true;
        }
        if(_windowClose) {
            cv::destroyAllWindows();
            return;
        }

        auto mats = processFrames(frames);
        renderMats(mats, renderType);
    }

    inline int getKey() {
        return _key;
    }

    operator bool() {
        return !_windowClose;
    }

private:
    std::string _name;
    int         _width;
    int         _height;
    bool        _windowClose;
    int         _key;

    std::vector<cv::Mat> processFrames(std::vector<ob_frame *> frames) {
        std::vector<cv::Mat> mats;
        ob_error *           error = NULL;
        void *               data;
        uint32_t             dataSize;
        ob_frame_type        type;
        ob_format            format;
        uint32_t             width;
        uint32_t             height;
        uint8_t              pixelAvailabelBitSize;
        for(auto frame: frames) {
            if(frame == nullptr || ob_frame_data_size(frame, &error) < 1024) {
                return mats;
            }
            data     = ob_frame_data(frame, &error);
            dataSize = ob_frame_data_size(frame, &error);
            type     = ob_frame_get_type(frame, &error);
            format   = ob_frame_format(frame, &error);
            width    = ob_video_frame_width(frame, &error);
            height   = ob_video_frame_height(frame, &error);

            if(type != OB_FRAME_COLOR) {
                pixelAvailabelBitSize = ob_video_frame_pixel_available_bit_size(frame, &error);
            }
            if(type == OB_FRAME_COLOR && format == OB_FORMAT_MJPG) {
                cv::Mat rawMat(1, dataSize, CV_8UC1, data);
                cv::Mat colorMat = cv::imdecode(rawMat, 1);
                mats.push_back(colorMat);
            }
            else if(type == OB_FRAME_COLOR && format == OB_FORMAT_UYVY) {
                cv::Mat rawMat(height, width, CV_8UC2, data);
                cv::Mat rstMat;
                cv::cvtColor(rawMat, rstMat, cv::COLOR_YUV2BGR_UYVY);
                mats.push_back(rstMat);
            }
            else if(type == OB_FRAME_COLOR && format == OB_FORMAT_RGB) {
                cv::Mat rawMat(height, width, CV_8UC3, data);
                cv::Mat colorMat;
                cv::cvtColor(rawMat, colorMat, cv::COLOR_RGB2BGR);
                mats.push_back(colorMat);
            }
            else if(type == OB_FRAME_DEPTH && (format == OB_FORMAT_YUYV || format == OB_FORMAT_YUY2 || format == OB_FORMAT_Y16)) {
                cv::Mat rawMat = cv::Mat(height, width, CV_16UC1, data);
                cv::Mat cvtMat, depthMat;
                float   scale = 1.0f / pow(2, pixelAvailabelBitSize - 10);
                cv::convertScaleAbs(rawMat, cvtMat, scale);
                cv::cvtColor(cvtMat, depthMat, cv::COLOR_GRAY2RGB);
                mats.push_back(depthMat);
            }
            else if(is_ir_frame(type) && (format == OB_FORMAT_YUYV || format == OB_FORMAT_YUY2 || format == OB_FORMAT_Y16)) {
                cv::Mat rawMat = cv::Mat(height, width, CV_16UC1, data);
                cv::Mat cvtMat, irMat;
                float   scale = 1.0f / pow(2, pixelAvailabelBitSize - 8);
                cv::convertScaleAbs(rawMat, cvtMat, scale);
                cv::cvtColor(cvtMat, irMat, cv::COLOR_GRAY2RGB);
                mats.push_back(irMat);
            }
            if(is_ir_frame(type) && format == OB_FORMAT_Y8) {
                cv::Mat rawMat = cv::Mat(height, width, CV_8UC1, data);
                cv::Mat irMat;
                cv::cvtColor(rawMat, irMat, cv::COLOR_GRAY2RGB);
                mats.push_back(irMat);
            }
            if(is_ir_frame(type) && format == OB_FORMAT_MJPG) {
                cv::Mat rawMat(1, dataSize, CV_8UC1, data);
                cv::Mat irMat = cv::imdecode(rawMat, 1);
                mats.push_back(irMat);
            }
        }
        return mats;
    }

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
                    cv::resize(mat, resizeMat, cv::Size(_width / mats.size(), _height / mats.size()));
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
                    cv::resize(mat, resizeMat, cv::Size(_width / mats.size(), _height / mats.size()));
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
                            mat = cv::Mat(_width / cols, _height / rows, CV_8UC3);
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
                cv::imshow(_name, outMat);
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
};