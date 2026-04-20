#include "thingProperties.h"
#include <Servo.h>

Servo myServo;

const int servoPin = 9;

void setup() {
  Serial.begin(9600);
  delay(1500);

  myServo.attach(servoPin);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
}

void onServoAngleChange() {
  myServo.write(180-servoAngle); // Reverse slider direction
}