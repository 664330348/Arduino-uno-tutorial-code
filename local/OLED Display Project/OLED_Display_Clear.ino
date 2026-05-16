#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED object (I2C)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

  // Initialize OLED (address 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // Stop if OLED not found
  }

  // Clear screen buffer
  display.clearDisplay();

  // Push empty buffer to OLED (this actually clears screen)
  display.display();
}

void loop() {
  // Nothing here
}