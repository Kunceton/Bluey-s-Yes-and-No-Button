# Bluey Yes/No Button 项目交接文档

## 1. 当前任务目标

制作一个布鲁伊（Bluey）动画片风格的 yes/no 按钮玩具，使用 M5Stack StickC Plus + Hat SPK2 + Unit Dual Button 三个模块。

**功能需求**：
- 按下红色物理按钮 → 播放 "No" 声音 + UI 红色 N 按钮按下动效
- 按下蓝色物理按钮 → 播放 "Yes" 声音 + UI 绿色 Y 按钮按下动效
- UI 界面模拟原始网站外观：黄色背景 + 绿色 Y 按钮 + 红色 N 按钮
- 音频文件存储在板载 4MB Flash（SPIFFS），不使用 TF 卡
- Hat SPK2 通过 I2S 接口输出音频

## 2. 已完成的工作

### 2.1 项目目录结构创建

```
M5Stack-StickC-Plus-Bluey-YesNo/
├── sounds/                          # 原始音频文件
│   ├── yes.mp3                      # 17KB
│   └── no.mp3                       # 24KB
├── firmware/BlueyButton/
│   ├── platformio.ini               # PlatformIO 配置
│   ├── src/main.cpp                 # 主程序代码
│   └── data/                        # SPIFFS 上传目录
│       ├── yes.mp3
│       └── no.mp3
├── docs/
│   ├── design.md                    # 设计文档
│   ├── requirement.md               # 需求规格
│   └── task.md                      # 任务清单
└── README.md
```

### 2.2 硬件信息收集

| 模块 | 接口 | 引脚定义 |
|------|------|----------|
| StickC Plus | Grove (I2C+I/O+UART) | G32 (SDA) / G33 (SCL) |
| Hat SPK2 | I2S (顶部 8Pin) | BCLK=G26, LRC=G0, DIN=G36 |
| Unit Dual Button | HY2.0-4P (PORT B) | Yellow=Red Btn, White=Blue Btn, GND, 5V |

### 2.3 音频文件获取

从 https://yesnobutton.neocities.org 下载了两个音频文件：
- `yes.mp3` - Blue Button 按下时播放的声音
- `no.mp3` - Red Button 按下时播放的声音

### 2.4 代码实现（本会话）

**platformio.ini** - 配置使用 `esp32dev` 开发板，安装 M5StickCPlus 和 Module-Audio 库，后添加 microMP3 解码库

**main.cpp** - 实现：
- UI 绘制：黄色矩形背景 + 绿色 Y 按钮（左侧）+ 红色 N 按钮（右侧）
- 按钮检测：GPIO 32/33，低电平触发
- I2S 音频输出：通过 ESP32 I2S 接口输出到 Hat SPK2
- 按钮动效：按下时按钮下沉 10px
- MP3 解码播放：使用 microMP3 库解码 MP3 文件并通过 I2S 输出

### 2.5 固件烧录（上次会话）

| 步骤 | 状态 | 说明 |
|------|------|------|
| 固件编译 | ✅ | `.pio/build/m5stickc_plus/firmware.bin` |
| SPIFFS 上传 | ✅ | 音频文件已写入 4MB Flash |
| 固件上传 | ✅ | 程序已烧录到设备 |

## 3. 未完成的工作和下一步计划

### 3.1 待验证项

1. **音频播放功能**：代码中 `playMP3()` 函数使用 microMP3 库解码，需要验证 Hat SPK2 是否正常发声

2. **按钮触发电平**：代码假设 Dual Button 是低电平触发（`digitalRead() == LOW`），需要实际测试确认

3. **Hat SPK2 I2S 引脚**：虽然用户提供了引脚定义（BCLK=G26, LRC=G0, DIN=G36），但需要实际验证音频是否正常输出

### 3.2 下一步计划

