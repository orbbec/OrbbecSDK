#pragma once

#include <string>

// Helper class that is responsible for enabling per-frame metadata on different platforms
// Currently implemented for Windows
class windows_metadata_helper {
public:
    static bool is_enabled(std::string pid, std::string vid);

    static bool is_running_as_admin();

    static void enable_metadata(std::string pid, std::string vid);
};