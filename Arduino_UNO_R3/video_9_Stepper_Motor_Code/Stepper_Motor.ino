#include <Stepper.h>  // Include the Stepper motor library

const int stepsPerRevolution = 2048; // Total steps per revolution for the stepper motor

// Create a Stepper object
// The numbers represent the Arduino pins connected to IN1, IN3, IN2, IN4 on the ULN2003 driver
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {

  // Set the rotation speed of the motor (RPM)
  myStepper.setSpeed(10);

  // Start serial communication for debugging
  Serial.begin(9600);
}

void loop() {

  // Rotate clockwise
  Serial.println("Clockwise");
  myStepper.step(stepsPerRevolution);  // Rotate one full revolution
  delay(1000);                         // Wait for 1 second

  // Rotate counterclockwise
  Serial.println("Counterclockwise");
  myStepper.step(-stepsPerRevolution); // Rotate one full revolution in opposite direction
  delay(1000);                         // Wait for 1 second
}