# Adafruit_EPD Examples for Adafruit Feather RP2040 ThinkInk

This directory contains Adafruit_EPD sketches configured for the Adafruit Feather RP2040 ThinkInk board and 24-pin ribbon e-paper panels.

## Hardware and SPI Configuration

The Adafruit_EPD library connects to the 24-pin FPC connector using secondary SPI (`SPI1`) and the board package pin definitions:

- `EPD_CS`: `PIN_EPD_CS` (GPIO19)
- `EPD_DC`: `PIN_EPD_DC` (GPIO18)
- `EPD_RESET`: `PIN_EPD_RESET` (GPIO17)
- `EPD_BUSY`: `PIN_EPD_BUSY` (GPIO16)
- `EPD_SPI`: `&SPI1` (SCK on GPIO22, MOSI on GPIO23)
- `SRAM_CS`: `-1` (uses the RP2040's 264 KB internal SRAM for display buffers)

## Sketch Catalog

### Standard Demos
- `graphicstest/`: Shapes, grid lines, text rendering, and screen clearing on 1.54" tricolor panels (`ThinkInk_154_Tricolor_Z90`, SSD1681).
- `ThinkInk_tricolor/`: Red/black/white test pattern featuring text banners, filled color rectangles, and sample text (`ThinkInk_154_Tricolor_Z90`).
- `ThinkInk_tricolor_image/`: Renders a 200x200 3-color bitmap image (`ThinkInk_154_Tricolor_Z90`).
- `ThinkInk_quadcolor/`: 4-color test pattern in red, yellow, black, and white on 2.13" panels (`ThinkInk_213_Quadcolor_AJHE5`, JD79661).
- `ThinkInk_quadcolor_image/`: Renders a 250x122 4-color bitmap image (`ThinkInk_213_Quadcolor_AJHE5`).

### Waveshare Panel Drivers
- `ThinkInk_Waveshare_1in54g/`: Waveshare 1.54" 200x200 4-color display (JD79660). Implements local class `ThinkInk_154_Quadcolor_Waveshare` subclassing `Adafruit_JD79661`.
- `ThinkInk_Waveshare_2in66/`: Waveshare 2.66" 296x152 monochrome panel (SSD1680). Uses `ThinkInk_266_Grayscale4_MFGN` supporting 4-level grayscale drawing.
- `ThinkInk_Waveshare_2in66b/`: Waveshare 2.66" 296x152 red/black/white panel (SSD1680). Uses `ThinkInk_266_Tricolor_Z90`.
- `ThinkInk_Waveshare_3in52/`: Waveshare 3.52" 360x240 red/black/white panel (UC8253). Uses `ThinkInk_352_Tricolor_Z40`.

### Good Display Custom Panel Drivers
These sketches implement custom subclass drivers inside the sketch file for panels not yet available in upstream Adafruit_EPD releases:

- `ThinkInk_GDEW0215T12/`: Good Display 2.15" 208x112 monochrome panel (UC8151D). Uses local class `ThinkInk_215_Mono_GDEW0215T12` subclassing `Adafruit_UC8151D`. Supports fast partial refresh.
- `ThinkInk_GDEM0154F61H/`: Good Display 1.54" 200x200 4-color panel (SSD2681). Uses local class `ThinkInk_154_Quadcolor_GDEM0154F61H` subclassing `Adafruit_SSD1681` with custom 4-color command sequences.
- `ThinkInk_GDEY0213F52/`: Good Display 2.13" 250x122 4-color panel (JD79676A). Uses local class `ThinkInk_213_Quadcolor_GDEY0213F52` subclassing `Adafruit_JD79661` with a 40-second busy timeout.
- `ThinkInk_GDEY0266T90H/`: Good Display 2.66" 360x184 monochrome panel (SSD1685). Uses local class `ThinkInk_266_Mono_GDEY0266T90H` subclassing `Adafruit_SSD1680`. Sets the 0x21 register source resolution to 184 to avoid edge noise artifacts.
- `ThinkInk_GDEQ0426T82_MacPaint/`: Good Display 4.26" 800x480 monochrome panel (SSD1677). Implements the MacPaint handwriting benchmark using upstream `ThinkInk_426_Grayscale4_GDEQ`. Compares full-frame buffer retransmissions (~1.5 s per point) against direct partial window streaming.

## Required Libraries

Install via the Arduino Library Manager:
- `Adafruit EPD` (version 4.5.0 or newer)
- `Adafruit GFX Library` (version 1.11.0 or newer)
- `Adafruit BusIO`
