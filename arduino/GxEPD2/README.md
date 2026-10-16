# GxEPD2 Examples for Adafruit Feather RP2040 ThinkInk

This directory contains examples using Jean-Marc Zingg's GxEPD2 library, adapted for the Adafruit Feather RP2040 ThinkInk and bare 24-pin ribbon panels.

## Hardware and SPI Configuration

The RP2040 routes the 24-pin connector to `SPI1`. Before calling `display.init()`, sketches configure the secondary SPI bus and bind it to GxEPD2:

```cpp
SPI1.setSCK(PIN_EPD_SCK);   // GPIO22
SPI1.setTX(PIN_EPD_MOSI);   // GPIO23
SPI1.begin();
display.epd2.selectSPI(SPI1, SPISettings(4000000, MSBFIRST, SPI_MODE0));
```

The RP2040 provides 264 KB of SRAM. Most sketches instantiate full-buffer display classes (e.g. `GxEPD2_BW<..., GxEPD2_...::HEIGHT>`), eliminating paging overhead for panels up to 3.7 inches. For larger panels such as the 4.26-inch 800x480 GDEQ0426T82, paged rendering or constrained buffer heights keep memory within limits.

## Sketch Catalog

### General Templates (Root Directory)
- `GxEPD2_ThinkInk_TriColor/`: Generic 3-color (red, black, white) test harness for Feather RP2040 ThinkInk. Demonstrates fonts, geometric primitives, and full screen updates.
- `GxEPD2_ThinkInk_TriColor_Image/`: Tricolor bitmap renderer reading packed pixel arrays from flash memory.
- `GxEPD2_ThinkInk_QuadColor/`: Generic 4-color (red, yellow, black, white) test harness with color bar patterns.
- `GxEPD2_ThinkInk_Image/`: Monochrome bitmap rendering template.

### Panel-Specific Implementations (`EPD/`)
Each subfolder contains a targeted `Demo/` sketch configured for a specific panel. Folders containing unreleased driver classes include local header files:

- `GDEH0154Z90/Demo/`: 1.54" 200x200 3-color panel (SSD1681). Uses upstream `GxEPD2_154_Z90c`.
- `GDEM0154F51H/Demo/`: Waveshare 1.54" 200x200 4-color panel (JD79660). Uses upstream `GxEPD2_154c_GDEM0154F51H`.
- `GDEM0154F61H/Demo/`: Good Display 1.54" 200x200 4-color panel (SSD2681). Uses local driver class `GxEPD2_154c_GDEM0154F61H.h`.
- `GDEM0213B74/Demo/`: Adafruit 6383 bare display, 2.13" 250x122 monochrome (SSD1680). Uses upstream `GxEPD2_213_B74`.
- `GDEY0213F51/Demo/`: Seeed / Good Display 2.13" 250x122 4-color panel (JD79661). Uses upstream `GxEPD2_213c_GDEY0213F51`.
- `GDEY0213F52/Demo/`: Good Display 2.13" 250x122 4-color panel (JD79676A). Uses local driver class `GxEPD2_213c_GDEY0213F52.h` with a 40-second busy timeout.
- `GDEW0215T12/Demo/`: Good Display 2.15" 208x112 monochrome panel (UC8151D). Uses local driver class `GxEPD2_215_GDEW0215T12.h`.
- `GDEY0266T90/Demo/`: Waveshare 2.66" 296x152 monochrome panel (SSD1680). Uses upstream `GxEPD2_266_GDEY0266T90`.
- `GDEY0266T90H/Demo/`: Good Display 2.66" 360x184 high-resolution monochrome panel (SSD1685). Uses local driver class `GxEPD2_266_GDEY0266T90H.h`.
- `GDEY0266Z90/Demo/`: Waveshare 2.66" 296x152 red/black/white panel (SSD1680). Uses upstream `GxEPD2_266c`.
- `GDEY037T03/Demo/`: Adafruit 6395 bare display, 3.7" 240x416 monochrome (SSD1677). Uses upstream `GxEPD2_370_GDEY037T03`.
- `GDEM037F51/Demo/`: Waveshare 3.7" 240x416 4-color panel (IST7163). Uses local driver class `GxEPD2_370c_GDEM037F51.h`.
- `GDEQ0426T82/MacPaint/`: Good Display 4.26" 800x480 monochrome panel (SSD1677). Implements the MacPaint handwriting partial update demo with trailing-dot redraw inside the paged loop to eliminate ghosting (~0.5 s per dot).

## Required Libraries

Install via the Arduino Library Manager:
- `GxEPD2` (version 1.5.0 or newer)
- `Adafruit GFX Library` (version 1.11.0 or newer)
