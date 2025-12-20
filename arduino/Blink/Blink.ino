void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  #ifdef ARDUINO_ADAFRUIT_FEATHER_RP2040_THINKINK
  Serial.println("ARDUINO_ADAFRUIT_FEATHER_RP2040_THINKINK");
  #endif

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
}
