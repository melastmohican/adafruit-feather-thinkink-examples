# Examples for Adafruit RP2040 Feather ThinkInk for 24-pin E-Paper Displays

## Adafruit Feather RP2040 ThinkINK
https://www.adafruit.com/product/5727 \
https://www.adafruit.com/product/6373 (ZJY122250-0213AJH-E5) \
https://www.adafruit.com/product/6383 (SSD1680Z) \
https://www.adafruit.com/product/6395 (UC8253) \
https://www.good-display.com/product/436.html (GDEM0154Z90 - SSD1681) \
https://www.seeedstudio.com/2-13-Quadruple-Color-ePaper-Display-with-122x250-Pixels-p-5779.html (GDEY0213F51 - JD79661) \
https://www.waveshare.com/1.54inch-e-paper-g.htm (Waveshare 1.54inch e-Paper (G) / GDEM0154F51H - JD79660) \
https://www.waveshare.com/3.7inch-e-paper-g.htm (Waveshare 3.7inch e-Paper (G) / GDEM037F51 - IST7163) \
https://www.good-display.com/product/462.html (GDEW0215T12 - UC8151D) \
https://www.good-display.com/product/555.html (GDEM0154F61H - SSD2681) \
https://www.good-display.com/product/463.html (GDEY0213F52 - JD79676A) \
https://www.good-display.com/product/501.html (GDEY0266T90H - SSD1685) \
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

