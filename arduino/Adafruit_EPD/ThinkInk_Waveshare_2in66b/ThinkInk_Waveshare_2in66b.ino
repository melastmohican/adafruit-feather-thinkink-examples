/*****************************************************************************
* | File        :   ThinkInk_Waveshare_2in66b.ino
* | Function    :   Drive the Waveshare 2.66inch e-Paper (B) with Adafruit_EPD
* | Info        :
*----------------
* The Waveshare 2.66" (B) panel (152x296 BWR) uses an SSD1680 controller, which
* Adafruit_EPD already drives at exactly this resolution as
* ThinkInk_266_Tricolor_MFGNR. Unlike the 3.52" panel in this repo, no local
* panel class is needed - this sketch is the stock class with the ThinkInk
* connector's pins.
*
* Board : rp2040:rp2040:adafruit_feather_thinkink
* Panel : Waveshare 2.66inch e-Paper (B), 152x296, SSD1680, 24-pin FPC connector
*
* MFGNR is the right class rather than a lucky guess: Waveshare's own product
* specification for this panel names the driver IC as SSD1680Z8 (mechanical
* drawing note, p6 of 2.66inch-e-paper-b-specification.pdf), and MFGNR is
* Adafruit's SSD1680Z variant class. The same page gives the resolution as
* "296gate x 152source", which is why the constructor takes the long axis first.
*
* Based on Adafruit's ThinkInk_tricolor example.
******************************************************************************/
#include "Adafruit_ThinkInk.h"

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

// 152x296 tricolor, SSD1680. The class passes (296, 152) to Adafruit_SSD1680 -
// long axis first - so the constructor here takes only the pins.
ThinkInk_266_Tricolor_MFGNR display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY,
                                    EPD_SPI);

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { // give USB CDC time to enumerate
    delay(10);
  }
  Serial.println("Waveshare 2.66in (B) on Adafruit_EPD / SSD1680");
  display.begin(THINKINK_TRICOLOR);

  // 180 degrees, matching the Waveshare_2in66br sketch's EPD_2IN66B_ROTATE_180:
  // seated in the ThinkInk connector this panel's native origin is the corner
  // opposite the one you want content to start from. Has to come after begin(),
  // which sets rotation 0 itself. Rotation 2 is even, so width()/height() stay
  // 296x152 and the layout below is unaffected.
  display.setRotation(2);

  Serial.printf("Panel reports %d x %d\r\n", display.width(), display.height());

  // Single-shot, not a loop: e-paper wants updating rarely, so two refreshes
  // and stop rather than cycling forever.
  Serial.println("Banner demo");
  display.clearBuffer();
  display.setTextSize(2);
  display.setCursor((display.width() - 96) / 2, (display.height() - 16) / 2);
  display.setTextColor(EPD_BLACK);
  display.print("Tri");
  display.setTextColor(EPD_RED);
  display.print("Color");
  display.display();

  delay(15000);

  // Thirds: white | black | red. Makes plane order and inversion obvious at a
  // glance - if black and red swap places, the buffer indices are crossed; if
  // white and black swap, it is the inversion flags.
  Serial.println("Color rectangle demo");
  display.clearBuffer();
  display.fillRect(display.width() / 3, 0, display.width() / 3,
                   display.height(), EPD_BLACK);
  display.fillRect((display.width() * 2) / 3, 0, display.width() / 3,
                   display.height(), EPD_RED);
  display.display();

  Serial.println("Done - panel left on the rectangle test pattern.");
}

void loop() {}
