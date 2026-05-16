#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define TOP_OFFSET 16   // yellow header area

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();

  // =========================
  // 🟡 TITLE AREA
  // =========================
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("OLED Part 2");

  // separator line
  display.drawLine(0, TOP_OFFSET, 127, TOP_OFFSET, SSD1306_WHITE);

  // =========================
  // 🔵 LEFT SIDE (Text Size 2)
  // =========================
  display.setTextSize(2);
  display.setCursor(0, TOP_OFFSET + 5);
  display.println("Size2");


  // vertical divider line
  display.drawLine(63, TOP_OFFSET, 63, 63, SSD1306_WHITE);

  // =========================
  // 🔵 RIGHT SIDE (Text Size 1)
  // =========================
  display.setTextSize(1);
  display.setCursor(70, TOP_OFFSET + 5);
  display.println("Size1");

  // =========================
  // OPTIONAL SHAPES (bottom area)
  // =========================
  display.drawRect(0, 50, 20, 12, SSD1306_WHITE);
  display.fillRect(25, 50, 20, 12, SSD1306_WHITE);

  display.drawCircle(85, 56, 6, SSD1306_WHITE);
  display.fillCircle(110, 56, 6, SSD1306_WHITE);

  // =========================
  // SHOW
  // =========================
  display.display();
}

void loop() {
}