#!/usr/bin/env bash

# --- Configuration ---
DISPLAY_NUM="99"
RESOLUTION="128x128x24"
PICO8_PATH="/home/luclement/Jeux/pico-8/pico8 /home/luclement/Projets/pico8-arcade/test.p8"

# 1. Start the virtual hidden display
Xvfb :${DISPLAY_NUM} -screen 0 ${RESOLUTION} +extension GLX +render -noreset &

# 2. Force PICO-8 to run completely inside display :99 and ignore your desktop desktop
env DISPLAY=:${DISPLAY_NUM} ${PICO8_PATH} &

# Keep script running to let you kill everything with Ctrl+C
trap 'kill $(jobs -p)' EXIT
wait