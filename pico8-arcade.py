from pyvirtualdisplay.smartdisplay import SmartDisplay, autocrop
from subprocess import Popen
import adafruit_blinka_raspberry_pi5_piomatter as piomatter
import time
import numpy as np

# setup pico-8 execution
PICO8_EXEC = '/home/luclement/Jeux/pico-8/pico8'
command = [PICO8_EXEC, '-windowed', '0','-splore']

geometry = piomatter.Geometry(
    width=64, # width of the board in pixels
    height=64, # height of the board in pixels
    n_addr_lines=5, # number of address lines used (ABCDE), usually all five used for 64x64 boards
    n_planes=10, # color depth -> bits per color
    n_temporal_planes=0, # temporal dithering
    rotation=piomatter.Orientation.Normal, # orientation of the board 
    serpentine=True # tells how the panels are connected. Here it doesn't matter bc single panel.
)

framebuffer = np.zeros(shape=(64, 64, 3), dtype=np.uint8)

matrix = piomatter.PioMatter(
    colorspace=piomatter.Colorspace.RGB888Packed, # format of the pixel data
    pinout=piomatter.Pinout.AdafruitMatrixHatBGR, # tells which pins are connected to the connector
    framebuffer=framebuffer, # where the data to put on display is (nparray)
    geometry=geometry # tells the driver the physical layout of the led matrix
)

# create an environment inside xvfb
with SmartDisplay(backend="xvfb", size=(128, 128)) as display, Popen(command, env=display.env()) as process:
    while process.poll() is None: # continue while pico-8 is running
        img = display.grab(autocrop=True)
        if img is None: continue
        img = img.resize((64, 64))

        # show on LED Matrix
        # framebuffer[:, :] = np.array(img) # update memory that will be displayed
        # matrix.show()
        # time.sleep(1/30)

        # show in image viewer
        img.show()
        time.sleep(2)
