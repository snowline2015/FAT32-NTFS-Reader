#include "FAT32.h"
BOOTSECTORFAT32 bs32;

int ReadSectorFAT32(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    RDETFAT32 rdet32;
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

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);    //Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
        return 1;
    }
    else
    {
        printf("Succeed in reading bootsector !\n");

        memset(&bs32, 0, sizeof(bs32));

        memcpy(&bs32.JUMP, sector, sizeof(bs32.JUMP));
        memcpy(&bs32.OEM, sector + 3, sizeof(bs32.OEM));
        memcpy(&bs32.BytePerSector, sector + 11, sizeof(bs32.BytePerSector));
        memcpy(&bs32.SectorPerCluster, sector + 13, sizeof(bs32.SectorPerCluster));
        memcpy(&bs32.ReservedSector, sector + 14, sizeof(bs32.ReservedSector));
        memcpy(&bs32.FatNum, sector + 16, sizeof(bs32.FatNum));
        memcpy(&bs32.EntryRDET, sector + 17, sizeof(bs32.EntryRDET));
        memcpy(&bs32.LowNumberSectors, sector + 19, sizeof(bs32.LowNumberSectors));
        memcpy(&bs32.DeviceType, sector + 21, sizeof(bs32.DeviceType));
        memcpy(&bs32.SectorPerFat16, sector + 22, sizeof(bs32.SectorPerFat16));
        memcpy(&bs32.SectorPerTrack, sector + 24, sizeof(bs32.SectorPerTrack));
        memcpy(&bs32.HeadPerDisk, sector + 26, sizeof(bs32.HeadPerDisk));

        memcpy(&bs32.NumberHiddenSectors, sector + 28, sizeof(bs32.NumberHiddenSectors));
        memcpy(&bs32.HighNumberSectors, sector + 32, sizeof(bs32.HighNumberSectors));
        memcpy(&bs32.SectorPerFat32, sector + 36, sizeof(bs32.SectorPerFat32));
        memcpy(&bs32.Bit8Flag, sector + 40, sizeof(bs32.Bit8Flag));
        memcpy(&bs32.FAT32Ver, sector + 42, sizeof(bs32.FAT32Ver));
        memcpy(&bs32.FirstRDETCluster, sector + 44, sizeof(bs32.FirstRDETCluster));
        memcpy(&bs32.AddiInfoSector, sector + 48, sizeof(bs32.AddiInfoSector));
        memcpy(&bs32.BackupSector, sector + 50, sizeof(bs32.BackupSector));
        memcpy(&bs32.LaterVerReserved, sector + 52, sizeof(bs32.LaterVerReserved));

        memcpy(&bs32.PhysicDisk, sector + 64, sizeof(bs32.PhysicDisk));
        memcpy(&bs32.Reserved, sector + 65, sizeof(bs32.Reserved));
        memcpy(&bs32.Signature, sector + 66, sizeof(bs32.Signature));
        memcpy(&bs32.VolumeSerial, sector + 67, sizeof(bs32.VolumeSerial));
        memcpy(&bs32.VolumeLabel, sector + 71, sizeof(bs32.VolumeLabel));
        memcpy(&bs32.FATID, sector + 82, sizeof(bs32.FATID));
        memcpy(&bs32.BootProgram, sector + 90, sizeof(bs32.BootProgram));
        memcpy(&bs32.EndSignature, sector + 510, sizeof(bs32.EndSignature));

        memset(&bs32, 0, sizeof(bs32));
    }
    
    //distance from begine until Root Directory or content of partetion
    ULONG distance = bs32.ReservedSector + bs32.FatNum * bs32.SectorPerFat32;
    distance *= bs32.BytePerSector; //convert distance number to bytes value

    SetFilePointer(device, distance, NULL, FILE_BEGIN);//set pointer to root directory begine or begine of data

    int clusterSize = bs32.BytePerSector * bs32.SectorPerCluster; //cluster size 
    int NumberOfEntries = clusterSize / sizeof(RDETFAT32); //number of record inside cluster
    RDETFAT32* root = new RDETFAT32[NumberOfEntries];//descripe the partetion
    if (!ReadFile(device, (BYTE*)root, clusterSize, &bytesRead, 0)) 
    {
        printf("ReadFile: %u\n", GetLastError());
        return 1;
    }
    else {
        DWORD clusterNumber;
        for (int i = 0; i < NumberOfEntries; i++)
        {
            if (root[i].FileName[0] == 0)   // entry trong
                break;
            if (root[i].FileName[0] == 0xE5)    // tap tin da bi xoa
                continue;
            if ((root[i].FileAttributes & 0xF) == 0xF)
                continue;
            for (int j = 0; j < 8; j++)
                cout << root[i].FileName[j];
            if ((root[i].FileAttributes & 0x10) != 0x10) {
                cout << ".";
                for (int j = 8; j < 11; j++)
                    cout << root[i].FileName[j];
            }
            if ((root[i].FileAttributes & 0x10) == 0x10) {
                cout << "\t<Folder>";
            }
            else {

                cout << "\t<File>";
            }
            clusterNumber = root[i].FirstClusterHigh << 16;
            clusterNumber |= root[i].FirstClusterLow;
            cout << "\t" << root[i].FileSize << "bytes" << "\t" << clusterNumber << "cluster" << endl;
        }
    }

    CloseHandle(device);
        
    return 0;
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

