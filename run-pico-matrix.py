# inspired by <https://github.com/adafruit/Adafruit_Blinka_Raspberry_Pi5_Piomatter/blob/main/examples/virtualdisplay.py>
from pyvirtualdisplay.smartdisplay import SmartDisplay
import numpy as np
from subprocess import Popen

PICO8_EXEC = '/home/luclement/Jeux/pico-8/pico8'
command = f'{PICO8_EXEC} -splore -windowed 0'

def main():
    framebuffer = np.zeros(shape=(128, 128, 3), dtype=np.uint8)

    with SmartDisplay(backend='xvfb', use_xauth=False, size=(128, 128), manage_global_env=False) as disp, Popen(command, env=disp.env()) as proc:
            while proc.poll() is None:
                img = disp.grab(autocrop=False)

                if img is None:
                    continue
                framebuffer[:, :] = np.array(img)
                matrix.show()
