int fanPin = 9;
int buttonPin = 11;
int potPin = A0;

int redPin = 6;
int greenPin = 5;
int bluePin = 3;

bool fanState = false;

int buttonState;
int lastButtonState = HIGH;

void setup() {

  Serial.begin(9600);

  pinMode(fanPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {

  buttonState = digitalRead(buttonPin);

  // button toggle
  if (buttonState == LOW && lastButtonState == HIGH) {
    fanState = !fanState;
    delay(200);
  }

  lastButtonState = buttonState;

  if (fanState) {

    int potValue = analogRead(potPin);

    int speedValue = map(potValue, 0, 1023, 0, 255);

    // motor start threshold
    if (speedValue < 60) {
      speedValue = 0;
    }

    analogWrite(fanPin, speedValue);
    
    // LED status
    if (speedValue == 0) {
      setColor(255,0,0);   // Red
    }
    else if (speedValue < 120) {
      setColor(0,0,255);   // Blue
    }
    else if (speedValue < 200) {
      setColor(0,255,0);   // Green
    }
    else {
      setColor(255,0,255); // Purple
    }

  } 
  else {

    analogWrite(fanPin,0);
    setColor(0,0,0); // LED OFF

  }

  delay(100);
}

void setColor(int r, int g, int b) {

  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);

}