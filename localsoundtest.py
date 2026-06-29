from pyvirtualdisplay.smartdisplay import SmartDisplay, autocrop
from subprocess import Popen

HOME = '/mnt/storage/Jeux'
PICO8_EXEC = f'{HOME}/pico-8/pico8'
command = [PICO8_EXEC, '-windowed', '0','-splore']

with SmartDisplay(backend="xvfb", size=(128, 128)) as display, Popen(command, env=display.env()) as process:
    while process.poll() is None: # continue while pico-8 is running
        pass
