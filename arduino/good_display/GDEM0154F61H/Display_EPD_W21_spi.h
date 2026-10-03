#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "Arduino.h"
#include <SPI.h>

// IO settings
// Adafruit Feather RP2040 ThinkInk: the panel sits in the board's dedicated
// 24-pin FPC EPD connector. Those signals reach only that connector, never the
// Feather headers, so there is nothing to wire and the pin numbers come from
// the board variant. They are on SPI1, not the default SPI.
// HSCLK---22
// HMOSI---23
#define EPD_SPI_PORT SPI1
#define isEPD_W21_BUSY digitalRead(PIN_EPD_BUSY)
#define EPD_W21_RST_0 digitalWrite(PIN_EPD_RESET, LOW)
#define EPD_W21_RST_1 digitalWrite(PIN_EPD_RESET, HIGH)
#define EPD_W21_DC_0  digitalWrite(PIN_EPD_DC, LOW)
#define EPD_W21_DC_1  digitalWrite(PIN_EPD_DC, HIGH)
#define EPD_W21_CS_0 digitalWrite(PIN_EPD_CS, LOW)
#define EPD_W21_CS_1 digitalWrite(PIN_EPD_CS, HIGH)

void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char datas);
void EPD_W21_WriteCMD(unsigned char command);

#endif
