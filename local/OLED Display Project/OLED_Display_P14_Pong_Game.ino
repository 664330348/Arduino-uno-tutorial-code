#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Paddle settings
int paddleHeight = 12;
int paddleWidth = 2;

// Left paddle (AI)
int leftPaddleY = 26;

// Right paddle (Player)
int rightPaddleY = 26;

// Ball
int ballX = 64;
int ballY = 32;
int ballVX = 2;
int ballVY = 1;
int ballSize = 2;

// Score
int scoreLeft = 0;
int scoreRight = 0;

// Buttons
#define BTN_DOWN 2
#define BTN_UP A0

void resetBall() {
  ballX = 64;
  ballY = 32;
  ballVX = (random(0, 2) == 0) ? 2 : -2;
  ballVY = (random(0, 2) == 0) ? 1 : -1;
}

void setup() {
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  randomSeed(analogRead(0));
}

void loop() {

  // ================= PLAYER (RIGHT PADDLE) =================
  // A0 UP
  if (digitalRead(BTN_UP) == LOW) {
    rightPaddleY -= 2;
  }

  // D2 DOWN
  if (digitalRead(BTN_DOWN) == LOW) {
    rightPaddleY += 2;
  }

  // clamp right paddle
  if (rightPaddleY < 0) rightPaddleY = 0;
  if (rightPaddleY > SCREEN_HEIGHT - paddleHeight)
    rightPaddleY = SCREEN_HEIGHT - paddleHeight;

  // ================= AI (LEFT PADDLE) =================
  if (ballY > leftPaddleY + paddleHeight / 2) leftPaddleY += 1;
  if (ballY < leftPaddleY + paddleHeight / 2) leftPaddleY -= 1;

  if (leftPaddleY < 0) leftPaddleY = 0;
  if (leftPaddleY > SCREEN_HEIGHT - paddleHeight)
    leftPaddleY = SCREEN_HEIGHT - paddleHeight;

  // ================= BALL MOVEMENT =================
  ballX += ballVX;
  ballY += ballVY;

  // top/bottom bounce
  if (ballY <= 0 || ballY >= SCREEN_HEIGHT) {
    ballVY = -ballVY;
  }

  // LEFT paddle collision (AI side)
  if (ballX <= 4 &&
      ballY > leftPaddleY &&
      ballY < leftPaddleY + paddleHeight) {
    ballVX = -ballVX;
  }

  // RIGHT paddle collision (Player side)
  if (ballX >= 122 &&
      ballY > rightPaddleY &&
      ballY < rightPaddleY + paddleHeight) {
    ballVX = -ballVX;
  }

  // scoring
  if (ballX < 0) {
    scoreRight++;
    resetBall();
  }

  if (ballX > 128) {
    scoreLeft++;
    resetBall();
  }

  // ================= DRAW =================
  display.clearDisplay();

  // center line
  for (int i = 0; i < 64; i += 6) {
    display.drawPixel(64, i, WHITE);
  }

  // paddles
  display.fillRect(2, leftPaddleY, paddleWidth, paddleHeight, WHITE);
  display.fillRect(124, rightPaddleY, paddleWidth, paddleHeight, WHITE);

  // ball
  display.fillRect(ballX, ballY, ballSize, ballSize, WHITE);

  // score
  display.setCursor(20, 0);
  display.print(scoreLeft);

  display.setCursor(100, 0);
  display.print(scoreRight);

  display.display();
}