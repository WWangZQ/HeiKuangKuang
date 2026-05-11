#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <windows.h>

extern int skill_num;
extern int chip_name;

int displayWidth(const std::string& s);
void moveCursor(int row, int col);
void printCentered(int row, const std::string& text);
void printCenteredColor(int row, const std::string& text, WORD color);
void printHLine(int row, char ch);
void printHLineColor(int row, char ch, WORD color);
void clearRow(int row);
void setColor(WORD color);
void resetColor();

#endif
