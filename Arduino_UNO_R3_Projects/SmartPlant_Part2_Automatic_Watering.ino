#include <LiquidCrystal.h>
#include <DHT.h>
#include <Servo.h>

// ================= LCD SETUP =================
// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// ================= DHT11 SENSOR =================
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ================= PIN DEFINITIONS =================
int soilPin = A0;
int relayPin = 8;
int buttonPin = 3;

// ================= SERVO =================
Servo myServo;
int servoPin = 10;

int servoAngle = 0;   // Current angle
int step = 5;         // Sweep step (smaller = smoother)

// ================= SERVO STATE MACHINE =================
/*
  IDLE       -> Pump OFF, servo holds position
  RESETTING  -> Slowly return to 0° before sweep
  SWEEPING   -> Continuous 0° ↔ 90° motion
*/
enum ServoState {
  IDLE,
  RESETTING,
  SWEEPING
};

ServoState servoState = IDLE;

// ================= MODE CONTROL =================
bool manualMode = false;
bool pumpState = false;

// ================= CALIBRATION =================
int dryValue = 500;
int wetValue = 170;

// ================= BUTTON STATE =================
bool lastButtonState = HIGH;
unsigned long pressStartTime = 0;
bool isPressing = false;

// ================= CUSTOM DEGREE SYMBOL =================
byte degreeChar[8] = {
  0b00110, 0b01001, 0b01001, 0b00110,
  0b00000, 0b00000, 0b00000, 0b00000
};

void setup() {
  lcd.begin(16, 2);
  dht.begin();
  Serial.begin(9600);

  lcd.createChar(0, degreeChar);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay OFF

  pinMode(buttonPin, INPUT_PULLUP);

  myServo.attach(servoPin);
  myServo.write(servoAngle);

  lcd.print("Smart Plant");
  delay(1500);
  lcd.clear();
}

void loop() {

  // ================= SENSOR =================
  float tempC = dht.readTemperature();
  float hum = dht.readHumidity();
  float tempF = tempC * 9.0 / 5.0 + 32.0;

  int soilValue = analogRead(soilPin);
  int soilPercent = map(soilValue, dryValue, wetValue, 0, 100);
  soilPercent = constrain(soilPercent, 0, 100);

  // ================= BUTTON =================
  bool buttonState = digitalRead(buttonPin);

  if (lastButtonState == HIGH && buttonState == LOW) {
    pressStartTime = millis();
    isPressing = true;
  }

  if (lastButtonState == LOW && buttonState == HIGH && isPressing) {

    unsigned long pressDuration = millis() - pressStartTime;

    // Long press: switch AUTO / MANUAL
    if (pressDuration > 1200) {
      manualMode = !manualMode;
      pumpState = false;
    }
    // Short press: toggle pump (manual mode only)
    else {
      if (manualMode) {
        pumpState = !pumpState;
      }
    }

    isPressing = false;
  }

  lastButtonState = buttonState;

  // ================= PUMP CONTROL =================
  bool pumpOn = false;

  if (manualMode) {
    pumpOn = pumpState;
  } else {
    pumpOn = (soilPercent < 15);
  }

  digitalWrite(relayPin, pumpOn ? LOW : HIGH);

  // ================= SERVO STATE MACHINE =================
  if (pumpOn) {

    // When pump turns ON → start resetting phase
    if (servoState == IDLE) {
      servoState = RESETTING;
    }

    // ---------- RESETTING ----------
    if (servoState == RESETTING) {

      // Smoothly return to 0°
      if (servoAngle > 3) {
        servoAngle -= 3;
        myServo.write(servoAngle);
        delay(50);
      } else {
        // Close enough → snap to 0 and start sweeping
        servoAngle = 0;
        myServo.write(servoAngle);

        step = abs(step);       // Ensure forward direction
        servoState = SWEEPING;  // Move to sweeping phase
      }
    }

    // ---------- SWEEPING ----------
    else if (servoState == SWEEPING) {

      servoAngle += step;

      // Reverse direction at limits
      if (servoAngle >= 90 || servoAngle <= 0) {
        step = -step;
      }

      myServo.write(servoAngle);
      delay(80);
    }

  } else {
    // Pump OFF → hold current position (no forced reset)
    servoState = IDLE;
  }

  // ================= LCD DISPLAY =================
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempF, 1);
  lcd.write(byte(0));
  lcd.print("F ");

  lcd.setCursor(9, 0);
  lcd.print("H:");
  lcd.print((int)hum);
  lcd.print("% ");

  lcd.setCursor(0, 1);
  lcd.print("S:");
  lcd.print(soilPercent);
  lcd.print("% ");

  lcd.setCursor(9, 1);
  lcd.print(manualMode ? "MANUAL" : "AUTO  ");

  delay(50);
}