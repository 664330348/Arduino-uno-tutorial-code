#include <DHT.h>
#include <IRremote.hpp>

// ===== DHT Setup =====
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== IR Setup =====
#define IR_RECEIVE_PIN A5

// ===== Pins =====
int fanPin = 9;
int buttonPin = 11;
int potPin = A0;

int redPin = 6;
int greenPin = 5;
int bluePin = 3;

// ===== AUTO Color =====
#define AUTO_COLOR_R 255
#define AUTO_COLOR_G 255
#define AUTO_COLOR_B 255

// ===== States =====
bool fanState = false;
bool isPaused = false;
int fanLevel = 1;

enum Mode { AUTO, MANUAL };
Mode currentMode = MANUAL;

int lastIRLevel = 0;

// Button
int buttonState;
int lastButtonState = HIGH;

// Temperature
float stableTemp = 0;
unsigned long lastTempReadTime = 0;
const int tempInterval = 3000;

// ===== IR Commands =====
#define IR_POWER_CMD   0x45
#define IR_SPEED1_CMD  0x0C
#define IR_SPEED2_CMD  0x18
#define IR_SPEED3_CMD  0x5E
#define IR_LEFT_CMD    0x44
#define IR_RIGHT_CMD   0x43
#define IR_PAUSE_CMD   0x40
#define IR_EQ_CMD      0x19

void setup() {
  Serial.begin(9600);
  dht.begin();
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  pinMode(fanPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.println("Smart Fan Ready");
}

void loop() {

  // ================= IR =================
  if (IrReceiver.decode()) {
    uint8_t cmd = IrReceiver.decodedIRData.command;

    Serial.print("CMD: ");
    Serial.println(cmd, HEX);

    if (cmd == IR_POWER_CMD || cmd == IR_SPEED1_CMD || cmd == IR_SPEED2_CMD ||
        cmd == IR_SPEED3_CMD || cmd == IR_LEFT_CMD || cmd == IR_RIGHT_CMD ||
        cmd == IR_PAUSE_CMD || cmd == IR_EQ_CMD) {

      // ===== POWER =====
      if (cmd == IR_POWER_CMD) {
        fanState = !fanState;
        isPaused = false;

        if (fanState) {
          currentMode = MANUAL;
          fanLevel = 1;
          lastIRLevel = 0;
        }

        Serial.println(fanState ? "ON" : "OFF");
      }

      else if (fanState) {

        switch (cmd) {

          // ===== MODE ONLY (EQ) =====
          case IR_EQ_CMD:
            currentMode = (currentMode == AUTO) ? MANUAL : AUTO;
            Serial.println(currentMode == AUTO ? "AUTO MODE" : "MANUAL MODE");
            break;

          // ===== MANUAL ONLY =====
          case IR_SPEED1_CMD:
            if (currentMode == MANUAL) {
              fanLevel = 1;
              lastIRLevel = 1;
            }
            break;

          case IR_SPEED2_CMD:
            if (currentMode == MANUAL) {
              fanLevel = 2;
              lastIRLevel = 2;
            }
            break;

          case IR_SPEED3_CMD:
            if (currentMode == MANUAL) {
              fanLevel = 3;
              lastIRLevel = 3;
            }
            break;

          case IR_LEFT_CMD:
            if (currentMode == MANUAL) {
              fanLevel = max(1, fanLevel - 1);
              lastIRLevel = fanLevel;
            }
            break;

          case IR_RIGHT_CMD:
            if (currentMode == MANUAL) {
              fanLevel = min(3, fanLevel + 1);
              lastIRLevel = fanLevel;
            }
            break;

          case IR_PAUSE_CMD:
            isPaused = !isPaused;
            Serial.println(isPaused ? "PAUSED" : "RESUME");
            break;
        }
      }
    }

    IrReceiver.resume();
  }

  // ================= BUTTON =================
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    fanState = !fanState;
    isPaused = false;

    if (fanState) {
      currentMode = MANUAL;
    }

    Serial.println(fanState ? "ON BTN" : "OFF BTN");
    delay(200);
  }
  lastButtonState = buttonState;

  // ================= TEMP =================
  if (millis() - lastTempReadTime > tempInterval) {
    float temp = dht.readTemperature();
    if (!isnan(temp)) stableTemp = temp;

    Serial.print("Temp: ");
    Serial.println(stableTemp);

    lastTempReadTime = millis();
  }

  // ================= FAN LOGIC =================
  int speedValue = 0;

  if (fanState && !isPaused) {

    int potValue = analogRead(potPin);
    int potLevel = 1;

    if (potValue > 341 && potValue <= 682) potLevel = 2;
    else if (potValue > 682) potLevel = 3;

    if (currentMode == MANUAL) {
      if (lastIRLevel != 0) fanLevel = lastIRLevel;
      else fanLevel = potLevel;
    }
    else {
      if (stableTemp >= 30) fanLevel = 3;
      else if (stableTemp >= 27) fanLevel = 2;
      else fanLevel = 1;
    }

    switch (fanLevel) {
      case 1: speedValue = 60; break;
      case 2: speedValue = 120; break;
      case 3: speedValue = 180; break;
    }

  } else {
    speedValue = 0;
  }

  analogWrite(fanPin, speedValue);

  // ================= RGB =================
  if (isPaused) {
    setColor(255, 80, 0);   // ⭐ PAUSE = ORANGE
  }
  else if (!fanState) {
    setColor(0, 0, 0);
  }
  else {
    if (currentMode == AUTO) {
      setColor(255, 255, 255); // AUTO = WHITE
    }
    else {
      switch (fanLevel) {
        case 1: setColor(0, 0, 255); break;
        case 2: setColor(0, 255, 0); break;
        case 3: setColor(255, 0, 255); break;
      }
    }
  }

  // ================= STATUS =================
  Serial.print("Mode: ");
  Serial.print(currentMode == AUTO ? "AUTO" : "MANUAL");

  Serial.print(" | Level: ");
  Serial.print(fanLevel);

  Serial.print(" | Fan: ");
  Serial.println(fanState ? "ON" : "OFF");

  delay(200);
}

// ================= RGB =================
void setColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}