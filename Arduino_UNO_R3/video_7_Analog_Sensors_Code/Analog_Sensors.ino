#include <LiquidCrystal.h>
#include <math.h>

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 2, 3, 4, 5);

const int thermistorPin = A0;
const int lightPin = A3;

const float SERIES_RESISTOR = 10000.0;    // 10kΩ
const float NOMINAL_RESISTANCE = 10000.0; // 10kΩ at 25°C
const float NOMINAL_TEMPERATURE = 25.0;   // 25°C
const float B_COEFFICIENT = 3950.0;       // Typical value

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {

  // ========= Read Temperature =========
  int adcValue = analogRead(thermistorPin);

  float voltage = adcValue / 1023.0;
  float resistance = SERIES_RESISTOR * (1.0 / voltage - 1.0);

  float steinhart;
  steinhart = resistance / NOMINAL_RESISTANCE;
  steinhart = log(steinhart);
  steinhart /= B_COEFFICIENT;
  steinhart += 1.0 / (NOMINAL_TEMPERATURE + 273.15);
  steinhart = 1.0 / steinhart;
  float tempC = steinhart - 273.15;
  float tempF = tempC * 9.0 / 5.0 + 32.0;

  // ========= Read Light =========
  int lightValue = analogRead(lightPin);
  String lightLevel;

  if(lightValue <= 300){
    lightLevel = "Dark";
  } else if(lightValue <= 700){
    lightLevel = "Normal";
  } else {
    lightLevel = "Bright";
  }

  // ========= LCD Display =========
  lcd.setCursor(0, 0);
  lcd.print("C:");
  lcd.print(tempC, 1);
  lcd.print(" F:");
  lcd.print(tempF, 1);
  lcd.print("   "); // clear leftover chars

  lcd.setCursor(0, 1);
  lcd.print("Light:");
  
  // Display light value
  lcd.print(lightValue);

  // Calculate spaces so the level is right-aligned
  int spaces = 16 - (6 + String(lightValue).length() + lightLevel.length());
  for(int i = 0; i < spaces; i++){
    lcd.print(" ");
  }

  // Display light level
  lcd.print(lightLevel);

  delay(1000);
}