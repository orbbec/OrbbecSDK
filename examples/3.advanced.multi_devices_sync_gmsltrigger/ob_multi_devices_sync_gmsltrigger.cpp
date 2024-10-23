// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

/*
Notes: MultiDeviceSyncGmslSocTrigger for GMSL device
on the nvidia arm64 xavier/orin platform ,this sample use nvidia platform soc pwm trigger to sync multi device.
*/
#ifdef __linux__
#include <libobsensor/ObSensor.hpp>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <string>

static const char *  DEVICE_PATH = "/dev/camsync";
static const uint8_t WRITE_MODE  = 1;
static const uint8_t READ_MODE   = 0;

typedef struct {
    uint8_t  mode;
    uint16_t fps;
} cs_param_t;

class PwmTrigger {
public:
    PwmTrigger() : fd(-1), isDeviceOpen(false) {}
    ~PwmTrigger() {
        closeDevice();
    }
    int startTrigger(uint16_t triggerFps) {
        if(!isDeviceOpen) {
            if(openDevice() < 0) {
                error("open device Failed!");
                return -1;
            }
        }
        cs_param_t wt_param = { WRITE_MODE, triggerFps };
        int        ret      = writeTriggerParam(wt_param);
        if(ret < 0) {
            error("write trigger parameter Failed!");
            return ret;
        }
        info("write param: ", wt_param);

        cs_param_t rd_param = { READ_MODE, 0 };
        ret                 = readTriggerParam(rd_param);
        if(ret < 0) {
            error("read trigger parameter Failed!");
        }
        info("read param: ", rd_param);

        return ret;
    }

    int stopTrigger() {
        int ret = 0;
        if(isDeviceOpen) {
            ret = closeDevice();
        }
        return ret;
    }

    void info(const std::string &msg, const cs_param_t &param) {
        std::cout << msg << " mode=" << static_cast<int>(param.mode) << ", fps=" << param.fps << std::endl;
    }
    void error(const std::string &msg) {
        std::cerr << "Error: " << msg << std::endl;
    }

private:
    int openDevice() {
        fd = open(DEVICE_PATH, O_RDWR);
        if(fd < 0) {
            error("open /dev/camsync failed");
            return fd;
        }
        isDeviceOpen = true;
        return fd;
    }
    int closeDevice() {
        if(isDeviceOpen) {
            isDeviceOpen = false;
            int ret      = close(fd);
            fd           = -1;
            if(ret < 0) {
                error("close /dev/camsync failed: " + std::to_string(errno));
                return ret;
            }
        }
        return 0;
    }
    int writeTriggerParam(const cs_param_t &param) {
        int ret = write(fd, &param, sizeof(param));
        if(ret < 0) {
            error("write /dev/camsync failed: " + std::to_string(errno));
        }
        return ret;
    }
    int readTriggerParam(cs_param_t &param) {
        int ret = read(fd, &param, sizeof(param));
        if(ret < 0) {
            error("read /dev/camsync failed: " + std::to_string(errno));
        }
        return ret;
    }

private:
    int  fd;
    bool isDeviceOpen;
};

int main(void) try {
    PwmTrigger                pwmTrigger;
    uint16_t                  fps            = 0;
    constexpr std::streamsize maxInputIgnore = 10000;
    while(true) {
        std::cout << "Please select options: \n"
                  << "------------------------------------------------------------\n"
                  << " 0 --> config GMSL SOC PWM trigger Source. Set trigger fps: \n"
                  << " 1 --> start Trigger \n"
                  << " 2 --> stop Trigger \n"
                  << " 3 --> exit \n"
                  << "------------------------------------------------------------\n"
                  << "input select item: ";
        int index = -1;
        // std::cin >> index;
        if(!(std::cin >> index)) {
            std::cin.clear();
            std::cin.ignore(maxInputIgnore, '\n');
            std::cout << "Invalid input. Please enter a number." << std::endl;
            continue;
        }
        std::cout << std::endl;

        switch(index) {
        case 0:
            std::cout << "Enter FPS (frames per second) (for example: 3000): ";
            std::cin >> fps;  // set the FPS here
            std::cout << "Setting FPS to " << fps << "..." << std::endl;
            break;
        case 1:
            if(pwmTrigger.startTrigger(fps) < 0) {
                std::cerr << "Failed to start trigger." << std::endl;
            }
            std::cout << "start pwm source TriggerSync... \n" << std::endl;
            break;
        case 2:
            pwmTrigger.stopTrigger();
            std::cout << "stop pwm source TriggerSync... \n" << std::endl;
            break;
        case 3:
            pwmTrigger.stopTrigger();
            std::cout << "Program exit & clse device! \n" << std::endl;
            return 0;
        default:
            std::cout << "-input Invalid index. \n"
                      << "-Please re-select and input valid param. \n";
        }
    }
    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    exit(EXIT_FAILURE);
}

#endif