1. 用户测试按钮和音频功能
2. 根据测试结果调整代码
3. 如 microMP3 库链接失败，考虑：
   - 使用 M5StickCPlus 内置 Speaker 类（见 `.pio/libdeps/m5stickc_plus/M5StickCPlus/src/utility/Speaker.h`）
   - 集成其他 MP3 解码库

## 4. 关键决策和原因

| 决策 | 原因 |
|------|------|
| 使用 PlatformIO 而非 Arduino CLI | PlatformIO 已安装且可用 |
| 使用 `esp32dev` 而非 `m5stickc_plus` board | PlatformIO 板库中无 m5stickc_plus board 定义 |
| 使用原始 GPIO 编号（32/33/26/0/36）而非 G32/G33 等宏 | M5StickCPlus 库中未定义这些宏 |
| 音频文件存入 SPIFFS 而非 SD 卡 | StickC Plus 无 TF 卡插槽 |
| UI 使用黄色矩形而非圆形背景 | 用户明确要求"黄色矩形填充" |
| 集成 microMP3 库用于 MP3 解码 | esphome/microMP3 是纯 C++ 实现的 MP3 解码器，适合 ESP32 |

## 5. 遇到的问题和解决方案

### 5.1 GPIO 宏定义问题

**问题**：`M5StickCPlus` 库中未定义 `G32`、`G33`、`G26` 等宏，导致编译失败

**解决**：使用原始 ESP32 GPIO 编号（32、33、26、0、36）

### 5.2 函数引用顺序问题

**问题**：`setup()` 中调用 `drawFrame()` 和 `drawButtons()`，但这些函数定义在调用之后，导致编译错误

**解决**：添加前向声明 `void drawFrame(); void drawButtons(bool, bool);`

### 5.3 Audio.h 库缺失（本会话）

**问题**：尝试使用 `ESP32-audioI2S` 库，但 PlatformIO 无法找到该包

**解决**：尝试了多个替代库

### 5.4 microMP3 库链接问题（本会话，未解决）

**问题**：microMP3 是 ESP-IDF 组件，使用 `idf_component_register` 注册，在 Arduino 框架下无法正确链接，导致链接错误：
```
undefined reference to `micro_mp3::Mp3Decoder::Mp3Decoder()'
```

**尝试的解决方案**：
1. 添加 `-I.pio/libdeps/m5stickc_plus/microMP3/include` build_flags 以解决头文件问题
2. microMP3 只支持 ESP-IDF 框架，不兼容纯 Arduino 框架

**待解决**：需要使用纯 Arduino 兼容的 MP3 解码库

### 5.5 开发板名称问题

**问题**：PlatformIO 报错 `Unknown board ID 'm5stickc_plus'`

**解决**：改用 `esp32dev` 作为通用开发板

### 5.6 进入刷机模式

**问题**：设备需要进入刷机模式才能烧录

**解决**：按住 A 按钮 + 按一下 RESET，然后松开 A（实际由用户操作）

## 6. 相关文件路径列表

### 6.1 项目文件

| 文件路径 | 说明 |
|----------|------|
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/` | 项目根目录 |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/firmware/BlueyButton/platformio.ini` | PlatformIO 配置 |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/firmware/BlueyButton/src/main.cpp` | 主程序代码（当前有编译问题） |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/firmware/BlueyButton/data/yes.mp3` | SPIFFS 音频文件 |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/firmware/BlueyButton/data/no.mp3` | SPIFFS 音频文件 |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/docs/design.md` | 设计文档 |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/docs/requirement.md` | 需求规格 |
| `/Users/louislau/Documents/My Project/ESP开发/M5Stack-StickC-Plus-Bluey-YesNo/docs/task.md` | 任务清单 |

### 6.2 编译产物

| 文件路径 | 说明 |
|----------|------|
| `.pio/build/m5stickc_plus/firmware.bin` | 编译后的固件 |
| `.pio/build/m5stickc_plus/spiffs.bin` | SPIFFS 镜像 |

