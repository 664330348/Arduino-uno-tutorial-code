#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h>

// ===================== OLED =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// ===================== JOYSTICK =====================
#define VRX A0
#define VRY A1
#define SW  2

// ===================== MENU =====================
const char* items[4] = {
  "OLED P1",
  "OLED P2",
  "OLED P3",
  "OLED P4"
};

int index = 0;

// ===================== STATE =====================
enum State {
  HOME,
  P1,
  P2,
  P3,
  P4
};

State state = HOME;

// ===================== JOYSTICK =====================
unsigned long lastMove = 0;

// ===================== BUTTON =====================
bool lastButton = HIGH;
bool buttonState = HIGH;
unsigned long lastDebounce = 0;

// =====================================================
// HEADER
// =====================================================
void drawHeader(const char* title) {

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("OLED P7");

  display.setCursor(0, 8);
  display.print(title);

  display.drawLine(0, 16, 128, 16, SSD1306_WHITE);
}

// =====================================================
// HOME PAGE
// =====================================================
void drawHome() {

  drawHeader("Joystick Menu");

  for (int i = 0; i < 4; i++) {

    int col = i % 2;
    int row = i / 2;

    int x = col * 64;
    int y = 25 + row * 18;

    display.setCursor(x, y);

    if (i == index) display.print("> ");
    else display.print("  ");

    display.print(items[i]);
  }
}

// =====================================================
// P1 (Hello World)
// =====================================================
void drawP1() {

  drawHeader("P1 Hello World");

  display.setCursor(10, 25);
  display.print("Hello");

  display.setCursor(10, 40);
  display.print("World");
}

// =====================================================
// P2 (Text Shapes)
// =====================================================
#define TOP_OFFSET 16

void drawP2() {

  drawHeader("P2 Text Shapes");

  display.setTextSize(2);
  display.setCursor(0, TOP_OFFSET + 5);
  display.print("Size2");

  display.drawLine(63, TOP_OFFSET, 63, 63, SSD1306_WHITE);

  display.setTextSize(1);
  display.setCursor(70, TOP_OFFSET + 5);
  display.print("Size1");

  display.drawRect(0, 50, 20, 12, SSD1306_WHITE);
  display.fillRect(25, 50, 20, 12, SSD1306_WHITE);

  display.drawCircle(85, 56, 6, SSD1306_WHITE);
  display.fillCircle(110, 56, 6, SSD1306_WHITE);
}

// =====================================================
// P3 (Bitmap Icons)
// =====================================================

// ---- icons ----
const unsigned char icon_heart_8x8[] PROGMEM = {
  0b01100110,0b11111111,0b11111111,0b11111111,
  0b01111110,0b00111100,0b00011000,0b00000000
};

const unsigned char icon_moon_8x8[] PROGMEM = {
  0b00111100,0b01111110,0b11110000,0b11100000,
  0b11100000,0b11110000,0b01111110,0b00111100
};

const unsigned char icon_check_8x8[] PROGMEM = {
  0b00000001,0b00000011,0b00000110,0b10001100,
  0b11011000,0b01110000,0b00100000,0b00000000
};

const unsigned char icon_folder_8x8[] PROGMEM = {
  0b00000000,0b01111100,0b10000010,0b10011010,
  0b10000010,0b11111110,0b00000000,0b00000000
};

const unsigned char icon_heart_14x14[] PROGMEM = {
  0x3C,0xF0,0x7F,0xF8,0xFF,0xFC,0xFF,0xFC,
  0xFF,0xFC,0xFF,0xFC,0x7F,0xF8,0x3F,0xF0,
  0x1F,0xE0,0x0F,0xC0,0x07,0x80,0x03,0x00,
  0x02,0x00,0x00,0x00
};

const unsigned char icon_wifi_14x14[] PROGMEM = {
  0x00,0x00,0x07,0x80,0x3F,0xF0,0xF0,0x3C,
  0xCF,0xCC,0x3F,0xF0,0x70,0x38,0x07,0x80,
  0x0F,0xC0,0x03,0x00,0x07,0x80,0x07,0x80,
  0x00,0x00,0x00,0x00
};

const unsigned char icon_heart_20x20[] PROGMEM = {
  0x06,0x0C,0x00,0x0F,0x1E,0x00,0x1F,0xBF,0x00,
  0x3F,0xFF,0x80,0x7F,0xFF,0xC0,0xFF,0xFF,0xE0,
  0xFF,0xFF,0xE0,0xFF,0xFF,0xE0,0x7F,0xFF,0xC0,
  0x3F,0xFF,0x80,0x1F,0xFF,0x00,0x0F,0xFE,0x00,
  0x07,0xFC,0x00,0x03,0xF8,0x00,0x01,0xF0,0x00,
  0x00,0xE0,0x00,0x00,0x40,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00
};

