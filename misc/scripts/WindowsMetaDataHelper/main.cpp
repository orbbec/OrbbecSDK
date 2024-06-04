#include "metadata-helper.h"
#include "iostream"

/****
 * Notes:
 * 1. The code must be run with administrator privileges, and the PID and VID passed into the code must be in decimal format.
 * 2. If multiple different types of device(different PID) are used, the registration code must be called for the PID of each device.
 */
int main(void) {
    std::string pid = "1643";
    std::string vid = "11205";
    if(windows_metadata_helper::is_running_as_admin()) {
        windows_metadata_helper::is_enabled(pid, vid);
        windows_metadata_helper::enable_metadata(pid, vid);
        std::cout << "register metadata success" << std::endl;
    }
    return 0;
}