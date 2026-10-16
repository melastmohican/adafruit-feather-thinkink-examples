# Arduino Examples for Adafruit Feather RP2040 ThinkInk

This directory contains Arduino sketches organized by library and display manufacturer for the Adafruit Feather RP2040 ThinkInk board.

## Hardware Configuration

The Adafruit Feather RP2040 ThinkInk routes its dedicated 24-pin FPC e-paper connector to secondary hardware SPI (`SPI1`). All sketches use the board variant pin definitions from the `arduino-pico` core.

### 24-Pin FPC Connector Pinout
- `PIN_EPD_CS`: GPIO19
- `PIN_EPD_DC`: GPIO18
- `PIN_EPD_RESET`: GPIO17
- `PIN_EPD_BUSY`: GPIO16
- `PIN_EPD_SCK`: GPIO22 (`SPI1` SCK)
- `PIN_EPD_MOSI`: GPIO23 (`SPI1` TX)
- `PIN_EPD_MISO`: GPIO20 (`SPI1` RX, unpopulated on 24-pin ribbon)

### Onboard Peripherals
- Status LED (`LED_BUILTIN`): GPIO13
- NeoPixel Data (`PIN_NEOPIXEL`): GPIO20
- NeoPixel Power (`NEOPIXEL_POWER`): GPIO21 (set HIGH to supply 3.3V to the pixel)
- STEMMA QT / Qwiic I2C bus: SDA on GPIO12, SCL on GPIO13
- Primary SPI bus (Feather header pins): SCK GPIO18, MOSI GPIO19, MISO GPIO20 (distinct from the EPD connector pins)

## Compilation Instructions

Use `arduino-cli` with the Earle Philhower `arduino-pico` core:

```bash
# Board FQBN
arduino-cli compile -b rp2040:rp2040:adafruit_feather_thinkink <path_to_sketch>

# Example: compile GxEPD2 demo for GDEY0266T90
arduino-cli compile -b rp2040:rp2040:adafruit_feather_thinkink arduino/GxEPD2/EPD/GDEY0266T90/Demo

# Example: compile Adafruit_EPD demo for GDEQ0426T82
arduino-cli compile -b rp2040:rp2040:adafruit_feather_thinkink arduino/Adafruit_EPD/ThinkInk_GDEQ0426T82_MacPaint
```

## Directory Structure

### 1. Board Bringup
- `Blink/`: Toggles the red status LED on GPIO13 at 1 Hz.
- `RGBlink/`: Powers the onboard NeoPixel via GPIO21 and cycles RGB colors on GPIO20.

### 2. Adafruit_EPD (`arduino/Adafruit_EPD/`)
Sketches using the Adafruit_EPD library and custom subclasses that adapt unreleased panels to Adafruit's GFX-compatible API. Full details: [Adafruit_EPD/README.md](file:///Users/mordor/Src/adafruit-feather-thinkink-examples/arduino/Adafruit_EPD/README.md).

- `graphicstest/`: Shapes, lines, and text benchmark on 1.54" tricolor panels (`ThinkInk_154_Tricolor_Z90`).
- `ThinkInk_tricolor/`: Red/black/white banner and rectangle test pattern (`ThinkInk_154_Tricolor_Z90`).
- `ThinkInk_tricolor_image/`: Full-screen tricolor bitmap display.
- `ThinkInk_quadcolor/`: 4-color banner and shapes on 2.13" JD79661 panels (`ThinkInk_213_Quadcolor_AJHE5`).
- `ThinkInk_quadcolor_image/`: 4-color bitmap image renderer.
- `ThinkInk_Waveshare_1in54g/`: Waveshare 1.54" 4-color display via custom class `ThinkInk_154_Quadcolor_Waveshare`.
- `ThinkInk_Waveshare_2in66/`: Waveshare 2.66" monochrome panel with 4-level grayscale (`ThinkInk_266_Grayscale4_MFGN`).
- `ThinkInk_Waveshare_2in66b/`: Waveshare 2.66" red/black/white panel (`ThinkInk_266_Tricolor_Z90`).
- `ThinkInk_Waveshare_3in52/`: Waveshare 3.52" red/black/white panel (`ThinkInk_352_Tricolor_Z40`).
- `ThinkInk_GDEW0215T12/`: Good Display 2.15" monochrome panel via custom class `ThinkInk_215_Mono_GDEW0215T12`.
- `ThinkInk_GDEM0154F61H/`: Good Display 1.54" 4-color panel via custom class `ThinkInk_154_Quadcolor_GDEM0154F61H`.
- `ThinkInk_GDEY0213F52/`: Good Display 2.13" 4-color panel via custom class `ThinkInk_213_Quadcolor_GDEY0213F52`.
- `ThinkInk_GDEY0266T90H/`: Good Display 2.66" high-resolution (360x184) monochrome panel via custom class `ThinkInk_266_Mono_GDEY0266T90H`.
- `ThinkInk_GDEQ0426T82_MacPaint/`: Good Display 4.26" monochrome 800x480 benchmark with upstream `ThinkInk_426_Grayscale4_GDEQ`.

