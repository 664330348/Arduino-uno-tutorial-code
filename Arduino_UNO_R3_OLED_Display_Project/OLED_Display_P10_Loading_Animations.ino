#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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
// BUTTON CONFIG
// =====================================================
#define BTN_PIN 2

bool lastBtnState = HIGH;
bool inStylePage = false;

// =====================================================
// STYLE STATE
// =====================================================
int currentStyle = 0;
const int styleCount = 4;

int progress = 0;
int spinnerIndex = 0;

unsigned long lastAnim = 0;
unsigned long lastSwitch = 0;

// =====================================================
// HEADER
// =====================================================
void drawHeader(const char* title) {

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("OLED P10");

  display.setCursor(0, 8);
  display.print(title);

  display.drawLine(0, 16, 127, 16, SSD1306_WHITE);
}

// =====================================================
// BUTTON CHECK
// =====================================================
void checkButton() {

  bool btnState = digitalRead(BTN_PIN);

  if (lastBtnState == HIGH && btnState == LOW) {
    inStylePage = !inStylePage;

    progress = 0;
    spinnerIndex = 0;

    delay(180);   // simple debounce
  }

  lastBtnState = btnState;
}

// =====================================================
// MAIN MENU SCREEN
// =====================================================
void drawMenu() {

  drawHeader("Animated Loading Screen");

  display.setTextSize(1);

  display.setCursor(0, 22);
  display.print("Select Style:");

  display.setCursor(0, 36);

  if (currentStyle == 0) display.print("> Progress Bar");
  if (currentStyle == 1) display.print("> Spinner");
  if (currentStyle == 2) display.print("> Dot Loading");
  if (currentStyle == 3) display.print("> Boot Screen");

  display.setCursor(0, 56);
  display.print("Press to enter");
}

// =====================================================
// STYLE 1: PROGRESS BAR
// =====================================================
void drawProgressBar() {

  drawHeader("Progress Bar");

  display.setTextSize(1);
  display.setCursor(20, 24);
  display.print("Initializing...");

  int barX = 14;
  int barY = 38;
  int barW = 100;
  int barH = 10;

  display.drawRect(barX, barY, barW, barH, SSD1306_WHITE);

  int fillWidth = map(progress, 0, 100, 0, barW - 2);

  display.fillRect(
    barX + 1,
    barY + 1,
    fillWidth,
    barH - 2,
    SSD1306_WHITE
  );

  display.setCursor(52, 54);
  display.print(progress);
  display.print("%");
}

// =====================================================
// STYLE 2: SPINNER
// =====================================================
void drawSpinner() {

  drawHeader("Spinner Loading");

  const char spinner[4] = {'|', '/', '-', '\\'};

  display.setTextSize(2);
  display.setCursor(52, 30);
  display.print(spinner[spinnerIndex]);

  display.setTextSize(1);
  display.setCursor(34, 54);
  display.print("Loading...");
}

// =====================================================
// STYLE 3: DOT LOADING
// =====================================================
void drawDotLoading() {

  drawHeader("Dot Loading");

  display.setTextSize(1);
  display.setCursor(35, 26);
  display.print("Please wait");

  int dotCount = spinnerIndex + 1;

  for (int i = 0; i < dotCount; i++) {
    display.fillCircle(48 + i * 12, 44, 3, SSD1306_WHITE);
  }

  display.setCursor(35, 56);
  display.print("Press to exit");
}

// =====================================================
// STYLE 4: BOOT SCREEN
// =====================================================
void drawBootScreen() {

  drawHeader("Boot Screen");

  display.setTextSize(2);
  display.setCursor(22, 24);
  display.print("SYSTEM");

  display.setTextSize(1);
  display.setCursor(30, 46);

  if (progress < 100) {
    display.print("Booting ");
    display.print(progress);
    display.print("%");
  } else {
    display.print("System Ready");
  }
}

// =====================================================
// UPDATE ANIMATION
// =====================================================
void updateAnimation() {

  if (millis() - lastAnim >= 80) {
    lastAnim = millis();

    progress++;
    if (progress > 100) progress = 0;

    spinnerIndex++;
    if (spinnerIndex > 3) spinnerIndex = 0;
  }
}

// =====================================================
// SETUP
// =====================================================
void setup() {

  pinMode(BTN_PIN, INPUT_PULLUP);

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

  checkButton();

  updateAnimation();

  if (!inStylePage) {

    if (millis() - lastSwitch >= 1800) {
      lastSwitch = millis();

      currentStyle++;
      if (currentStyle >= styleCount) {
        currentStyle = 0;
      }
    }

    display.clearDisplay();
    drawMenu();
    display.display();
  }
  else {

    display.clearDisplay();

    if (currentStyle == 0) drawProgressBar();
    if (currentStyle == 1) drawSpinner();
    if (currentStyle == 2) drawDotLoading();
    if (currentStyle == 3) drawBootScreen();

    display.display();
  }
}