/*
int ReadRDETFAT32(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
    RDETFAT32 bs32;
    int retCode = 0;
    DWORD bytesRead, dwFilePointer;
    HANDLE device = NULL;

    //if (!AfxWinInit(::GetModuleHandle(NULL), NULL,
    //    ::GetCommandLine(), 0))
    //{
    //    // TODO: change error code to suit your needs
    //    cerr << _T("Fatal Error: MFC initialization failed") << endl;
    //    retCode = 1;
    //}

    memset(&sector, 0, 512);

    device = CreateFile(drive,    // Drive to open
        GENERIC_READ,           // Access mode
        FILE_SHARE_READ,        // Share Mode
        NULL,                   // Security Descriptor
        OPEN_EXISTING,          // How to create
        0,                      // File attributes
        NULL);                  // Handle to template

    if (device == INVALID_HANDLE_VALUE) // Open Error
    {
        printf("CreateFile: %u\n", GetLastError());
        return 1;
    }

    SetFilePointer(device, (512 * 19), NULL, FILE_BEGIN);   //Set a Point to Read

    if (device != NULL)
    {
        dwFilePointer = SetFilePointer(device, (512 * 2), NULL, FILE_BEGIN);
        // Test for failure
        if (dwFilePointer != INVALID_SET_FILE_POINTER)
        {
            int iSector = 19;
            BOOL bNoEntry = FALSE;
            // Iterate through root directory sectors
            do
            {
                if (!ReadFile(device, sector, 512, &bytesRead, NULL))
                {
                    printf("ReadFile: %u\n", GetLastError());
                }
                else
                {
                    BYTE* psector = sector;
                    // Iterate through 32 byte entries
                    for (int i = 0; i < (512 / 32); i++)
                    {
                        memcpy(&bs32, psector, 32);

                        // No entry beyond this used.
                        if (bs32.FileName[0] == 0x00)
                        {
                            // Stop iteration
                            bNoEntry = TRUE;
                            break;
                        }
                        else
                        {
                            // This if loops checks for file deletion
                            // status
                            if (bs32.FileName[0] == 0xE5)
                            {
                                printf("First character of file after deletion :0x % x\n", bs32.FileName[0]);
                                printf("File status: Deleted.\n");
                            }

                            printf("File Name: %s\n", bs32.FileName);
                            if (bs32.FileAttributes[0] & 0x01)
                                printf("File Attribute    : Read Only File\n");
                            if (bs32.FileAttributes[0] & 0x02)
                                printf("File Attribute    : Hidden File\n");
                            if (bs32.FileAttributes[0] & 0x04)
                                printf("File Attribute    : System File\n");
                            if (bs32.FileAttributes[0] & 0x08)
                                printf("File Attribute    : Volume Label\n");
                            if (bs32.FileAttributes[0] & 0x0f)
                                printf("File Attribute    : Long File Name\n");
                            if (bs32.FileAttributes[0] & 0x10)
                                printf("File Attribute    : Directory\n");
                            if (bs32.FileAttributes[0] & 0x20)
                                printf("File Attribute    : Archive\n");

                            WORD nYear = (*bs32.CreatedDate >> 9);
                            WORD nMonth = (*bs32.CreatedDate << 7);
                            nMonth = nMonth >> 12;
                            WORD nDay = (*bs32.CreatedDate << 11);
                            nDay = nDay >> 11;

                            printf("Create Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

                            nYear = (*bs32.LastModifiedDate >> 9);
                            nMonth = (*bs32.LastModifiedDate << 7);
                            nMonth = nMonth >> 12;
                            nDay = (*bs32.LastModifiedDate << 11);
                            nDay = nDay >> 11;

                            printf("Modification Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

                            nYear = (*bs32.LastAccessedDate >> 9);
                            nMonth = (*bs32.LastAccessedDate << 7);
                            nMonth = nMonth >> 12;
                            nDay = (*bs32.LastAccessedDate << 11);
                            nDay = nDay >> 11;

                            printf("Accessed Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));
                            printf("Start Cluster Address: %d\n", bs32.FirstClusterLow);
                            printf("File Size: %d bytes\n", bs32.Size);

                            psector += 32;
                        }    // End of else
                    }

                    if (bNoEntry)
                        break;
                    else
                    {
                        iSector += 1;
                    }
                }
            } while (iSector <= 33);
        }

        CloseHandle(device);
    }

    return 0;
}
*/