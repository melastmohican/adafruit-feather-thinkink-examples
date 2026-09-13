# Examples for Adafruit RP2040 Feather ThinkInk for 24-pin E-Paper Displays

## Adafruit Feather RP2040 ThinkINK
https://www.adafruit.com/product/5727 \
https://www.adafruit.com/product/6373 (ZJY122250-0213AJH-E5) \
https://www.adafruit.com/product/6383 (SSD1680Z) \
https://www.adafruit.com/product/6395 (UC8253) \
https://www.good-display.com/product/436.html (GDEM0154Z90 - SSD1681) \
https://www.seeedstudio.com/2-13-Quadruple-Color-ePaper-Display-with-122x250-Pixels-p-5779.html (GDEY0213F51 - JD79661) \
https://learn.adafruit.com/adafruit-rp2040-feather-thinkink \
https://github.com/adafruit/Adafruit-Feather-RP2040-ThinkInk \
https://github.com/earlephilhower/arduino-pico/blob/master/variants/adafruit_feather_thinkink/pins_arduino.h \
https://github.com/adafruit/awesome-feather \
https://github.com/adafruit/Fritzing-Library/blob/master/parts/Adafruit%20Feather%20RP2040%20ThinkInk.fzpz \
https://github.com/adafruit/Adafruit_EPD \
https://learn.adafruit.com/bare-e-ink-displays-crash-course \
https://github.com/ZinggJM/GxEPD2 \
[Tri-Color e ink display 1.54 inch e-ink small display screen, GDEM0154Z90](https://www.good-display.com/product/436.html)

### 13. 2.66inch e-Paper (B)
- **Link to page:** https://www.waveshare.com/2.66inch-e-Paper-B.htm
- **Link to Wiki:** http://www.waveshare.com/wiki/2.66inch_e-Paper_Module_(B)
- **Link to Manual:** http://www.waveshare.com/wiki/2.66inch_e-Paper_Module_(B)_Manual
- **Colors:** red, black, white
- **Resolution:** 296×152 · grayscale levels: 2
- **Full Refresh supported:** Yes (15 s)
- **Partial Refresh supported:** No
- **IC driver:** SSD1680  `[C]`
- **Source code:** https://github.com/waveshareteam/e-Paper/blob/master/RaspberryPi_JetsonNano/c/lib/e-Paper/EPD_2in66b.c
- **Datasheet:** https://files.waveshare.com/upload/e/ec/2.66inch-e-paper-b-specification.pdf
- **GxEPD2 support/driver:** [GxEPD2_266c](https://github.com/ZinggJM/GxEPD2/blob/master/src/epd3c/GxEPD2_266c.h) (SSD1680)
- **Good Display reference:** [GDEY0266Z90](https://www.good-display.com/product/430.html)
- **Good Display source code:** https://www.good-display.com/product/430.html (demo code on product page)
- **Rust embedded driver:** [epd-waveshare](https://crates.io/crates/epd-waveshare) ([GitHub](https://github.com/rust-embedded-community/epd-waveshare)) · [ssd1680](https://crates.io/crates/ssd1680) ([GitHub](https://github.com/mbv/ssd1680)) · [epd-datafuri](https://crates.io/crates/epd-datafuri) ([GitHub](https://github.com/ScottCUSA/magtag_esp_hal))

DEPG0266RWS800F34HP N2405P10213-01-32043-1

### 2.66inch e-Paper (monochrome) / GDEY0266T90
- **Link to page:** https://www.waveshare.com/2.66inch-e-paper.htm
- **Link to Wiki:** https://www.waveshare.com/wiki/2.66inch_e-Paper_Module
- **Colors:** black, white (supports 4-level grayscale)
- **Resolution:** 296×152 · grayscale levels: 4
- **Full Refresh supported:** Yes (~2 s)
- **Fast Refresh supported:** Yes (1.0 s - 1.5 s)
- **Partial Refresh supported:** Yes (~0.5 s)
- **IC driver:** SSD1680
- **Source code:** https://github.com/waveshareteam/e-Paper
- **GxEPD2 support/driver:** [GxEPD2_266_GDEY0266T90](https://github.com/ZinggJM/GxEPD2/blob/master/src/epd/GxEPD2_266_GDEY0266T90.h)
- **Good Display reference:** [GDEY0266T90](https://www.good-display.com/product/389.html)
- **Sketches:**
  - `arduino/Waveshare_2in66/` (Waveshare vendor driver)
  - `arduino/Adafruit_EPD/ThinkInk_Waveshare_2in66/` (Adafruit_EPD `ThinkInk_266_Grayscale4_MFGN`)
  - `arduino/GxEPD2/EPD/GDEY0266T90/Demo/` (GxEPD2 `GxEPD2_266_GDEY0266T90`)
  - `arduino/good_display/GDEY0266T90/` (Good Display vendor sample)

### Waveshare 3.52inch e-Paper (B)
- **Link to page:** https://www.waveshare.com/3.52inch-e-paper-hat-b.htm
- **Link to Wiki:** https://www.waveshare.com/wiki/3.52inch_e-Paper_HAT_(B)
- **Link to Manual:** https://www.waveshare.com/wiki/3.52inch_e-Paper_HAT_(B)_Manual
- **Colors:** red, black, white
- **Resolution:** 360×240 · grayscale levels: 2
- **Full Refresh supported:** Yes (16 s)
- **Partial Refresh supported:** No
- **IC driver:** UC8253
- **Source code:** https://github.com/waveshareteam/e-Paper/blob/master/E-paper_Separate_Program/3in52_e-Paper_B
- **Datasheet:** https://files.waveshare.com/wiki/3.52inch%20e-Paper%20HAT%20(B)/3.52inch-e-Paper_(B)-user-manual.pdf
- **GxEPD2 support/driver:** Not in GxEPD2
- **Good Display reference:** —
- **Good Display source code:** —
- **Rust embedded driver:** [epd-waveshare](https://crates.io/crates/epd-waveshare) ([GitHub](https://github.com/rust-embedded-community/epd-waveshare)) Currently not supported until PR https://github.com/rust-embedded-community/epd-waveshare/pull/255 is merged


3.52 inch N14
Q7GE30419NP3L0033

