import board
import neopixel
from adafruit_led_animation.animation.rainbow import Rainbow
import adafruit_led_animation.helper as helper

pixels = neopixel.NeoPixel(board.NEOPIXEL, 1, auto_write=False) # Use auto_write=False for animations
rainbow = Rainbow(pixels, speed=0.1, period=2) # Speed and period control the look

while True:
    rainbow.animate() # Runs the rainbow cycle
    pixels.show()
