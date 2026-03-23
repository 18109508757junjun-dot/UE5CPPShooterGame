# 📁 第三人称射击游戏核心逻辑框架 (TPS C++ Core)

[![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.7-white?logo=unrealengine&logoColor=white&color=0E1128)](https://www.unrealengine.com/)
[![Language](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)

## 📝 项目简介
本项目是一个专注于 **高性能、模块化** 的第三人称射击游戏后端逻辑实现。项目核心旨在展示 **C++ 与蓝图的高效协同架构**，以及利用 C++ 驱动复杂 **AI 行为树** 的开发思路。

> [!IMPORTANT]
> **注意：** 本仓库为 **Code-Only** 模式，仅上传了 `Source` 目录下的 C++ 核心源码。美术资产（模型、音效、粒子）及蓝图资源未上传。

---

## 🛠 技术栈 (Tech Stack)

| 分类 | 工具/技术 |
| :--- | :--- |
| **引擎版本** | **Unreal Engine 5.7** |
| **核心语言** | **C++** (底层逻辑), **Blueprints** (UI 与数据配置) |
| **版本控制** | **Git** |
| **开发环境** | **Visual Studio**, **GitHub Copilot** (AI 辅助逻辑优化) |

---

## 🧠 核心模块深度解析

### 1. 角色动力学与动画系统 (Character & Animation)
* **实现方案：** 通过 **C++ 驱动动画蓝图 (ABP)**，结合 **混合空间 (Blend Space)** 与 **状态机 (State Machine)** 实现流畅的位移反馈。

### 2. 增强输入串联 (Enhanced Input Integration)
* **核心代码：** `ShooterCharacter.cpp`
* **技术细节：** 完整实现了 **Input Mapping Context (IMC)** 与 **Input Actions** 的代码层绑定。通过增强输入系统，实现了从玩家操作到底层逻辑（开火、瞄准、移动）的精准响应与扩展性。

### 3. 战斗与射线检测逻辑 (Weapon System)
* **核心代码：** `Gun.cpp`
* **技术实现：** * 采用 **射线追踪 (Line Tracing)** 机制进行命中判定，确保射击反馈的实时性。
* **表现增强：** 在 C++ 逻辑中动态触发 **Niagara 粒子特效** 与 **立体音效 (Sound FX)**，演示了逻辑触发表现层的标准流程。

### 4. AI 行为驱动 (Tactical AI System)
* **核心代码：** `ShooterAIController.cpp` 及 **BT** 前缀源码
* **架构设计：** 基于 **行为树 (Behavior Tree)** 构建。通过 C++ 自定义 **BTTask**（控制攻击行为）与 **BTService**（实时更新感知状态），实现了 AI 从“巡逻”到“发现并追击玩家”的智能状态切换。

### 5. UI 交互层 (UI Framework)
* **实现方案：** 通过 **Widget Blueprint** 搭建 HUD 与菜单界面，采用 C++ 定义底层数据接口，蓝图负责视觉表现，遵循典型的 **MVC 设计模式**。

---

## 📂 如何阅读本项目 (How to Navigate)

由于缺少资产，源码主要集中在 `Source/` 目录下。建议重点审阅以下核心路径：

* **角色逻辑：** `Source/.../ShooterCharacter.cpp` 
    * *关键点：输入处理机制、组件初始化、生命周期管理。*
* **AI 驱动：** `Source/.../ShooterAIController.cpp`
    * *关键点：C++ 如何与黑板 (Blackboard) 进行数据交互，驱动 AI 决策。*
* **武器逻辑：** `Source/.../Gun.cpp` 
    * *关键点：射线检测算法实现、特效挂载逻辑。*
* **行为树扩展：** 查看以 `BT` 为前缀的一系列源码。
    * *关键点：观察 C++ 驱动的行为树节点实现。*

> **提示：** 源码中关键逻辑均附带详细的中文注释及目录说明。

---

## 📜 许可声明 (License)
本项目仅用于个人学习交流与作品集展示。
* **代码部分：** 均为个人原创实现。
* **美术素材：** 来源于网络公开资源（如虚幻商城等），版权归原作者所有。
