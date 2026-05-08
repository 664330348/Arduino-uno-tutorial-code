#include <TM1637Display.h>

#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

// Buttons
const int btnAdd = 13;
const int btnReset = 10;

// Data
int count = 0;
int brightness = 1;

// Timing
unsigned long addPressStart = 0;
unsigned long resetPressStart = 0;

bool addWasPressed = false;
bool resetWasPressed = false;

const int longPressTime = 500;
const int debounceTime = 30;

// limits
const int BRIGHTNESS_MIN = 0;
const int BRIGHTNESS_MAX = 7;

void setup() {
  pinMode(btnAdd, INPUT_PULLUP);
  pinMode(btnReset, INPUT_PULLUP);

  display.setBrightness(brightness);
}

void loop() {

  // =========================
  // D13 button (count / brightness +)
  // =========================
  bool addState = digitalRead(btnAdd);

  if (addState == LOW && !addWasPressed) {
    // first press detected
    addWasPressed = true;
    addPressStart = millis();
  }

  if (addState == HIGH && addWasPressed) {
    // released → decide action
    unsigned long duration = millis() - addPressStart;

    if (duration < longPressTime) {
      // short press → count +1
      count++;
      if (count > 9999) count = 0;
    } else {
      // long press → brightness +
      brightness++;
      if (brightness > BRIGHTNESS_MAX) brightness = BRIGHTNESS_MAX;
      display.setBrightness(brightness);
    }

    delay(debounceTime);
    addWasPressed = false;
  }

  // =========================
  // D10 button (reset / brightness -)
  // =========================
  bool resetState = digitalRead(btnReset);

  if (resetState == LOW && !resetWasPressed) {
    resetWasPressed = true;
    resetPressStart = millis();
  }

  if (resetState == HIGH && resetWasPressed) {
    unsigned long duration = millis() - resetPressStart;

    if (duration < longPressTime) {
      // short press → reset
      count = 0;
    } else {
      // long press → brightness -
      brightness--;
      if (brightness < BRIGHTNESS_MIN) brightness = BRIGHTNESS_MIN;
      display.setBrightness(brightness);
    }

    delay(debounceTime);
    resetWasPressed = false;
  }

  // update display
  display.showNumberDec(count, true);
}