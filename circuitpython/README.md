# CircuitPython Examples for Adafruit Feather RP2040 ThinkInk

This directory contains CircuitPython scripts, libraries, and bitmap assets configured for the Adafruit Feather RP2040 ThinkInk.

## Setup Instructions

1. Install CircuitPython 9.x or newer on the Adafruit Feather RP2040 ThinkInk.
2. Connect the board via USB to mount the `CIRCUITPY` drive.
3. Copy the contents of `circuitpython/lib/` to the `lib/` directory on `CIRCUITPY`.
4. Copy the required `.bmp` asset files to the root of `CIRCUITPY`.
5. Rename the desired script to `code.py` on the `CIRCUITPY` drive to run it automatically on boot.

## Hardware Pin Definitions

In CircuitPython, the board module defines dedicated pin names for the 24-pin connector:

- SPI bus: `busio.SPI(board.EPD_SCK, MOSI=board.EPD_MOSI, MISO=None)`
- Chip select: `board.EPD_CS`
- Data / command: `board.EPD_DC`
- Reset: `board.EPD_RESET`
- Busy: `board.EPD_BUSY`
- Onboard NeoPixel: `board.NEOPIXEL`

Display bus initialization uses `FourWire`:

```python
import board
import busio
import displayio
from fourwire import FourWire

displayio.release_displays()
spi = busio.SPI(board.EPD_SCK, MOSI=board.EPD_MOSI, MISO=None)
display_bus = FourWire(spi, command=board.EPD_DC, chip_select=board.EPD_CS, reset=board.EPD_RESET, baudrate=1000000)
```

## Script Catalog

### Onboard NeoPixel Examples
- `rgblink.py`: Blinks the onboard NeoPixel through red and green cycles with manual `pixels.show()` calls.
- `neopixel_rgb.py`: Steps the onboard NeoPixel through primary colors (red, green, blue) at 500 ms intervals.
- `neopixel_colorwheel.py`: Cycles 256 colors smoothly across the NeoPixel using `rainbowio.colorwheel`.
- `neopixel_led_animation.py`: Runs a continuous rainbow animation using the `adafruit_led_animation` helper library.

### Display Examples
- `code.py`: Default entry script that prints a boot greeting to the serial console.
- `epd_simpletest.py`: Framebuffer drawing on 2.13" 4-color panels (JD79661) using `adafruit_epd.jd79661.Adafruit_JD79661`. Draws text and color blocks.
- `ssd1681_simpletest.py`: Displays `display-ruler.bmp` on a 200x200 tricolor panel using `adafruit_ssd1681.SSD1681`.
- `ssd1681_image.py`: Renders `mocha200x200.bmp` on a 200x200 tricolor panel using `adafruit_ssd1681.SSD1681`.
- `jd79661_simpletest.py`: Displays `display-ruler-640x360.bmp` cropped onto a 250x122 4-color panel using `adafruit_jd79661.JD79661`.
- `jd79661_image.py`: Renders `mocha250x122.bmp` onto a 250x122 4-color panel using `adafruit_jd79661.JD79661`.

## Asset Files

- `mocha200x200.bmp`: 200x200 3-color indexed bitmap for SSD1681 displays.
- `mocha250x122.bmp`: 250x122 4-color indexed bitmap for JD79661 displays.
- `display-ruler.bmp`: 200x200 coordinate alignment ruler.
- `display-ruler-640x360.bmp`: 640x360 coordinate alignment ruler.
- `blinka.bmp`: 200x200 CircuitPython mascot bitmap.
- `font5x8.bin`: Binary font table for custom text rendering.
- `settings.toml`: CircuitPython configuration file for environmental variables.
- `sd/`: Mount point directory structure for external SD card storage.

## Bundled Libraries (`lib/`)

- `adafruit_epd/`: Core driver package for low-level e-paper controllers.
- `adafruit_ssd1681.mpy`: Displayio driver for SSD1681-based tricolor displays.
- `adafruit_jd79661.mpy`: Displayio driver for JD79661-based 4-color displays.
- `adafruit_led_animation/`: Library for LED animations, color cycles, and chases.
- `neopixel.mpy`: Driver for the onboard WS2812 NeoPixel LED.
