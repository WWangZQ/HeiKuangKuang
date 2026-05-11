#include "utils.h"
#include <algorithm>

using namespace std;

const int G_W = 80;
const int G_H = 25;
int skill_num = 5;
int chip_name = 5;

static WORD g_defaultColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
static bool g_colorInited = false;

static void initConsoleColor() {
    if (g_colorInited) return;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        g_defaultColor = csbi.wAttributes;
    }
    g_colorInited = true;
}

int displayWidth(const string& s) {
    int w = 0;
    for (int i = 0; i < (int)s.size(); ) {
        unsigned char c = (unsigned char)s[i];
        if      (c < 0x80) { w += 1; i += 1; }
        else if (c < 0xE0) { w += 1; i += 2; }
        else if (c < 0xF0) { w += 2; i += 3; }
        else               { w += 2; i += 4; }
    }
    return w;
}

void moveCursor(int row, int col) {
    COORD pos = {(SHORT)col, (SHORT)row};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printCentered(int row, const string& text) {
    int col = max(0, (G_W - displayWidth(text)) / 2);
    moveCursor(row, col);
    cout << text << flush;
}

void printCenteredColor(int row, const string& text, WORD color) {
    setColor(color);
    printCentered(row, text);
    resetColor();
}

void printHLine(int row, char ch) {
    moveCursor(row, 0);
    for (int i = 0; i < G_W; i++) cout << ch;
    cout << flush;
}

void printHLineColor(int row, char ch, WORD color) {
    setColor(color);
    printHLine(row, ch);
    resetColor();
}

void clearRow(int row) {
    moveCursor(row, 0);
    for (int i = 0; i < G_W; i++) cout << ' ';
}

void setColor(WORD color) {
    initConsoleColor();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void resetColor() {
    initConsoleColor();
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), g_defaultColor);
}
