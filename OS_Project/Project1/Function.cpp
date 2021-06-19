#include "Header.h"

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    BootSectorFAT32 ok;
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    bool success = ReadFile(device, sector, 512, &bytesRead, NULL);
    if (!success)
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        memcpy(ok.OEM, sector, sizeof(ok.OEM));
        memcpy(ok.Version, sector + 3, sizeof(ok.Version));
        memcpy(ok.BytePerSector, sector + 11, sizeof(ok.BytePerSector));
        memcpy(ok.SectorPerCluster, sector + 13, sizeof(ok.SectorPerCluster));
        memcpy(ok.ReservedSector, sector + 14, sizeof(ok.ReservedSector));
        memcpy(ok.FatNum, sector + 16, sizeof(ok.FatNum));
        memcpy(ok.EntryRDET, sector + 17, sizeof(ok.EntryRDET));
        memcpy(ok.SectorVol, sector + 19, sizeof(ok.SectorVol));
        memcpy(ok.DeviceType, sector + 21, sizeof(ok.DeviceType));
        memcpy(ok.SectorPerFat, sector + 22, sizeof(ok.SectorPerFat));
        memcpy(ok.SectorPerTrack, sector + 24, sizeof(ok.SectorPerTrack));
        memcpy(ok.HeadPerDisk, sector + 26, sizeof(ok.HeadPerDisk));

        memcpy(ok.Distance, sector + 28, sizeof(ok.Distance));
        memcpy(ok.VolSize, sector + 32, sizeof(ok.VolSize));
        memcpy(ok.SizePerFAT, sector + 36, sizeof(ok.SizePerFAT));
        memcpy(ok.Bit8On, sector + 40, sizeof(ok.Bit8On));
        memcpy(ok.FAT32Ver, sector + 42, sizeof(ok.FAT32Ver));
        memcpy(ok.FirstRDETCluster, sector + 44, sizeof(ok.FirstRDETCluster));
        memcpy(ok.AddiInfoSector, sector + 48, sizeof(ok.AddiInfoSector));
        memcpy(ok.SaveBootSector, sector + 50, sizeof(ok.SaveBootSector));
        memcpy(ok.DanhRieng, sector + 52, sizeof(ok.DanhRieng));

        memcpy(ok.PhysicDisk, sector + 64, sizeof(ok.PhysicDisk));
        memcpy(ok.Danhrieng, sector + 65, sizeof(ok.Danhrieng));
        memcpy(ok.KiHieuNhanDienOS, sector + 66, sizeof(ok.KiHieuNhanDienOS));
        memcpy(ok.SerialNumber, sector + 67, sizeof(ok.SerialNumber));
        memcpy(ok.VolumeLabel, sector + 71, sizeof(ok.VolumeLabel));
        memcpy(ok.FATType, sector + 82, sizeof(ok.FATType));
        memcpy(ok.BootProgram, sector + 90, sizeof(ok.BootProgram));
        memcpy(ok.DauHieuKetThuc, sector + 510, sizeof(ok.DauHieuKetThuc));
        printf("Success!\n");
    }
}