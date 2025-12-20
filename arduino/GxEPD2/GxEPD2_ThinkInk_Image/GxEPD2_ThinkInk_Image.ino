// GxEPD2 Example for Adafruit Feather RP2040 ThinkInk
// with 2.13" 250x122 Quad-Color (4-color) eInk Display (JD79661 chipset)
//
// Based on GxEPD2 library by Jean-Marc Zingg
// https://github.com/ZinggJM/GxEPD2

#include <GxEPD2_4C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "mocha122x250.h"  // Generated 4-color bitmap of mocha dog

// Display: 2.13" 4-color (122x250) with JD79661 driver
// Adafruit Feather RP2040 ThinkInk pin definitions from board package:
// PIN_EPD_CS=19, PIN_EPD_DC=18, PIN_EPD_RESET=17, PIN_EPD_BUSY=16
// PIN_EPD_SCK=22, PIN_EPD_MOSI=23
GxEPD2_4C<GxEPD2_213c_GDEY0213F51, GxEPD2_213c_GDEY0213F51::HEIGHT> display(
  GxEPD2_213c_GDEY0213F51(PIN_EPD_CS, PIN_EPD_DC, PIN_EPD_RESET, PIN_EPD_BUSY)
);

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
  Serial.println();
  Serial.println("GxEPD2 ThinkInk Image Example");
  Serial.println("==================================");
  
  // Remap SPI1 to EPD pins (GPIO22=SCK, GPIO23=MOSI)
  SPI1.setSCK(PIN_EPD_SCK);
  SPI1.setTX(PIN_EPD_MOSI);
  SPI1.begin();
  display.epd2.selectSPI(SPI1, SPISettings(4000000, MSBFIRST, SPI_MODE0));
  
  // Initialize display with longer reset pulse for 4-color displays
  display.init(115200, true, 20, false);
  
  Serial.print("Display size: ");
  Serial.print(display.width());
  Serial.print("x");
  Serial.println(display.height());
  Serial.println("Display initialized!");
  Serial.println();

  drawBitmap();
  delay(3000);
}

void loop()
{
  // Nothing to do in loop
}

void drawBitmap()
{
  Serial.println("drawBitmap");
  //display.setRotation(0);
  display.setFullWindow();
  display.drawNative(image122x250, 0, 0, 0, 122, 250, false, false, true);
  Serial.println("drawBitmap done");
}



