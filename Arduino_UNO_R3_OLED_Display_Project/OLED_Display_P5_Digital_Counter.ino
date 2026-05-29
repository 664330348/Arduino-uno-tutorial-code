#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================
// Display Settings
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
// Counter Variables
// =====================
int currentValue = 0;   // current displayed value
int oldValue = 0;       // previous value for animation

// =====================
// Animation Control
// =====================
bool animating = false;
int animFrame = 0;
const int ANIM_MAX = 14;

// =====================
// Button State Tracking
// =====================
bool lastBtnState = HIGH;
unsigned long pressStart = 0;
bool longPressHandled = false;

// =====================
// Layout Configuration
// =====================
const int HEADER_H = 17;   // top section height (header area)
const int LINE_Y = 16;     // separator line position

const int CONTENT_TOP = 17; // content area starts here
const int CONTENT_H = 47;   // content area height

const int CENTER_Y = CONTENT_TOP + CONTENT_H / 2; // vertical center of content

// =====================
// Draw Header (Title + Divider Line)
// =====================
void drawHeader() {
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("OLED Part 5");

  display.setCursor(0, 8);
  display.print("Digital Counter");

  // fixed separator line at y = 16
  display.drawLine(0, LINE_Y, 128, LINE_Y, SSD1306_WHITE);
}

// =====================
// Clear Content Area Only
// =====================
void clearContent() {
  display.fillRect(0, CONTENT_TOP, 128, CONTENT_H, SSD1306_BLACK);
}

// =====================
// Draw Centered Number with Offset
// =====================
void drawNumber(int value, int yOffset) {
  char buf[10];
  sprintf(buf, "%d", value);

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(buf, 0, 0, &x1, &y1, &w, &h);

  int x = (128 - w) / 2;
  int y = CENTER_Y - h / 2 + yOffset;

  // prevent drawing outside visible area
  if (y < CONTENT_TOP - 20 || y > 64) return;

  display.setCursor(x, y);
  display.print(buf);
}

// =====================
// Render Animation Frame
// =====================
void renderFrame() {
  display.clearDisplay();

  drawHeader();
  clearContent();

  float t = (float)animFrame / ANIM_MAX;

  // smooth easing (smoothstep)
  float ease = t * t * (3 - 2 * t);

  // old number moves upward
  int oldY = (int)(-18 * ease);

  // new number moves into center
  int newY = (int)(18 * (1 - ease));

  // show old number in first half of animation
  if (animFrame < ANIM_MAX / 2) {
    drawNumber(oldValue, oldY);
  }

  // show new number in second part
  if (animFrame >= ANIM_MAX / 3) {
    drawNumber(currentValue, newY);
  }

  display.display();
}

// =====================
// Button Handling
// =====================
void handleButton() {
  bool state = digitalRead(BTN_PIN);

  // button pressed
  if (lastBtnState == HIGH && state == LOW) {
    pressStart = millis();
    longPressHandled = false;
  }

  // long press detection (1 second)
  if (state == LOW && !longPressHandled) {
    if (millis() - pressStart > 1000) {
      oldValue = currentValue;
      currentValue = 0;   // reset to zero
      startAnimation();
      longPressHandled = true;
    }
  }

  // short press (increment)
  if (lastBtnState == LOW && state == HIGH) {
    if (!longPressHandled) {
      oldValue = currentValue;
      currentValue++;
      startAnimation();
    }
  }

  lastBtnState = state;
}

// =====================
// Start Animation
// =====================
void startAnimation() {
  animFrame = 0;
  animating = true;
}

// =====================
// Update Animation
// =====================
void updateAnimation() {
  renderFrame();

  animFrame++;
  if (animFrame > ANIM_MAX) {
    animating = false;
  }
}

// =====================
// Setup
// =====================
void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true); // stop if OLED init fails
  }

  display.clearDisplay();
  display.display();
}

// =====================
// Main Loop
// =====================
void loop() {
  handleButton();

  if (animating) {
    updateAnimation();
  } else {
    display.clearDisplay();

    drawHeader();
    drawNumber(currentValue, 0);

    display.display();
  }

  delay(25);
}