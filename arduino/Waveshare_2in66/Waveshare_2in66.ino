/*****************************************************************************
* | File        :   Waveshare_2in66.ino
* | Function    :   2.66inch e-Paper demo - 152x296, black/white monochrome
* | Info        :
*----------------
* Adapted from Waveshare's original epd2in66.ino to run on the Adafruit Feather
* RP2040 ThinkInk (Adafruit product 5727) with the panel seated directly in the
* board's 24-pin FPC EPD connector.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   Waveshare 2.66inch e-Paper (monochrome), 152x296, SSD1680
* Connection  :   24-pin FPC EPD connector (the EPD signals are not on the
*                 Feather headers, so nothing here is jumper-wired)
*
* Changes from the Waveshare original:
*  - epdif.h/.cpp: pins switched to the board variant's PIN_EPD_* symbols
*    (CS 19, DC 18, RST 17, BUSY 16, SCK 22, MOSI 23), driven over SPI1 rather
*    than the default SPI. PWR_PIN dropped: the 24-pin connector has no software
*    power gate, so there is nothing to switch on.
*  - epd2in66.cpp: `static` removed from out-of-class member definitions - the
*    vendor copy does not compile as shipped in C++. WaitUntilIdle() gained a 40s
*    timeout so a stalled panel fails loudly instead of hanging forever, and
*    Clear() no longer runs one row past the RAM window.
*    DisplayFrame() rotates the bitmap 180 degrees (EPD_2IN66_ROTATE_180): the
*    demo image is authored for Waveshare's own driver board, and in this
*    connector it otherwise lands inverted.
*  - This file: a bounded wait on Serial before printing.
*
* Everything else is Waveshare's, unchanged; their licence applies.
******************************************************************************/

#include <SPI.h>
#include "epd2in66.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

UBYTE image[500];
Paint paint(image, 48, 80);    // width should be a multiple of 8

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println(F("Waveshare 2.66in e-Paper on Feather RP2040 ThinkInk"));

  Epd epd;
  if (epd.Init() != 0) {
    Serial.println(F("e-Paper init failed..."));
    return;
  }
  Serial.println(F("e-Paper Clear..."));
  epd.Clear();

  Serial.println(F("Draw image bitmap..."));
  epd.DisplayFrame(IMAGE_DATA);
  delay(4000);

  Serial.println(F("Partial refresh timer demo (01 -> 07 on top of image)..."));
  epd.Init_Partial();
  paint.SetRotate(ROTATE_270);

  for (UBYTE i = 1; i <= 7; i++) {
    char time_string[] = {'0', '0', ':', '0', (char)('0' + i), '\0'};

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(10, 10, time_string, &Font16, COLORED);
    Serial.print(F("Partial update refresh: "));
    Serial.println(time_string);
    epd.DisplayFrame_part(paint.GetImage(), IMAGE_DATA, 20, 100, 48, 80);
    delay(500);
  }

  // The wipe-to-white is left off deliberately (matching Waveshare_2in66br):
  // ending on the drawn spec image rather than a blank panel. Flip to 1 for
  // Waveshare's original wipe behaviour.
#if 0
  Serial.println(F("e-Paper Clear..."));
  epd.Clear();
#endif

  Serial.println(F("Entering deep sleep..."));
  epd.Sleep();
}

void loop() {
  // Demo runs once in setup()
}
