# Bluey Yes/No Button - Design Document

## 1. Overview

**Project Name:** Bluey Yes/No Button
**Type:** Interactive Toy / Hardware Project
**Core Functionality:** Recreate the iconic yes/no button toy from Bluey cartoon using M5Stack modules

## 2. Hardware Design

### 2.1 Module Configuration

| Module | Role | Connection |
|--------|------|------------|
| M5StickC Plus | Main Controller | - |
| Hat SPK2 | I2S Speaker | Top 8Pin Interface |
| Unit Dual Button | Input Buttons | Grove Port (G32/G33) |

### 2.2 Pin Mapping

**Dual Button (Grove Port B)**
| Wire | Signal | GPIO |
|------|--------|------|
| Yellow | RED_BTN | G32 |
| White | BLUE_BTN | G33 |
| Red | 5V | - |
| Black | GND | - |

**Hat SPK2 (I2S Audio)**
| Signal | GPIO |
|--------|------|
| BCLK | G26 |
| LRC | G0 |
| DIN | G36 |

### 2.3 Power Architecture

- USB Type-C power input (5V)
- Internal 120mAh battery
- Hat SPK2: 4.5V-5.5V, max 3.2W output

## 3. UI Design

### 3.1 Screen Layout

```
+----------------------------------+
|  [Yellow Rectangle Background]   |
|                                  |
|    (Green Y)      (Red N)        |
|      O              O            |
|                                  |
+----------------------------------+
```

### 3.2 Color Palette

| Element | Color | Hex |
|---------|-------|-----|
| Background | Yellow | #e2e18f |
| Frame Border | Golden Yellow | #cfc07a |
| Y Button | Green | #98e7a9 |
| Y Button Border | Dark Green | #376d42 |
| N Button | Red | #be3066 |
| N Button Border | Dark Red | #5c1923 |

### 3.3 Button Animation

- **Press Effect:** Button下沉13像素 + 阴影减少
- **Duration:** 按钮按下期间持续显示

## 4. Software Architecture

### 4.1 Audio System

- **Amplifier IC:** MAX98357 (I2S interface)
- **Audio Format:** MP3 (stored in SPIFFS)
- **Files:** `yes.mp3`, `no.mp3`
- **Storage:** 4MB SPIFFS partition

### 4.2 State Machine

```
IDLE -> [RED_BTN pressed] -> PLAY_NO -> IDLE
IDLE -> [BLUE_BTN pressed] -> PLAY_YES -> IDLE
```

## 5. File Structure

```
M5Stack-StickC-Plus-Bluey-YesNo/
├── firmware/BlueyButton/
│   ├── BlueyButton.ino
│   └── data/           # SPIFFS files
│       ├── yes.mp3
│       └── no.mp3
├── sounds/
│   ├── yes.mp3
│   └── no.mp3
├── docs/
│   ├── design.md
│   ├── requirement.md
│   └── task.md
└── README.md
```

## 6. Dependencies

- M5StickCPlus library
- Audio library (ESP32 Audio Tools)
- SPIFFS file system
