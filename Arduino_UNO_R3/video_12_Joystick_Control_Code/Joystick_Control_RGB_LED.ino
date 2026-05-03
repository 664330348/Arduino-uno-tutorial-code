// ===== Joystick Pins =====
int xPin = A5;   // Joystick X-axis analog input
int yPin = A4;   // Joystick Y-axis analog input
int swPin = 2;   // Joystick push button (digital input)

// ===== RGB LED Pins =====
int redPin = 10;
int greenPin = 9;
int bluePin = 8;

// ===== System State =====
bool isOn = false;   // Toggle LED system ON/OFF

// ===== Joystick Center Calibration =====
int center = 512;    // Neutral position of joystick

// ===== Smoothing Variables =====
float lastX = 0;
float lastY = 0;
float lastHue = 0;

void setup() {
  pinMode(swPin, INPUT_PULLUP);   // Button uses internal pull-up resistor

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  Serial.begin(9600);  // Debug output
}

void loop() {

  // ===== Button Toggle Control =====
  // Press joystick button to turn system ON/OFF
  if (digitalRead(swPin) == LOW) {
    isOn = !isOn;
    delay(300); // debounce delay
  }

  // ===== If system is OFF, turn off LED and exit =====
  if (!isOn) {
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    return;
  }

  // ===== Read raw joystick values =====
  float x = analogRead(xPin) - center;
  float y = analogRead(yPin) - center;

  // ===== Exponential Moving Average (smoothing X/Y movement) =====
  x = lastX * 0.85 + x * 0.15;
  y = lastY * 0.85 + y * 0.15;

  lastX = x;
  lastY = y;

  // ===== Convert Cartesian (x,y) to Polar Angle =====
  float angle = atan2(y, x);
  if (angle < 0) angle += 2 * PI; // normalize to 0–2π

  float hue = angle / (2 * PI); // convert angle to Hue (0–1 range)

  // ===== Smooth Hue Transition =====
  hue = lastHue * 0.9 + hue * 0.1;
  lastHue = hue;

  // ===== Calculate Saturation based on joystick distance =====
  float dist = sqrt(x * x + y * y);
  float sat = dist / 512.0;

  // ===== Dead Zone (ignore small movements near center) =====
  if (dist < 40) sat = 0;

  if (sat > 1) sat = 1;

  float val = 1.0; // brightness fixed at max

  // ===== Convert HSV to RGB =====
  int r, g, b;
  hsvToRgb(hue, sat, val, r, g, b);

  // ===== Output PWM signal to RGB LED =====
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);

  delay(10); // small delay for stability
}

// ===== HSV to RGB Conversion Function =====
void hsvToRgb(float h, float s, float v, int &r, int &g, int &b) {

  float rF, gF, bF;

  int i = int(h * 6);           // determine color sector
  float f = h * 6 - i;          // fractional part
  float p = v * (1 - s);
  float q = v * (1 - f * s);
  float t = v * (1 - (1 - f) * s);

  // ===== Select RGB based on sector =====
  switch (i % 6) {
    case 0: rF = v; gF = t; bF = p; break;
    case 1: rF = q; gF = v; bF = p; break;
    case 2: rF = p; gF = v; bF = t; break;
    case 3: rF = p; gF = q; bF = v; break;
    case 4: rF = t; gF = p; bF = v; break;
    case 5: rF = v; gF = p; bF = q; break;
  }

  // ===== Convert float RGB (0–1) to PWM (0–255) =====
  r = rF * 255;
  g = gF * 255;
  b = bF * 255;
}