### Waveshare 1.54inch e-Paper (G) / GDEM0154F51H
- **Link to page:** https://www.waveshare.com/1.54inch-e-paper-g.htm
- **Link to Wiki:** https://www.waveshare.com/wiki/1.54inch_e-Paper_Module_(G)
- **SKU:** 30441 (FPC-8101)
- **Colors:** black, white, yellow, red (4 native colors)
- **Resolution:** 200×200 · 2 bits per pixel
- **Full Refresh supported:** Yes (~20 s)
- **Fast Refresh supported:** Yes (~12-15 s)
- **Partial Refresh supported:** No
- **IC driver:** JD79660AA
- **Source code:** https://github.com/waveshareteam/e-Paper
- **GxEPD2 support/driver:** [GxEPD2_154c_GDEM0154F51H](https://github.com/ZinggJM/GxEPD2/blob/master/src/epd4c/GxEPD2_154c_GDEM0154F51H.h)
- **Good Display reference:** [GDEM0154F51H](https://www.good-display.com/product/534.html)
- **Sketches:**
  - `arduino/Waveshare_1in54g/` (Waveshare vendor driver)
  - `arduino/Adafruit_EPD/ThinkInk_Waveshare_1in54g/` (Adafruit_EPD `ThinkInk_154_Quadcolor_Waveshare`)
  - `arduino/GxEPD2/EPD/GDEM0154F51H/Demo/` (GxEPD2 `GxEPD2_154c_GDEM0154F51H`)
  - `arduino/good_display/GDEM0154F51H/` (Good Display vendor sample)


### Waveshare 3.7inch e-Paper (G) / GDEM037F51
- **Link to page:** https://www.waveshare.com/3.7inch-e-paper-g.htm
- **SKU:** 31065 (FPC-2303)
- **Colors:** black, white, yellow, red (4 native colors)
- **Resolution:** 240×416 · 2 bits per pixel
- **Full Refresh supported:** Yes (~20 s)
- **Fast Refresh supported:** Yes (~12-15 s)
- **Partial Refresh supported:** No
- **IC driver:** IST7163
- **Source code:** https://github.com/waveshareteam/e-Paper
- **GxEPD2 support/driver:** Not in GxEPD2 upstream; local class `GxEPD2_370c_GDEM037F51` in the demo folder
- **Sketches:**
  - `arduino/Waveshare_3in7g/` (Waveshare vendor driver)
  - `arduino/GxEPD2/EPD/GDEM037F51/Demo/` (GxEPD2 with local `GxEPD2_370c_GDEM037F51`)
  - `arduino/good_display/GDEM037F51/` (Good Display vendor sample)

### Good Display 2.15inch e-Paper (monochrome) / GDEW0215T12 (formerly GDEW0215T11)
- **Link to page:** https://www.good-display.com/product/462.html
- **FPC Marking:** `WFT0215CZA4`
- **Colors:** black, white
- **Resolution:** 208×112 · 1 bit per pixel
- **Full Refresh supported:** Yes (~3 s)
- **Partial Refresh supported:** Yes (~0.5 s)
- **IC driver:** UC8151D `[C]`
- **Datasheet:** [GDEW0215T12 Specification](https://v4.cecdn.yun300.cn/100001_1909185148/GDEW0215T12-new.pdf) · [IC Driver UC8151D](https://v4.cecdn.yun300.cn/100001_1909185148/UC8151D.pdf)
- **GxEPD2 support/driver:** Not in GxEPD2 upstream; local class `GxEPD2_215_GDEW0215T12` in demo folder
- **Good Display reference:** [GDEW0215T12](https://www.good-display.com/product/462.html)
- **Good Display source code:** https://www.good-display.com/product/462.html
- **Rust embedded driver:** [uc8151](https://crates.io/crates/uc8151) ([GitHub](https://github.com/9names/uc8151)) · [epd-waveshare](https://crates.io/crates/epd-waveshare) ([GitHub](https://github.com/rust-embedded-community/epd-waveshare))
- **Sketches:**
  - `arduino/Adafruit_EPD/ThinkInk_GDEW0215T12/` (Adafruit_EPD `ThinkInk_215_Mono_GDEW0215T12`)
  - `arduino/GxEPD2/EPD/GDEW0215T12/Demo/` (GxEPD2 with local `GxEPD2_215_GDEW0215T12`)
  - `arduino/good_display/GDEW0215T12/` (Good Display vendor sample)

WFT0215CZA4 HZ 2546

### Good Display 1.54inch 4-Color e-Paper / GDEM0154F61H
- **Link to page:** https://www.good-display.com/product/555.html
- **FPC Marking:** `FPC-8101`
- **Colors:** black, white, yellow, red (4 native colors)
- **Resolution:** 200×200 · 2 bits per pixel
- **Full Refresh supported:** Yes (~20 s)
- **Fast Refresh supported:** Yes (~12-15 s)
- **Partial Refresh supported:** No
- **IC driver:** SSD2681
- **Datasheet:** [GDEM0154F61H Specification](https://v4.cecdn.yun300.cn/100001_1909185148/GDEM0154F61H.pdf) · [IC Driver SSD2681](https://v4.cecdn.yun300.cn/100001_1909185148/SSD2681.pdf)
- **GxEPD2 support/driver:** Not in GxEPD2 upstream; local class `GxEPD2_154c_GDEM0154F61H` in demo folder
- **Good Display reference:** [GDEM0154F61H](https://www.good-display.com/product/555.html)
- **Good Display source code:** https://www.good-display.com/product/555.html
- **Sketches:**
  - `arduino/Adafruit_EPD/ThinkInk_GDEM0154F61H/` (Adafruit_EPD `ThinkInk_154_Quadcolor_GDEM0154F61H`)
  - `arduino/GxEPD2/EPD/GDEM0154F61H/Demo/` (GxEPD2 with local `GxEPD2_154c_GDEM0154F61H`)
  - `arduino/good_display/GDEM0154F61H/` (Good Display vendor sample)

### Good Display 2.13inch 4-Color e-Paper / GDEY0213F52
- **Link to page:** https://www.good-display.com/product/463.html
- **FPC Marking:** `FPC-J002`
- **Colors:** black, white, yellow, red (4 native colors)
- **Resolution:** 250×122 (128×250 native RAM, 122 visible) · 2 bits per pixel
- **Full Refresh supported:** Yes (~11 s)
- **Partial Refresh supported:** No
- **IC driver:** JD79676A `[C]`
- **Datasheet:** [GDEY0213F52 Specification](https://v4.cecdn.yun300.cn/100001_1909185148/GDEY0213F52_Specification.pdf) · [IC Driver JD79676A](https://v4.cecdn.yun300.cn/100001_1909185148/JD79676A.pdf)
- **GxEPD2 support/driver:** Not in GxEPD2 upstream; local class `GxEPD2_213c_GDEY0213F52` in demo folder (needs a 40 s BUSY timeout)
- **Adafruit_EPD support:** Not upstream; local class `ThinkInk_213_Quadcolor_GDEY0213F52` subclassing `Adafruit_JD79661` with Good Display's short init (`0xE9 0x01`, `0x04`)
- **Rust embedded driver:** None published for JD79676A that I found
- **Good Display reference:** [GDEY0213F52](https://www.good-display.com/product/463.html)
- **Good Display source code:** [AU-GDEY0213F52-20240827.rar](https://v4.cecdn.yun300.cn/100001_1909185148/AU-GDEY0213F52-20240827.rar)
- **Sketches:**
  - `arduino/Adafruit_EPD/ThinkInk_GDEY0213F52/` (Adafruit_EPD `ThinkInk_213_Quadcolor_GDEY0213F52`)
  - `arduino/GxEPD2/EPD/GDEY0213F52/Demo/` (GxEPD2 with local `GxEPD2_213c_GDEY0213F52`)
  - `arduino/good_display/GDEY0213F52/` (Good Display vendor sample)

### Good Display 2.66inch e-Paper (monochrome, 360×184) / GDEY0266T90H
- **Link to page:** https://www.good-display.com/product/501.html
- **FPC Marking:** `FPC-H011`
- **Colors:** black, white
- **Resolution:** 360×184 (184×360 native RAM) · 1 bit per pixel
- **Full Refresh supported:** Yes (~2 s)
- **Fast Refresh supported:** Yes (~1.5 s and ~1.0 s)
- **Partial Refresh supported:** Yes (~0.4 s)
- **IC driver:** SSD1685 `[C]`
- **Datasheet:** [GDEY0266T90H Specification](https://v4.cecdn.yun300.cn/100001_1909185148/GDEY0266T90H.pdf) · [IC Driver SSD1685](https://v4.cecdn.yun300.cn/100001_1909185148/SSD1685.pdf)
- **GxEPD2 support/driver:** Not in GxEPD2 upstream; local class `GxEPD2_266_GDEY0266T90H` in demo folder, derived from upstream `GxEPD2_290_GDEY029T71H` (same SSD1685)
- **Adafruit_EPD support:** Not upstream; local class `ThinkInk_266_Mono_GDEY0266T90H` subclassing `Adafruit_SSD1680`
- **Rust embedded driver:** None published for SSD1685 that I found
- **Notes:** Set `0x21` source resolution to 184 (`B[7:6]=01`); the power-on default is 200 and shows a noise strip along one edge. GxEPD2 uses RAM entry mode `0x03`; Good Display's own `0x01` shows a mirrored image.
- **Good Display reference:** [GDEY0266T90H](https://www.good-display.com/product/501.html)
- **Good Display source code:** [AU-GDEY0266T90H-2FP-20230915.rar](https://v4.cecdn.yun300.cn/100001_1909185148/AU-GDEY0266T90H-2FP-20230915.rar)
- **Sketches:**
  - `arduino/Adafruit_EPD/ThinkInk_GDEY0266T90H/` (Adafruit_EPD `ThinkInk_266_Mono_GDEY0266T90H`)
  - `arduino/GxEPD2/EPD/GDEY0266T90H/Demo/` (GxEPD2 with local `GxEPD2_266_GDEY0266T90H`)
  - `arduino/good_display/GDEY0266T90H/` (Good Display vendor sample)
