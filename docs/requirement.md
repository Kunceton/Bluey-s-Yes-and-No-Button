# Bluey Yes/No Button - Requirement Specification

## 1. Project Overview

**Project Name:** Bluey Yes/No Button
**Inspired by:** Bluey cartoon "Yes/No Button" toy
**URL:** https://theyesnobutton.com

## 2. Functional Requirements

### 2.1 Core Features

| ID | Requirement | Priority | Status |
|----|-------------|----------|--------|
| FR-01 | 按下红色物理按钮，播放 "No" 声音 | Must | ✅ |
| FR-02 | 按下蓝色物理按钮，播放 "Yes" 声音 | Must | ✅ |
| FR-03 | UI 显示绿色 Y 按钮（模拟原始玩具） | Must | ✅ |
| FR-04 | UI 显示红色 N 按钮（模拟原始玩具） | Must | ✅ |
| FR-05 | 红色物理按钮按下时，UI 红色 N 按钮颜色变深 | Must | ✅ |
| FR-06 | 蓝色物理按钮按下时，UI 绿色 Y 按钮颜色变深 | Must | ✅ |
| FR-07 | 背景显示黄色（模拟原始玩具） | Must | ✅ |
| FR-08 | 屏幕顶部显示 Bluey Logo | Should | ✅ |
| FR-09 | 开机时播放提示音 | Should | ✅ |
| FR-10 | 5分钟无操作自动关机 | Should | ✅ |
| FR-11 | 长按侧边按钮4秒关机 | Should | 🔄 ON HOLD |
| FR-12 | 关机时播放降序提示音 | Should | 🔄 ON HOLD |

### 2.2 User Interactions

| Action | Result |
|--------|--------|
| 按下 Dual Button 红色按钮 | 播放 no.wav + UI N 按钮颜色变深 |
| 按下 Dual Button 蓝色按钮 | 播放 yes.wav + UI Y 按钮颜色变深 |
| 释放按钮 | 按钮颜色恢复 |
| 开机 | 播放三音调提示音 + 显示 UI |
| 5分钟无操作 | 自动关机 |

### 2.3 Hardware Requirements

| ID | Requirement | Notes |
|----|-------------|-------|
| HR-01 | M5StickC Plus 主控 | 4MB Flash, 135x240 TFT |
| HR-02 | Hat SPK2 音频输出 | MAX98357 I2S, 3.2W |
| HR-03 | Unit Dual Button 输入 | HY2.0-4P 接口 |
| HR-04 | I2S 引脚正确配置 | BCLK=26, LRC=0, DIN=25 |

## 3. Non-Functional Requirements

### 3.1 Audio Quality

- 音频源：官方 Bluey Yes/No Button 网站
- 格式：WAV（无压缩）
- 处理：+6.5dB 增益，峰值约 0dBFS
- 采样率：原始文件（无重采样）

### 3.2 UI Responsiveness

- 按钮响应：无明显延迟
- 动画效果：颜色变化代替位置移动，避免闪烁
- Logo 显示：直接绘制 RGB565 像素数据

### 3.3 Power

- USB Type-C 供电
- 支持电池供电 (120mAh)
- 5分钟自动关机（PowerOff）

## 4. UI Design Requirements

### 4.1 Screen Layout

| Element | Position | Size |
|---------|----------|------|
| Bluey Logo | (0, 0) | 135 x 65 pixels |
| Frame Border | 四周 | 细边框线 |
| Y Button | 居中，Y=115 | 半径 32 |
| N Button | 居中，Y=190 | 半径 32 |

### 4.2 Color Specification

| Element | Normal | Pressed |
|---------|--------|---------|
| Y Button Background | #3648 | #1C24 |
| Y Button Text/Border | #87D3 | #1C24 |
| N Button Background | #B96A | #8B35 |
| N Button Text/Border | #F5D7 | #8B35 |

## 5. Acceptance Criteria

| Criteria | Verification |
|----------|--------------|
| 按下红色按钮播放 "No" 声音 | 听觉验证 |
| 按下蓝色按钮播放 "Yes" 声音 | 听觉验证 |
| UI 正确显示 Logo + Y/N 按钮 | 视觉验证 |
| 按钮按下时颜色变深 | 视觉验证 |
| 开机提示音正常播放 | 听觉验证 |
| 5分钟无操作后自动关机 | 时间验证 |
| 音频文件存储在 SPIFFS 而非 SD 卡 | 代码审查 |

## 6. Out of Scope

- TF 卡支持（StickC Plus 无此插槽）
- 蓝牙连接
- 其他外部设备控制
- 网络功能