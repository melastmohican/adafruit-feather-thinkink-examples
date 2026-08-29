/*****************************************************************************
* | File        :   Waveshare_2in66b_PanelID.ino
* | Function    :   Ask the 2.66inch e-Paper (B) panel to identify itself
* | Info        :
*----------------
* Read-only diagnostic for the Waveshare 2.66inch e-Paper (B) / Good Display
* GDEY0266Z90 / DKE DEPG0266RWS on an Adafruit Feather RP2040 ThinkInk.
* Companion to the Waveshare_2in66br demo sketch; the epdif / epd2in66b files
* here are copies of that sketch's, plus EpdIf::ReleaseSpi()/RestoreSpi().
*
* Cloned from Waveshare_3in52_PanelID. The bit-bang transport is the same, but
* the controller is not, so every register differs. Waveshare's product
* specification (2.66inch-e-paper-b-specification.pdf, section 7 Command Table)
* gives this panel's driver IC as SSD1680Z8 and documents three reads:
*
*   0x0A  Read Register for Initial Code Setting  (A..D, documented "Reserved")
*   0x2D  OTP Register Read for Display Option - the useful one, 11 bytes:
*           A     VCOM OTP Selection      (cmd 0x37 byte A)
*           B     VCOM Register           (cmd 0x2C)
*           C..G  Display Mode            (cmd 0x37 bytes B..F)  [5 bytes]
*           H..K  Waveform Version        (cmd 0x37 bytes G..J)  [4 bytes]
*   0x2F  Status Bit Read [POR 0x01]:
*           A[5] HV Ready (0=ready), A[4] VCI Detect (0=normal),
*           A[2] Busy, A[1:0] Chip ID [POR=01]
*         A[5]/A[4] are only valid after commands 0x14 / 0x15, which this
*         sketch does not send, so treat those two bits as meaningless here.
*
* The waveform version in 0x2D is the interesting field: this SKU is
* multi-sourced (the panel on hand is silkscreened DEPG0266RWS800F34HP, a DKE
* part, while the libraries drive it as Good Display GDEY0266Z90), so it is the
* only way short of reading the label to tell whether two panels really match.
*
* On the ThinkInk the panel's SDA line is a single bidirectional pin (GPIO23,
* shared with EPD_MOSI - confirmed by the spec's pin table, pin 14 SDA "I/O"),
* so reads have to be bit-banged with hardware SPI1 released. That is what
* EpdIf::ReleaseSpi() / EpdIf::RestoreSpi() are for.
*
* SAFETY: this sketch only ever issues read commands. It never sends 0x30
* (Program OTP of Waveform Setting) or 0x39 (OTP program mode), which would
* write OTP irreversibly, nor 0x32 / 0x37 (write LUT / display option). It also
* never triggers a display update (0x20), so whatever is currently on the panel
* stays there.
*
* Board : rp2040:rp2040:adafruit_feather_thinkink
******************************************************************************/
#include "epd2in66b.h"

// Read clock. SSD1680 wants a slow serial clock for reads; one microsecond per
// half-cycle puts us near 500kHz, comfortably slow. Same value as the 3.52"
// sketch, which read reliably at this rate.
#define BB_HALF_CYCLE_US 1

// How many bytes to actually clock back. The 3.52" panel needed a leading dummy
// byte that its manual never mentioned, so over-read here too and eyeball where
// the payload starts rather than trusting an offset.
#define N_INIT_CODE 8    // 4 documented
#define N_DISP_OPT  16   // 11 documented
#define N_STATUS    8    // 1 documented

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

// 0x2D reports shadow registers that are only populated once the OTP has been
// loaded. Epd::Init() never loads it, so without this preamble the read comes
// back all zeros - reset values, not a failed read.
//
// 0x22 = 0xB1 is enable-clock + load-temperature + load-LUT/OTP + disable-clock.
// The "display" bit (0x04) is NOT set, so the following 0x20 loads the OTP
// without refreshing the panel - the same preamble Good Display's own
// EPD_HW_Init_Fast() opens with. Set to 0 to see the un-loaded values.
#define LOAD_OTP_BEFORE_READ 1

static void bb_cmd(uint8_t c)
{
    digitalWrite(DC_PIN, LOW);
    digitalWrite(CS_PIN, LOW);
    bb_write_byte(c);
    digitalWrite(CS_PIN, HIGH);
}

