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
| Yellow | RED_BTN (N) | G32 |
| White | BLUE_BTN (Y) | G33 |
| Red | 5V | - |
| Black | GND | - |

**Hat SPK2 (I2S Audio)**
| Signal | GPIO |
|--------|------|
| BCLK | G26 |
| LRC | G0 |
| DIN | G25 |

### 2.3 Power Architecture

- USB Type-C power input (5V)
- Internal 120mAh battery
- Hat SPK2: 4.5V-5.5V, max 3.2W output
- Auto power-off after 5 minutes of inactivity

## 3. UI Design

### 3.1 Screen Layout

```
+----------------------------------+
|        [Bluey Logo 135x65]       |
+----------------------------------+
|                                  |
|         [Frame Border]           |
|                                  |
|            (Green Y)              |
|              O                    |
|                                  |
|            (Red N)               |
|              O                    |
|                                  |
+----------------------------------+
```

**Screen Resolution:** 135 x 240 (M5StickC Plus portrait mode)

### 3.2 Color Palette

| Element | Color | Hex |
|---------|-------|-----|
| Background | Yellow | #FFE0 |
| Frame Border | Golden Yellow | #DED6 |
| Y Button Normal | Dark Green | #3648 |
| Y Button Pressed | Darker Green | #1C24 |
| Y Button Text | Light Green | #87D3 |
| N Button Normal | Dark Red | #B96A |
| N Button Pressed | Darker Red | #8B35 |
| N Button Text | Light Red | #F5D7 |

### 3.3 Button Animation

- **Press Effect:** Button color darkens (not position change)
- **Y Button:** 颜色从 #3648 变为 #1C24（按下）
- **N Button:** 颜色从 #B96A 变为 #8B35（按下）
- **Radius:** 32 pixels (diameter 64)
- **Y Position:** Y=115
- **N Position:** Y=190

### 3.4 Logo

- **Source:** bluey-logo.png
- **Display Size:** 135 x 65 pixels (full width)
- **Position:** Top of screen (Y=0)
- **Format:** RGB565 pixel data embedded in code

## 4. Software Architecture

### 4.1 Audio System

- **Amplifier IC:** MAX98357 (I2S interface)
- **Audio Format:** WAV (stored in SPIFFS)
- **Files:** `yes.wav`, `no.wav`
- **Storage:** 4MB SPIFFS partition
- **Audio Source:** https://theyesnobutton.com
- **Processing:** +6.5dB gain, normalized to ~0dBFS peak

### 4.2 Boot Sound

- **Type:** M5StickC Plus built-in beep
- **Melody:** Three ascending tones (880Hz → 1760Hz → 2637Hz)
- **Duration:** ~600ms total

### 4.3 Power Management

- **Auto Power-Off:** 5 minutes (300 seconds) of inactivity
- **Implementation:** `M5.Axp.PowerOff()`
- **Timer Reset:** Any button press resets the timer

### 4.4 State Machine

```
IDLE -> [RED_BTN pressed] -> PLAY_NO -> IDLE
IDLE -> [BLUE_BTN pressed] -> PLAY_YES -> IDLE
```

## 5. File Structure

```
M5Stack-StickC-Plus-Bluey-YesNo/
├── firmware/BlueyButton/
│   ├── platformio.ini       # PlatformIO configuration
│   ├── src/
│   │   ├── main.cpp         # Main program
│   │   └── bluey_logo.h     # Logo pixel data (RGB565)
│   └── data/                # SPIFFS files
│       ├── yes.wav
│       └── no.wav
├── sounds/                  # Original/processing audio files
├── image/                   # Project images
│   ├── bluey-logo.png
│   ├── Yes&No Button.jpg
│   ├── Front.jpg
│   └── Back.jpg
├── docs/
│   ├── design.md
│   ├── requirement.md
│   └── task.md
└── README.md
```

## 6. Dependencies

- M5StickCPlus library (v0.1.1)
- ESP32-audioI2S library (v2.3.0)
- ESP8266Audio library (v1.9.0)
- SPIFFS file system