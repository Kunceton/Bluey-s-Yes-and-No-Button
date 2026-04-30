# Bluey Yes/No Button - Requirement Specification

## 1. Project Overview

**Project Name:** Bluey Yes/No Button
**Inspired by:** Bluey cartoon "Yes/No Button" toy
**URL:** https://yesnobutton.neocities.org

## 2. Functional Requirements

### 2.1 Core Features

| ID | Requirement | Priority | Status |
|----|-------------|----------|--------|
| FR-01 | 按下红色物理按钮，播放 "No" 声音 | Must | ✅ |
| FR-02 | 按下蓝色物理按钮，播放 "Yes" 声音 | Must | ✅ |
| FR-03 | UI 显示绿色 Y 按钮（模拟原始玩具） | Must | ✅ |
| FR-04 | UI 显示红色 N 按钮（模拟原始玩具） | Must | ✅ |
| FR-05 | 红色物理按钮按下时，UI 红色 N 按钮显示按下动效 | Should | ✅ |
| FR-06 | 蓝色物理按钮按下时，UI 绿色 Y 按钮显示按下动效 | Should | ✅ |
| FR-07 | 背景显示黄色（模拟原始玩具） | Should | ✅ |

### 2.2 User Interactions

| Action | Result |
|--------|--------|
| 按下 Dual Button 红色按钮 | 播放 no.mp3 + UI N 按钮按下动效 |
| 按下 Dual Button 蓝色按钮 | 播放 yes.mp3 + UI Y 按钮按下动效 |
| 释放按钮 | 按钮恢复原状 |

### 2.3 Hardware Requirements

| ID | Requirement | Notes |
|----|-------------|-------|
| HR-01 | M5StickC Plus 主控 | 4MB Flash, 135x240 TFT |
| HR-02 | Hat SPK2 音频输出 | MAX98357 I2S, 3.2W |
| HR-03 | Unit Dual Button 输入 | HY2.0-4P 接口 |

## 3. Non-Functional Requirements

### 3.1 Audio Quality

- 采样率: 8kHz ~ 96kHz
- 信噪比: ≥ 103dB
- 无明显延迟

### 3.2 UI Responsiveness

- 按钮响应时间: < 50ms
- 动画流畅，无卡顿

### 3.3 Power

- USB Type-C 供电
- 支持电池供电 (120mAh)

## 4. Acceptance Criteria

| Criteria | Verification |
|----------|--------------|
| 按下红色按钮播放 "No" 声音 | 听觉验证 |
| 按下蓝色按钮播放 "Yes" 声音 | 听觉验证 |
| UI 正确显示 Y/N 按钮 | 视觉验证 |
| 按钮按下时 UI 有动效 | 视觉验证 |
| 音频文件存储在 Flash 而非 SD 卡 | 代码审查 |

## 5. Out of Scope

- TF 卡支持（StickC Plus 无此插槽）
- 蓝牙连接
- 其他外部设备控制
