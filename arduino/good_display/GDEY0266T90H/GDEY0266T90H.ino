/*****************************************************************************
* | File        :   GDEY0266T90H.ino
* | Function    :   2.66inch e-Paper demo - 360x184 (184x360 native), black/white monochrome
* | Info        :
*----------------
* Good Display's own GDEY0266T90H sample (AU-GDEY0266T90H-2FP-20230915), adapted to run on the Adafruit
* Feather RP2040 ThinkInk with the panel seated in the board's 24-pin FPC
* EPD connector.
*
* The higher-resolution sibling of the GDEY0266T90: 360x184 on an SSD1685
* controller (FPC-H011), which Waveshare does not sell and stock GxEPD2 and
* Adafruit_EPD do not cover. Command set is the SSD1680's, but the RAM is
* 184 pixels wide (23 bytes/row) by 360 rows = 8280 bytes. Of the driver paths
* in this repo, this Good Display vendor sample is the one that exposes the
* manufacturer's official fast-refresh (1.5s and 1.0s) and partial-refresh waveforms.
*
* Board       :   rp2040:rp2040:adafruit_feather_thinkink
* Panel       :   GDEY0266T90H, 360x184, SSD1685, FPC-H011
* Connection  :   24-pin FPC EPD connector (the EPD signals are not on the
*                 Feather headers, so nothing here is jumper-wired)
*
* Changes from the Good Display original:
*  - Display_EPD_W21_spi.h/.cpp: pins are the variant's PIN_EPD_* symbols and
*    SPI_Write goes through EPD_SPI_PORT, which is SPI1 here. The ESP8266-only
*    Sys_run()/LED_run() helpers are gone.
*  - Display_EPD_W21.cpp: Epaper_READBUSY() given a 40s timeout, so a stalled
*    panel fails loudly instead of spinning forever.
*  - This file: SPI begin() now precedes beginTransaction() - the original calls
*    them the other way round - plus Serial for progress, and the full-screen
*    passes use Good Display's own EPD_HW_Init_180() (see EPD_INIT_180 below).
*    The halt at the end of loop() is unconditional; the original guards it with
*    #ifdef Arduino_UNO, so on any other board loop() returns and the core runs
*    the entire demo again, forever.
******************************************************************************/
#include <SPI.h>
//EPD
#include "Display_EPD_W21_spi.h"
#include "Display_EPD_W21.h"
#include "Ap_29demo.h"

// This panel's origin is the corner opposite the one content should start from
// - a panel/FPC fact, not a board one - so the stock init puts the image 180
// degrees round. Good Display ships a rotated init for exactly this, so unlike
// the other 2.66" sketches in this repo no image-flipping code is needed - it
// is just the other init sequence (data entry 0x02 and a mirrored RAM window).
// Set to 0 for Good Display's default orientation.
#define EPD_INIT_180  1

#if EPD_INIT_180
#define EPD_HW_Init_Full()  EPD_HW_Init_180()
#else
#define EPD_HW_Init_Full()  EPD_HW_Init()
#endif

void setup() {
   Serial.begin(115200);
   while (!Serial && millis() < 3000) {
     delay(10); // give USB CDC time to enumerate, but still run standalone
   }
   Serial.println(F("GDEY0266T90H (Good Display sample) on Feather RP2040 ThinkInk"));

   pinMode(PIN_EPD_BUSY,  INPUT);  //BUSY
   pinMode(PIN_EPD_RESET, OUTPUT); //RES
   pinMode(PIN_EPD_DC,    OUTPUT); //DC
   pinMode(PIN_EPD_CS,    OUTPUT); //CS

   //SPI
   // Remap SPI1 onto the EPD pins (GPIO22=SCK, GPIO23=MOSI) before begin().
   EPD_SPI_PORT.setSCK(PIN_EPD_SCK);
   EPD_SPI_PORT.setTX(PIN_EPD_MOSI);
   // begin() first: the original calls beginTransaction() before begin(), which
   // configures a port that does not exist yet.
   EPD_SPI_PORT.begin ();
   EPD_SPI_PORT.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
}

void loop() {
   unsigned char i;
#if 1 //Full screen refresh, fast refresh, and partial refresh demonstration.

      Serial.println(F("Full screen clear (white)..."));
      EPD_HW_Init_Full(); //Full screen refresh initialization.
      EPD_WhiteScreen_White(); //Clear screen function.
      EPD_DeepSleep(); //Enter sleep mode
      delay(2000);

      /************Full display(2s)*******************/
      Serial.println(F("Full screen update (Image 1)..."));
      EPD_HW_Init_Full();
      EPD_WhiteScreen_ALL(gImage_1); //To Display one image using full screen refresh.
      EPD_DeepSleep();
      delay(2000);

      /************Fast refresh mode(1.5s)*******************/
      Serial.println(F("Fast refresh mode 1 (1.5s)..."));
      EPD_HW_Init_Fast();
      EPD_WhiteScreen_ALL_Fast(gImage_1);
      EPD_DeepSleep();
      delay(2000);

      /************Fast refresh mode(1s)*******************/
      Serial.println(F("Fast refresh mode 2 (1.0s)..."));
      EPD_HW_Init_Fast2();
      EPD_WhiteScreen_ALL_Fast2(gImage_1);
      EPD_DeepSleep();
      delay(1000);

      /************Partial refresh demonstration*******************/
      Serial.println(F("Partial refresh clock demo..."));
      EPD_HW_Init(); //Electronic paper initialization.
      EPD_SetRAMValue_BaseMap(gImage_basemap); //Background color basemap
      for(i=0;i<6;i++) {
        EPD_Dis_Part_Time(64,132+32*0,Num[i],         //x-A,y-A,DATA-A
                          64,132+32*1,Num[0],         //x-B,y-B,DATA-B
                          64,132+32*2,gImage_numdot, //x-C,y-C,DATA-C
                          64,132+32*3,Num[0],        //x-D,y-D,DATA-D
                          64,132+32*4,Num[1],32,64); //x-E,y-E,DATA-E,Resolution 32*64
        delay(500);
      }

      EPD_DeepSleep();
      delay(2000);

      Serial.println(F("Clear screen..."));
      EPD_HW_Init_Full();
      EPD_WhiteScreen_White();
      EPD_DeepSleep();
      delay(2000);
#endif

   Serial.println(F("Demo complete - the program stops here."));
   while(1);
}
