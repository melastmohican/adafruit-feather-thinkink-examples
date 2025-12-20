// GxEPD2 Example for Adafruit Feather RP2040 ThinkInk
// with 2.13" 250x122 Quad-Color (4-color) eInk Display (JD79661 chipset)
//
// Based on GxEPD2 library by Jean-Marc Zingg
// https://github.com/ZinggJM/GxEPD2

#include <GxEPD2_4C.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include "bitmaps/Bitmaps4c128x250.h" // 2.13" 4-color

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
  Serial.println("GxEPD2 ThinkInk QuadColor Example");
  Serial.println("==================================");
  
  // Configure SPI1 with ThinkInk EPD pins (non-default SPI1 pins)
  Serial.println("Configuring SPI1 with EPD pins...");
  Serial.print("  CS="); Serial.print(PIN_EPD_CS);
  Serial.print(", DC="); Serial.print(PIN_EPD_DC);
  Serial.print(", RST="); Serial.print(PIN_EPD_RESET);
  Serial.print(", BUSY="); Serial.println(PIN_EPD_BUSY);
  Serial.print("  SCK="); Serial.print(PIN_EPD_SCK);
  Serial.print(", MOSI="); Serial.println(PIN_EPD_MOSI);
  
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

  // Run demos
  helloWorld();
  delay(3000);

  helloFullScreenPartialMode();
  delay(3000);

  helloArduino();
  delay(3000);
  
  helloEpaper();
  delay(3000);

  showFont("FreeMonoBold9pt7b", &FreeMonoBold9pt7b);
  delay(3000);

  display.writeScreenBuffer();
  drawBitmaps();
  delay(3000);

  drawGraphics();
  delay(3000);
  
  showAllColors();
  delay(3000);
  
  drawColorBars();
  delay(3000);
  
  drawShapes();
  delay(3000);
  
  // Power off display
  display.powerOff();
  Serial.println("Demo complete!");
}

void loop()
{
  // Nothing to do in loop
}

const char HelloWorld[] = "Hello World!";
const char HelloArduino[] = "Hello Arduino!";
const char HelloEpaper[] = "Hello E-Paper!";

// Display "Hello World!" centered on screen
void helloWorld()
{
  Serial.println("Running: helloWorld");
  
  const char text[] = "Hello World!";
  
  display.setRotation(1);  // Landscape mode
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  
  // Calculate centered position
  int16_t tbx, tby;
  uint16_t tbw, tbh;
  display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = ((display.height() - tbh) / 2) - tby;
  
  display.setFullWindow();
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(text);
  } while (display.nextPage());
}

void helloFullScreenPartialMode()
{
  Serial.println("helloFullScreenPartialMode");
  const char fullscreen[] = "full screen update";
  const char fpm[] = "fast partial mode";
  const char spm[] = "slow partial mode";
  const char npm[] = "no partial mode";
  display.setPartialWindow(0, 0, display.width(), display.height());
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(GxEPD_BLACK);
  const char* updatemode;
  if (display.epd2.hasFastPartialUpdate)
  {
    updatemode = fpm;
  }
  else if (display.epd2.hasPartialUpdate)
  {
    updatemode = spm;
  }
  else
  {
    updatemode = npm;
  }
  // do this outside of the loop
  int16_t tbx, tby; uint16_t tbw, tbh;
  // center update text
  display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t utx = ((display.width() - tbw) / 2) - tbx;
  uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
  // center update mode
  display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t umx = ((display.width() - tbw) / 2) - tbx;
  uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
  // center HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
  uint16_t hwy = ((display.height() - tbh) / 2) - tby;
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(hwx, hwy);
    display.print(HelloWorld);
    display.setCursor(utx, uty);
    display.print(fullscreen);
    display.setCursor(umx, umy);
    display.print(updatemode);
  }
  while (display.nextPage());
  Serial.println("helloFullScreenPartialMode done");
}

