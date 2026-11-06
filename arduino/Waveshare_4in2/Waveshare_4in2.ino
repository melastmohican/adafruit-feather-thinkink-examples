/*****************************************************************************
* | File        :   Waveshare_4in2.ino
* | Function    :   4.2inch e-Paper V2 demo - 400x300, black/white monochrome
* | Info        :
*----------------
* Adapted from Waveshare's official epd4in2_V2 demo to run on the Adafruit
* Feather RP2040 ThinkInk with the panel seated in the on-board 24-pin FPC
* connector.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   Waveshare 4.2inch e-Paper V2 / Good Display GDEY042T81, 400x300, SSD1683
* Connection  :   Feather RP2040 ThinkInk onboard 24-pin FPC connector
*
* Pinout (ThinkInk 24-pin connector):
*   RST  -> PIN_EPD_RESET (GPIO17)
*   CS   -> PIN_EPD_CS    (GPIO19)
*   BUSY -> PIN_EPD_BUSY  (GPIO16, Active HIGH on SSD1683: 1 = busy, 0 = idle)
*   DC   -> PIN_EPD_DC    (GPIO18)
*   SCK  -> PIN_EPD_SCK   (GPIO22, SPI1)
*   MOSI -> PIN_EPD_MOSI  (GPIO23, SPI1)
*
* Adaptations:
*  - epdif.h/.cpp: pins mapped to the Feather RP2040 ThinkInk SPI1 pins.
*  - Single persistent SPI1 transaction.
*  - ReadBusy() guarded with a 40-second timeout.
*  - Serial at 115200 baud with bounded wait.
******************************************************************************/

#include <SPI.h>
#include "epd4in2_V2.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println(F("Waveshare 4.2in e-Paper V2 on Feather RP2040 ThinkInk"));

  Epd epd;
  if (epd.Init() != 0) {
    Serial.println(F("e-Paper init failed..."));
    return;
  }

  Serial.println(F("1. e-Paper Clear..."));
  epd.Clear();

  Serial.println(F("2. Drawing 2-gray demo bitmap (IMAGE_BUTTERFLY)..."));
  epd.Display(IMAGE_BUTTERFLY);
  delay(3000);

  Serial.println(F("3. Demonstrating partial refresh with Paint..."));
  unsigned char image[1500];
  Paint paint(image, 400, 28);    // width is a multiple of 8

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(20, 5, "ThinkInk + Waveshare 4.2\"", &Font16, COLORED);
  epd.Display_Partial_Not_refresh(paint.GetImage(), 0, 32, paint.GetWidth(), 32 + paint.GetHeight());

  paint.Clear(COLORED);
  paint.DrawStringAt(20, 5, "Fast & Partial Refresh Supported", &Font16, UNCOLORED);
  epd.Display_Partial(paint.GetImage(), 0, 64, paint.GetWidth(), 64 + paint.GetHeight());
  delay(2000);

  Serial.println(F("4. Drawing 4-gray image..."));
  epd.Init_4Gray();
  epd.Set_4GrayDisplay(gImage_4in2_4Gray1, 100, 100, 200, 150);
  delay(3000);

  Serial.println(F("5. Entering deep sleep..."));
  epd.Sleep();
  Serial.println(F("Demo completed."));
}

void loop() {
  // Demo runs once in setup()
}

