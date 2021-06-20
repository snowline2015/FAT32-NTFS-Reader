#include "Header.h"

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    BootSectorFAT32 fat32;
    int retCode = 0;
    DWORD bytesRead, bytesWrite;
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

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        memcpy(fat32.OEM, sector, sizeof(fat32.OEM));
        memcpy(fat32.Version, sector + 3, sizeof(fat32.Version));
        memcpy(fat32.BytePerSector, sector + 11, sizeof(fat32.BytePerSector));
        memcpy(fat32.SectorPerCluster, sector + 13, sizeof(fat32.SectorPerCluster));
        memcpy(fat32.ReservedSector, sector + 14, sizeof(fat32.ReservedSector));
        memcpy(fat32.FatNum, sector + 16, sizeof(fat32.FatNum));
        memcpy(fat32.EntryRDET, sector + 17, sizeof(fat32.EntryRDET));
        memcpy(fat32.SectorVol, sector + 19, sizeof(fat32.SectorVol));
        memcpy(fat32.DeviceType, sector + 21, sizeof(fat32.DeviceType));
        memcpy(fat32.SectorPerFat, sector + 22, sizeof(fat32.SectorPerFat));
        memcpy(fat32.SectorPerTrack, sector + 24, sizeof(fat32.SectorPerTrack));
        memcpy(fat32.HeadPerDisk, sector + 26, sizeof(fat32.HeadPerDisk));

        memcpy(fat32.Distance, sector + 28, sizeof(fat32.Distance));
        memcpy(fat32.VolSize, sector + 32, sizeof(fat32.VolSize));
        memcpy(fat32.SizePerFAT, sector + 36, sizeof(fat32.SizePerFAT));
        memcpy(fat32.Bit8On, sector + 40, sizeof(fat32.Bit8On));
        memcpy(fat32.FAT32Ver, sector + 42, sizeof(fat32.FAT32Ver));
        memcpy(fat32.FirstRDETCluster, sector + 44, sizeof(fat32.FirstRDETCluster));
        memcpy(fat32.AddiInfoSector, sector + 48, sizeof(fat32.AddiInfoSector));
        memcpy(fat32.SaveBootSector, sector + 50, sizeof(fat32.SaveBootSector));
        memcpy(fat32.DanhRieng, sector + 52, sizeof(fat32.DanhRieng));

        memcpy(fat32.PhysicDisk, sector + 64, sizeof(fat32.PhysicDisk));
        memcpy(fat32.Danhrieng, sector + 65, sizeof(fat32.Danhrieng));
        memcpy(fat32.KiHieuNhanDienOS, sector + 66, sizeof(fat32.KiHieuNhanDienOS));
        memcpy(fat32.SerialNumber, sector + 67, sizeof(fat32.SerialNumber));
        memcpy(fat32.VolumeLabel, sector + 71, sizeof(fat32.VolumeLabel));
        memcpy(fat32.FATType, sector + 82, sizeof(fat32.FATType));
        memcpy(fat32.BootProgram, sector + 90, sizeof(fat32.BootProgram));
        memcpy(fat32.DauHieuKetThuc, sector + 510, sizeof(fat32.DauHieuKetThuc));
    }
        
    int value;
    value = reversedBytes(fat32.VolSize, sizeof(fat32.VolSize));
    cout << value << endl;

    printf("Bootstrap: %s\nSectors per cluster: %d", fat32.OEM, fat32.VolSize);
}

//int buffToInteger(byte* buffer)
//{
//    //int a = static_cast<int>(static_cast<unsigned char>(buffer[3]) << 24 |
//    //    static_cast<unsigned char>(buffer[2]) << 16 |
//    //    static_cast<unsigned char>(buffer[1]) << 8 |
//    //    static_cast<unsigned char>(buffer[0]));
//    //return a;
//
//    int Int32 = 0;
//    Int32 = (Int32 << 8) + buffer[3];
//    Int32 = (Int32 << 8) + buffer[2];
//    Int32 = (Int32 << 8) + buffer[1];
//    Int32 = (Int32 << 8) + buffer[0];
//    return Int32;
//}

unsigned int reversedBytes(uint8_t* byte, unsigned int count) {
    unsigned int result = 0;
    for (int i = count - 1; i >= 0; i--) 
        result = (result << 8) | byte[i];
    
    return result;
}