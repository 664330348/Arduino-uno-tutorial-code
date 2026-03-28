#include <DHT.h>

// ===== DHT Setup =====
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ===== Pins =====
int fanPin = 9;
int buttonPin = 11;
int potPin = A0;

int redPin = 6;
int greenPin = 5;
int bluePin = 3;

// ===== States =====
bool fanState = false;
bool isManualMode = false;

// Button debounce
int buttonState;
int lastButtonState = HIGH;

// ===== Temperature Stability =====
float stableTemp = 0;
unsigned long lastTempReadTime = 0;
const int tempInterval = 3000;

// ===== Fan Level =====
// 0 = OFF, 1 = LOW, 2 = MEDIUM, 3 = HIGH
int fanLevel = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(fanPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {

  // ===== Button Toggle =====
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    fanState = !fanState;
    delay(200);
  }
  lastButtonState = buttonState;

  // ===== Read Temperature Every 3 Seconds =====
  if (millis() - lastTempReadTime > tempInterval) {
    float temp = dht.readTemperature();
    if (!isnan(temp)) {
      stableTemp = temp;
    }
    lastTempReadTime = millis();

    Serial.print("Stable Temp: ");
    Serial.println(stableTemp);
  }

  if (fanState) {

    int potValue = analogRead(potPin);
    int speedValue = 0;

    // ===== Detect Mode =====
    if (potValue > 50) {
      isManualMode = true;
    } else {
      isManualMode = false;
    }

    // ===== MANUAL MODE =====
    if (isManualMode) {

      speedValue = map(potValue, 0, 1023, 60, 255);

      // Sync fan level with speed (for LED)
      if (speedValue < 120) {
        fanLevel = 1;
      }
      else if (speedValue < 200) {
        fanLevel = 2;
      }
      else {
        fanLevel = 3;
      }
    }

    // ===== AUTO MODE (HYSTERESIS) =====
    else {

      // OFF ↔ LOW
      if (fanLevel == 0 && stableTemp >= 23) {
        fanLevel = 1;
      } 
      else if (fanLevel == 1 && stableTemp < 23) {
        fanLevel = 0;
      }

      // LOW ↔ MEDIUM
      else if (fanLevel == 1 && stableTemp >= 27) {
        fanLevel = 2;
      } 
      else if (fanLevel == 2 && stableTemp < 27) {
        fanLevel = 1;
      }

      // MEDIUM ↔ HIGH
      else if (fanLevel == 2 && stableTemp >= 30) {
        fanLevel = 3;
      } 
      else if (fanLevel == 3 && stableTemp < 30) {
        fanLevel = 2;
      }

      // Convert level to speed
      switch (fanLevel) {
        case 0: speedValue = 0; break;
        case 1: speedValue = 100; break;
        case 2: speedValue = 180; break;
        case 3: speedValue = 255; break;
      }
    }

    // ===== Apply Speed =====
    analogWrite(fanPin, speedValue);

    // ===== LED Display =====
    if (fanLevel == 0) {
      setColor(255, 0, 0);   // OFF (Red)
    }
    else if (fanLevel == 1) {
      setColor(0, 0, 255);   // LOW (Blue)
    }
    else if (fanLevel == 2) {
      setColor(0, 255, 0);   // MEDIUM (Green)
    }
    else {
      setColor(255, 0, 255); // HIGH (Purple)
    }

  } 
  else {
    // ===== Fan OFF =====
    analogWrite(fanPin, 0);
    setColor(0, 0, 0);
    fanLevel = 0;
  }

  delay(100);
}

// ===== RGB Control =====
void setColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}