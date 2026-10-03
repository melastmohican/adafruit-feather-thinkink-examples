/*****************************************************************************
* | File        :   GDEM0154F61H.ino
* | Function    :   1.54inch 4-color e-Paper demo, 200x200, Black/White/Yellow/Red
* | Info        :
*----------------
* Good Display official GDEM0154F61H sample (SSD2681 controller), adapted to
* run on the Adafruit Feather RP2040 ThinkInk with the panel seated in the
* onboard 24-pin FPC connector.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   GDEM0154F61H, 200x200, SSD2681 controller, FPC-8101
* Connection  :   Feather RP2040 ThinkInk onboard 24-pin FPC connector
*
* Pinout (ThinkInk 24-pin connector):
*   RST  -> PIN_EPD_RESET (GPIO17)
*   CS   -> PIN_EPD_CS    (GPIO19)
*   BUSY -> PIN_EPD_BUSY  (GPIO16, active LOW on SSD2681: 0 = busy, 1 = ready/idle)
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

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F(" GDEM0154F61H (Good Display) on Feather ThinkInk "));
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

void loop()
{
  // 1. Full screen update with official Good Display 4-color bitmap
  Serial.println(F("1. Full Screen Update (gImage_1, ~20s)..."));
  EPD_init();
  PIC_display(gImage_1);
  EPD_sleep();
  Serial.println(F("   Holding display for 5s..."));
  delay(5000);

  // 2. Fast screen update mode (~12-15s)
  Serial.println(F("2. Fast Screen Update Mode (gImage_1)..."));
  EPD_init_Fast();
  PIC_display(gImage_1);
  EPD_sleep();
  Serial.println(F("   Holding display for 5s..."));
  delay(5000);

  // 3. Single color fill tests
  Serial.println(F("3. Single color fill tests..."));
  Serial.println(F("   - Black fill"));
  EPD_init();
  Display_All_Black();
  EPD_sleep();
  delay(3000);

  Serial.println(F("   - Yellow fill"));
  EPD_init();
  Display_All_Yellow();
  EPD_sleep();
  delay(3000);

  Serial.println(F("   - Red fill"));
  EPD_init();
  Display_All_Red();
  EPD_sleep();
  delay(3000);

  Serial.println(F("   - White clear"));
  EPD_init();
  Display_All_White();
  EPD_sleep();
  delay(3000);

  Serial.println(F("Demo sequence complete. Program halted."));
  while (1) {
    delay(1000);
  }
}
