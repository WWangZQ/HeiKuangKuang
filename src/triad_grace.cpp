#include "triad_grace.h"
#include "utils.h"
#include "cards.h"
#include <conio.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <windows.h>
using namespace std;

extern int skill_num;
extern int chip_name;

static string roundSymbolName(int current) {
    if (current == 1) return "青龙";
    if (current == 2) return "白虎";
    if (current == 3) return "朱雀";
    return "玄武";
}

static WORD tgSeasonColor(const string& season) {
    if (season == "春") return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    if (season == "夏") return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    if (season == "秋") return FOREGROUND_RED | FOREGROUND_INTENSITY;
    return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
}

static void displayCard(int row, int col, const Card& card) {
    const int inner = 8;
    auto center = [&](const string& s) -> string {
        int sw = displayWidth(s);
        int left  = (inner - sw) / 2;
        int right = inner - sw - left;
        return string(left, ' ') + s + string(right, ' ');
    };
    string dateLeft  = " " + card.date + string(inner - 5, ' ');
    string dateRight = string(inner - 4, ' ') + card.date;

    setColor(tgSeasonColor(card.season));
    moveCursor(row,     col); cout << "┌────────┐";
    moveCursor(row + 1, col); cout << "│" << dateLeft             << "│";
    moveCursor(row + 2, col); cout << "│" << center(card.season)  << "│";
    moveCursor(row + 3, col); cout << "│" << center(card.name)    << "│";
    moveCursor(row + 4, col); cout << "│" << dateRight            << "│";
    moveCursor(row + 5, col); cout << "└────────┘";
    resetColor();
}

static void displayCardBack(int row, int col) {
    setColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    moveCursor(row,     col); cout << "┌────────┐";
    moveCursor(row + 1, col); cout << "│ ~~~~~~ │";
    moveCursor(row + 2, col); cout << "│  扣牌  │";
    moveCursor(row + 3, col); cout << "│  扣牌  │";
    moveCursor(row + 4, col); cout << "│ ~~~~~~ │";
    moveCursor(row + 5, col); cout << "└────────┘";
    resetColor();
}

// 手牌等级：3=炸弹（4张同季节），2=豹子（3张同季节），1=对子（2张同季节），0=散牌
static int seasonIndex(const string& season) {
    if (season == "春") return 0;
    if (season == "夏") return 1;
    if (season == "秋") return 2;
    return 3;
}

static int handRank(const vector<int>& idx) {
    int cnt[4] = {0, 0, 0, 0};
    for (int cardIdx : idx) {
        cnt[seasonIndex(CARDS[cardIdx].season)]++;
    }
    int mx = 0;
    for (int i = 0; i < 4; i++) {
        if (cnt[i] > mx) mx = cnt[i];
    }
    if (mx >= 4) return 3;
    if (mx >= 3) return 2;
    if (mx >= 2) return 1;
    return 0;
}

static int handSum(const vector<int>& idx) {
    int total = 0;
    for (int cardIdx : idx) total += CARDS[cardIdx].sum;
    return total;
}

static string rankName(int rank) {
    if (rank == 3) return "炸弹";
    if (rank == 2) return "豹子";
    if (rank == 1) return "对子";
    return "散牌";
}

