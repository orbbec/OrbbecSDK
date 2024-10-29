// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#pragma once
#include <stdint.h>
#include "utils_types.h"

#include <sstream>

namespace ob_smpl {
char waitForKeyPressed(uint32_t timeout_ms = 0);

uint64_t getNowTimesMs();

int getInputOption();

template <typename T> std::string toString(const T a_value, const int n = 6) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return std::move(out).str();
}
}  // namespace ob_smpl
