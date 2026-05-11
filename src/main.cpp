#include <iostream>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include "utils.h"
#include "triad_grace.h"
#include "blackjack.h"
using namespace std;

// ===================== 【高级配色方案】 =====================
// 标题：金色渐变（黄+亮）
static const WORD COLOR_TITLE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
// 边框：青蓝色（高级科技感）
static const WORD COLOR_BORDER = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
// 玩家：翠绿色（清新醒目）
static const WORD COLOR_PLAYER = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
// 对手：玫红色/亮紫（对比强烈）
static const WORD COLOR_OPPONENT = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
// 状态信息：纯白色（最清晰）
static const WORD COLOR_STATUS = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
// 提示文字：天蓝色
static const WORD COLOR_TIPS = FOREGROUND_BLUE | FOREGROUND_GREEN;
// 强调文字：亮黄色
static const WORD COLOR_HL = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
// ===========================================================

void hideCursor() {
    CONSOLE_CURSOR_INFO info = {1, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void fixConsoleWindow() {
    system("mode con: cols=80 lines=25");
    HWND hwnd = GetConsoleWindow();
    if (hwnd) {
        LONG style = GetWindowLong(hwnd, GWL_STYLE);
        style &= ~(WS_MAXIMIZEBOX | WS_SIZEBOX);
        SetWindowLong(hwnd, GWL_STYLE, style);
        SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                     SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
}

void showWelcome() {
    system("cls");
    printHLineColor(1, '=', COLOR_BORDER);
    printCenteredColor(3, "四  象  灵  弈", COLOR_TITLE);
    printCenteredColor(5, "作者: 25级软工3班 队长周灼霖 队员刘宇质", COLOR_STATUS);
    printHLineColor(6, '=', COLOR_BORDER);
    printCenteredColor(8, "技能值: " + to_string(skill_num) + "   气运值: " + to_string(chip_name), COLOR_HL);
    printCenteredColor(10, "四局顺序：青龙局  白虎局  朱雀局  玄武局", COLOR_BORDER);
    printCenteredColor(12, "第一二局：五行局", COLOR_PLAYER);
    printCenteredColor(13, "第三四局：四象局", COLOR_OPPONENT);
    printHLineColor(15, '-', COLOR_STATUS);
    printCenteredColor(17, "按 Enter 开始", COLOR_TIPS);
    printHLineColor(19, '-', COLOR_STATUS);
    while (_getch() != 13);
}

void showGoodbye() {
    system("cls");
    printHLineColor(3, '=', COLOR_BORDER);
    printCenteredColor(6, "感 谢 游 玩 四 象 灵 弈", COLOR_TITLE);
    printCenteredColor(8, "期 待 下 次 再 会", COLOR_PLAYER);
    printCenteredColor(10, "作者: 25级软工3班 队长周灼霖 队员刘宇质", COLOR_STATUS);
    printHLineColor(13, '-', COLOR_STATUS);
    printCenteredColor(15, "技能值: " + to_string(skill_num) + "   气运值: " + to_string(chip_name), COLOR_HL);
    printHLineColor(18, '=', COLOR_BORDER);
    printCenteredColor(20, "按 Enter 退出", COLOR_TIPS);
    while (_getch() != 13);
}

int main(){
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    fixConsoleWindow();
    hideCursor();
    srand((unsigned)time(nullptr));

    showWelcome();
    for (int i = 1; i <= 4; i++) {
        if (i <= 2)
            startTriadGrace(i, 4);
        else
            startBlackJack(i, 4);
    }

    showGoodbye();
    return 0;
}