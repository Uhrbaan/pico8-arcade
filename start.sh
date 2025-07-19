#!/bin/zsh

set -xe

# virtual display identifier
DISPLAY_NUM=8

# pico8 executable 
PICO8_EXEC='/home/luclement/Jeux/pico-8/pico8'

# run the xvfb container in the background
xvfb-run --server-num=$DISPLAY_NUM --server-args="-screen 0 128x128x24" -- bash -c "$PICO8_EXEC" & 

# run the program to show the content of the virtual frame buffer onto the LED matrix
python3 ./run-pico-matrix.py $DISPLAY_NUM

# close (or kill I don't care) all processes
killall 'xvfb-run'
killall 'pico8'
killall 'Xvfb'
