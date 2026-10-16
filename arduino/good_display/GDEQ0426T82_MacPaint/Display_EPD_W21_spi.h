#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "Arduino.h"
#include <SPI.h>

// Adafruit Feather RP2040 ThinkInk: the panel sits in the board's dedicated
// 24-pin FPC EPD connector on secondary SPI (SPI1).
#define EPD_SPI_PORT SPI1
#define isEPD_W21_BUSY digitalRead(PIN_EPD_BUSY)
#define EPD_W21_RST_0 digitalWrite(PIN_EPD_RESET,LOW)
#define EPD_W21_RST_1 digitalWrite(PIN_EPD_RESET,HIGH)
#define EPD_W21_DC_0  digitalWrite(PIN_EPD_DC,LOW)
#define EPD_W21_DC_1  digitalWrite(PIN_EPD_DC,HIGH)
#define EPD_W21_CS_0 digitalWrite(PIN_EPD_CS,LOW)
#define EPD_W21_CS_1 digitalWrite(PIN_EPD_CS,HIGH)

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);

#endif
