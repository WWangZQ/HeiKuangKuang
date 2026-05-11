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
    printHLineColor(2, '=', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(4, "四  象  灵  弈", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printHLineColor(6, '=', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(8, "技能值: " + to_string(skill_num) + "   气运值: " + to_string(chip_name), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(10, "四局顺序：青龙局  白虎局  朱雀局  玄武局", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(12, "第一二局：三才弈（青龙/白虎）", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(13, "第三四局：黑弈（朱雀/玄武）", FOREGROUND_RED | FOREGROUND_INTENSITY);
    printHLineColor(16, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(17, "按 Enter 开始", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printHLineColor(18, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    while (_getch() != 13);
}

void showGoodbye() {
    system("cls");
    printHLineColor(7, '=', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(10, "感 谢 游 玩 四 象 灵 弈", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(12, "期 待 下 次 再 会", FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printHLineColor(14, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(16, "技能值: " + to_string(skill_num) + "   气运值: " + to_string(chip_name), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printHLineColor(20, '=', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(21, "按 Enter 退出", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
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