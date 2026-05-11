#ifndef CARDS_H
#define CARDS_H

#include <string>

struct Card {
    std::string name;
    std::string season;
    std::string date;
    int sum;
};

extern const Card CARDS[];
extern const int CARD_COUNT;

#endif
