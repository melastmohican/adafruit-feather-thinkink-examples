/*****************************************************************************
* | File        :   ThinkInk_GDEY0213F52.ino
* | Function    :   Drive Good Display GDEY0213F52 with Adafruit_EPD
* | Info        :
*----------------
* Drives the Good Display GDEY0213F52 (250x122, 4-color: Black, White,
* Yellow, Red) via Adafruit_EPD.
*
* The JD79676A controller shares its 2-bit RAM layout and command structure
* (0x10 RAM start, 0x12 refresh, 0x02 power off, 0x07 sleep, active-low BUSY)
* with JD79661, which Adafruit_EPD supports via Adafruit_JD79661. Good Display's
* own JD79676A sample needs only 0xE9 0x01 and POWER_ON (the waveform comes from
* OTP), so this sketch defines a panel class locally
* (ThinkInk_213_Quadcolor_GDEY0213F52) with that short init table and leaves
* stock Adafruit_EPD unmodified. Adafruit's own JD79661 init (PSR/PWR/TRES...)
* is deliberately not used: it targets a different controller.
*
* The panel RAM is 128 pixels wide (122 visible); Adafruit_JD79661 pads the
* 122-pixel width to 128, giving the 8000-byte buffer the controller expects.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   Good Display GDEY0213F52, 250x122, JD79676A controller, FPC-J002
* Connection  :   Feather RP2040 ThinkInk onboard 24-pin FPC connector
*
* Pinout (ThinkInk 24-pin connector):
*   RST  -> PIN_EPD_RESET (GPIO17)
*   CS   -> PIN_EPD_CS    (GPIO19)
*   BUSY -> PIN_EPD_BUSY  (GPIO16, Active LOW on JD79676A: 0 = busy, 1 = ready/idle)
*   DC   -> PIN_EPD_DC    (GPIO18)
*   SCK  -> PIN_EPD_SCK   (GPIO22, SPI1)
*   MOSI -> PIN_EPD_MOSI  (GPIO23, SPI1)
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

// Landscape 250x122. Use 3 instead of 1 if the panel shows content upside down.
#define DEMO_ROTATION 1

// JD79676A initialization sequence expressed as an Adafruit_EPD command list:
// {command, arg_count, args...}, {0xFF, delay_ms}, terminated by 0xFE.
static const uint8_t gdey0213f52_init_code[] = {
    0xFF, 10,
    0xE9, 1, 0x01,
    0x04, 0,                                     // POWER_ON
    0xFE
};

class ThinkInk_213_Quadcolor_GDEY0213F52 : public Adafruit_JD79661 {
 public:
  ThinkInk_213_Quadcolor_GDEY0213F52(int16_t DC, int16_t RST, int16_t CS,
                                     int16_t SRCS, int16_t BUSY = -1,
                                     SPIClass *spi = &SPI)
      : Adafruit_JD79661(122, 250, DC, RST, CS, SRCS, BUSY, spi) {}

  void begin(thinkinkmode_t mode = THINKINK_QUADCOLOR) {
    Adafruit_JD79661::begin(true);

    inkmode = mode;
    _epd_init_code = gdey0213f52_init_code;
    default_refresh_delay = 11000;
    setRotation(DEMO_ROTATION);
    powerDown();
  }
};

ThinkInk_213_Quadcolor_GDEY0213F52 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS,
                                           EPD_BUSY, EPD_SPI);

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 3000) {
    delay(10);
  }
  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F(" Adafruit_EPD: GDEY0213F52 on Feather ThinkInk           "));
  Serial.println(F("=================================================="));

  display.begin();

  Serial.println(F("Drawing test graphics in buffer..."));
  display.clearBuffer();

  const int W = display.width();    // 250 in landscape
  const int H = display.height();   // 122 in landscape

  // Top header banner
  display.fillRect(0, 0, W, 24, EPD_RED);
  display.setTextColor(EPD_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 5);
  display.print("2.13\" 4-COLOR");

  // Quad-color text lines
  display.setTextSize(1);
  display.setTextColor(EPD_BLACK);
  display.setCursor(10, 32);
  display.print("BLACK: Primary Text");

  display.setTextColor(EPD_RED);
  display.setCursor(10, 44);
  display.print("RED:   Alerts");

  display.setTextColor(EPD_YELLOW);
  display.setCursor(10, 56);
  display.print("YELLOW: Warnings");

  display.drawFastHLine(10, 68, W - 20, EPD_BLACK);

  // 4 Color swatches
  const uint16_t swColors[] = {EPD_BLACK, EPD_WHITE, EPD_RED, EPD_YELLOW};
  const char* swNames[] = {"BLK", "WHT", "RED", "YEL"};
  int sw = 36, sh = 24, gap = 4;
  int sx = 10;
  int sy = 76;

  for (int i = 0; i < 4; i++) {
    int x = sx + i * (sw + gap);
    display.fillRoundRect(x, sy, sw, sh, 3, swColors[i]);
    display.drawRoundRect(x, sy, sw, sh, 3, EPD_BLACK);
    display.setTextColor(swColors[i] == EPD_BLACK || swColors[i] == EPD_RED ? EPD_WHITE : EPD_BLACK);
    display.setCursor(x + 9, sy + 8);
    display.print(swNames[i]);
  }

  // Concentric geometric shapes, right of the swatches
  display.drawCircle(W - 100, 52, 18, EPD_BLACK);
  display.fillCircle(W - 100, 52, 14, EPD_RED);
  display.fillCircle(W - 100, 52, 8, EPD_YELLOW);

  display.drawRect(W - 70, 34, 50, 36, EPD_RED);
  display.fillRect(W - 65, 39, 40, 26, EPD_YELLOW);
  display.fillRect(W - 58, 46, 26, 12, EPD_BLACK);

  display.fillTriangle(W - 75, 112, W - 50, 80, W - 25, 112, EPD_RED);
  display.drawTriangle(W - 75, 112, W - 50, 80, W - 25, 112, EPD_BLACK);

  // Footer status bar
  display.fillRect(0, H - 14, W - 100, 14, EPD_BLACK);
  display.setTextColor(EPD_YELLOW);
  display.setCursor(6, H - 11);
  display.print("Adafruit_EPD + JD79676A");

  Serial.println(F("Sending image buffer to display (refresh ~11s)..."));
  display.display();

  Serial.println(F("Powering down display..."));
  display.powerDown();

  Serial.println(F("Demo completed successfully."));
}

void loop()
{
  // Halt execution
}