static void bb_data(uint8_t d)
{
    digitalWrite(DC_PIN, HIGH);
    digitalWrite(CS_PIN, LOW);
    bb_write_byte(d);
    digitalWrite(CS_PIN, HIGH);
}

static void bb_wait_idle(uint32_t timeout_ms)
{
    uint32_t t0 = millis();
    while (digitalRead(BUSY_PIN)) {          // BUSY is active HIGH on SSD1680
        if (millis() - t0 > timeout_ms) {
            Serial.printf("        (BUSY timeout during OTP load)\r\n");
            return;
        }
        delay(10);
    }
}

// Send one command byte, then clock n bytes back with CS held low for the whole
// transaction.
static void read_reg(uint8_t cmd, uint8_t *buf, size_t n)
{
    digitalWrite(DC_PIN, LOW);
    digitalWrite(CS_PIN, LOW);
    bb_write_byte(cmd);

    digitalWrite(DC_PIN, HIGH);
    for (size_t i = 0; i < n; i++) {
        buf[i] = bb_read_byte();
    }
    digitalWrite(CS_PIN, HIGH);
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

// Decode 0x2F assuming the payload starts at the given offset. Printed for the
// first two candidate offsets so a leading dummy byte is obvious either way.
static void decode_status(const uint8_t *buf, size_t off)
{
    uint8_t a = buf[off];
    Serial.printf("        if payload starts at +%u: 0x%02X -> "
                  "Chip ID %u, Busy %u (HV/VCI bits not valid, see header)\r\n",
                  (unsigned)off, a, a & 0x03, (a >> 2) & 1);
}

void setup()
{
    // One-shot output, so wait for a host to actually open the port before
    // anything is printed.
    Serial.begin(115200);
    while (!Serial) {
        delay(10);
    }

    Serial.printf("=== Waveshare 2.66inch e-Paper (B) panel ID readback ===\r\n");
    Serial.printf("    driver IC per Waveshare spec: SSD1680Z8, 296gate x 152source\r\n");

    // Registers are only meaningful with the panel powered up. Init() configures
    // and powers on; it sends no 0x20, so the current image is left alone.
    Serial.printf("Powering panel...\r\n");
    Epd epd;
    if (epd.Init() != 0) {
        Serial.printf("e-Paper init failed - stopping.\r\n");
        return;
    }

    // Hand GPIO22/23 over from SPI1 to bit-banging so SDA can be read.
    EpdIf::ReleaseSpi();

#if LOAD_OTP_BEFORE_READ
    Serial.printf("Loading OTP (no display update)...\r\n");
    bb_cmd(0x22);
    bb_data(0xB1);
    bb_cmd(0x20);
    bb_wait_idle(5000);
#endif

    uint8_t initcode[N_INIT_CODE], dispopt[N_DISP_OPT], status[N_STATUS];
    read_reg(0x0A, initcode, sizeof(initcode));
    read_reg(0x2D, dispopt,  sizeof(dispopt));
    read_reg(0x2F, status,   sizeof(status));

    // Back to hardware SPI so Sleep() can be sent normally.
    EpdIf::RestoreSpi();

    dump("INIT  - initial code setting", 0x0A, initcode, sizeof(initcode));
    Serial.printf("        spec calls A..D \"Reserved\"; shown for completeness\r\n");

    dump("DOPT  - display option / OTP", 0x2D, dispopt, sizeof(dispopt));
    Serial.printf("        layout: VCOM_OTP_SEL, VCOM, DISP_MODE[5], WAVEFORM_VER[4]\r\n");
    Serial.printf("        the last 4 payload bytes are the panel fingerprint\r\n");

    dump("STAT  - status bits",          0x2F, status, sizeof(status));
    decode_status(status, 0);
    decode_status(status, 1);
    Serial.printf("        POR value is 0x01, so Chip ID 1 with everything else clear\r\n");
    Serial.printf("        is the expected healthy answer\r\n");

    Serial.printf("\r\nAll 0x00 or all 0xFF means the panel never drove SDA and the\r\n");
    Serial.printf("readback did not take - not that the panel is faulty.\r\n");

    epd.Sleep();
    Serial.printf("\r\nPanel asleep. Done.\r\n");
}

void loop()
{
}