### 6.3 关键库文件路径

| 文件路径 | 说明 |
|----------|------|
| `.pio/libdeps/m5stickc_plus/microMP3/include/micro_mp3/mp3_decoder.h` | microMP3 解码器头文件 |
| `.pio/libdeps/m5stickc_plus/microMP3/CMakeLists.txt` | microMP3 构建配置（ESP-IDF only） |
| `.pio/libdeps/m5stickc_plus/M5StickCPlus/src/utility/Speaker.h` | M5StickCPlus 内置 Speaker 类 |

## 7. 关键参数速查

| 参数 | 值 |
|------|-----|
| 红色按钮 GPIO | 32 |
| 蓝色按钮 GPIO | 33 |
| I2S BCLK | 26 |
| I2S LRC | 0 |
| I2S DIN | 36 |
| 按钮触发电平 | 低电平（假设） |
| SPIFFS 分区地址 | 0x00290000 |
| 固件分区地址 | 0x00010000 |

## 8. 当前 platformio.ini 内容

```ini
[env:m5stickc_plus]
platform = espressif32
board = esp32dev
framework = arduino
board_build.partitions = default.csv
board_build.flash_mode = dio
board_build.f_flash = 40000000L
board_build.psram_type = qio
monitor_speed = 115200
upload_speed = 1500000
build_flags =
    -I.pio/libdeps/m5stickc_plus/microMP3/include
lib_deps =
	m5stack/M5StickCPlus@^0.1.1
	m5stack/Module-Audio@^0.0.1
	esphome/microMP3@^0.2.0
```

## 9. 当前 main.cpp 状态

代码已完成，包含：
- I2S 配置和初始化
- microMP3 解码器集成
- MP3 解码和播放函数
- UI 绘制和按钮动效

**问题**：链接失败，需要将 microMP3 替换为 Arduino 兼容的 MP3 解码库

## 10. 建议的下一步

1. **方案 A**：移除 microMP3，使用 M5StickCPlus 内置 `SPEAKER` 类（可能仅支持简单 beep 音，不支持 MP3）

2. **方案 B**：搜索并集成纯 Arduino MP3 解码库，如：
   - `celliesprojects/ESP32_VS1053_Stream` - 需要 VS1053 硬件
   - 其他纯软件解码方案

3. **方案 C**：将项目改为 ESP-IDF 框架（但需要较大改动）

4. **方案 D**：使用硬件 MP3 解码模块（如 DFPlayer Mini）通过 UART 控制

---

## 11. 本会话工作记录（2026-05-01）

### 11.1 问题分析

根据交接文档，原来的 microMP3 库在 Arduino 框架下无法链接。调查发现：
- `earlephilhower/ESP8266Audio` 库包含纯软件 MP3 解码器（HELIX MP3）
- `esphome/ESP32-audioI2S` 依赖 ESP8266Audio，提供 Audio 类封装

### 11.2 解决方案

**更换音频库**：
- 移除：`esphome/microMP3@^0.2.0`（Arduino 不兼容）
- 移除：`m5stack/Module-Audio@^0.0.1`（可能有冲突）
- 添加：`esphome/ESP32-audioI2S@^2.3.0`（I2S 封装 + HELIX 解码）
- 添加：`earlephilhower/ESP8266Audio@^1.9.0`（MP3/AAC/WAV 解码器）

### 11.3 代码更新

**platformio.ini**（最终版本）：
```ini
[env:m5stickc_plus]
platform = espressif32
board = esp32dev
framework = arduino
board_build.partitions = default.csv
board_build.flash_mode = dio
board_build.f_flash = 40000000L
board_build.psram_type = qio
monitor_speed = 115200
upload_speed = 1500000
lib_deps =
	m5stack/M5StickCPlus@^0.1.1
	esphome/ESP32-audioI2S@^2.3.0
	earlephilhower/ESP8266Audio@^1.9.0
```

