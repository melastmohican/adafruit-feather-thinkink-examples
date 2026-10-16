/*****************************************************************************
* | File        :   ThinkInk_GDEQ0426T82_MacPaint.ino
* | Function    :   Drive Good Display GDEQ0426T82 with Adafruit_EPD (MacPaint Demo)
* | Info        :
*----------------
* Port of upiir's Seeed ePaper Partial Update "hello" animation
* for the 4.26" 800x480 Monochrome E-Paper Panel (GDEQ0426T82 / SSD1677)
* using Adafruit_EPD (ThinkInk_426_Grayscale4_GDEQ).
*
* Note: Custom MacPaint partial update demo.
* In Adafruit_SSD1677, displayPartial() retransmits the full 48 KB RAM buffer
* twice over SPI per update, which takes ~1.5 seconds per point.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   Good Display GDEQ0426T82, 800x480, SSD1677 controller
* Connection  :   Feather RP2040 ThinkInk onboard 24-pin FPC connector
*
* Pinout (ThinkInk 24-pin connector):
*   RST  -> PIN_EPD_RESET (GPIO17)
*   CS   -> PIN_EPD_CS    (GPIO19)
*   BUSY -> PIN_EPD_BUSY  (GPIO16, Active HIGH on SSD1677: 1 = busy, 0 = idle)
*   DC   -> PIN_EPD_DC    (GPIO18)
*   SCK  -> PIN_EPD_SCK   (GPIO22, SPI1)
*   MOSI -> PIN_EPD_MOSI  (GPIO23, SPI1)
******************************************************************************/
#include "Adafruit_ThinkInk.h"
#include "image.h"
#include "positions.h"

// Pin mapping for Adafruit Feather RP2040 ThinkInk
#ifdef ARDUINO_ADAFRUIT_FEATHER_RP2040_THINKINK
#define EPD_DC PIN_EPD_DC       // ThinkInk 24-pin connector DC
#define EPD_CS PIN_EPD_CS       // ThinkInk 24-pin connector CS
#define EPD_BUSY PIN_EPD_BUSY   // ThinkInk 24-pin connector Busy
#define SRAM_CS -1              // use onboard RAM
#define EPD_RESET PIN_EPD_RESET // ThinkInk 24-pin connector Reset
#define EPD_SPI &SPI1           // secondary SPI for ThinkInk
#else
#define EPD_DC 10
#define EPD_CS 9
#define EPD_BUSY 7
#define SRAM_CS 6
#define EPD_RESET 8
#define EPD_SPI &SPI
#endif

ThinkInk_426_Grayscale4_GDEQ display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY, EPD_SPI);

#define MACPAINT_X_OFFSET  80
#define MACPAINT_Y_OFFSET  0
#define MACPAINT_WIDTH     640
#define MACPAINT_HEIGHT    480

const int16_t BRUSH_RADIUS = 4;
int16_t animIndex = 0;
bool initialRenderDone = false;

void drawMacPaintBackground()
{
  Serial.println(F("[Adafruit] Performing initial full refresh with MacPaint canvas..."));
  display.clearBuffer();
  // Fill background with white
  display.fillRect(0, 0, display.width(), display.height(), EPD_WHITE);
  // Draw centered 640x480 MacPaint bitmap
  display.drawBitmap(MACPAINT_X_OFFSET, MACPAINT_Y_OFFSET,
                     gImage_macos_paint_640x480,
                     MACPAINT_WIDTH, MACPAINT_HEIGHT,
                     EPD_BLACK);
  display.display();
  Serial.println(F("[Adafruit] Full refresh complete. Ready for partial updates."));
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }

  Serial.println(F("=================================================="));
  Serial.println(F(" Feather RP2040 ThinkInk + GDEQ0426T82 Adafruit_EPD"));
  Serial.println(F(" MacPaint Partial Refresh Handwriting Demo       "));
  Serial.println(F("=================================================="));

  display.begin(THINKINK_MONO);

  drawMacPaintBackground();
  initialRenderDone = true;
  delay(1000);
}

void loop()
{
  if (!initialRenderDone) return;

  if (animIndex < NUM_POSITIONS) {
    int16_t x0 = pgm_read_word(&positions[animIndex][0]);
    int16_t y0 = pgm_read_word(&positions[animIndex][1]);

    int16_t minX = x0 - BRUSH_RADIUS;
    int16_t maxX = x0 + BRUSH_RADIUS;
    int16_t minY = y0 - BRUSH_RADIUS;
    int16_t maxY = y0 + BRUSH_RADIUS;

    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX >= 800) maxX = 799;
    if (maxY >= 480) maxY = 479;

    // Draw solid discrete circle directly onto Adafruit_EPD frame buffer
    display.fillCircle(x0, y0, BRUSH_RADIUS, EPD_BLACK);

    // Call Adafruit partial display
    display.displayPartial(minX, minY, maxX, maxY);

    animIndex++;

    if (animIndex % 25 == 0 || animIndex >= NUM_POSITIONS) {
      Serial.print(F("[Adafruit] Drawing progress: "));
      Serial.print(animIndex);
      Serial.print(F(" / "));
      Serial.print(NUM_POSITIONS);
      Serial.println(F(" points"));
    }

    delay(20);
  } else {
    Serial.println(F("[Adafruit] Handwriting animation finished."));
    Serial.println(F("[Adafruit] Holding display for 8 seconds before restarting..."));
    delay(8000);

    drawMacPaintBackground();
    animIndex = 0;
    delay(1000);
  }
}
