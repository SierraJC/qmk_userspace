#!/usr/bin/env bash

set -eEuo pipefail

wget https://bootstrap.pypa.io/get-pip.py
python3 get-pip.py
python3 -m pip install qmk
rm get-pip.py

python3 -m pip install --upgrade milc

userspacePath="$1"
qmkHome="$userspacePath/qmk_firmware"

git config --global --add safe.directory "$userspacePath"
git submodule update --init --recursive
git config --global --add safe.directory "$qmkHome"

qmk config user.qmk_home="$qmkHome"
qmk config user.overlay_dir="$userspacePath"

qmk git-submodule
