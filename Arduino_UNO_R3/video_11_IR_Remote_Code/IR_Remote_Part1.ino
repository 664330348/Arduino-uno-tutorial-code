#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11

void setup() {
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  Serial.println("IR receiver ready. Press a button.");
}

void loop() {
  if (IrReceiver.decode()) {

    unsigned long code = IrReceiver.decodedIRData.decodedRawData;

    // Ignore repeat signal
    if(code != 0) {
      Serial.print("HEX: ");
      Serial.println(code, HEX);
    }

    IrReceiver.resume();
  }
}