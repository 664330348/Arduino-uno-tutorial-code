int ledPin = 13;
int buttonPin = 9;

bool ledState = false;         // Store the current state of the LED
bool lastButtonState = HIGH;  // Store the previous button state (using INPUT_PULLUP)

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Enable internal pull-up resistor
}

void loop() {
  bool buttonState = digitalRead(buttonPin);

  // Detect the moment when the button is pressed (HIGH -> LOW)
  if (lastButtonState == HIGH && buttonState == LOW) {
    ledState = !ledState;              // Toggle LED state
    digitalWrite(ledPin, ledState);    // Update LED based on the state
    delay(200);                        // Simple debounce delay
  }

  lastButtonState = buttonState;
}
