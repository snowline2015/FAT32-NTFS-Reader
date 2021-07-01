#ifndef GUI_H
#define GUI_H
#include "FAT32.h"

void gotoxy(int x, int y);
int wherex();
int wherey();
std::wstring s2ws(const std::string& s);
int buffToInteger(byte* buffer);
unsigned int reversedBytes(uint8_t* byte);

#endif