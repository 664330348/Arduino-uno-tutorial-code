#include <LiquidCrystal.h>
#include <DHT.h>

// ================= LCD SETUP =================
// Initialize LCD pins: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(12, 11, 7, 6, 5, 4);

// ================= DHT11 SENSOR =================
#define DHTPIN 2          // DHT11 data pin connected to digital pin 2
#define DHTTYPE DHT11     // Sensor type: DHT11
DHT dht(DHTPIN, DHTTYPE);

// ================= SOIL SENSOR =================
// Soil moisture sensor connected to analog pin A0
int soilPin = A0;

// ================= CALIBRATION VALUES =================
// These values must be calibrated based on your own sensor:
// dryValue = reading in dry air / dry soil
// wetValue = reading in water / fully wet soil
int dryValue = 500;
int wetValue = 170;

// ================= CUSTOM DEGREE SYMBOL =================
// LCD1602 does not support the "°" symbol directly,
// so we create a custom character
byte degreeChar[8] = {
  0b00110,
  0b01001,
  0b01001,
  0b00110,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup() {
  lcd.begin(16, 2);      // Initialize LCD (16 columns, 2 rows)
  dht.begin();           // Start DHT11 sensor

  Serial.begin(9600);    // Start Serial Monitor for debugging

  // Store custom degree symbol in LCD memory slot 0
  lcd.createChar(0, degreeChar);

  // Startup message
  lcd.print("Smart Plant");
  delay(1500);
  lcd.clear();
}

void loop() {

  // ================= READ TEMPERATURE & HUMIDITY =================
  float tempC = dht.readTemperature();   // Temperature in Celsius
  float hum = dht.readHumidity();        // Humidity in %

  // Convert Celsius to Fahrenheit
  float tempF = tempC * 9.0 / 5.0 + 32.0;

  // Convert humidity to integer for cleaner display
  int humInt = (int)hum;

  // ================= READ SOIL MOISTURE =================
  int soilValue = analogRead(soilPin);   // Raw analog value (0–1023)

  // Convert raw value to percentage using calibration
  int soilPercent = map(soilValue, dryValue, wetValue, 0, 100);

  // Limit value between 0% and 100%
  soilPercent = constrain(soilPercent, 0, 100);

  // ================= SOIL STATUS LOGIC =================
  String status;

  // Determine soil condition based on moisture level
  if (soilPercent < 25) status = "DRY";        // Too dry
  else if (soilPercent < 55) status = "GOOD";  // Optimal range
  else status = "WET";                         // Too wet

  // ================= LCD LINE 1 =================
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(tempF, 1);      // Temperature with 1 decimal
  lcd.write(byte(0));       // Custom degree symbol
   lcd.print("F");
  // Fixed position for Humidity text
  lcd.setCursor(10, 0);
  lcd.print("H:");
  lcd.print(humInt);        // Humidity %
  lcd.print("%   ");        // Clear leftover characters

  // ================= LCD LINE 2 (FIXED POSITION DISPLAY) =================

  // Print label
  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.print(soilPercent);
  lcd.print("%   ");   // Clear leftover digits

  // Fixed position for status text
  lcd.setCursor(10, 1);
  lcd.print(status);   // print soilValue to find out the dryValue and wetValue
  lcd.print("   ");    // Clear leftover characters

  delay(1000); // Update every 1 second
}