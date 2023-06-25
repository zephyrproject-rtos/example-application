#!/bin/bash

# Check if the script is called by the specific user
USER_NAME=$(whoami)
if [ "$USER_NAME" != "mainUser" ]; then
    echo "This script can only be executed in an active docker container. Expected USER=mainUser instead got USER=$USER_NAME"
    exit 1
fi

SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)

# File path to be overwritten
manifest_file_path="/home/mainUser/zephyrproject/.west/config"

# Get the folder name
project_name=$(basename "$(basename "$(dirname "$SCRIPT_DIR")")")

# Multiline string content with folder name
manifest_string="[manifest]
path = $project_name
file = west.yml

[zephyr]
base = zephyr"

# Overwrite the file with the multiline string
echo "$manifest_string" > "$manifest_file_path"

# update west to use the new manifest file.
west update

# enable west autocompletion in the dev container
echo 'eval "$(west completion bash)"' >> ~/.bashrc