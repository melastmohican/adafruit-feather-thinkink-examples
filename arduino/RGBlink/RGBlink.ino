#include <Adafruit_NeoPixel.h>

#define NUM_PIXELS 1

// Initialize the NeoPixel object using the built-in PIN_NEOPIXEL constant
Adafruit_NeoPixel pixel(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  // Essential for RP2040 Feathers: Power the onboard NeoPixel
  #if defined(NEOPIXEL_POWER)
    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, HIGH);
  #endif

  pixel.begin();
  pixel.setBrightness(30); // Keep it dim to save eyes/power
}

void loop() {
  // Cycle through the full hue range (0 to 65535)
  for (long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
    
    // Use gamma32 for more natural color transitions
    pixel.setPixelColor(0, pixel.gamma32(pixel.ColorHSV(firstPixelHue)));
    
    pixel.show();
    delay(10);
  }
}
