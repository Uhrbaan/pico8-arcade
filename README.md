# Pico8-arcade
> Note: the code of pico8-arcade.py is mostly inspired from [this](https://github.com/adafruit/Adafruit_Blinka_Raspberry_Pi5_Piomatter/blob/main/examples/virtualdisplay.py), adapted to work on the older rpi4b.

## Inspiration
The idea came from this [post on the lexaloffle forums](https://www.lexaloffle.com/bbs/?tid=45501), and parts of the code and parts came from Adafruit's guide on the [LED Matrix Wall Arcade for Pico-8](https://learn.adafruit.com/led-matrix-wall-arcade/overview).

## Parts
- Raspberry Pi 4B (with an sd card)
- 4 P3(2121)64x64-32S-

## Setup
The following commands are supposed to make the program work with a fresh installation of rasbian lite on the raspberry pi 4b.

```sh
# Install necessary packages
sudo apt update
sudo apt upgrade
sudo apt install git cython3 xvfb libsdl2-dev wget
```

```sh
# Dowload pico-8 and unzip it
cd
unzip pico-8_0.2.6b_raspi.zip
```

```sh
# Install the rgbmatrix python package on system.
cd
git clone https://github.com/hzeller/rpi-rgb-led-matrix.git
cd rpi-rgb-led-matrix
make build-python PYTHON=$(which python3)
sudo make install-python PYTHON=$(which python3)
```

```sh
# Clone this repository and make the python environment
cd
git clone https://github.com/Uhrbaan/pico8-arcade.git
cd pico8-arcade
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install pyvirtualdisplay pillow
```

```sh
# You can then simply run the pyhton script.
# sudo /home/pico/pico8-arcade/.env/bin/python3 /home/pico/pico8-arcade/pico8-arcade.py
```

```sh
# Start the script at the boot of the pi
sudo ln -s /home/pico/pico8-arcade/pico8-arcade.service /etc/systemd/system/pico8-arcade.service
sudo systemctl daemon-reload
sudo systemctl enable pico8-arcade.service
sudo systemctl start pico8-arcade.service
```

```sh
# You can check on the service with
systemctl status pico8-arcade.service
journalctl -u pico8-arcade.service --since "10 minutes ago" --no-pager
```