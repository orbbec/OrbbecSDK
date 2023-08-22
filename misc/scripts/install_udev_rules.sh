#!/bin/bash

# Check if user is root/running with sudo
if [ "$(id -u)" != "0" ]; then
    echo "Please run this script with sudo"
    exit 1
fi

# Determine the script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ "$(uname -s)" != "Darwin" ]; then
    # Install UDEV rules for USB device
    UDEV_RULES_FILE="/etc/udev/rules.d/99-obsensor-libusb.rules"
    cp "${SCRIPT_DIR}/99-obsensor-libusb.rules" "${UDEV_RULES_FILE}"
    echo "USB rules file installed at ${UDEV_RULES_FILE}"
fi

echo "Script execution complete"
