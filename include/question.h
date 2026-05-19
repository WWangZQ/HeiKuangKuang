#ifndef QUESTION_H
#define QUESTION_H

#include <string>

struct Question {
	std::string prompt;
	std::string optionA;
	std::string optionB;
	std::string optionC;
	char answer;
};

constexpr int QUESTION_COUNT = 8;

extern const Question QUESTIONS[QUESTION_COUNT];

// 返回 true 表示答对并获得奖励（气运值/技能值各 +2）
bool askQuestionAndRewardIfCorrect();

// 询问玩家是否继续做题，true 表示继续，false 表示停止
bool askWhetherContinueQuestioning();

#endif
