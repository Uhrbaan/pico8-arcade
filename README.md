# Pico8-arcade
> Note: the code of pico8-arcade.py is mostly inspired from [this](https://github.com/adafruit/Adafruit_Blinka_Raspberry_Pi5_Piomatter/blob/main/examples/virtualdisplay.py), adapted to work on the older rpi4b.

## Inspiration
The idea came from this [post on the lexaloffle forums](https://www.lexaloffle.com/bbs/?tid=45501), and parts of the code and parts came from Adafruit's guide on the [LED Matrix Wall Arcade for Pico-8](https://learn.adafruit.com/led-matrix-wall-arcade/overview).

## Parts
- Raspberry Pi 4B (with an sd card)
- 4 P3(2121)64x64-32S-

## Installation


```sh
# Dowload pico-8 and unzip it
cd
unzip pico-8_0.2.6b_raspi.zip

# Install the rgbmatrix python package on system.
cd
git clone https://github.com/hzeller/rpi-rgb-led-matrix.git
cd rpi-rgb-led-matrix
make build-python PYTHON=$(which python3)
sudo make install-python PYTHON=$(which python3)

# Clone this repository and make the python environment
cd
git clone https://github.com/Uhrbaan/pico8-arcade.git
cd pico8-arcade
python3 -m venv .env --system-site-packages
source .env/bin/activate
pip install pyvirtualdisplay pillow

# You can then simply run the pyhton script.
python3 pico8-arcade.py
```
