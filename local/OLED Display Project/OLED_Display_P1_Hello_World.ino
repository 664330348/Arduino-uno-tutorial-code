#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED display object using I2C connection
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

  // Initialize OLED display (I2C address is usually 0x3C)
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    // If initialization fails, stop the program here
    while (true);
  }

  // Clear the display buffer
  display.clearDisplay();

  // Set text size (1 = small, 2 = medium, 3 = large, etc.)
  display.setTextSize(2);

  // Set text color (WHITE = pixels ON for OLED)
  display.setTextColor(WHITE);

  // Set cursor position for first line
  display.setCursor(10, 20);
  display.print("Hello");

  // Set cursor position for second line
  display.setCursor(10, 40);
  display.print("World");

  // Push the buffer content to the OLED screen
  display.display();
}

void loop() {
  // Nothing here (static display)
}