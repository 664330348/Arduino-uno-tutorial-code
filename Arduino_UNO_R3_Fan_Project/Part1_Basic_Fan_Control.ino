int fanPin = 9;

void setup() {
  pinMode(fanPin, OUTPUT);
}

void loop() {

  // Start the fan at high speed
  analogWrite(fanPin, 200);
  delay(3000);

  // Medium speed
  analogWrite(fanPin, 120);
  delay(3000);

  // Low speed
  analogWrite(fanPin, 60);
  delay(3000);

  // Stop the fan
  analogWrite(fanPin, 0);
  delay(3000);

}