**main.cpp**（最终版本）：
- 使用 `Audio` 类替代手写 I2S + microMP3
- `audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT)` 配置 Hat SPK2
- `audio.connecttoFS(SPIFFS, "/yes.mp3")` 直接播放 SPIFFS 中的 MP3
- `audio.loop()` 在主循环处理音频流

### 11.4 I2S 引脚修正

通过查看官方示例 `M5StickC-Plus/examples/Hat/SPEAKER2/SPEAKER2.ino` 确认正确引脚：
- I2S_DOUT = **25**（原文档误写为 G36）
- I2S_BCLK = 26
- I2S_LRC = 0

### 11.5 测试结果

| 测试项 | 结果 |
|--------|------|
| 固件编译 | ✅ 成功 |
| 固件上传 | ✅ 成功 |
| SPIFFS 上传 | ✅ 成功 |
| 音频播放 | ✅ 有声音 |
| 播放卡顿 | ⚠️ 有轻微卡顿（已优化 loop() 去除 delay） |
| 音量 | ⚠️ 较小（已设置 volume=21，但效果有限） |

### 11.6 当前问题

1. **音量偏小**：电脑播放 MP3 声音洪亮，但设备输出较小。已尝试：
   - 设置 `audio.setVolume(21)`（最大）
   - 设置 `audio.setI2SCommFMT_LSB(true)`
   - 移除可能导致冲突的 Module-Audio 库
   
   **待解决**：可能需要用 ffmpeg/audacity 放大 MP3 文件音量后再上传

2. **轻微卡顿**：已移除 loop() 中的 delay(10)，改善但不彻底

### 11.7 最终代码（关键部分）

```cpp
// main.cpp 关键配置
Audio audio;

void setup() {
  // ...
  audio.setPinout(I2S_BCLK_PIN, I2S_LRC_PIN, I2S_DIN_PIN);  // 26, 0, 25
  audio.setVolume(21);
  audio.setI2SCommFMT_LSB(true);
  // ...
}

void loop() {
  M5.update();
  audio.loop();
  
  // 按钮检测和音频触发...
}
```

### 11.8 更新后的关键参数速查

| 参数 | 值 |
|------|-----|
| 红色按钮 GPIO | 32 |
| 蓝色按钮 GPIO | 33 |
| I2S BCLK | 26 |
| I2S LRC | 0 |
| I2S DIN | **25**（修正） |
| 按钮触发电平 | 低电平 |
| 音量设置 | 21（最大） |

### 11.9 待完成

1. 解决音量偏小问题（建议用音频编辑工具放大 MP3 文件）
2. 进一步优化播放流畅度
3. 完整功能测试

---

## 12. 本会话工作记录（2026-05-01 晚间）

### 12.1 测试结果反馈

用户测试上次固件后反馈：
- **音量**：有改善但仍偏小，户外可能听不到
- **延迟**：按下按钮后仍有延迟才发出声音
- **连续按键**：快速连续按同一个按键，仍无法连续发声

### 12.2 代码修改

**main.cpp** 关键改动：

1. **按钮处理逻辑优化**：
```cpp
// 红色按钮处理 (N)
if (redRead && !redBtnPressed) {
  redBtnPressed = true;
  audio.stopSong();  // 停止当前播放
  audio.setVolume(21);  // 确保音量重置
  audio.connecttoFS(SPIFFS, "/no.mp3");
  drawButtons(true, false);
}
```
- 每次按键时调用 `audio.stopSong()` 停止当前播放
- 重新设置 `audio.setVolume(21)` 确保音量不被之前状态影响

2. **EQ 增益增强**：
```cpp
audio.setTone(6, 6, 6); // 增加 EQ 增益 (低/中/高)
```

3. **移除 LSB 格式设置**：
- 删除了 `audio.setI2SCommFMT_LSB(true)`，因为可能与 Hat SPK2 不兼容

### 12.3 音频文件处理

