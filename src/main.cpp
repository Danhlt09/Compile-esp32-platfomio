#include <TFT_eSPI.h>
#include <SPI.h>

#define PIN_BUTTON 23

// Chân SPI (đặt cho ESP32)
#define TFT_MOSI 18
#define TFT_SCLK 5
#define TFT_CS   22
#define TFT_DC   21
#define TFT_RST  19

TFT_eSPI tft = TFT_eSPI(); // Khởi tạo TFT với cấu hình mặc định, sẽ ép chân bằng SPI.begin

// Dino thông số
int dinoX = 10;
int dinoY = 80;
int dinoVY = 0;
bool isJumping = false;

int obstacleX = 160;

int score = 0;

// Kích thước vẽ
const int dinoW = 15;
const int dinoH = 15;
const int obstacleW = 10;
const int obstacleH = 15;

// Hàm xóa hình chữ nhật (để giảm ghosting)
void clearRect(int x, int y, int w, int h) {
  tft.fillRect(x, y, w, h, TFT_BLACK);
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Khởi tạo SPI chân tùy chỉnh
  SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  // In điểm ban đầu
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(0,0);
  tft.print("Score: 0");
}

void loop() {
  // Xử lý nhảy
  if (digitalRead(PIN_BUTTON) == LOW && !isJumping) {
    isJumping = true;
    dinoVY = -12;
  }

  if (isJumping) {
    dinoY += dinoVY;
    dinoVY += 1; // trọng lực

    if (dinoY >= 80) {
      dinoY = 80;
      isJumping = false;
      dinoVY = 0;
    }
  }

  // Cập nhật vật cản
  obstacleX -= 6;
  if (obstacleX < -obstacleW) {
    obstacleX = 160;
    score++;
  }

  // Kiểm tra va chạm đơn giản
  bool collided = false;
  if (obstacleX < dinoX + dinoW && obstacleX + obstacleW > dinoX) {
    if (dinoY + dinoH > 80) {
      collided = true;
    }
  }

  if (collided) {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 60);
    tft.setTextSize(3);
    tft.print("GAME OVER");
    tft.setCursor(20, 100);
    tft.setTextSize(2);
    tft.print("Score: ");
    tft.print(score);
    delay(2000);

    // Reset game
    obstacleX = 160;
    score = 0;
    dinoY = 80;
    dinoVY = 0;
    isJumping = false;
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0,0);
    tft.setTextSize(2);
    tft.print("Score: 0");
    return;
  }

  // Xóa phần cũ (chỉ vùng cần thiết)
  clearRect(dinoX, dinoY, dinoW, dinoH);
  clearRect(obstacleX + 6, 80, obstacleW, obstacleH); // Xóa vật cản cũ

  // Vẽ dino
  tft.fillRect(dinoX, dinoY, dinoW, dinoH, TFT_WHITE);

  // Vẽ vật cản
  tft.fillRect(obstacleX, 80, obstacleW, obstacleH, TFT_WHITE);

  // Cập nhật điểm
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setCursor(0, 0);
  tft.setTextSize(2);
  tft.print("Score: ");
  tft.print(score);

  delay(20); // ~33 FPS
}
