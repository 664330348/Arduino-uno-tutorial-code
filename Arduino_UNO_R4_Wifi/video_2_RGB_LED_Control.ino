#include "thingProperties.h"

// Define RGB LED pins (PWM pins)
const int redPin = 11;
const int greenPin = 10;
const int bluePin = 9;

void setup() {
  Serial.begin(9600);
  delay(1500);

  // Set RGB pins as OUTPUT
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
}

// This function is automatically called when the RGB_LED variable changes in IoT Cloud
void onRGBLEDChange() {
  uint8_t r, g, b;

  // Get RGB values (0-255 each) from IoT Cloud Color variable
  RGB_LED.getValue().getRGB(r, g, b);

  // Write PWM values to each LED channel
  // This controls the brightness of Red, Green, and Blue
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}