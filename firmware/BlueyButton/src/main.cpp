#include <M5StickCPlus.h>
#include <SPIFFS.h>
#include <Audio.h>
#include "bluey_logo.h"

// ============================================
// 电源管理 - 5分钟无操作自动关机
// ============================================
#define POWER_OFF_TIME_MS   300000  // 5分钟 = 300000ms
#define POWER_CHECK_INTERVAL 1000   // 每秒检查一次

unsigned long lastInteractionTime = 0;

// 记录用户交互
void recordInteraction() {
  lastInteractionTime = millis();
}

// 检查是否需要关机
void checkPowerOff() {
  if (millis() - lastInteractionTime > POWER_OFF_TIME_MS) {
    playShutdownSound();  // 播放关机音效
    delay(600);           // 等待音效播放完成
    M5.Axp.PowerOff();    // 关机
  }
}

// 开机提示音 - 使用 M5StickC Plus 内置 Beep 类
void playStartSound() {
  M5.Beep.begin();
  M5.Beep.setVolume(3);
  M5.Beep.tone(880, 150);   // 880Hz, 150ms
  delay(200);
  M5.Beep.tone(1760, 150);  // 1760Hz, 150ms
  delay(200);
  M5.Beep.tone(2637, 200);  // 2637Hz, 200ms
}

// 关机提示音 - 降序三音调
void playShutdownSound() {
  M5.Beep.begin();
  M5.Beep.setVolume(3);
  M5.Beep.tone(2637, 150);  // 2637Hz, 150ms
  delay(200);
  M5.Beep.tone(1760, 150);  // 1760Hz, 150ms
  delay(200);
  M5.Beep.tone(880, 200);   // 880Hz, 200ms
}

// ============================================
// 硬件引脚定义 (使用 ESP32 GPIO 编号)
// ============================================
#define RED_BTN_PIN  32
#define BLUE_BTN_PIN 33

// Hat SPK2 I2S 引脚 (已确认)
#define I2S_BCLK_PIN  26
#define I2S_LRC_PIN    0
#define I2S_DIN_PIN   25

// ============================================
// UI 颜色定义 - 黄色背景 + 绿色Y按钮 + 红色N按钮
// ============================================
#define BG_COLOR         0xFFE0   // 黄色背景 (#ffff80 淡黄)
#define FRAME_COLOR      0xDED6   // 边框颜色 (深黄)

// Y 按钮 - 深绿色圆形，浅绿色字母
#define Y_BTN_COLOR      0x3648   // 深绿色 (#364834)
#define Y_BTN_LIGHT      0x87D3   // 浅绿色 (#87d37f)
#define Y_BTN_PRESSED    0x1C24   // 更深的绿色（按下状态）

// N 按钮 - 深红色圆形，浅红色字母
#define N_BTN_COLOR      0xB96A   // 深红色
#define N_BTN_LIGHT      0xF5D7   // 浅红色
#define N_BTN_PRESSED    0x8B35   // 更深的红色（按下状态）

// 屏幕分辨率 (M5StickC Plus 是竖屏: 135 x 240)
#define SCREEN_WIDTH    135
#define SCREEN_HEIGHT   240

// Logo 位置 (居中显示在屏幕顶部，两边达到屏幕边缘)
#define LOGO_X          0         // 左侧对齐
#define LOGO_Y          0         // 顶部开始

// 按钮参数 - 垂直排列，居中，在 Logo 下方
#define BTN_RADIUS      32        // 按钮半径
#define SCREEN_CX       67        // 屏幕中心 X (135/2)
#define Y_BTN_Y         115       // Y 按钮 Y 坐标 (Logo 下方)
#define N_BTN_Y         190       // N 按钮 Y 坐标 (靠近屏幕底部)

// ============================================
// 前向声明
// ============================================
void drawLogo();
void drawFrame();
void drawButtonY(bool pressed, uint16_t btnColor, uint16_t txtColor);
void drawButtonN(bool pressed, uint16_t btnColor, uint16_t txtColor);
void clearButtonArea();

// ============================================
// 全局变量
// ============================================
Audio audio;
bool redBtnPressed = false;  // 红色物理按钮 -> N 按钮
bool blueBtnPressed = false;  // 蓝色物理按钮 -> Y 按钮
bool yPressed = false;  // Y 按钮显示状态
bool nPressed = false;  // N 按钮显示状态
bool needRedraw = true;  // 是否需要完整重绘

// ============================================
// 初始化
// ============================================
void setup() {
  M5.begin();
  Serial.begin(115200);

  // 初始化 SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mount failed!");
    return;
  }

  // 列出 SPIFFS 文件
  File root = SPIFFS.open("/");
  File file = root.openNextFile();
  Serial.println("SPIFFS files:");
  while (file) {
    Serial.print("  ");
    Serial.println(file.name());
    file = root.openNextFile();
  }

  // 配置 I2S 用于 Hat SPK2
  audio.setPinout(I2S_BCLK_PIN, I2S_LRC_PIN, I2S_DIN_PIN);
  audio.setVolume(21); // 0-21, 最大音量
  audio.setTone(6, 6, 6); // 增加 EQ 增益 (低/中/高)

  // 配置 TFT - 黄色背景填充 (竖屏 135x240)
  M5.Lcd.fillScreen(BG_COLOR);
  M5.Lcd.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BG_COLOR);

  // 绘制 UI
  drawLogo();
  drawFrame();
  clearButtonArea();
  drawButtonY(false, Y_BTN_COLOR, Y_BTN_LIGHT);
  drawButtonN(false, N_BTN_COLOR, N_BTN_LIGHT);

  // 配置按钮引脚 (上拉输入)
  pinMode(RED_BTN_PIN, INPUT_PULLUP);
  pinMode(BLUE_BTN_PIN, INPUT_PULLUP);

  // 初始化电源管理计时器
  lastInteractionTime = millis();

  // 播放开机提示音
  playStartSound();

  Serial.println("Bluey Yes/No Button Ready!");
  Serial.println("RED_BTN -> N (No)");
  Serial.println("BLUE_BTN -> Y (Yes)");
}

