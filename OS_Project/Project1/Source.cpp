#include "Header.h"


int main(int argc, char** argv)
{
    BYTE sector[512];
    ReadSector(L"\\\\.\\C:", 0, sector);
    return 0;
}