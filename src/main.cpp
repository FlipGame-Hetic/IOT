#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Config.h>

// Setup pixel for the integrated LED
Adafruit_NeoPixel pixel(NUM_LED, TEST_LED, NEO_GRB + NEO_KHZ800);

// Function for the rainbow effect
uint32_t wheel(byte pos) {
  pos = 255 - pos;
  if (pos < 85) {
    return pixel.Color(255 - pos * 3, 0, pos * 3);
  }
  if (pos < 170) {
    pos -= 85;
    return pixel.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return pixel.Color(pos * 3, 255 - pos * 3, 0);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("[ESP32-C6] Starting...");
  
  pixel.begin();
}

void loop() {
  for (int j = 0; j < 256; j++) {
    pixel.setPixelColor(0, wheel(j));
    pixel.show();
    delay(10);
  }
}