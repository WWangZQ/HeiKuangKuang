#include "question.h"
#include "utils.h"

#include <conio.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const Question QUESTIONS[QUESTION_COUNT] = {
    {"二十四节气里，昼夜时长完全平分的节气是？", "立春", "春分", "谷雨", 'B'},
    {"传统腊八节最初起源目的是？", "祭祀先祖", "庆祝丰收", "祈求降雨", 'A'},
    {"四象对应四季，白虎对应哪个季节？", "春", "秋", "冬", 'B'},
    {"民间所说“三元节”，其中下元节是农历？", "七月十五", "十月十五", "正月十五", 'B'},
    {"二十四节气中，最先到来的是？", "雨水", "惊蛰", "立春", 'C'},
    {"传统五行里，火对应哪个方位？", "南方", "西方", "东方", 'A'},
    {"冬至在古时传统里属于？", "小节日", "亚岁大节", "普通民俗日", 'B'},
    {"寒食节固定在清明节前几天？", "一天", "三天", "五天", 'A'},
};

static void drawQuestionPanel(const Question& q, const string& tip) {
    system("cls");
    printHLineColor(1, '=', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(3, "当前为答题环节", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printHLineColor(5, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);

    printCenteredColor(7, "题目：" + q.prompt, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(10, "A. " + q.optionA, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(11, "B. " + q.optionB, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(12, "C. " + q.optionC, FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    printHLineColor(15, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(17, "按 A / B / C 作答", FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    if (!tip.empty()) {
        printCenteredColor(20, tip, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }
    printCenteredColor(22, "当前技能值: " + to_string(skill_num) + "   当前气运值: " + to_string(chip_name), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}

bool askQuestionAndRewardIfCorrect() {
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned)time(nullptr));
        seeded = true;
    }

    int idx = rand() % QUESTION_COUNT;
    const Question& q = QUESTIONS[idx];

    drawQuestionPanel(q, "");
    while (true) {
        int key = _getch();
        if (key >= 'a' && key <= 'z') key = key - 'a' + 'A';
        if (key != 'A' && key != 'B' && key != 'C') continue;

        if (key == q.answer) {
            skill_num += 2;
            chip_name += 2;
            drawQuestionPanel(q, "回答正确！技能值 +2，气运值 +2");
            Sleep(900);
            return true;
        }

        drawQuestionPanel(q, "回答错误！需继续答题直到恢复数值");
        Sleep(900);
        return false;
    }
}

bool askWhetherContinueQuestioning() {
    system("cls");
    printHLineColor(1, '=', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(4, "是否继续做题？", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printCenteredColor(7, "Y = 继续", FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printCenteredColor(8, "N = 停止", FOREGROUND_RED | FOREGROUND_INTENSITY);
    printHLineColor(11, '-', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    while (true) {
        int key = _getch();
        if (key >= 'a' && key <= 'z') key = key - 'a' + 'A';
        if (key == 'Y') return true;
        if (key == 'N') return false;
    }
}

