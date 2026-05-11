#include "blackjack.h"
#include "utils.h"
#include "cards.h"
#include <conio.h>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <windows.h>
using namespace std;

extern int chip_name;
extern int skill_num;

static string roundSymbolName(int current) {
    if (current == 1) return "青龙";
    if (current == 2) return "白虎";
    if (current == 3) return "朱雀";
    return "玄武";
}

WORD seasonColor(const string& season) {
    if (season == "春") return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    if (season == "夏") return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    if (season == "秋") return FOREGROUND_RED | FOREGROUND_INTENSITY;
    return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
}

int getCardValue(const Card& card) {
    int val = card.sum;
    if (val > 10) val = 10;
    return val;
}

// 6行卡片：season 和 name 居中，日期两侧留空格
//  ┌────────┐
//  │ 0101   │
//  │   春   │
//  │  春节  │
//  │   0101 │
//  └────────┘
void displayCard(int row, int col, const Card& card) {
    const int inner = 8;  // 内部宽度（显示列数）

    // 在 inner 列内居中字符串
    auto center = [&](const string& s) -> string {
        int sw = displayWidth(s);
        int left  = (inner - sw) / 2;
        int right = inner - sw - left;
        return string(left, ' ') + s + string(right, ' ');
    };

    // 日期4个ASCII字符，左侧留1格，右侧不留空格
    string dateLeft  = " " + card.date + string(inner - 5, ' ');  // " 0101   "
    string dateRight = string(inner - 4, ' ') + card.date;        // "    0101"

    setColor(seasonColor(card.season));
    moveCursor(row,     col); cout << "┌────────┐";
    moveCursor(row + 1, col); cout << "│" << dateLeft          << "│";
    moveCursor(row + 2, col); cout << "│" << center(card.season) << "│";
    moveCursor(row + 3, col); cout << "│" << center(card.name)   << "│";
    moveCursor(row + 4, col); cout << "│" << dateRight         << "│";
    moveCursor(row + 5, col); cout << "└────────┘";
    resetColor();
}

// 重绘整个游戏界面
void redraw(int current, int total,
            const vector<int>& pCards, int pSum, bool pStand,
            const vector<int>& cCards, int cSum, bool cStand,
            const string& status) {
    (void)total;
    system("cls");
    string header = "第 " + to_string(current) + " 局（" + roundSymbolName(current) + "局）五行局  技能值: " + to_string(skill_num) + "  气运值: " + to_string(chip_name);
    printHLineColor(0, '=', FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(0, header, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printHLineColor(1, '=', FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 玩家牌区（rows 3-10）
    printCenteredColor(3, "玩家手牌", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    int col = 4;
    for (int i = 0; i < (int)pCards.size() && col < 70; i++) {
        displayCard(4, col, CARDS[pCards[i]]);
        col += 12;
    }
    printCenteredColor(11, "总和：" + to_string(pSum) + (pStand ? "  已停牌" : ""), FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    // 电脑牌区（rows 13-20）
    printCenteredColor(13, "对手手牌", FOREGROUND_RED | FOREGROUND_INTENSITY);
    col = 4;
    for (int i = 0; i < (int)cCards.size() && col < 70; i++) {
        displayCard(14, col, CARDS[cCards[i]]);
        col += 12;
    }
    printCenteredColor(21, "总和：" + to_string(cSum) + (cStand ? "  已停牌" : ""), FOREGROUND_RED | FOREGROUND_INTENSITY);

    if (!status.empty())
        printCenteredColor(23, status, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}



void startBlackJack(int current, int total) {
    int playerSum = 0, computerSum = 0;
    vector<int> playerCards, computerCards;
    bool playerStand = false, computerStand = false;

    // 初始化洗牌：生成所有牌的下标并随机打乱
    vector<int> deck(CARD_COUNT);
    for (int i = 0; i < CARD_COUNT; i++) deck[i] = i;
    for (int i = CARD_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
    int deckTop = 0;  // 下一张待发的牌

    // 各发一张牌
    playerCards.push_back(deck[deckTop++]);
    computerCards.push_back(deck[deckTop++]);
    playerSum   = getCardValue(CARDS[playerCards[0]]);
    computerSum = getCardValue(CARDS[computerCards[0]]);

    while (true) {
        // --- 玩家回合 ---
        if (!playerStand) {
            redraw(current, total, playerCards, playerSum, playerStand,
                   computerCards, computerSum, computerStand, "要牌(Y) 或 停牌(N)？");
            int key = _getch();
            if (key == 'Y' || key == 'y') {
                int idx = deck[deckTop++];
                playerCards.push_back(idx);
                playerSum += getCardValue(CARDS[idx]);
                if (playerSum > 36) {
                    chip_name--;
                    redraw(current, total, playerCards, playerSum, playerStand,
                           computerCards, computerSum, computerStand, "你超过36了，你输了！");
                    printCenteredColor(22, "按 Enter 继续", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    while (_getch() != 13);
                    return;
                }
            } else if (key == 'N' || key == 'n') {
                playerStand = true;
            }
        }

        // 若双方都停牌，结束
        if (playerStand && computerStand) break;

        // --- 电脑思考 ---
        redraw(current, total, playerCards, playerSum, playerStand,
               computerCards, computerSum, computerStand, "电脑思考中...");
        Sleep(1000);

        // --- 电脑决策 ---
        if (!computerStand) {
            if (computerSum <= 27) {
                int idx = deck[deckTop++];
                computerCards.push_back(idx);
                computerSum += getCardValue(CARDS[idx]);
                if (computerSum > 36) {
                    chip_name++;
                    redraw(current, total, playerCards, playerSum, playerStand,
                           computerCards, computerSum, computerStand, "电脑超过36了，你赢了！");
                    printCenteredColor(22, "按 Enter 继续", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                    while (_getch() != 13);
                    return;
                }
                redraw(current, total, playerCards, playerSum, playerStand,
                       computerCards, computerSum, computerStand, "电脑选择要牌");
            } else {
                computerStand = true;
                redraw(current, total, playerCards, playerSum, playerStand,
                       computerCards, computerSum, computerStand, "电脑选择停牌");
            }
            Sleep(1000);
        }

        // 再次检查双方是否都停牌
        if (playerStand && computerStand) break;
    }

    // 比较结果
    string result;
    if (playerSum > computerSum) {
        result = "你更接近36，你赢了！";
        chip_name++;
    } else if (playerSum < computerSum) {
        result = "电脑更接近36，你输了！";
        chip_name--;
    } else {
        result = "平局，气运值不变！";
    }
    redraw(current, total, playerCards, playerSum, playerStand,
           computerCards, computerSum, computerStand, result);
    printCenteredColor(22, "按 Enter 继续", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    while (_getch() != 13);
}
