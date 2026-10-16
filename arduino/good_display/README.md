# Good Display Direct Register Drivers for Adafruit Feather RP2040 ThinkInk

This directory contains bare-metal C++ sample drivers adapted from Good Display reference code for the Adafruit Feather RP2040 ThinkInk.

## Hardware and SPI Configuration

Each example includes low-level SPI files (`Display_EPD_W21_spi.h` and `Display_EPD_W21_spi.cpp`) that route commands and image buffers directly through RP2040 secondary SPI (`SPI1`):

- `EPD_W21_CS`: `PIN_EPD_CS` (GPIO19)
- `EPD_W21_DC`: `PIN_EPD_DC` (GPIO18)
- `EPD_W21_RST`: `PIN_EPD_RESET` (GPIO17)
- `EPD_W21_BUSY`: `PIN_EPD_BUSY` (GPIO16)
- SPI bus pins: SCK on GPIO22, MOSI on GPIO23

Before issuing SPI commands, the driver remaps `SPI1` pins and calls `SPI1.begin()`:

```cpp
SPI1.setSCK(PIN_EPD_SCK);
SPI1.setTX(PIN_EPD_MOSI);
SPI1.begin();
```

## Panel Catalog

- `GDEM0154F51H/`: 1.54" 200x200 4-color panel (JD79660 controller). Demonstrates 4-color image arrays and display refresh. Full refresh takes ~20 s, fast refresh takes ~12 s to 15 s.
- `GDEM0154F61H/`: 1.54" 200x200 4-color panel (SSD2681 controller). Uses proprietary 4-color register initializations. Full refresh takes ~20 s, fast refresh takes ~12 s to 15 s.
- `GDEM037F51/`: 3.7" 240x416 4-color panel (IST7163 controller). Demonstrates full-frame 4-color image rendering. Full refresh takes ~20 s.
- `GDEQ0426T82_MacPaint/`: 4.26" 800x480 monochrome panel (SSD1677 controller). Implements the MacPaint handwriting benchmark using a 48 KB canvas buffer in RP2040 SRAM. Employs direct 18-byte partial window streaming over SPI1 to achieve ~0.45 s per dot without ghosting.
- `GDEW0215T12/`: 2.15" 208x112 monochrome panel (UC8151D controller). Demonstrates full refresh (~3 s) and fast partial window updates (~0.5 s).
- `GDEY0213F52/`: 2.13" 250x122 4-color panel (JD79676A controller). Demonstrates 4-color image display with 11-second refresh sequence.
- `GDEY0266T90/`: 2.66" 296x152 monochrome panel (SSD1680 controller). Demonstrates full refresh (~2 s), fast refresh (~1.0 s to 1.5 s), and partial refresh (~0.5 s).
- `GDEY0266T90H/`: 2.66" 360x184 high-resolution monochrome panel (SSD1685 controller). Sets register 0x21 to 184 source lines to prevent noise borders. Demonstrates full refresh (~2 s) and partial refresh (~0.4 s).
- `GDEY0266Z90/`: 2.66" 296x152 red/black/white panel (SSD1680 controller). Demonstrates full 3-color refresh (~15 s).
