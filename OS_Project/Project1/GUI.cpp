#include "GUI.h"

void gotoxy(int x, int y)
{
    static HANDLE h = NULL;
    if (!h)
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD c = { x, y };
    SetConsoleCursorPosition(h, c);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &csbi
    ))
        return -1;
    return csbi.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE),
        &csbi
    ))
        return -1;
    return csbi.dwCursorPosition.Y;
}

std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

int buffToInteger(byte* buffer)
{
    //int a = static_cast<int>(static_cast<unsigned char>(buffer[3]) << 24 |
    //    static_cast<unsigned char>(buffer[2]) << 16 |
    //    static_cast<unsigned char>(buffer[1]) << 8 |
    //    static_cast<unsigned char>(buffer[0]));
    //return a;

    int Int32 = 0;
    Int32 = (Int32 << 8) + buffer[3];
    Int32 = (Int32 << 8) + buffer[2];
    Int32 = (Int32 << 8) + buffer[1];
    Int32 = (Int32 << 8) + buffer[0];
    return Int32;
}

unsigned int reversedBytes(uint8_t* byte) {
    unsigned int result = 0;
    for (int i = sizeof(byte) - 1; i >= 0; i--)
        result = (result << 8) | byte[i];

    return result;
}
