#include "NTFS.h"
BOOTSECTORNTFS ntfs;

int ReadSectorNTFS(LPCWSTR  drive, int readPoint, BYTE sector[512]) {
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

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);    //Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
        return 1;
    }
    else
    {
        memset(&ntfs, 0, 512);

        memcpy(&ntfs.JUMP, sector, sizeof(ntfs.JUMP));
        memcpy(&ntfs.OEM, sector + 3, sizeof(ntfs.OEM));

        memcpy(&ntfs.BytePerSector, sector + 11, sizeof(ntfs.BytePerSector));
        memcpy(&ntfs.SectorPerCluster, sector + 13, sizeof(ntfs.SectorPerCluster));
        memcpy(&ntfs.ReservedSector, sector + 14, sizeof(ntfs.ReservedSector));
        memcpy(&ntfs.Unknown1, sector + 16, sizeof(ntfs.Unknown1));
        memcpy(&ntfs.Unknown2, sector + 19, sizeof(ntfs.Unknown2));
        memcpy(&ntfs.MediaDescriptor, sector + 21, sizeof(ntfs.MediaDescriptor));
        memcpy(&ntfs.Unknown3, sector + 22, sizeof(ntfs.Unknown3));
        memcpy(&ntfs.SectorPerTrack, sector + 24, sizeof(ntfs.SectorPerTrack));
        memcpy(&ntfs.HeadPerDisk, sector + 26, sizeof(ntfs.HeadPerDisk));
        memcpy(&ntfs.NumberHiddenSectors, sector + 28, sizeof(ntfs.NumberHiddenSectors));
        memcpy(&ntfs.Unknown4, sector + 32, sizeof(ntfs.Unknown4)); 

        memcpy(&ntfs.Unknown5, sector + 36, sizeof(ntfs.Unknown5));
        memcpy(&ntfs.TotalSectors, sector + 40, sizeof(ntfs.TotalSectors));
        memcpy(&ntfs.$MFTCluster, sector + 48, sizeof(ntfs.$MFTCluster));
        memcpy(&ntfs.$MFTMirrCluster, sector + 56, sizeof(ntfs.$MFTMirrCluster));
        memcpy(&ntfs.ClustersPerFileRecordSegment, sector + 64, sizeof(ntfs.ClustersPerFileRecordSegment));
        memcpy(&ntfs.Unknown6, sector + 65, sizeof(ntfs.Unknown6));
        memcpy(&ntfs.ClustersPerIndexBuffer, sector + 68, sizeof(ntfs.ClustersPerIndexBuffer));
        memcpy(&ntfs.Unknown7, sector + 69, sizeof(ntfs.Unknown7));
        memcpy(&ntfs.VolumeSerial, sector + 72, sizeof(ntfs.VolumeSerial));
        memcpy(&ntfs.Checksum, sector + 80, sizeof(ntfs.Checksum));

        memcpy(&ntfs.Bootstrap, sector + 84, sizeof(ntfs.Bootstrap));
        memcpy(&ntfs.EndOfSectorMarker, sector + 510, sizeof(ntfs.EndOfSectorMarker));
    }
    CloseHandle(device);
    return 0;
}