**音量增强方案**：
- 安装 sox（通过 brew）
- 使用 `sox yes.mp3 yes_loud.mp3 gain +12` 将音频增益提高 12dB
- 使用 `sox no.mp3 no_loud.mp3 gain +12` 处理 no.mp3
- 用增强后的文件替换原文件并上传 SPIFFS

**测试结果**：
- 本地测试音量有明显提高
- 但仍偏小，户外可能听不到

### 12.4 问题分析

#### 延迟和连续按键问题（库架构限制）

深入分析 ESP32-audioI2S 库源码后发现：

1. **延迟原因**：
   - `connecttoFS()` 每次调用都会执行 `setDefaults()` → `stopSong()` → 重新初始化
   - 解码器初始化需要：分配内存、读取文件头、解析 MP3 帧、找到同步词
   - 整个过程约需 100-200ms

2. **连续按键问题**：
   - 由于初始化时间较长，在解码器初始化期间按下新按键会被忽略
   - 库本身不支持中断正在初始化的播放请求

3. **代码层面已无法优化**：这是库的架构限制，非代码 bug

#### 音量问题

- **可能原因**：
  1. MP3 源文件本身音量较低
  2. Hat SPK2 的 I2S 输入灵敏度与软件增益不完全匹配
  3. ESP32 I2S 输出幅度受限

### 12.5 当前状态

| 项目 | 状态 | 说明 |
|------|------|------|
| 固件编译 | ✅ | 成功 |
| 固件上传 | ✅ | 成功 |
| SPIFFS 上传 | ✅ | 音频文件已上传（含 12dB 增益） |
| 音量 | ⚠️ | 有改善但仍偏小 |
| 延迟 | ❌ | 库架构限制，无法通过代码解决 |
| 连续按键 | ❌ | 库架构限制，无法通过代码解决 |

### 12.6 待解决问题

1. **音量仍偏小**：需要进一步增大增益（考虑 +18dB 或更高），但要注意避免失真
2. **延迟问题**：需要考虑更换方案（如硬件 MP3 解码器）
3. **连续按键问题**：同上

### 12.7 建议的下一步

#### 方案 A：进一步增大音频增益
- 尝试将 MP3 文件增益提高到 +18dB 或 +20dB
- 使用 sox 的 `-c 1` 参数检查是否单声道导致增益受限
- 确保峰值不超过 0dBFS 避免失真

#### 方案 B：硬件放大
- 使用外置音频功放模块（如 PAM8403）放大信号
- 或使用 DFPlayer Mini 硬件解码器通过 UART 控制

#### 方案 C：更换音频格式
- 将 MP3 转换为 WAV（无压缩，延迟更低）
- 或使用更简单的音频格式减少解码时间

### 12.8 关键文件路径

| 文件 | 路径 |
|------|------|
| 主程序 | `firmware/BlueyButton/src/main.cpp` |
| 平台配置 | `firmware/BlueyButton/platformio.ini` |
| 音频文件（当前） | `firmware/BlueyButton/data/yes.mp3` (已增强 12dB) |
| 音频文件（当前） | `firmware/BlueyButton/data/no.mp3` (已增强 12dB) |
| 原始音频备份 | `sounds/yes.mp3`, `sounds/no.mp3` |

### 12.9 当前 I2S 配置

```cpp
#define I2S_BCLK_PIN  26
#define I2S_LRC_PIN    0
#define I2S_DIN_PIN   25

audio.setPinout(I2S_BCLK_PIN, I2S_LRC_PIN, I2S_DIN_PIN);
audio.setVolume(21);
audio.setTone(6, 6, 6);
```

### 12.10 相关库文件路径

| 库 | 路径 |
|----|------|
| ESP32-audioI2S | `.pio/libdeps/m5stickc_plus/ESP32-audioI2S/src/` |
| M5StickCPlus | `.pio/libdeps/m5stickc_plus/M5StickCPlus/src/` |
| ESP8266Audio | `.pio/libdeps/m5stickc_plus/ESP8266Audio/` |

