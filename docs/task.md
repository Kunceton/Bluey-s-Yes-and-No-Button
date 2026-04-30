# Bluey Yes/No Button - Task List

## Phase 1: Research & Planning ✅

- [x] 搜索 M5Stack 模块接口文档
  - StickC Plus GPIO 定义
  - Hat SPK2 I2S 接口规格
  - Unit Dual Button 规格
- [x] 获取音频文件
  - 从 yesnobutton.neocities.org 下载 yes.mp3 / no.mp3
- [x] 创建项目目录结构
- [x] 编写设计文档
- [x] 编写需求文档

## Phase 2: Hardware Setup

- [ ] 连接硬件模块
  - [ ] Hat SPK2 插入 StickC Plus 顶部 8Pin
  - [ ] Unit Dual Button 插入 Grove 接口 (G32/G33)
- [ ] 验证 I2S 引脚连接（待确认）

## Phase 3: Software Development

- [ ] 配置 Arduino 开发环境
  - [ ] 安装 M5StickCPlus 库
  - [ ] 安装 Audio 库
  - [ ] 选择开发板 M5StickC-Plus
- [ ] 上传音频文件到 SPIFFS
  - [ ] 上传 yes.mp3
  - [ ] 上传 no.mp3
- [ ] 烧录主程序
- [ ] 测试并调整参数
  - [ ] 确认按钮触发电平（高/低）
  - [ ] 确认 I2S 引脚定义
  - [ ] 调整 UI 位置和大小

## Phase 4: Verification

- [ ] 功能验证
  - [ ] 红色按钮播放 No 声音
  - [ ] 蓝色按钮播放 Yes 声音
  - [ ] UI 动效正常显示
- [ ] 整理项目文档

## Phase 5: Finalization

- [ ] 更新 README.md
- [ ] 更新文档（design.md, requirement.md）
- [ ] 清理临时文件

---

## Task Status Summary

| Phase | Tasks | Completed | Pending |
|-------|-------|-----------|---------|
| Phase 1 | 6 | 6 | 0 |
| Phase 2 | 2 | 0 | 2 |
| Phase 3 | 6 | 0 | 6 |
| Phase 4 | 4 | 0 | 4 |
| Phase 5 | 3 | 0 | 3 |
| **Total** | **21** | **6** | **15** |
