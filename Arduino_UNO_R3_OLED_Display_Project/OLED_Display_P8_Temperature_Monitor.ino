#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// =====================================================
// OLED CONFIG
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// =====================================================
// DHT11 CONFIG
// =====================================================
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// =====================================================
// TEMPERATURE TRACKING
// =====================================================
float minTemp = 999;
float maxTemp = -999;

// =====================================================
// HEADER
// =====================================================
void drawHeader(const char* title) {

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("OLED P8");

  display.setCursor(0, 8);
  display.print(title);

  display.drawLine(0, 16, 128, 16, SSD1306_WHITE);
}

// =====================================================
// DRAW TEMPERATURE BAR
// =====================================================
void drawTempBar(float temp) {

  const int barX = 10;
  const int barY = 46;
  const int barW = 108;
  const int barH = 8;

  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);

  // Scale 0°C ~ 50°C
  int fillWidth = map(
    constrain((int)temp, 0, 50),
    0,
    50,
    0,
    barW - 2
  );

  display.fillRect(
    barX + 1,
    barY + 1,
    fillWidth,
    barH - 2,
    SSD1306_WHITE
  );
}

// =====================================================
// DRAW STATUS
// =====================================================
const char* getStatus(float temp) {

  if (temp < 20)
    return "COOL";

  if (temp > 30)
    return "HOT";

  return "NORMAL";
}

// =====================================================
// DRAW DASHBOARD
// =====================================================
void drawDashboard(float temp) {

  drawHeader("Temperature Monitor");

  // =========================
  // Large Temperature
  // =========================

  display.setTextSize(3);

  String tempText = String(temp, 1);

  int16_t x1, y1;
  uint16_t w, h;

  display.getTextBounds(
    tempText,
    0,
    0,
    &x1,
    &y1,
    &w,
    &h
  );

  int x = (SCREEN_WIDTH - w) / 2;

  display.setCursor(x, 20);
  display.print(tempText);

  display.setTextSize(1);
  display.print("C");

  // =========================
  // Progress Bar
  // =========================

  drawTempBar(temp);

  // =========================
  // Min / Max
  // =========================

  display.setTextSize(1);

  display.setCursor(0, 56);
  display.print("L:");
  display.print(minTemp, 1);

  display.setCursor(45, 56);
  display.print("H:");
  display.print(maxTemp, 1);

  display.setCursor(88, 56);
  display.print(getStatus(temp));
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  dht.begin();

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C)) {

    for (;;);
  }

  display.clearDisplay();
  display.display();
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  float temp = dht.readTemperature();

  if (isnan(temp)) {
    return;
  }

  // Update min/max
  if (temp < minTemp)
    minTemp = temp;

  if (temp > maxTemp)
    maxTemp = temp;

  display.clearDisplay();

  drawDashboard(temp);

  display.display();

  delay(1000);
}