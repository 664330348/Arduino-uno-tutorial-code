#include <Stepper.h>
#include "thingProperties.h"

// Number of steps per full revolution for your stepper motor (28BYJ-48 is typically 2048)
const int stepsPerRevolution = 2048;

Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

int lastAngle = 0;

void setup() {
  Serial.begin(9600);
  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  myStepper.setSpeed(10);
}

void loop() {
  ArduinoCloud.update();
}

void onAngleChange()  {
  // Convert IoT Cloud slider value (0–36) into actual servo/stepper angle (0–360 degrees)
  int targetAngle = angle * 10;

  // Convert angle into corresponding step position
  int targetSteps = map(targetAngle, 0, 360, 0, stepsPerRevolution);

  // Convert previous angle into steps for comparison
  int lastSteps   = map(lastAngle, 0, 360, 0, stepsPerRevolution);

  // Calculate how many steps the motor needs to move
  int diff = targetSteps - lastSteps;

  // Move motor only if there is a change
  if (diff != 0) {
    myStepper.step(diff);
    lastAngle = targetAngle;
  }
}