#include "thingProperties.h"
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  dht.begin();

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
}

void loop() {
  ArduinoCloud.update();

  float c = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(c) || isnan(h)) return;

  // 🌡 Celsius
  tempC = c;

  // 🌡 Fahrenheit
  tempF = (c * 9.0 / 5.0) + 32.0;

  // 💧 Humidity
  humidity = h;

  delay(1000);
}