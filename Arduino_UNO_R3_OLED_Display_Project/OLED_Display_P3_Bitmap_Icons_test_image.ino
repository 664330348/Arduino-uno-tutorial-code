#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Replace this with your own bitmap array
const unsigned char bitmap[] PROGMEM = {
};

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  // Replace width and height with your bitmap size
  display.drawBitmap(0, 0, bitmap, 128, 64, SSD1306_WHITE);

  display.display();
}

void loop() {
}