void helloArduino()
{
  Serial.println("helloArduino");
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  // align with centered HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  // height might be different
  display.getTextBounds(HelloArduino, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t y = ((display.height() / 4) - tbh / 2) - tby; // y is base line!
  // make the window big enough to cover (overwrite) descenders of previous text
  uint16_t wh = FreeMonoBold9pt7b.yAdvance;
  uint16_t wy = (display.height() / 4) - wh / 2;
  display.setPartialWindow(0, wy, display.width(), wh);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    //display.drawRect(x, y - tbh, tbw, tbh, GxEPD_BLACK);
    display.setCursor(x, y);
    display.print(HelloArduino);
  }
  while (display.nextPage());
  delay(1000);
  Serial.println("helloArduino done");
}

void helloEpaper()
{
  Serial.println("helloEpaper");
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  if (display.epd2.WIDTH < 104) display.setFont(0);
  display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  // align with centered HelloWorld
  display.getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  // height might be different
  display.getTextBounds(HelloEpaper, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t y = ((display.height() * 3 / 4) - tbh / 2) - tby; // y is base line!
  // make the window big enough to cover (overwrite) descenders of previous text
  uint16_t wh = FreeMonoBold9pt7b.yAdvance;
  uint16_t wy = (display.height() * 3 / 4) - wh / 2;
  display.setPartialWindow(0, wy, display.width(), wh);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloEpaper);
  }
  while (display.nextPage());
  Serial.println("helloEpaper done");
}

// test partial window issue on GDEW0213Z19 and GDEH029Z13
void stripeTest()
{
  helloStripe(104);
  delay(2000);
  helloStripe(96);
}

const char HelloStripe[] = "Hello Stripe!";

void helloStripe(uint16_t pw_xe) // end of partial window in physcal x direction
{
  Serial.print("HelloStripe("); Serial.print(pw_xe); Serial.println(")");
  display.setRotation(3);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(HelloStripe, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t wh = FreeMonoBold9pt7b.yAdvance;
  uint16_t wy = pw_xe - wh;
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  uint16_t y = wy - tby;
  display.setPartialWindow(0, wy, display.width(), wh);
  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);
    display.setCursor(x, y);
    display.print(HelloStripe);
  }
  while (display.nextPage());
  Serial.println("HelloStripe done");
}

void showFont(const char name[], const GFXfont* f)
{
  display.setFullWindow();
  display.setRotation(0);
  display.setTextColor(GxEPD_BLACK);
  display.firstPage();
  do
  {
    drawFont(name, f);
  }
  while (display.nextPage());
}

void drawFont(const char name[], const GFXfont* f)
{
  //display.setRotation(0);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println(name);
  display.println(" !\"#$%&'()*+,-./");
  display.println("0123456789:;<=>?");
  display.println("@ABCDEFGHIJKLMNO");
  display.println("PQRSTUVWXYZ[\\]^_");
  if (display.epd2.hasColor)
  {
    display.setTextColor(GxEPD_RED);
  }
  display.println("`abcdefghijklmno");
  display.println("pqrstuvwxyz{|}~ ");
}

// Show all 4 colors with labels
void showAllColors()
{
  Serial.println("Running: showAllColors");
  
  display.setRotation(1);  // Landscape
  display.setFont(&FreeMonoBold9pt7b);
  display.setFullWindow();
  
  uint16_t w = display.width();
  uint16_t h = display.height();
  uint16_t boxH = h / 4;
  
  display.firstPage();
  do {
    // White section (top)
    display.fillRect(0, 0, w, boxH, GxEPD_WHITE);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(10, boxH/2 + 5);
    display.print("WHITE");
    
    // Black section
    display.fillRect(0, boxH, w, boxH, GxEPD_BLACK);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(10, boxH + boxH/2 + 5);
    display.print("BLACK");
    
    // Red section
    display.fillRect(0, boxH*2, w, boxH, GxEPD_RED);
    display.setTextColor(GxEPD_WHITE);
    display.setCursor(10, boxH*2 + boxH/2 + 5);
    display.print("RED");
    
    // Yellow section (bottom)
    display.fillRect(0, boxH*3, w, boxH, GxEPD_YELLOW);
    display.setTextColor(GxEPD_BLACK);
    display.setCursor(10, boxH*3 + boxH/2 + 5);
    display.print("YELLOW");
  } while (display.nextPage());
}