// ============================================
// 主循环
// ============================================
void loop() {
  M5.update();
  audio.loop();

  // 读取按钮状态 (低电平触发)
  bool redRead  = (digitalRead(RED_BTN_PIN) == LOW);
  bool blueRead = (digitalRead(BLUE_BTN_PIN) == LOW);

  // 红色物理按钮 -> N 按钮
  if (redRead && !redBtnPressed) {
    redBtnPressed = true;
    nPressed = true;
    yPressed = false;
    audio.stopSong();
    audio.setVolume(21);
    audio.connecttoFS(SPIFFS, "/no.wav");
    needRedraw = true;
    recordInteraction();  // 记录用户交互
  } else if (!redRead && redBtnPressed) {
    redBtnPressed = false;
    nPressed = false;
    needRedraw = true;
  }

  // 蓝色物理按钮 -> Y 按钮
  if (blueRead && !blueBtnPressed) {
    blueBtnPressed = true;
    yPressed = true;
    nPressed = false;
    audio.stopSong();
    audio.setVolume(21);
    audio.connecttoFS(SPIFFS, "/yes.wav");
    needRedraw = true;
    recordInteraction();  // 记录用户交互
  } else if (!blueRead && blueBtnPressed) {
    blueBtnPressed = false;
    yPressed = false;
    needRedraw = true;
  }

  // 检查电源管理
  checkPowerOff();

  // 按需重绘
  if (needRedraw) {
    clearButtonArea();
    // Y 按钮：按下时颜色变深
    uint16_t yBtnColor = yPressed ? Y_BTN_PRESSED : Y_BTN_COLOR;
    uint16_t yTxtColor = yPressed ? Y_BTN_PRESSED : Y_BTN_LIGHT;
    drawButtonY(yPressed, yBtnColor, yTxtColor);
    // N 按钮：按下时颜色变深
    uint16_t nBtnColor = nPressed ? N_BTN_PRESSED : N_BTN_COLOR;
    uint16_t nTxtColor = nPressed ? N_BTN_PRESSED : N_BTN_LIGHT;
    drawButtonN(nPressed, nBtnColor, nTxtColor);
    needRedraw = false;
  }
}

// ============================================
// 绘制 Bluey Logo
// ============================================
void drawLogo() {
  for (int y = 0; y < LOGO_HEIGHT; y++) {
    for (int x = 0; x < LOGO_WIDTH; x++) {
      M5.Lcd.drawPixel(LOGO_X + x, LOGO_Y + y, bluey_logo[y][x]);
    }
  }
}

// ============================================
// 绘制黄色矩形边框 (Logo 下方) - 浅色边框
// ============================================
void drawFrame() {
  int frameY = LOGO_Y + LOGO_HEIGHT + 5;
  // 只画一条细边框线，颜色与背景接近
  M5.Lcd.drawRoundRect(4, frameY, 127, 202, 8, FRAME_COLOR);
}

// ============================================
// 清除按钮区域 (黄色填充)
// ============================================
void clearButtonArea() {
  int clearY = LOGO_Y + LOGO_HEIGHT + 8;
  M5.Lcd.fillRoundRect(8, clearY, 119, 192, 6, BG_COLOR);
}

// ============================================
// 绘制 Y 按钮 - 颜色变化代替动效
// ============================================
void drawButtonY(bool pressed, uint16_t btnColor, uint16_t txtColor) {
  int yX = SCREEN_CX;
  int yY = Y_BTN_Y;
  M5.Lcd.fillCircle(yX, yY, BTN_RADIUS, btnColor);
  M5.Lcd.drawCircle(yX, yY, BTN_RADIUS, txtColor);
  M5.Lcd.setTextColor(txtColor);
  M5.Lcd.setTextSize(4);
  M5.Lcd.drawString("Y", yX - 10, yY - 12);
}

// ============================================
// 绘制 N 按钮 - 颜色变化代替动效
// ============================================
void drawButtonN(bool pressed, uint16_t btnColor, uint16_t txtColor) {
  int nX = SCREEN_CX;
  int nY = N_BTN_Y;
  M5.Lcd.fillCircle(nX, nY, BTN_RADIUS, btnColor);
  M5.Lcd.drawCircle(nX, nY, BTN_RADIUS, txtColor);
  M5.Lcd.setTextColor(txtColor);
  M5.Lcd.setTextSize(4);
  M5.Lcd.drawString("N", nX - 10, nY - 12);
}