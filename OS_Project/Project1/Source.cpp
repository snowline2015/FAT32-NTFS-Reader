#include "FAT32.h"
#include "NTFS.h"

int main(int argc, char** argv)
{
    BYTE sector[512];
    //ReadBootSectorFAT32(L"\\\\.\\E:", 0, sector);
    //ReadRDETFAT32(L"\\\\.\\E:");

    ReadBootSectorNTFS(L"\\\\.\\D:", 0, sector);
    //NTFSParse(L"\\\\.\\D:");

    return 0;
}