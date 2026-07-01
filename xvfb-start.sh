#!/usr/bin/env bash

DISPLAY_NUM="99"
RESOLUTION="128x128x24"
PICO8_PATH="/home/pico/pico-8/pico8_64 -splore"

# Start a Xvfb
Xvfb :${DISPLAY_NUM} -screen 0 ${RESOLUTION} +extension GLX +render -noreset &

echo "Waiting for Xvfb display :${DISPLAY_NUM} to initialize..."
while [ ! -S "/tmp/.X11-unix/X${DISPLAY_NUM}" ]; do
    sleep 0.1
done
echo "Xvfb is ready!"

# Run pico-8 inside Xvfb
env DISPLAY=:${DISPLAY_NUM} ${PICO8_PATH} &

# Start the grabber
./xvfb-grab

# Keep script running to let you kill everything with Ctrl+C
trap 'kill $(jobs -p)' EXIT
wait