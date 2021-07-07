#ifndef GUI_H
#define GUI_H
#include "FAT32.h"
#include <io.h>
#include <fcntl.h>


void gotoxy(int x, int y);
int wherex();
int wherey();
std::wstring s2ws(const std::string& s);
std::string dayOfWeek(WORD buffer);

#endif