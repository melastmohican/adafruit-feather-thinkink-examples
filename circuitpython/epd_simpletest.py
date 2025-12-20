import board
import busio
import digitalio
import displayio

from adafruit_epd.epd import Adafruit_EPD
from adafruit_epd.jd79661 import Adafruit_JD79661

displayio.release_displays()

#pinout is for the Feather RP2040 ThinkInk
spi = busio.SPI(board.EPD_SCK, MOSI=board.EPD_MOSI, MISO=None)
cs = digitalio.DigitalInOut(board.EPD_CS)
dc = digitalio.DigitalInOut(board.EPD_DC)
srcs = None  # can be None to use internal memory
rst = digitalio.DigitalInOut(board.EPD_RESET) 
busy =  digitalio.DigitalInOut(board.EPD_BUSY)

print("Creating display")
display = Adafruit_JD79661(
    122, 250,
    spi,
    cs_pin=cs,
    dc_pin=dc,
    sramcs_pin=None,  
    rst_pin=rst,
    busy_pin=busy,
)

display.rotation = 1
if type(display) == Adafruit_JD79661:
    WHITE = Adafruit_JD79661.WHITE
    BLACK = Adafruit_JD79661.BLACK
    RED = Adafruit_JD79661.RED
    YELLOW = Adafruit_JD79661.YELLOW
else:
    WHITE = Adafruit_EPD.WHITE
    BLACK = Adafruit_EPD.BLACK
    RED = Adafruit_EPD.RED

# clear the buffer
print("Clear buffer")
display.fill(WHITE)
display.pixel(10, 100, BLACK)

print("Draw Rectangles")
display.fill_rect(5, 5, 10, 10, RED)
display.rect(0, 0, 20, 30, BLACK)

print("Draw lines")
if type(display) == Adafruit_JD79661:
    display.line(0, 0, display.width - 1, display.height - 1, YELLOW)
    display.line(0, display.height - 1, display.width - 1, 0, YELLOW)
else:
    display.line(0, 0, display.width - 1, display.height - 1, BLACK)
    display.line(0, display.height - 1, display.width - 1, 0, RED)

print("Draw text")
display.text("hello world", 25, 10, BLACK)
display.display()
