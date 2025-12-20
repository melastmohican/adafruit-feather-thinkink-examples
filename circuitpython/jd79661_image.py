import time

import board
import busio
import displayio
from fourwire import FourWire

import adafruit_jd79661

displayio.release_displays()

#pinout is for the Feather RP2040 ThinkInk
spi = busio.SPI(board.EPD_SCK, MOSI=board.EPD_MOSI, MISO=None)
epd_cs = board.EPD_CS
epd_dc = board.EPD_DC
epd_reset = board.EPD_RESET
epd_busy = board.EPD_BUSY
display_bus = FourWire(spi, command=epd_dc, chip_select=epd_cs, reset=epd_reset, baudrate=1000000)

display = adafruit_jd79661.JD79661(
    display_bus,
    width=250,
    height=122,
    busy_pin=epd_busy,
    rotation=90,
    colstart=0,
    highlight_color=0x00FF00,
    highlight_color2=0xFF0000,
)

pic = displayio.OnDiskBitmap("/mocha250x122.bmp")
t = displayio.TileGrid(pic, pixel_shader=pic.pixel_shader)

g = displayio.Group()
g.append(t)
display.root_group = g

display.auto_refresh = False
display.refresh()
print("refreshed")
time.sleep(display.time_to_refresh + 5)

# Always refresh a little longer. It's not a problem to refresh
# a few seconds more, but it's terrible to refresh too early
# (the display will throw an exception when if the refresh
# is too soon)
print("waited correct time")


# Keep the display the same
while True:
    time.sleep(10)
