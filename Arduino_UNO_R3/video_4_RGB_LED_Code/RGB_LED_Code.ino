// Define RGB pins (PWM)
#define RED   6
#define GREEN 5
#define BLUE  3

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // 🔴 Start with Red and stay for 2 seconds
  analogWrite(RED, 255);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  delay(2000);
}

void loop() {

  // 🔴 Red → 🟢 Green
  for (int i = 0; i <= 255; i++) {
    analogWrite(RED, 255 - i);
    analogWrite(GREEN, i);
    analogWrite(BLUE, 0);
    
    // 🟢 Stay Green for 2 seconds
    if (i == 255) {
      delay(2000);
    } else {
      delay(10);
    }
  }

  // 🟢 Green → 🔵 Blue
  for (int i = 0; i <= 255; i++) {
    analogWrite(RED, 0);
    analogWrite(GREEN, 255 - i);
    analogWrite(BLUE, i);
    
    // 🔵 Stay Blue for 2 seconds
    if (i == 255) {
      delay(2000);
    } else {
      delay(10);
    }
  }

  // 🔵 Blue → 🔴 Red
  for (int i = 0; i <= 255; i++) {
    analogWrite(RED, i);
    analogWrite(GREEN, 0);
    analogWrite(BLUE, 255 - i);
    
    // 🔴 Stay Red for 2 seconds
    if (i == 255) {
      delay(2000);
    } else {
      delay(10);
    }
  }
}
