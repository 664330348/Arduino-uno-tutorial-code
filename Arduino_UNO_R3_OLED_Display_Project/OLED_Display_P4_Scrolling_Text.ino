#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// =========================
// Data
// =========================
const char* timeList[] = {
  "0:00","1:00","2:00","3:00","4:00","5:00","6:00","7:00","8:00","9:00","10:00","11:00","12:00"
};

const char* marqueeText = " Arduino UNO R3 OLED Scrolling Text Demo ";

int timeOffset = 0;
int textOffset = 0;
int tick = 0;

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for (;;);
  }

  display.setTextWrap(false);   // Key: disable automatic line wrapping
}

void loop() {
  drawUI();

  delay(100);

  // Left side slow scrolling
  tick++;
  if (tick >= 20) {
    tick = 0;
    timeOffset = (timeOffset + 1) % 8;
  }

  // Right side scrolling
  textOffset++;
  if (textOffset > 200) textOffset = 0;
}

void drawUI() {

  display.clearDisplay();

  // =========================
  // HEADER
  // =========================
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("OLED Part 4");

  display.setCursor(0, 8);
  display.println("Scrolling Text");

  display.drawLine(0, 16, 128, 16, SSD1306_WHITE);

  // =========================
  // LEFT AREA
  // =========================
  int leftX = 5;
  int startY = 20;

  for (int i = 0; i < 4; i++) {
    int idx = (timeOffset + i) % 8;
    display.setCursor(leftX, startY + i * 10);
    display.print(timeList[idx]);
  }

  display.drawLine(42, 16, 42, 64, SSD1306_WHITE);

  // =========================
  // RIGHT AREA (FIXED)
  // =========================
  int rightX = 44;
  int rightY = 30;

  // Clear only the right area
  display.fillRect(44, 17, 84, 47, SSD1306_BLACK);

  // Key fix: use character shifting instead of negative coordinate drawing
  int len = strlen(marqueeText);
  int charOffset = textOffset / 6;   // approx 1 character per 6 pixels

  String visible = "";

  for (int i = 0; i < len; i++) {
    int idx = (i + charOffset) % len;
    visible += marqueeText[idx];
  }

  // Draw only inside right area starting point (no overflow to left side)
  display.setCursor(rightX, rightY);
  display.print(visible);

  display.display();
}