#include "thingProperties.h"

const int ledPin = 13; // NEW: define the PIN for the LED

void setup() {
  Serial.begin(9600);
  delay(1500); 

  pinMode(ledPin, OUTPUT); // NEW: set the LED pin as an output
  
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
}

void onLedChange()  {
  // NEW: turn the LED ON or OFF based on the value from Arduino IoT Cloud
  digitalWrite(ledPin, led);
}
