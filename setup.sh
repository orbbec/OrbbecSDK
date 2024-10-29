#!/bin/bash

# Copyright (c) Orbbec Inc. All Rights Reserved.
# Licensed under the MIT License.

# restore current directory
current_dir=$(pwd)

# cd to the directory where this script is located
cd "$(dirname "$0")"
project_dir=$(pwd)

# set up environment variables
shared_dir=$project_dir/scripts
examples_dir=$project_dir/examples

# build the examples
if [ -d "$examples_dir" ] && [ -f "$project_dir/build_examples.sh" ]; then
    $project_dir/build_examples.sh
fi

# install udev rules
if [ -d "$shared_dir" ] && [ -f "$shared_dir/install_udev_rules.sh" ]; then
    $shared_dir/install_udev_rules.sh
fi

cd $current_dir
