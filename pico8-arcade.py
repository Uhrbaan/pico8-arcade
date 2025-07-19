from pyvirtualdisplay.smartdisplay import SmartDisplay, autocrop
from subprocess import Popen
from rgbmatrix import RGBMatrix, RGBMatrixOptions
import time
from PIL import Image

# setup pico-8 execution
PICO8_EXEC = '/home/pico/Projects/pico8-arcade/pico-8/pico8'
PICO8_CART = '/home/pico/Projects/pico8-arcade/test.p8.png'
command = [PICO8_EXEC, '-windowed', '0','-run', PICO8_CART]

options = RGBMatrixOptions()
options.rows = 64 
options.cols = 64
options.chain_length = 1 # For a single 64x64 panel
options.parallel = 1
options.hardware_mapping = 'adafruit-hat' # Or 'regular' or 'rpi-pwm' etc. based on wiring
options.gpio_slowdown = 4 # Adjust as needed for flicker vs CPU usage
options.disable_hardware_pulsing = False # Only set True if you cannot disable audio

matrix = RGBMatrix(options = options)

img_matrix = Image.new("RGB", (options.cols, options.rows))
graphics = matrix.CreateFrameCanvas() # Create a canvas for drawing

# create an environment inside xvfb
with SmartDisplay(backend="xvfb", size=(128, 128)) as display, Popen(command, env=display.env()) as process:
    while process.poll() is None: # continue while pico-8 is running
        img = display.grab(autocrop=False)
        if img is None: continue
        img = img.crop((0, 0, 64, 64))

        # show on LED Matrix
        graphics.SetImage(img)
        matrix.SwapOnVSync(graphics) # Display the frame
        # time.sleep(1/30)
