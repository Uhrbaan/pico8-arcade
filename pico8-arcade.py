from pyvirtualdisplay.smartdisplay import SmartDisplay, autocrop
from subprocess import Popen
from rgbmatrix import RGBMatrix, RGBMatrixOptions
from PIL import Image, ImageEnhance

HOME = '/home/pico'

# setup pico-8 execution
PICO8_EXEC = f'{HOME}/pico-8/pico8_64'
PICO8_CART = f'{HOME}/pico8-arcade/test.p8.png'
#command = [PICO8_EXEC, '-windowed', '0','-run', PICO8_CART]
command = [PICO8_EXEC, '-windowed', '0','-splore']

options = RGBMatrixOptions()
options.rows = 64
options.cols = 64
options.chain_length = 4 # For a single 64x64 panel
options.parallel = 1
options.hardware_mapping = 'adafruit-hat' # Or 'regular' or 'rpi-pwm' etc. based on wiring
options.gpio_slowdown = 3 # Adjust as needed for flicker vs CPU usage
options.disable_hardware_pulsing = True # Only set True if you cannot disable audio
options.brightness = 60

matrix = RGBMatrix(options = options)

width = options.cols * options.chain_length
height = options.rows

display = Image.new("RGB", (width, height))
graphics = matrix.CreateFrameCanvas() # Create a canvas for drawing

# create an environment inside xvfb
with SmartDisplay(backend="xvfb", size=(128, 128)) as display, Popen(command, env=display.env()) as process:
    while process.poll() is None: # continue while pico-8 is running
        img = display.grab(autocrop=False)
        if img is None: continue

        enhancer = ImageEnhance.Contrast(img)
        enhancer.enhance(3)

        upper = img.crop((0, 0, 128, 64))
        lower = img.crop((0, 64, 128, 128))

        display.paste(lower, (0, 0))
        display.paste(upper, (128, 0))


        graphics.SetImage(display)
        matrix.SwapOnVSync(graphics) # Display the frame
        # time.sleep(1/30)
