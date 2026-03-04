#include <Servo.h>

Servo myServo;       // Create a servo object
int potPin = A5;     // Potentiometer connected to analog pin A0
int potValue = 0;    // Variable to store potentiometer value
int angle = 0;       // Variable to store mapped servo angle

void setup() {
  myServo.attach(9);     // Attach servo signal wire to digital pin 9
  Serial.begin(9600);    // Start serial monitor (for debugging)
}

void loop() {

  potValue = analogRead(potPin);  // Read analog value from potentiometer (0 - 1023)
  angle = map(potValue, 0, 1023, 0, 180);  // Convert potentiometer value to servo angle (0 - 180 degrees)
  myServo.write(angle);  // Move servo to mapped angle

  delay(15); // Small delay for stable movement
}