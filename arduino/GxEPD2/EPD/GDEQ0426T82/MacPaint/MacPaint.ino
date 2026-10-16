// MacPaint.ino
//
// Port of upiir's Seeed ePaper Partial Update "hello" animation
// for the 4.26" 800x480 Monochrome E-Paper Panel (GDEQ0426T82 / SSD1677)
// on Adafruit Feather RP2040 ThinkInk.
//
// Original project by upiir:
//   https://github.com/upiir/seeed_eink_partial_update
//   YouTube: https://youtu.be/7UocgE-gukA
//
// Pinout (ThinkInk 24-pin connector):
//   RST  -> PIN_EPD_RESET (GPIO17)
//   CS   -> PIN_EPD_CS    (GPIO19)
//   BUSY -> PIN_EPD_BUSY  (GPIO16, Active HIGH on SSD1677: 1 = busy, 0 = idle)
//   DC   -> PIN_EPD_DC    (GPIO18)
//   SCK  -> PIN_EPD_SCK   (GPIO22, SPI1)
//   MOSI -> PIN_EPD_MOSI  (GPIO23, SPI1)
//

#include <SPI.h>
#include <GxEPD2_BW.h>
#include "image.h"
#include "positions.h"

// 4.26" Monochrome 800x480 GDEQ0426T82 display configuration
#define MAX_DISPLAY_BUFFER_SIZE 65536u
#define MAX_HEIGHT(EPD) \
    (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 8) \
         ? EPD::HEIGHT \
         : (MAX_DISPLAY_BUFFER_SIZE) / (EPD::WIDTH / 8))

GxEPD2_BW<GxEPD2_426_GDEQ0426T82, MAX_HEIGHT(GxEPD2_426_GDEQ0426T82)>
    display(GxEPD2_426_GDEQ0426T82(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RESET, PIN_EPD_BUSY));

// Horizontal offset to center the 640x480 MacPaint canvas on the 800x480 screen
#define MACPAINT_X_OFFSET  80
#define MACPAINT_Y_OFFSET  0
#define MACPAINT_WIDTH     640
#define MACPAINT_HEIGHT    480

// Radius for the drawn brush circle (in pixels)
const int16_t BRUSH_RADIUS = 4;

int16_t animIndex = 0;
bool initialRenderDone = false;

void drawMacPaintBackground()
{
  Serial.println(F("[EPD] Performing initial full refresh with MacPaint canvas..."));
  display.setRotation(0); // Native landscape (800x480)
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.drawBitmap(MACPAINT_X_OFFSET, MACPAINT_Y_OFFSET,
                       gImage_macos_paint_640x480,
                       MACPAINT_WIDTH, MACPAINT_HEIGHT,
                       GxEPD_BLACK);
  } while (display.nextPage());
  Serial.println(F("[EPD] Full refresh complete. Ready for partial updates."));
}

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }

  Serial.println(F("=================================================="));
  Serial.println(F(" Feather RP2040 ThinkInk + GDEQ0426T82 E-Paper    "));
  Serial.println(F(" MacPaint Partial Refresh Handwriting Demo       "));
  Serial.println(F(" Port of upiir/seeed_eink_partial_update         "));
  Serial.println(F("=================================================="));

  // Configure secondary SPI (SPI1) for ThinkInk 24-pin connector
  SPI1.setSCK(PIN_EPD_SCK);
  SPI1.setTX(PIN_EPD_MOSI);
  SPI1.begin();
  display.epd2.selectSPI(SPI1, SPISettings(4000000, MSBFIRST, SPI_MODE0));

  display.init(115200);

  // Render the initial full background
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

    // Small, fast partial bounding box exactly around the dot
    int16_t minX = x0 - BRUSH_RADIUS;
    int16_t maxX = x0 + BRUSH_RADIUS;
    int16_t minY = y0 - BRUSH_RADIUS;
    int16_t maxY = y0 + BRUSH_RADIUS;

    if (minX < 0) minX = 0;
    if (minY < 0) minY = 0;
    if (maxX >= 800) maxX = 799;
    if (maxY >= 480) maxY = 479;

    int16_t boxW = maxX - minX + 1;
    int16_t boxH = maxY - minY + 1;

    // Apply partial window and draw current dot plus recent overlapping dots
    // to prevent GxEPD2's fillScreen(WHITE) from biting notches into previous dots
    display.setPartialWindow(minX, minY, boxW, boxH);
    display.firstPage();
    do {
      int16_t startDot = (animIndex > 16) ? animIndex - 16 : 0;
      for (int16_t i = startDot; i <= animIndex; i++) {
        int16_t xi = pgm_read_word(&positions[i][0]);
        int16_t yi = pgm_read_word(&positions[i][1]);
        display.fillCircle(xi, yi, BRUSH_RADIUS, GxEPD_BLACK);
      }
    } while (display.nextPage());

    animIndex++;

    if (animIndex % 25 == 0 || animIndex >= NUM_POSITIONS) {
      Serial.print(F("[EPD] Drawing progress: "));
      Serial.print(animIndex);
      Serial.print(F(" / "));
      Serial.print(NUM_POSITIONS);
      Serial.println(F(" points"));
    }

    delay(20);
  } else {
    Serial.println(F("[EPD] Handwriting animation finished."));
    Serial.println(F("[EPD] Holding display for 8 seconds before restarting..."));
    delay(8000);

    // Redraw the clean background to start the cycle again
    drawMacPaintBackground();
    animIndex = 0;
    delay(1000);
  }
}
