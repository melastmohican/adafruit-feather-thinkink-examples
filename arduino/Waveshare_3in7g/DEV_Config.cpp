/*****************************************************************************
* | File      	:   DEV_Config.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-02-19
* | Info        :
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#include "DEV_Config.h"

void GPIO_Config(void)
{
    pinMode(EPD_BUSY_PIN,  INPUT);
    pinMode(EPD_RST_PIN , OUTPUT);
    pinMode(EPD_DC_PIN  , OUTPUT);
    pinMode(EPD_CS_PIN , OUTPUT);

    digitalWrite(EPD_CS_PIN , HIGH);
}

void GPIO_Mode(UWORD GPIO_Pin, UWORD Mode)
{
    if(Mode == 0) {
        pinMode(GPIO_Pin , INPUT);
	} else {
		pinMode(GPIO_Pin , OUTPUT);
	}
}
/******************************************************************************
function:	Module Initialize, the BCM2835 library and initialize the pins, SPI protocol
parameter:
Info:
******************************************************************************/
UBYTE DEV_Module_Init(void)
{
	//gpio
	GPIO_Config();

	//serial
	Serial.begin(115200);
	while (!Serial && millis() < 3000) {	// give USB CDC time to enumerate
		delay(10);
	}

	// spi: remap SPI1 onto the EPD pins (GPIO22=SCK, GPIO23=MOSI) before begin
	EPD_SPI_PORT.setSCK(EPD_SCK_PIN);
	EPD_SPI_PORT.setTX(EPD_MOSI_PIN);
	EPD_SPI_PORT.begin();
    EPD_SPI_PORT.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

	return 0;
}


void DEV_GPIO_Init(void)
{
    EPD_SPI_PORT.end();
    pinMode(EPD_SCK_PIN, OUTPUT);
    pinMode(EPD_MOSI_PIN, OUTPUT);
}

void DEV_SPI_Init(void)
{
    EPD_SPI_PORT.setSCK(EPD_SCK_PIN);
    EPD_SPI_PORT.setTX(EPD_MOSI_PIN);
    EPD_SPI_PORT.begin();
    EPD_SPI_PORT.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}


/******************************************************************************
function:
			SPI read and write
******************************************************************************/
void DEV_SPI_WriteByte(UBYTE data)
{
    EPD_SPI_PORT.transfer(data);
}

void DEV_SPI_Write_nByte(UBYTE *pData, UDOUBLE len)
{
    for (int i = 0; i < len; i++)
        DEV_SPI_WriteByte(pData[i]);
}

void DEV_SPI_SendByte(UBYTE data)
{
    GPIO_Mode(EPD_MOSI_PIN, OUTPUT);
    digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);
    for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0) digitalWrite(EPD_MOSI_PIN, GPIO_PIN_RESET); 
        else                    digitalWrite(EPD_MOSI_PIN, GPIO_PIN_SET);

        data <<= 1;
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
    }
    digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
}

UBYTE DEV_SPI_ReadByte()
{
    UBYTE j=0xff;
    GPIO_Mode(EPD_MOSI_PIN, INPUT);
    digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);
    for (int i = 0; i < 8; i++)
    {
        j = j << 1;
        if (digitalRead(EPD_MOSI_PIN))  j = j | 0x01;
        else                            j = j & 0xfe;
        
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
    }
    digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
    GPIO_Mode(EPD_MOSI_PIN, 1);
    return j;
}




void DEV_Module_Exit(void)
{
    // Nothing to do on the Feather RP2040 ThinkInk: the 24-pin EPD connector has
    // no software power gate. The panel is already in deep sleep by this point
    // (EPD_3IN7G_Sleep), which is as low-power as it gets here.
}

