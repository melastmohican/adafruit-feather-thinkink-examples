/*****************************************************************************
* | File        :   Waveshare_3in52_PanelID.ino
* | Function    :   Ask the 3.52inch e-Paper (B) panel to identify itself
* | Info        :
*----------------
* Read-only diagnostic for the Waveshare 3.52inch e-Paper (B) on an Adafruit
* Feather RP2040 ThinkInk. Companion to the Waveshare_3in52 demo sketch; the
* DEV_Config / EPD_3in52b files here are copies of that sketch's.
*
* The panel's user manual advertises "10-byte OTP space for module
* identification", and its command table exposes three read commands:
*
*   0x70  REV   - CHIP_REV[7:0] then LUT_REV[23:0]
*   0x71  FLG   - status flags (PTL_FLAG, I2C_ERR, I2C_BUSYN, DATA_FLAG,
*                 PON, POF, BUSY_N)
*   0xA2  ROTP  - OTP contents from address 0 upward
*
* On the ThinkInk the panel's SDIO line is a single bidirectional pin (GPIO23,
* shared with EPD_MOSI), so reads have to be bit-banged with hardware SPI1
* released. That is what DEV_GPIO_Init() / DEV_SPI_Init() are for.
*
* SAFETY: this sketch only ever issues read commands. It never sends 0xA0
* (Program Mode), 0xA1 (Active Programming) or 0xA3 (OTP Programming Address),
* which would write OTP irreversibly. It also never triggers a refresh, so
* whatever is currently on the panel stays there.
*
* Board : rp2040:rp2040:adafruit_feather_thinkink
******************************************************************************/
#include "EPD_3in52b.h"

// Read clock: the datasheet wants a serial clock cycle of at least 350ns for
// reads (~2.8MHz ceiling). One microsecond per half-cycle puts us near 500kHz,
// comfortably slow.
#define BB_HALF_CYCLE_US 1

static void bb_write_byte(uint8_t data)
{
    pinMode(EPD_MOSI_PIN, OUTPUT);
    for (int i = 0; i < 8; i++) {
        digitalWrite(EPD_MOSI_PIN, (data & 0x80) ? HIGH : LOW);   // set while clock low
        data <<= 1;
        digitalWrite(EPD_SCK_PIN, HIGH);                          // mode 0: latch on rise
        delayMicroseconds(BB_HALF_CYCLE_US);
        digitalWrite(EPD_SCK_PIN, LOW);
        delayMicroseconds(BB_HALF_CYCLE_US);
    }
}

static uint8_t bb_read_byte(void)
{
    uint8_t v = 0;
    pinMode(EPD_MOSI_PIN, INPUT);
    for (int i = 0; i < 8; i++) {
        digitalWrite(EPD_SCK_PIN, HIGH);
        delayMicroseconds(BB_HALF_CYCLE_US);
        v = (v << 1) | (digitalRead(EPD_MOSI_PIN) ? 1 : 0);       // sample on the rise
        digitalWrite(EPD_SCK_PIN, LOW);
        delayMicroseconds(BB_HALF_CYCLE_US);
    }
    return v;
}

// Send one command byte, then clock n bytes back with CS held low for the whole
// transaction. Whether this controller needs a leading dummy byte is not stated
// in the manual, so the caller just reads extra and we eyeball where the data
// actually starts.
static void read_reg(uint8_t cmd, uint8_t *buf, size_t n)
{
    digitalWrite(EPD_DC_PIN, LOW);
    digitalWrite(EPD_CS_PIN, LOW);
    bb_write_byte(cmd);

    digitalWrite(EPD_DC_PIN, HIGH);
    for (size_t i = 0; i < n; i++) {
        buf[i] = bb_read_byte();
    }
    digitalWrite(EPD_CS_PIN, HIGH);
}

static void dump(const char *label, uint8_t cmd, const uint8_t *buf, size_t n)
{
    Serial.printf("\r\n%s  (command 0x%02X, %u bytes)\r\n", label, cmd, (unsigned)n);
    for (size_t i = 0; i < n; i += 16) {
        Serial.printf("  +%02u  ", (unsigned)i);
        for (size_t j = 0; j < 16; j++) {
            if (i + j < n) Serial.printf("%02X ", buf[i + j]);
            else           Serial.print("   ");
        }
        Serial.print(" |");
        for (size_t j = 0; j < 16 && i + j < n; j++) {
            uint8_t c = buf[i + j];
            Serial.print((c >= 0x20 && c < 0x7F) ? (char)c : '.');
        }
        Serial.println("|");
    }
}

void setup()
{
    // One-shot output, so wait for a host to actually open the port before
    // anything is printed.
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }

    DEV_Module_Init();
    Serial.printf("=== Waveshare 3.52inch e-Paper (B) panel ID readback ===\r\n");

    // Registers are only meaningful with the panel powered up. Init configures
    // and powers on; it does not refresh, so the current image is left alone.
    Serial.printf("Powering panel...\r\n");
    EPD_3IN52B_Init();

    // Hand GPIO22/23 over from SPI1 to bit-banging so SDIO can be read.
    DEV_GPIO_Init();

    uint8_t rev[8], flg[8], otp[64];
    read_reg(0x70, rev, sizeof(rev));
    read_reg(0x71, flg, sizeof(flg));
    read_reg(0xA2, otp, sizeof(otp));

    // Back to hardware SPI so sleep() can be sent normally.
    DEV_SPI_Init();

    dump("REV   - chip / LUT revision", 0x70, rev, sizeof(rev));
    Serial.printf("        expecting CHIP_REV default 0x09, LUT_REV default 0xFFFFFF\r\n");

    dump("FLG   - status flags",        0x71, flg, sizeof(flg));
    Serial.printf("        expecting something near the 0x13 default\r\n");

    dump("ROTP  - OTP contents",        0xA2, otp, sizeof(otp));
    Serial.printf("        module identification is documented as 10 bytes\r\n");

    Serial.printf("\r\nAll 0x00 or all 0xFF means the panel never drove SDIO and the\r\n");
    Serial.printf("readback did not take - not that the panel is faulty.\r\n");

    EPD_3IN52B_sleep();
    Serial.printf("\r\nPanel asleep. Done.\r\n");
}

void loop()
{
}
