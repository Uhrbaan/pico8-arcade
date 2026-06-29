#!/usr/bin/env bash

# --- Configuration ---
DISPLAY_NUM="99"
RESOLUTION="128x128x24"
PICO8_PATH="/home/pico/pico-8/pico8_64 -splore"

# 1. Start the virtual hidden display
Xvfb :${DISPLAY_NUM} -screen 0 ${RESOLUTION} +extension GLX +render -noreset &

echo "Waiting for Xvfb display :${DISPLAY_NUM} to initialize..."
while [ ! -S "/tmp/.X11-unix/X${DISPLAY_NUM}" ]; do
    sleep 0.1
done
echo "Xvfb is ready!"

# 2. Force PICO-8 to run completely inside display :99 and ignore your desktop desktop
env DISPLAY=:${DISPLAY_NUM} ${PICO8_PATH} &

sleep 2

# 3. Start the xvfb grabber to display
./xvfb-grab

# Keep script running to let you kill everything with Ctrl+C
trap 'kill $(jobs -p)' EXIT
wait