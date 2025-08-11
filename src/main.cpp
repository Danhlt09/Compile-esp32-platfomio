#define ST7735_DRIVER
#define TFT_WIDTH  160
#define TFT_HEIGHT 80
#define TFT_BL     -1    // BLK được cấp 3.3V trực tiếp, tắt điều khiển chân BLK

#define TFT_MOSI 18
#define TFT_SCLK 5
#define TFT_CS   22
#define TFT_DC   21
#define TFT_RST  19
#define BUTTON_PIN 23

#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft.init();
  tft.setRotation(1);   // Xoay ngang
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("Hello TFT!");
}

void loop() {
  // Chờ nút bấm để đổi màu màn hình
  if (digitalRead(BUTTON_PIN) == LOW) {
    tft.fillScreen(TFT_BLUE);
    delay(300);
    tft.fillScreen(TFT_BLACK);
  }
}