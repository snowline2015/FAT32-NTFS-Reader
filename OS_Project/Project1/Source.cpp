#include "FAT32.h"
#include "NTFS.h"


int main(int argc, char** argv)
{
    BYTE sector[512];
    ReadSectorFAT32(L"\\\\.\\E:", 0, sector);
    ReadRDETFAT32(L"\\\\.\\E:");

    //ReadSectorNTFS(L"\\\\.\\D:", 0, sector);
    return 0;
}