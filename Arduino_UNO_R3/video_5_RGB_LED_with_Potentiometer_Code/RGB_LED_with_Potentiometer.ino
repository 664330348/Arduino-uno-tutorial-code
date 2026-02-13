#define RED   6
#define GREEN 5
#define BLUE  3

#define POT   A0

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  Serial.begin(9600);
}

void loop() {

  int potValue = analogRead(POT);   // 0–1023

  int r = 0;
  int g = 0;
  int b = 0;

  if (potValue <= 512) {
    // Red -> Green
    int mappedValue = map(potValue, 0, 512, 0, 255);
    r = 255 - mappedValue;
    g = mappedValue;
    b = 0;
  }
  else {
    // Green -> Blue
    int mappedValue = map(potValue, 512, 1023, 0, 255);
    r = 0;
    g = 255 - mappedValue;
    b = mappedValue;
  }

  analogWrite(RED, r);
  analogWrite(GREEN, g);
  analogWrite(BLUE, b);

  delay(200);
}
