# 黑框框 · 四象灵弈

一个运行在 Windows 控制台（“黑框框”）里的中国传统文化主题卡牌小游戏，使用 C++17 编写。

> 作者：25级软工3班 —— 队长周灼霖、队员刘宇质

## 游戏介绍

游戏共四局，按 **青龙局 → 白虎局 → 朱雀局 → 玄武局** 的顺序进行：

- **第一、二局（五行局）**：基于节气/节日卡牌的对弈玩法
- **第三、四局（四象局）**：类 21 点的比点数玩法，卡牌点数按日期计算

玩家有两项核心数值：

- **技能值**：使用技能消耗
- **气运值**：对局筹码

任一数值耗尽时会进入**答题环节**，回答传统文化题目（节气、节日、五行、四象等）即可恢复数值：每答对一题，**技能值 +2、气运值 +2**；也可以主动继续答题攒数值。

## 运行

直接双击仓库根目录下的 `game.exe` 即可游玩（仅支持 Windows）；也可以按下一节的说明自行从源码编译。

## 从源码编译

需要支持 C++17 的 g++（如 MinGW-w64）：

```bash
g++ -std=c++17 -finput-charset=UTF-8 -fexec-charset=UTF-8 -Wall -Wextra -O2 \
    -I include \
    src/main.cpp src/utils.cpp src/cards.cpp \
    src/triad_grace.cpp src/blackjack.cpp src/question.cpp \
    -o game.exe
```

也可以用 VS Code 打开本仓库，直接运行默认构建任务 **Build Game (g++)**（已配置在 `.vscode/tasks.json` 中）。

## 项目结构

```
├── include/            # 头文件
│   ├── blackjack.h     # 四象局（类21点）
│   ├── cards.h         # 卡牌定义（日期/季节/节日）
│   ├── question.h      # 答题系统
│   ├── triad_grace.h   # 五行局
│   └── utils.h         # 控制台工具（光标/颜色/居中输出）
├── src/                # 对应实现
├── .vscode/            # VS Code 构建配置
└── game.exe            # 编译好的可执行文件
```

## 说明

- 程序使用 Windows 控制台 API（`windows.h`、`conio.h`），仅支持 Windows 平台
- 启动时会将控制台设置为 UTF-8 编码并固定窗口大小为 80×25