// Draw vertical color bars
void drawColorBars()
{
  Serial.println("Running: drawColorBars");
  
  display.setRotation(1);  // Landscape
  display.setFullWindow();
  
  uint16_t w = display.width();
  uint16_t h = display.height();
  uint16_t barW = w / 4;
  
  display.firstPage();
  do {
    display.fillRect(0, 0, barW, h, GxEPD_WHITE);
    display.fillRect(barW, 0, barW, h, GxEPD_BLACK);
    display.fillRect(barW*2, 0, barW, h, GxEPD_RED);
    display.fillRect(barW*3, 0, barW, h, GxEPD_YELLOW);
  } while (display.nextPage());
}

// Draw various shapes using all colors
void drawShapes()
{
  Serial.println("Running: drawShapes");
  
  display.setRotation(1);  // Landscape
  display.setFullWindow();
  
  uint16_t w = display.width();
  uint16_t h = display.height();
  
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);
    
    // Draw border
    display.drawRect(0, 0, w, h, GxEPD_BLACK);
    display.drawRect(2, 2, w-4, h-4, GxEPD_BLACK);
    
    // Filled circles in each quadrant
    display.fillCircle(w/4, h/4, 20, GxEPD_BLACK);
    display.fillCircle(w*3/4, h/4, 20, GxEPD_RED);
    display.fillCircle(w/4, h*3/4, 20, GxEPD_YELLOW);
    display.fillCircle(w*3/4, h*3/4, 20, GxEPD_BLACK);
    
    // Center text
    display.setFont(&FreeMonoBold9pt7b);
    display.setTextColor(GxEPD_RED);
    const char* text = "4-Color";
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display.getTextBounds(text, 0, 0, &tbx, &tby, &tbw, &tbh);
    display.setCursor((w - tbw) / 2 - tbx, (h - tbh) / 2 - tby);
    display.print(text);
    
    // Diagonal lines
    display.drawLine(0, 0, w/4 - 25, h/4 - 25, GxEPD_BLACK);
    display.drawLine(w, 0, w*3/4 + 25, h/4 - 25, GxEPD_RED);
    display.drawLine(0, h, w/4 - 25, h*3/4 + 25, GxEPD_YELLOW);
    display.drawLine(w, h, w*3/4 + 25, h*3/4 + 25, GxEPD_BLACK);
  } while (display.nextPage());
}

void drawBitmaps()
{
  display.setRotation(0);
  display.setFullWindow();
#ifdef _GxBitmaps128x250_H_
  drawBitmaps128x250();
#endif
#ifdef _GxBitmaps3c128x250_H_
  drawBitmaps3c128x250();
#endif
  // 4-color
#if defined(_GxBitmaps4c128x250_H_)
  drawBitmaps4c128x250();
#endif
}

#if defined(_GxBitmaps4c128x250_H_)
void drawBitmaps4c128x250()
{
  if (display.epd2.panel == GxEPD2::GDEY0213F51)
  {
    display.drawNative(Bitmap4c128x250, 0, 0, 0, 128, 250, false, false, true);
    delay(5000);
  }
}
#endif

void drawGraphics()
{
  display.setRotation(0);
  display.firstPage();
  do
  {
    display.drawRect(display.width() / 8, display.height() / 8, display.width() * 3 / 4, display.height() * 3 / 4, GxEPD_BLACK);
    display.drawLine(display.width() / 8, display.height() / 8, display.width() * 7 / 8, display.height() * 7 / 8, GxEPD_BLACK);
    display.drawLine(display.width() / 8, display.height() * 7 / 8, display.width() * 7 / 8, display.height() / 8, GxEPD_BLACK);
    display.drawCircle(display.width() / 2, display.height() / 2, display.height() / 4, GxEPD_BLACK);
    display.drawPixel(display.width() / 4, display.height() / 2 , GxEPD_BLACK);
    display.drawPixel(display.width() * 3 / 4, display.height() / 2 , GxEPD_BLACK);
  }
  while (display.nextPage());
  delay(1000);
}



