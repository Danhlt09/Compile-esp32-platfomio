#define ST7735_DRIVER
#define TFT_WIDTH  80
#define TFT_HEIGHT 160
#define TFT_BL     -1     // Tắt điều khiển Backlight GPIO2

#define TFT_MOSI 18
#define TFT_SCLK 5
#define TFT_CS   22
#define TFT_DC   21
#define TFT_RST  19
#define BUTTON_PIN 23

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

const int dinoW = 15;
const int dinoH = 15;
const int obstacleW = 10;
const int obstacleH = 15;

int dinoX = 10;
int dinoY = 80;
int dinoVY = 0;
bool isJumping = false;

int obstacleX = 160;

int score = 0;

int lastDinoY = dinoY;
int lastObstacleX = obstacleX;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print("Score: 0");
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW && !isJumping) {
    isJumping = true;
    dinoVY = -12;
  }

  if (isJumping) {
    dinoY += dinoVY;
    dinoVY += 1;

    if (dinoY >= 80) {
      dinoY = 80;
      isJumping = false;
      dinoVY = 0;
    }
  }

  obstacleX -= 6;
  if (obstacleX < -obstacleW) {
    obstacleX = 160;
    score++;
  }

  bool collided = false;
  if (obstacleX < dinoX + dinoW && obstacleX + obstacleW > dinoX) {
    if (dinoY + dinoH > 80) {
      collided = true;
    }
  }

  if (collided) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 60);
    tft.setTextSize(3);
    tft.print("GAME OVER");
    tft.setCursor(10, 100);
    tft.setTextSize(2);
    tft.print("Score: ");
    tft.print(score);
    delay(2000);

    obstacleX = 160;
    score = 0;
    dinoY = 80;
    dinoVY = 0;
    isJumping = false;
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.setTextSize(2);
    tft.print("Score: 0");

    lastDinoY = dinoY;
    lastObstacleX = obstacleX;

    return;
  }

  // Xóa vùng cũ
  tft.fillRect(dinoX, lastDinoY, dinoW, dinoH, TFT_BLACK);
  tft.fillRect(lastObstacleX, 80, obstacleW, obstacleH, TFT_BLACK);

  // Vẽ mới
  tft.fillRect(dinoX, dinoY, dinoW, dinoH, TFT_WHITE);
  tft.fillRect(obstacleX, 80, obstacleW, obstacleH, TFT_WHITE);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.print("Score: ");
  tft.print(score);

  lastDinoY = dinoY;
  lastObstacleX = obstacleX;

  delay(30);
}