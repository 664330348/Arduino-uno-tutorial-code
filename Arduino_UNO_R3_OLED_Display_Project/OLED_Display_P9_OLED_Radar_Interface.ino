#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <EEPROM.h>
#include <math.h>

// =====================================================
// OLED CONFIG
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo servo;

// =====================================================
// PIN
// =====================================================
#define TRIG 9
#define ECHO 10
#define SERVO_PIN 3

// =====================================================
// EEPROM
// =====================================================
#define ADDR_ANGLE 0
#define ADDR_DIR   1

// =====================================================
// RADAR CONFIG (UPDATED)
// =====================================================
const int ringCm[3] = {10, 20, 30};
const int ringCount = 3;

const int maxCm = 30;
const int maxRadiusPx = 30;

// radar center
const int cx = 78;
const int cy = 60;

// =====================================================
// STATE
// =====================================================
int angle;
int dir;

// sensor
int rawDistance = -1;
int distance = -1;
int lastValid = -1;

// =====================================================
// TIMERS
// =====================================================
unsigned long tServo = 0;
unsigned long tSensor = 0;
unsigned long tOLED = 0;

// =====================================================
// SCALE
// =====================================================
float cmToPx(int cm) {
  return (cm / (float)maxCm) * maxRadiusPx;
}

// =====================================================
// SENSOR
// =====================================================
int getDistance() {

  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  long duration = pulseIn(ECHO, HIGH, 20000);

  if (duration == 0) return -1;

  int d = duration * 0.034 / 2;

  if (d <= 0 || d > 200) return -1;

  return d;
}

// =====================================================
// EEPROM SAVE
// =====================================================
void saveState() {
  EEPROM.update(ADDR_ANGLE, angle);
  EEPROM.update(ADDR_DIR, dir == 1 ? 1 : 0);
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  servo.attach(SERVO_PIN);

  // load state
  angle = EEPROM.read(ADDR_ANGLE);
  dir = EEPROM.read(ADDR_DIR);

  if (angle < 0 || angle > 180) angle = 0;
  if (dir != 0 && dir != 1) dir = 1;
  if (dir == 0) dir = -1;

  servo.write(angle);
  delay(200);
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  // =================================================
  // SERVO SWEEP (stable + no offset bug)
  // =================================================
  if (micros() - tServo >= 20000) {
    tServo = micros();

    servo.write(angle);

    int oldAngle = angle;

    angle += dir * 2;

    if (angle >= 180) {
      angle = 180;
      dir = -1;
    }
    else if (angle <= 0) {
      angle = 0;
      dir = 1;
    }

    // save only real change
    if (angle != oldAngle) {
      saveState();
    }
  }

  // =================================================
  // SENSOR
  // =================================================
  if (millis() - tSensor >= 80) {
    tSensor = millis();

    rawDistance = getDistance();

    if (rawDistance > 0 && rawDistance <= 200) {
      lastValid = rawDistance;
    }

    distance = lastValid;
  }

  // =================================================
  // OLED
  // =================================================
  if (millis() - tOLED >= 100) {
    tOLED = millis();

    display.clearDisplay();

    // =================================================
    // TITLE AREA (your version)
    // =================================================
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("OLED Part 9");

    display.setCursor(0, 8);
    display.println("Radar Interface");

    display.drawLine(0, 16, 127, 16, SSD1306_WHITE);

    // =================================================
    // RADAR GRID (3 rings)
    // =================================================
    for (int i = 0; i < ringCount; i++) {

      int r = cmToPx(ringCm[i]);

      for (int a = 0; a <= 180; a += 5) {

        float rad1 = a * 3.14159 / 180;
        float rad2 = (a + 5) * 3.14159 / 180;

        int x1 = cx + cos(rad1) * r;
        int y1 = cy - sin(rad1) * r;

        int x2 = cx + cos(rad2) * r;
        int y2 = cy - sin(rad2) * r;

        display.drawLine(x1, y1, x2, y2, SSD1306_WHITE);
      }
    }

    // =================================================
    // SWEEP LINE
    // =================================================
    float rad = angle * 3.14159 / 180;

    int x = cx + cos(rad) * maxRadiusPx;
    int y = cy - sin(rad) * maxRadiusPx;

    display.drawLine(cx, cy, x, y, SSD1306_WHITE);

    // =================================================
    // TARGET POINT
    // =================================================
    if (distance > 0 && distance <= maxCm) {

      float dPx = cmToPx(distance);

      int ox = cx + cos(rad) * dPx;
      int oy = cy - sin(rad) * dPx;

      display.fillCircle(ox, oy, 2, SSD1306_WHITE);
    }

    // =================================================
    // STATUS
    // =================================================
    display.setCursor(0, 54);
    display.print("D:");
    display.print(distance);
    display.print("cm");

    display.display();
  }
}