### 3. GxEPD2 (`arduino/GxEPD2/`)
Sketches using Jean-Marc Zingg's GxEPD2 library, configured for RP2040 `SPI1` with both full-frame buffering and paged rendering. Full details: [GxEPD2/README.md](file:///Users/mordor/Src/adafruit-feather-thinkink-examples/arduino/GxEPD2/README.md).

- `GxEPD2_ThinkInk_TriColor/`: Generic 3-color test template.
- `GxEPD2_ThinkInk_TriColor_Image/`: 3-color bitmap template.
- `GxEPD2_ThinkInk_QuadColor/`: Generic 4-color test template.
- `GxEPD2_ThinkInk_Image/`: Monochrome bitmap template.
- `EPD/GDEH0154Z90/Demo/`: 1.54" 200x200 tricolor (SSD1681).
- `EPD/GDEM0154F51H/Demo/`: Waveshare 1.54" 200x200 4-color (JD79660).
- `EPD/GDEM0154F61H/Demo/`: Good Display 1.54" 200x200 4-color (SSD2681) using local driver class.
- `EPD/GDEM0213B74/Demo/`: Adafruit 6383 2.13" 250x122 monochrome (SSD1680).
- `EPD/GDEY0213F51/Demo/`: 2.13" 250x122 4-color (JD79661).
- `EPD/GDEY0213F52/Demo/`: Good Display 2.13" 250x122 4-color (JD79676A) using local driver class.
- `EPD/GDEW0215T12/Demo/`: Good Display 2.15" 208x112 monochrome (UC8151D) using local driver class.
- `EPD/GDEY0266T90/Demo/`: Waveshare 2.66" 296x152 monochrome (SSD1680).
- `EPD/GDEY0266T90H/Demo/`: Good Display 2.66" 360x184 monochrome (SSD1685) using local driver class.
- `EPD/GDEY0266Z90/Demo/`: Waveshare 2.66" 296x152 tricolor (SSD1680).
- `EPD/GDEY037T03/Demo/`: Adafruit 6395 3.7" 240x416 monochrome (SSD1677).
- `EPD/GDEM037F51/Demo/`: Waveshare 3.7" 240x416 4-color (IST7163) using local driver class.
- `EPD/GDEQ0426T82/MacPaint/`: 4.26" 800x480 handwriting partial update demo with trailing-dot redraw.

### 4. Good Display Direct Register Drivers (`arduino/good_display/`)
Vendor-provided bare-metal C++ drivers adapted for the Feather RP2040 ThinkInk pinout and `SPI1` bus. Full details: [good_display/README.md](file:///Users/mordor/Src/adafruit-feather-thinkink-examples/arduino/good_display/README.md).

- `GDEM0154F51H/`: 1.54" 4-color (JD79660).
- `GDEM0154F61H/`: 1.54" 4-color (SSD2681).
- `GDEM037F51/`: 3.7" 4-color (IST7163).
- `GDEQ0426T82_MacPaint/`: 4.26" monochrome (SSD1677) 48 KB canvas buffering and 18-byte partial window streaming.
- `GDEW0215T12/`: 2.15" monochrome (UC8151D).
- `GDEY0213F52/`: 2.13" 4-color (JD79676A).
- `GDEY0266T90/`: 2.66" monochrome (SSD1680).
- `GDEY0266T90H/`: 2.66" 360x184 monochrome (SSD1685).
- `GDEY0266Z90/`: 2.66" tricolor (SSD1680).

### 5. Waveshare Vendor Drivers (`arduino/`)
Ported Waveshare Arduino library examples adapted to use `SPI1` and `PIN_EPD_*` pin mappings:

- `Waveshare_1in54g/`: 1.54" 200x200 4-color vendor driver (JD79660). Full refresh (~20 s) and fast refresh (~15 s).
- `Waveshare_2in66/`: 2.66" 296x152 monochrome vendor driver (SSD1680). Full refresh (~2 s) and partial refresh (~0.5 s).
- `Waveshare_2in66b_PanelID/`: 2.66" tricolor vendor driver with OTP panel ID readout over SPI.
- `Waveshare_2in66br/`: 2.66" tricolor vendor driver demonstrating text, shapes, and bitmap images.
- `Waveshare_3in52/`: 3.52" 360x240 tricolor vendor driver (UC8253).
- `Waveshare_3in52_PanelID/`: 3.52" tricolor vendor driver with OTP panel ID query.
- `Waveshare_3in7g/`: 3.7" 240x416 4-color vendor driver (IST7163). Full refresh (~20 s) and fast refresh (~15 s).
