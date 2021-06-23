#include "FAT32.h"
#include "NTFS.h"


int main(int argc, char** argv)
{
    BYTE sector[512];
    DWORD ok = 0x10;
    ReadSectorFAT32(L"\\\\.\\E:", 0, sector);
    ReadRDETFAT32(L"\\\\.\\E:");
    //ReadSRDETFAT32(L"\\\\.\\E:", ok);
    return 0;
}