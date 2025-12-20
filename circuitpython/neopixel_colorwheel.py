import board
import neopixel
from rainbowio import colorwheel
import time

pixels = neopixel.NeoPixel(board.NEOPIXEL, 1) # Or change '1' to the number of pixels
pixels.brightness = 0.3 # Keep it lower initially
i = 0 # Counter for colorwheel

while True:
    pixels.fill(colorwheel(i)) # Fill with the color from the wheel
    pixels.show()
    i = (i + 1) % 256 # Cycle through 0-255
    time.sleep(0.05) # Adjust for speed (smaller = faster)
