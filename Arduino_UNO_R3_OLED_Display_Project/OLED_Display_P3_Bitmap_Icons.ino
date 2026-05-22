#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// OLED CONFIG
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// =====================================================
// LAYOUT
// =====================================================
#define TOP_OFFSET 16

// table columns
#define COL1_X 0
#define COL2_X 43
#define COL3_X 86

// =====================================================
// 8x8 ICONS
// =====================================================
const unsigned char icon_heart_8x8[] PROGMEM = {
  0b01100110,
  0b11111111,
  0b11111111,
  0b11111111,
  0b01111110,
  0b00111100,
  0b00011000,
  0b00000000
};


const unsigned char icon_moon_8x8[] PROGMEM = {
  0b00111100,
  0b01111110,
  0b11110000,
  0b11100000,
  0b11100000,
  0b11110000,
  0b01111110,
  0b00111100
};

const unsigned char icon_check_8x8[] PROGMEM = {
  0b00000001,
  0b00000011,
  0b00000110,
  0b10001100,
  0b11011000,
  0b01110000,
  0b00100000,
  0b00000000
};

const unsigned char icon_folder_8x8[] PROGMEM = {
  0b00000000,
  0b01111100,
  0b10000010,
  0b10011010,
  0b10000010,
  0b11111110,
  0b00000000,
  0b00000000
};
// =====================================================
// 16x16 ICONS
// =====================================================
const unsigned char icon_heart_14x14[] PROGMEM = {
  0x3C, 0xF0,
  0x7F, 0xF8,
  0xFF, 0xFC,
  0xFF, 0xFC,
  0xFF, 0xFC,
  0xFF, 0xFC,
  0x7F, 0xF8,
  0x3F, 0xF0,
  0x1F, 0xE0,
  0x0F, 0xC0,
  0x07, 0x80,
  0x03, 0x00,
  0x02, 0x00,
  0x00, 0x00
};

const unsigned char icon_wifi_14x14[] PROGMEM = {
  0x00, 0x00,
  0x07, 0x80,
  0x3F, 0xF0,
  0xF0, 0x3C,
  0xCF, 0xCC,
  0x3F, 0xF0,
  0x70, 0x38,
  0x07, 0x80,
  0x0F, 0xC0,
  0x03, 0x00,
  0x07, 0x80,
  0x07, 0x80,
  0x00, 0x00,
  0x00, 0x00,
};

// =====================================================
// 20x20 ICONS
// =====================================================
const unsigned char icon_heart_20x20[] PROGMEM = {
  0x06,0x0C,0x00,
  0x0F,0x1E,0x00,
  0x1F,0xBF,0x00,
  0x3F,0xFF,0x80,
  0x7F,0xFF,0xC0,
  0xFF,0xFF,0xE0,
  0xFF,0xFF,0xE0,
  0xFF,0xFF,0xE0,
  0x7F,0xFF,0xC0,
  0x3F,0xFF,0x80,
  0x1F,0xFF,0x00,
  0x0F,0xFE,0x00,
  0x07,0xFC,0x00,
  0x03,0xF8,0x00,
  0x01,0xF0,0x00,
  0x00,0xE0,0x00,
  0x00,0x40,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00,
  0x00,0x00,0x00
};

// =====================================================
// DRAW BOXED ICON
// =====================================================
void drawBoxedBitmap(
  int x,
  int y,
  const unsigned char* bitmap,
  int w,
  int h
) {

  display.drawRect(
    x - 2,
    y - 2,
    w + 4,
    h + 4,
    SSD1306_WHITE
  );

  display.drawBitmap(
    x,
    y,
    bitmap,
    w,
    h,
    SSD1306_WHITE
  );
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();

  // =================================================
  // TITLE AREA
  // =================================================
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("OLED Part 3");

  display.setCursor(0, 8);
  display.println("Bitmap Icons");

  // separator line
  display.drawLine(
    0,
    TOP_OFFSET,
    127,
    TOP_OFFSET,
    SSD1306_WHITE
  );

  // =================================================
  // TABLE LINES
  // =================================================
  // vertical lines
  display.drawLine(42, 16, 42, 63, SSD1306_WHITE);
  display.drawLine(85, 16, 85, 63, SSD1306_WHITE);
  // horizontal header line
  display.drawLine(0, 26, 127, 26, SSD1306_WHITE);

  // =================================================
  // COLUMN TITLES
  // =================================================
  display.setCursor(8, 18);
  display.print("8x8");

  display.setCursor(48, 18);
  display.print("14x14");

  display.setCursor(92, 18);
  display.print("20x20");

  // =================================================
  // 8x8 ICONS
  // =================================================
  drawBoxedBitmap(6, 32, icon_heart_8x8, 8, 8);
  drawBoxedBitmap(22, 32, icon_folder_8x8, 8, 8);
  drawBoxedBitmap(6, 45, icon_moon_8x8, 8, 8);
  drawBoxedBitmap(22, 45, icon_check_8x8, 8, 8);

  // =================================================
  // 16x16 ICONS
  // =================================================
  drawBoxedBitmap(56, 30, icon_heart_14x14, 14, 14);
  drawBoxedBitmap(56, 48, icon_wifi_14x14, 14, 14);

  // =================================================
  // 20x20 ICONS
  // =================================================
  drawBoxedBitmap(100, 35, icon_heart_20x20, 20, 20);

  // =================================================
  // SHOW
  // =================================================
  display.display();
}

void loop() {

}