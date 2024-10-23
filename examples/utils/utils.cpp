// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include "utils.hpp"
#include "utils_c.h"

#include <chrono>

namespace ob_smpl {
char waitForKeyPressed(uint32_t timeout_ms) {
    return ob_smpl_wait_for_key_press(timeout_ms);
}

uint64_t getNowTimesMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int getInputOption() {
    char inputOption = ob_smpl::waitForKeyPressed();
    if(inputOption == ESC_KEY) {
        return -1;
    }
    return inputOption - '0';
}

}  // namespace ob_smpl