void drawBoxedBitmap(int x,int y,const unsigned char* b,int w,int h){
  display.drawRect(x-2,y-2,w+4,h+4,SSD1306_WHITE);
  display.drawBitmap(x,y,b,w,h,SSD1306_WHITE);
}

void drawP3() {

  drawHeader("P3 Bitmap Icons");

  display.drawLine(42, 16, 42, 63, SSD1306_WHITE);
  display.drawLine(85, 16, 85, 63, SSD1306_WHITE);
  display.drawLine(0, 26, 127, 26, SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(8, 18);
  display.print("8x8");

  display.setCursor(48, 18);
  display.print("14x14");

  display.setCursor(92, 18);
  display.print("20x20");

  drawBoxedBitmap(6, 32, icon_heart_8x8, 8, 8);
  drawBoxedBitmap(22, 32, icon_folder_8x8, 8, 8);
  drawBoxedBitmap(6, 45, icon_moon_8x8, 8, 8);
  drawBoxedBitmap(22, 45, icon_check_8x8, 8, 8);

  drawBoxedBitmap(56, 30, icon_heart_14x14, 14, 14);
  drawBoxedBitmap(56, 48, icon_wifi_14x14, 14, 14);

  drawBoxedBitmap(100, 35, icon_heart_20x20, 20, 20);
}

// =====================================================
// P4 (Scrolling_Text)
// =====================================================
const char* timeList[] = {
  "0:00","1:00","2:00","3:00","4:00","5:00",
  "6:00","7:00","8:00","9:00","10:00","11:00","12:00"
};

const char* marqueeText = " Arduino UNO R3 OLED Scrolling Text Demo ";

int timeOffset = 0;
int textOffset = 0;
int tick = 0;

void drawP4() {

  drawHeader("P4 Scroll Text");

  display.setTextWrap(false);

  // LEFT
  for (int i = 0; i < 4; i++) {
    int idx = (timeOffset + i) % 8;
    display.setCursor(5, 20 + i * 10);
    display.print(timeList[idx]);
  }

  display.drawLine(42, 16, 42, 64, SSD1306_WHITE);

  // RIGHT
  display.fillRect(44, 17, 84, 47, SSD1306_BLACK);

  int len = strlen(marqueeText);
  int charOffset = textOffset / 2;

  String out = "";
  for (int i = 0; i < 18; i++) {
    out += marqueeText[(i + charOffset) % len];
  }

  display.setCursor(44, 30);
  display.print(out);

  // Left side slow scrolling
  tick++;
  if (tick >= 20) {
    tick = 0;
    timeOffset = (timeOffset + 1) % 8;
  }

  // Right side scrolling
  textOffset++;
  if (textOffset > 200) textOffset = 0;
}

// =====================================================
// JOYSTICK
// =====================================================
void handleJoystick() {

  if (state != HOME) return;

  if (millis() - lastMove < 500) return;

  int x = analogRead(VRX);
  int y = analogRead(VRY);

  int col = index % 2;
  int row = index / 2;

  bool moved = false;

  if (x < 300) { col = (col == 0) ? 1 : 0; moved = true; }
  if (x > 700) { col = (col == 1) ? 0 : 1; moved = true; }
  if (y < 300) { row = (row == 0) ? 1 : 0; moved = true; }
  if (y > 700) { row = (row == 1) ? 0 : 1; moved = true; }

  if (moved) {
    index = row * 2 + col;
    lastMove = millis();
  }
}

// =====================================================
// BUTTON
// =====================================================
void handleButton() {

  bool reading = digitalRead(SW);

  if (reading != lastButton) {
    lastDebounce = millis();
  }

  if (millis() - lastDebounce > 50) {

    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == LOW) {

        if (state == HOME) {
          if (index == 0) state = P1;
          else if (index == 1) state = P2;
          else if (index == 2) state = P3;
          else if (index == 3) state = P4;
        } else {
          state = HOME;
        }
      }
    }
  }

  lastButton = reading;
}

// =====================================================
// RENDER
// =====================================================
void render() {

  display.clearDisplay();

  if (state == HOME) drawHome();
  else if (state == P1) drawP1();
  else if (state == P2) drawP2();
  else if (state == P3) drawP3();
  else if (state == P4) drawP4();

  display.display();
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  pinMode(SW, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }
}

// =====================================================
// LOOP
// =====================================================
void loop() {

  handleJoystick();
  handleButton();
  render();

  delay(30);
}