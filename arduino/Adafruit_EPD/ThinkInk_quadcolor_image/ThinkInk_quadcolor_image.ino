#include "Adafruit_ThinkInk.h"
#include "mocha122x250.h"  // Generated 4-color bitmap of mocha dog

#ifdef ARDUINO_ADAFRUIT_FEATHER_RP2040_THINKINK // detects if compiling for
                                                // Feather RP2040 ThinkInk
#define EPD_DC PIN_EPD_DC       // ThinkInk 24-pin connector DC
#define EPD_CS PIN_EPD_CS       // ThinkInk 24-pin connector CS
#define EPD_BUSY PIN_EPD_BUSY   // ThinkInk 24-pin connector Busy
#define SRAM_CS -1              // use onboard RAM
#define EPD_RESET PIN_EPD_RESET // ThinkInk 24-pin connector Reset
#define EPD_SPI &SPI1           // secondary SPI for ThinkInk
#else
#define EPD_DC 10
#define EPD_CS 9
#define EPD_BUSY 7 // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 6
#define EPD_RESET 8  // can set to -1 and share with microcontroller Reset!
#define EPD_SPI &SPI // primary SPI
#endif

// 2.13" Quadcolor EPD with JD79661 chipset
ThinkInk_213_Quadcolor_AJHE5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// 3.52" Quadcolor EPD with JD79667 chipset
//ThinkInk_352_Quadcolor_AJHE5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

// Color mapping: 2-bit value to EPD color
// 0b00 = Black, 0b01 = White, 0b10 = Yellow, 0b11 = Red
const uint16_t colorMap[4] = {EPD_BLACK, EPD_WHITE, EPD_YELLOW, EPD_RED};

// Draw 4-color (2bpp) image from PROGMEM
void draw4ColorBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h) {
  int byteIndex = 0;
  for (int row = 0; row < h; row++) {
    for (int col = 0; col < w; col += 4) {
      uint8_t packedByte = pgm_read_byte(&bitmap[byteIndex++]);
      // Each byte contains 4 pixels, 2 bits each, MSB first
      for (int p = 0; p < 4 && (col + p) < w; p++) {
        uint8_t colorIdx = (packedByte >> (6 - p * 2)) & 0x03;
        display.drawPixel(x + col + p, y + row, colorMap[colorIdx]);
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  Serial.println("ThinkInk Image Example");
  display.begin(THINKINK_QUADCOLOR);
  display.setRotation(0);
  display.clearBuffer();
  draw4ColorBitmap(0, 0, image122x250, 122, 250);
  display.display();
}

void loop() {
  
}

