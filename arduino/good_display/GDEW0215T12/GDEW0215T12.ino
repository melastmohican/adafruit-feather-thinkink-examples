/*****************************************************************************
* | File        :   GDEW0215T12.ino
* | Function    :   2.15inch e-Paper demo - 208x112, black/white monochrome
* | Info        :
*----------------
* Good Display's official GDEW0215T12 sample (AU-GDEW0215T12), adapted to run
* on the Adafruit Feather RP2040 ThinkInk with the panel seated in the
* onboard 24-pin FPC connector.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   GDEW0215T12 (formerly GDEW0215T11), 208x112, UC8151D controller
* Connection  :   Feather RP2040 ThinkInk onboard 24-pin FPC connector
*
* Pinout (ThinkInk 24-pin connector):
*   RST  -> PIN_EPD_RESET (GPIO17)
*   CS   -> PIN_EPD_CS    (GPIO19)
*   BUSY -> PIN_EPD_BUSY  (GPIO16, active LOW on UC8151D: 0 = busy, 1 = ready)
*   DC   -> PIN_EPD_DC    (GPIO18)
*   SCK  -> PIN_EPD_SCK   (GPIO22, SPI1)
*   MOSI -> PIN_EPD_MOSI  (GPIO23, SPI1)
*
* Changes from the Good Display original:
*  - Display_EPD_W21_spi.h/.cpp: pins mapped to Feather RP2040 ThinkInk SPI1 pins.
*  - Display_EPD_W21.cpp: busy checking uses a 40s timeout so a disconnected or
*    stalled panel fails cleanly instead of hanging forever.
*  - SPI1 transaction is opened once with 4 MHz clock and left open.
*  - Loop halts unconditionally with while(1) after running the demo sequence.
******************************************************************************/
#include <SPI.h>
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "Ap_29demo.h"

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F(" GDEW0215T12 (Good Display) on Feather ThinkInk "));
  Serial.println(F("=================================================="));

  pinMode(PIN_EPD_BUSY,  INPUT);
  pinMode(PIN_EPD_RESET, OUTPUT);
  pinMode(PIN_EPD_DC,    OUTPUT);
  pinMode(PIN_EPD_CS,    OUTPUT);

  // Remap SPI1 onto the EPD pins (GPIO22=SCK, GPIO23=MOSI) before begin().
  EPD_SPI_PORT.setSCK(PIN_EPD_SCK);
  EPD_SPI_PORT.setTX(PIN_EPD_MOSI);
  EPD_SPI_PORT.begin();
  EPD_SPI_PORT.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  unsigned char i;

  Serial.println(F("1. Full screen clear (white)..."));
  EPD_Init();
  EPD_WhiteScreen_White();
  EPD_DeepSleep();
  delay(2000);

  Serial.println(F("2. Full screen image display (gImage_1)..."));
  EPD_Init();
  EPD_WhiteScreen_ALL(gImage_1);
  EPD_DeepSleep();
  delay(2000);

  Serial.println(F("3. Partial refresh clock demonstration..."));
  EPD_Init();
  EPD_SetRAMValue_BaseMap(gImage_basemap);
  for (i = 0; i < 6; i++) {
    EPD_Dis_Part_Time(48, 46, Num[i], Num[0], gImage_numdot, Num[0], Num[1], 5, 24, 32);
    delay(500);
  }
  EPD_DeepSleep();
  delay(2000);

  Serial.println(F("4. Full screen clear and enter deep sleep..."));
  EPD_Init();
  EPD_WhiteScreen_White();
  EPD_DeepSleep();

  Serial.println(F("Demo completed. Halted."));
  while (1) {
    delay(1000);
  }
}
