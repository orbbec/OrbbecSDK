// Copyright (c) Orbbec Inc. All Rights Reserved.
// Licensed under the MIT License.

#include <libobsensor/ObSensor.hpp>

#include "utils.hpp"

#include <iostream>

int main() try {
    // Create a pipeline with default device.
    ob::Pipeline pipe;

    // Get the device from the pipeline.
    std::shared_ptr<ob::Device> device = pipe.getDevice();

    while(true){

        // Get preset list from device.
        std::shared_ptr<ob::DevicePresetList> presetLists = device->getAvailablePresetList();
        if (presetLists && presetLists->getCount() == 0) {
            std::cout << "The current device does not support preset mode" << std::endl;
            std::cout << "\nPress any key to exit.";
            ob_smpl::waitForKeyPressed();
            return 0;
        }

        std::cout << "Available Presets:" << std::endl;
        for(uint32_t index = 0; index < presetLists->getCount(); index++) {
            // Print available preset name.
            std::cout << " - " << index << "." << presetLists->getName(index) << std::endl;
        }

        // Print current preset name.
        std::cout << "Current PresetName: " << device->getCurrentPresetName() << std::endl;

        std::cout << "Enter index of preset to load: ";

        // Select preset to load.
        int  inputOption = ob_smpl::getInputOption();
        auto presetName = presetLists->getName(inputOption);

        // Load preset.
        device->loadPreset(presetName);

        // Print current preset name.
        std::cout << "Current PresetName: " << device->getCurrentPresetName() << std::endl;
    }

    // Stop Pipeline.
    pipe.stop();

    printf("\nProgram ended successfully. Press any key to exit.");
    ob_smpl::getInputOption();

    return 0;
}
catch(ob::Error &e) {
    std::cerr << "function:" << e.getFunction() << "\nargs:" << e.getArgs() << "\nmessage:" << e.what() << "\ntype:" << e.getExceptionType() << std::endl;
    std::cout << "\nPress any key to exit.";
    ob_smpl::waitForKeyPressed();
    exit(EXIT_FAILURE);
}