---

## 13. 本会话工作记录（2026-05-01 深夜）

### 13.1 音频优化

**音量分析**：
| 文件 | 峰值电平 | RMS 电平 | 评估 |
|------|----------|----------|------|
| yes.mp3 | -21.83 dBFS | -38.22 dBFS | 偏低 |
| no.mp3 | -24.88 dBFS | -42.59 dBFS | 偏低 |

**音频增强方案**：
- 使用 sox 的 `compand` 动态压缩替代简单增益
- 添加强噪声门 (-80 dB 阈值) 抑制底噪
- 添加高通滤波 (80Hz) 去除低频 rumble
- 转换为 WAV 格式（无压缩，解码更简单，延迟更低）

**最终音频参数**：
| 文件 | 峰值 | RMS | 底噪 |
|------|------|-----|------|
| yes.wav | -6.62 dBFS | -21.56 dBFS | -23.96 dBFS |
| no.wav | -4.65 dBFS | -22.92 dBFS | -55.47 dBFS |

### 13.2 UI 修正

**问题诊断**：
- M5StickC Plus 屏幕分辨率是 **135 x 240**（竖屏），不是 240 x 135
- 原代码使用错误的分辨率导致按钮重叠和显示不完整

**UI 颜色修正**：
| 元素 | 之前 | 现在 |
|------|------|------|
| 背景色 | 橙色/黄绿色 | 黄色 (0xFFE0) |
| Y 按钮 | 浅绿色圆形 | 深绿色圆形 (0x3648) + 浅绿色字母 (0x87D3) |
| N 按钮 | 浅红色圆形 | 深红色圆形 (0xB96A) + 浅红色字母 (0xF5D7) |

**按钮布局**：上下排列，居中显示

### 13.3 按钮动效优化

**问题**：每次按钮状态变化都重绘整个区域导致 UI 闪烁

**解决方案**：
- 将 `drawButtons()` 拆分为独立的 `drawButtonY()` 和 `drawButtonN()`
- 添加 `needRedraw` 标志，只在状态改变时重绘
- 分离边框和按钮绘制逻辑

### 13.4 当前代码结构

```cpp
// 全局状态
bool redBtnPressed = false;   // 红色物理按钮 -> N 按钮
bool blueBtnPressed = false;   // 蓝色物理按钮 -> Y 按钮
bool yPressed = false;         // Y 按钮显示状态
bool nPressed = false;         // N 按钮显示状态
bool needRedraw = true;        // 是否需要完整重绘

// 绘制函数
void drawFrame();              // 绘制边框
void clearButtonArea();        // 清除按钮区域
void drawButtonY(bool pressed); // 绘制 Y 按钮
void drawButtonN(bool pressed); // 绘制 N 按钮
```

### 13.5 关键文件路径

| 文件 | 路径 |
|------|------|
| 主程序 | `firmware/BlueyButton/src/main.cpp` |
| 平台配置 | `firmware/BlueyButton/platformio.ini` |
| 音频文件（增强后） | `firmware/BlueyButton/data/yes.wav` |
| 音频文件（增强后） | `firmware/BlueyButton/data/no.wav` |

### 13.6 当前状态

| 项目 | 状态 |
|------|------|
| 固件编译 | ✅ 成功 |
| 固件上传 | ✅ 成功 |
| SPIFFS 上传 | ✅ 成功（音频已优化） |
| 音量 | ⚠️ 已优化，仍需实测 |
| UI 显示 | ✅ 颜色和位置正确 |
| 按钮动效 | ⚠️ 优化中，待实测 |

### 13.7 待验证项

1. 音频音量是否足够（户外测试）
2. 按钮动效是否无闪烁
3. 连续按键响应是否正常

---

*最后更新：2026-05-01 深夜*
