#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// OLED CONFIG
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// =====================================================
// BUTTON CONFIG
// =====================================================
#define BTN_PIN 2

bool lastBtnState = HIGH;
unsigned long pressStart = 0;
bool longPressHandled = false;

// =====================================================
// STOPWATCH STATE
// =====================================================
bool running = false;

unsigned long startTime = 0;
unsigned long elapsedTime = 0;

// =====================================================
// HEADER
// =====================================================
void drawHeader(const char* title) {

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("OLED P11");

  display.setCursor(0, 8);
  display.print(title);

  display.drawLine(0, 16, 127, 16, SSD1306_WHITE);
}

// =====================================================
// FORMAT TIME
// =====================================================
void formatTime(unsigned long ms, char* buffer) {

  unsigned long totalSeconds = ms / 1000;

  int minutes = totalSeconds / 60;
  int seconds = totalSeconds % 60;
  int hundredths = (ms % 1000) / 10;

  sprintf(buffer, "%02d:%02d.%02d", minutes, seconds, hundredths);
}

// =====================================================
// BUTTON HANDLER
// =====================================================
void handleButton() {

  bool btnState = digitalRead(BTN_PIN);

  // Button pressed
  if (lastBtnState == HIGH && btnState == LOW) {
    pressStart = millis();
    longPressHandled = false;
  }

  // Long press reset
  if (btnState == LOW && !longPressHandled) {
    if (millis() - pressStart >= 1000) {

      running = false;
      elapsedTime = 0;
      startTime = millis();

      longPressHandled = true;
    }
  }

  // Button released
  if (lastBtnState == LOW && btnState == HIGH) {

    unsigned long pressDuration = millis() - pressStart;

    // Short press start / pause
    if (pressDuration < 1000 && !longPressHandled) {

      if (!running) {
        running = true;
        startTime = millis() - elapsedTime;
      }
      else {
        running = false;
        elapsedTime = millis() - startTime;
      }
    }

    delay(80);  // simple debounce
  }

  lastBtnState = btnState;
}

// =====================================================
// DRAW STOPWATCH SCREEN
// =====================================================
void drawStopwatch() {

  drawHeader("Mini Stopwatch");

  unsigned long currentElapsed;

  if (running) {
    currentElapsed = millis() - startTime;
  }
  else {
    currentElapsed = elapsedTime;
  }

  char timeText[12];
  formatTime(currentElapsed, timeText);

  // Main time
  display.setTextSize(2);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(
    timeText,
    0,
    0,
    &x1,
    &y1,
    &w,
    &h
  );

  int x = (SCREEN_WIDTH - w) / 2;

  display.setCursor(x, 26);
  display.print(timeText);

  // Status
  display.setTextSize(1);

  display.setCursor(0, 54);

  if (running) {
    display.print("Status: RUNNING");
  }
  else {
    display.print("Status: PAUSED");
  }
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  pinMode(BTN_PIN, INPUT_PULLUP);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C)) {
    for (;;);
  }

  display.clearDisplay();
  display.display();
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  handleButton();

  display.clearDisplay();

  drawStopwatch();

  display.display();

  delay(30);
}