#!/bin/env bash

set -xe

if [ "$EUID" -ne 0 ]
  then echo "Script must be run as root."
  exit
fi

ln -s ./pico8-arcade.service /etc/systemd/system/pico8-arcade.service
sudo systemctl daemon-reload
sudo systemctl enable pico8-arcade.service
sudo systemctl start pico8-arcade.service