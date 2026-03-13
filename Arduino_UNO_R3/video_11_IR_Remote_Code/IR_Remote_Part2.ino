#include <IRremote.hpp>

#define IR_RECEIVE_PIN 11

#define BLUE 3
#define GREEN 5
#define RED 6

int colorIndex = 0;

// Function to set RGB color
void setColor(int r, int g, int b) {
  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);
}

// Function to display the current color
void showColor() {

  switch (colorIndex) {

    case 0:
      setColor(255,0,0);
      Serial.println("Current Color: RED");
      break;

    case 1:
      setColor(0,255,0);
      Serial.println("Current Color: GREEN");
      break;

    case 2:
      setColor(0,0,255);
      Serial.println("Current Color: BLUE");
      break;

    case 3:
      setColor(255,255,0);
      Serial.println("Current Color: YELLOW");
      break;

    case 4:
      setColor(0,255,255);
      Serial.println("Current Color: CYAN");
      break;

    case 5:
      setColor(255,0,255);
      Serial.println("Current Color: PURPLE");
      break;

    case 6:
      setColor(255,255,255);
      Serial.println("Current Color: WHITE");
      break;
  }
}

void setup() {

  Serial.begin(9600);

  // Set RGB pins as outputs
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // Start the IR receiver
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  Serial.println("Use LEFT and RIGHT buttons to change color");

  // Show the initial color
  showColor();
}

void loop() {

  // Check if an IR signal is received
  if (IrReceiver.decode()) {

    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    
    Serial.print("HEX code: ");
    Serial.println(code, HEX);

    // RIGHT button → next color
    if(code == 0xA55AFF00) {  // Replace with the actual HEX codes you got from the right buttons

      colorIndex++;
      if(colorIndex > 6) colorIndex = 0;

      showColor();
    }

    // LEFT button → previous color
    if(code == 0xF708FF00) {  // Replace with the actual HEX codes you got from the left buttons

      colorIndex--;
      if(colorIndex < 0) colorIndex = 6;

      showColor();
    }

    // Enable receiving the next signal
    IrReceiver.resume();
  }
}