static void redraw(int current, bool revealed,
                   const vector<int>& pCards, const vector<int>& cCards,
                   const string& status) {
    system("cls");
    string header = "第 " + to_string(current) + " 局（" + roundSymbolName(current) + "局） 技能值: " + to_string(skill_num) + "  气运值: " + to_string(chip_name);
    printHLineColor(0, '=', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(0, header, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printHLineColor(1, '=', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 对手牌区 rows 2-9
    printCenteredColor(2, "对手手牌", FOREGROUND_RED | FOREGROUND_INTENSITY);
    int col = 4;
    for (int i = 0; i < (int)cCards.size(); i++) {
        if (revealed)
            displayCard(3, col, CARDS[cCards[i]]);
        else
            displayCardBack(3, col);
        col += 12;
    }
    if (revealed)
        printCenteredColor(9, "合计: " + to_string(handSum(cCards)) + "  " + rankName(handRank(cCards)), FOREGROUND_RED | FOREGROUND_INTENSITY);
    else
        printCenteredColor(9, "合计: ???", FOREGROUND_RED | FOREGROUND_INTENSITY);

    printHLineColor(10, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 玩家牌区 rows 11-18
    printCenteredColor(11, "玩家手牌", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    col = 4;
    for (int i = 0; i < (int)pCards.size(); i++) {
        displayCard(12, col, CARDS[pCards[i]]);
        col += 12;
    }
    printCenteredColor(18, "合计: " + to_string(handSum(pCards)) + "  " + rankName(handRank(pCards)), FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    printHLineColor(19, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    string skillText = "S: 发动技能";
    if (current == 1) skillText = "S: 限定技能-换牌";
    else if (current == 2) skillText = "S: 限定技能-双方各摸一张";
    printCenteredColor(20, "O: 开牌        " + skillText, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    if (!status.empty())
        printCenteredColor(23, status, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}

static void showResult(int current,
                       const vector<int>& pCards, const vector<int>& cCards,
                       const string& result) {
    system("cls");
    string header = "第 " + to_string(current) + " 局（" + roundSymbolName(current) + "局） 技能值: " + to_string(skill_num) + "  气运值: " + to_string(chip_name);
    printHLineColor(0, '=', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(0, header, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printHLineColor(1, '=', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 对手牌
    printCenteredColor(2, "对手手牌", FOREGROUND_RED | FOREGROUND_INTENSITY);
    int col = 4;
    for (int i = 0; i < (int)cCards.size(); i++) { displayCard(3, col, CARDS[cCards[i]]); col += 12; }
    printCenteredColor(9, "合计: " + to_string(handSum(cCards)) + "  " + rankName(handRank(cCards)), FOREGROUND_RED | FOREGROUND_INTENSITY);

    printHLineColor(10, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    // 玩家牌
    printCenteredColor(11, "玩家手牌", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    col = 4;
    for (int i = 0; i < (int)pCards.size(); i++) { displayCard(12, col, CARDS[pCards[i]]); col += 12; }
    printCenteredColor(18, "合计: " + to_string(handSum(pCards)) + "  " + rankName(handRank(pCards)), FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    printHLineColor(19, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(20, result, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(21, "气运值：" + to_string(chip_name), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printHLineColor(22, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(23, "按 Enter 继续", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    while (_getch() != 13);
}

static void doSkillSwap(int current, vector<int>& pCards, vector<int>& cCards) {
    // 第一步：选择己方要换出的牌
    int pSel = -1;
    while (pSel < 0) {
        redraw(current, false, pCards, cCards, "");
        clearRow(20);
        printCenteredColor(20, "技能：选择你要换出的牌 (1/2/3)", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        clearRow(23);
        int k = _getch();
        if (k == '1') pSel = 0;
        else if (k == '2') pSel = 1;
        else if (k == '3') pSel = 2;
    }
    // 第二步：选择对手要换入的牌
    int cSel = -1;
    while (cSel < 0) {
        redraw(current, false, pCards, cCards, "");
        clearRow(20);
        printCenteredColor(20, "技能：选择对手要换入的牌 (1/2/3)", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        clearRow(23);
        printCenteredColor(23, "已选你的第 " + to_string(pSel + 1) + " 张牌", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        int k = _getch();
        if (k == '1') cSel = 0;
        else if (k == '2') cSel = 1;
        else if (k == '3') cSel = 2;
    }
    swap(pCards[pSel], cCards[cSel]);
    skill_num -= 2;
}

static bool doSkillDrawOneEach(int current, const vector<int>& deck, int& drawPos,
                               vector<int>& pCards, vector<int>& cCards) {
    if (drawPos + 1 >= CARD_COUNT) return false;

    pCards.push_back(deck[drawPos++]);
    cCards.push_back(deck[drawPos++]);
    skill_num--;

    redraw(current, false, pCards, cCards, "限定技能生效：双方各摸一张");
    Sleep(700);
    return true;
}

void startTriadGrace(int current, int total) {
    (void)total;
    // 洗牌并发3+3张（共6张不重复）
    vector<int> deck(CARD_COUNT);
    for (int i = 0; i < CARD_COUNT; i++) deck[i] = i;
    for (int i = CARD_COUNT - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(deck[i], deck[j]);
    }
    vector<int> pCards = {deck[0], deck[1], deck[2]};
    vector<int> cCards = {deck[3], deck[4], deck[5]};
    int drawPos = 6;

    bool skillUsed = false;
    redraw(current, false, pCards, cCards, "请选择操作...");

    // 开牌逻辑（O键 和 技能换牌后 共用）
    auto doReveal = [&]() {
        int pRank = handRank(pCards);
        int cRank = handRank(cCards);
        string result;
        if (pRank > cRank) {
            result = "玩家 " + rankName(pRank) + " 胜！气运值 +1";
            chip_name++;
        } else if (pRank < cRank) {
            result = "对手 " + rankName(cRank) + " 胜！气运值 -1";
            chip_name--;
        } else {
            int pSum = handSum(pCards);
            int cSum = handSum(cCards);
            if (pSum > cSum) {
                result = rankName(pRank) + "同级，玩家点数 " + to_string(pSum) + " 胜！气运值 +1";
                chip_name++;
            } else if (pSum < cSum) {
                result = rankName(cRank) + "同级，对手点数 " + to_string(cSum) + " 胜！气运值 -1";
                chip_name--;
            } else {
                result = rankName(pRank) + "同级，点数相同，平局！气运值不变";
            }
        }
        redraw(current, true, pCards, cCards, "开牌中...");
        Sleep(800);
        showResult(current, pCards, cCards, result);
    };

    while (true) {
        int key = _getch();
        if (key == 'O' || key == 'o') {
            doReveal();
            break;
        } else if (key == 'S' || key == 's') {
            if (skillUsed) {
                redraw(current, false, pCards, cCards, "本局已使用过技能");
            } else {
                if (current == 1) {
                    if (skill_num < 2) {
                        redraw(current, false, pCards, cCards, "技能值不足2点，无法发动第一局技能");
                        continue;
                    }
                    doSkillSwap(current, pCards, cCards);
                    skillUsed = true;
                    doReveal();
                    break;
                }
                if (current == 2) {
                    if (skill_num < 1) {
                        redraw(current, false, pCards, cCards, "技能值不足，无法发动第二局技能");
                        continue;
                    }
                    if (!doSkillDrawOneEach(current, deck, drawPos, pCards, cCards)) {
                        redraw(current, false, pCards, cCards, "牌堆不足，无法发动摸牌技能");
                        continue;
                    }
                    skillUsed = true;
                    doReveal();
                    break;
                }
                redraw(current, false, pCards, cCards, "本局无可用限定技能");
            }
        }
    }
}

