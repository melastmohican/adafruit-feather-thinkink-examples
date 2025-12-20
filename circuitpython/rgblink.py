import time
import board
import neopixel

pixels = neopixel.NeoPixel(board.NEOPIXEL, 1, brightness=0.3, auto_write=False)
# brightness is 0.0 to 1.0 (e.g., 0.3 is 30%)
# auto_write=False means you need to call pixels.show()

# --- Main Loop ---
while True:
    # Turn on Red
    pixels.fill((255, 0, 0)) # RGB for Red
    pixels.show()
    time.sleep(0.5) # Wait for half a second

    # Turn off (black)
    pixels.fill((0, 0, 0))
    pixels.show()
    time.sleep(0.5) # Wait for half a second

    # Turn on Green (optional)
    pixels.fill((0, 255, 0)) # RGB for Green
    pixels.show()
    time.sleep(0.5)

    # Turn off again
    pixels.fill((0, 0, 0))
    pixels.show()
    time.sleep(0.5)
