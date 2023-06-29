#!/bin/bash

latest_version=$(git tag --list | grep -E '^v([0-9]+\.){1,3}[0-9]+$' | sort -V | tail -n 1)

# If no versions found, set it to v0.0.0
if [ -z "$latest_version" ]; then
    latest_version="v0.0.0"
fi

echo $latest_version
