#include "Header.h"


int main(int argc, char** argv)
{
    BYTE sector[512];
    ReadSector(L"\\\\.\\E:", 0, sector);
    return 0;
}