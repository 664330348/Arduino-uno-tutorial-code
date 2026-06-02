#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================
// Display Setup
// =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// =====================
// Button
// =====================
#define BTN_PIN 2

// =====================
// UI State Machine
// =====================
enum State {
  STATE_MENU,
  STATE_COUNTER,
  STATE_INFO,
  STATE_RESET
};

State state = STATE_MENU;

// =====================
// Menu System
// =====================
int menuIndex = 0;
const int menuSize = 3;

const char* menuItems[] = {
  "Counter",
  "Reset",
  "Info"
};

// =====================
// Counter Value
// =====================
int counter = 0;

// =====================
// Reset Timer
// =====================
unsigned long resetStartTime = 0;
int countdown = 3;

// =====================
// Button State Tracking
// =====================
bool lastBtn = HIGH;
unsigned long pressStart = 0;
bool longPressHandled = false;

// =====================
// Draw Header (Top Bar)
// =====================
void drawHeader(const char* title) {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("OLED P6");

  display.setCursor(0, 8);
  display.print(title);

  // separator line
  display.drawLine(0, 16, 128, 16, SSD1306_WHITE);
}

// =====================
// Draw Menu Screen
// =====================
void drawMenu() {
  drawHeader("Button Interface");

  for (int i = 0; i < menuSize; i++) {
    display.setCursor(0, 20 + i * 10);

    if (i == menuIndex) display.print("> ");
    else display.print("  ");

    display.print(menuItems[i]);
  }
}

// =====================
// Draw Counter Screen
// =====================
void drawCounter() {
  drawHeader("Counter");

  display.setTextSize(2);
  display.setCursor(0, 30);
  display.print("Value:");
  display.print(counter);

  display.setTextSize(1);
  display.setCursor(0, 55);
  display.print("Short:+1 Long:Back");
}

// =====================
// Draw Info Screen
// =====================
void drawInfo() {
  drawHeader("Info");

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.print("Arduino UNO OLED");
  display.setCursor(0, 42);
  display.print("P6 Button UI");
}

// =====================
// Draw Reset Screen (Countdown UI)
// =====================
void drawReset() {
  drawHeader("Reset");

  // Line 1 (center text)
  display.setTextSize(1);

  String msg = "Reset Done!";
  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(msg, 0, 0, &x1, &y1, &w, &h);
  int x1pos = (128 - w) / 2;

  display.setCursor(x1pos, 30);
  display.print(msg);

  // Line 2 (countdown number)
  display.setTextSize(2);

  String num = String(countdown);

  display.getTextBounds(num, 0, 0, &x1, &y1, &w, &h);
  int x2pos = (128 - w) / 2;

  display.setCursor(x2pos, 45);
  display.print(num);
}

// =====================
// Render Screen
// =====================
void render() {
  display.clearDisplay();

  if (state == STATE_MENU) drawMenu();
  else if (state == STATE_COUNTER) drawCounter();
  else if (state == STATE_INFO) drawInfo();
  else if (state == STATE_RESET) drawReset();

  display.display();
}

// =====================
// Button Logic
// =====================
void handleButton() {
  bool stateBtn = digitalRead(BTN_PIN);

  // button pressed
  if (lastBtn == HIGH && stateBtn == LOW) {
    pressStart = millis();
    longPressHandled = false;
  }

  unsigned long duration = millis() - pressStart;

  // =====================
  // Long Press (>= 1 second)
  // =====================
  if (stateBtn == LOW && !longPressHandled) {

    if (duration > 1000) {

      // MENU navigation
      if (state == STATE_MENU) {
        if (menuIndex == 0) state = STATE_COUNTER;

        else if (menuIndex == 1) {
          counter = 0;
          state = STATE_RESET;

          resetStartTime = millis();
          countdown = 3;
        }

        else if (menuIndex == 2) state = STATE_INFO;
      }

      // COUNTER: go back
      else if (state == STATE_COUNTER) {
        state = STATE_MENU;
      }

      // INFO: go back
      else if (state == STATE_INFO) {
        state = STATE_MENU;
      }

      longPressHandled = true;
    }
  }

  // =====================
  // Short Press
  // =====================
  if (lastBtn == LOW && stateBtn == HIGH) {
    if (!longPressHandled) {

      if (state == STATE_MENU) {
        menuIndex = (menuIndex + 1) % menuSize;
      }

      else if (state == STATE_COUNTER) {
        counter++;
      }
    }
  }

  lastBtn = stateBtn;
}

// =====================
// Reset Countdown Logic
// =====================
void updateReset() {
  if (state != STATE_RESET) return;

  if (millis() - resetStartTime >= 1000) {
    resetStartTime = millis();
    countdown--;
  }

  if (countdown <= 0) {
    state = STATE_MENU;
  }
}

// =====================
// Setup
// =====================
void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.display();
}

// =====================
// Main Loop
// =====================
void loop() {
  handleButton();
  updateReset();
  render();
  delay(30);
}