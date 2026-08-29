/*****************************************************************************
* | File        :   Waveshare_2in66br.ino
* | Function    :   2.66inch e-Paper (B) demo - 152x296, black/white/red
* | Info        :
*----------------
* Adapted from Waveshare's original epd2in66b.ino to run on the Adafruit Feather
* RP2040 ThinkInk (Adafruit product 5727) with the panel seated directly in the
* board's 24-pin FPC EPD connector.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   Waveshare 2.66inch e-Paper (B), 152x296 BWR, SSD1680
* Connection  :   24-pin FPC EPD connector (the EPD signals are not on the
*                 Feather headers, so nothing here is jumper-wired)
*
* Changes from the Waveshare original:
*  - epdif.h/.cpp: pins switched to the board variant's PIN_EPD_* symbols
*    (CS 19, DC 18, RST 17, BUSY 16, SCK 22, MOSI 23), driven over SPI1 rather
*    than the default SPI. PWR_PIN dropped: the 24-pin connector has no software
*    power gate, so there is nothing to switch on.
*  - epd2in66b.cpp: `static` removed from the five out-of-class member
*    definitions - the vendor copy does not compile as shipped. WaitUntilIdle()
*    gained a 40s timeout so a stalled panel fails loudly instead of hanging
*    forever, and Clear() no longer runs one row past the RAM window.
*    Sleep() is unchanged: SSD1680 deep-sleeps straight from 0x10, unlike the
*    UC8253 3.52" panel in this repo, which wants POWER_OFF first.
*    DisplayFrame() rotates the bitmap 180 degrees (EPD_2IN66B_ROTATE_180): the
*    demo images are authored for Waveshare's own driver board, and in this
*    connector they otherwise land with the logo bottom-right instead of
*    top-left. Same class of fix as the 3.52" sketch's Paint rotation, which
*    also exists only to match the built-in artwork's orientation.
*  - This file: wait for USB CDC to enumerate before printing, or the banner and
*    the init result go nowhere. The closing Clear() is disabled so the demo ends
*    on the image instead of a blank panel.
*
* Everything else is Waveshare's, unchanged; their licence below applies.
*
    @filename   :   epd2in66b.ino
    @brief      :   2.66inch b e-paper display demo
    @author     :   Waveshare

    Copyright (C) Waveshare     Dec 02 2020

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documnetation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to  whom the Software is
   furished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include <SPI.h>
#include "epd2in66b.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {  // give USB CDC time to enumerate
    delay(10);
  }
  Epd epd;
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed...");
    return;
  }
  Serial.print("2.66inch b e-Paper demo...\r\n ");
  Serial.print("e-Paper Clear...\r\n ");
  epd.Clear();  
  
  Serial.print("draw image...\r\n ");
  epd.DisplayFrame(gImage_2in66bb, gImage_2in66br);
  delay(4000);

  // The wipe-to-white is left off deliberately: ending on the drawn image rather
  // than a blank panel. A cleared BWR panel looks grey next to a mono one - that
  // is the panel's white point, not a fault. Flip to 1 for Waveshare's behaviour.
#if 0
  Serial.print("clear......\r\n ");
  epd.Clear();
#endif

  Serial.print("sleep......\r\n ");
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
