#include "FAT32.h"
BOOTSECTORFAT32 bs32;

int ReadSectorFAT32(LPCWSTR  drive, int readPoint, BYTE sector[512])
{
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
        memset(&bs32, 0, 512);

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


int ReadRDETFAT32(LPCWSTR drive)
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;
    LongFileDir LFD;
    string SubName = "";

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

    // SB + SF * NF
    ULONG distance = bs32.ReservedSector + bs32.FatNum * bs32.SectorPerFat32;
    distance *= bs32.BytePerSector; // convert to bytes

    SetFilePointer(device, distance, NULL, FILE_BEGIN);

    int clusterSize = bs32.BytePerSector * bs32.SectorPerCluster;   // cluster size 
    int NumberOfEntries = clusterSize / sizeof(RDETFAT32);  // number of record inside cluster
    RDETFAT32* root = new RDETFAT32[NumberOfEntries];   // descripe the partition

    if (!ReadFile(device, (BYTE*)root, clusterSize, &bytesRead, 0))
    {
        printf("ReadFile: %u\n", GetLastError());
        return 1;
    }
    else {
        for (int i = 0; i < NumberOfEntries; i++)
        {
            // Xet entry chinh
            if (root[i].FileName[0] == 0x00)
                break;
            if (root[i].FileName[0] == 0xE5)   
                continue;
                
            // Xet entry phu
            if (root[i].FileAttributes == 0x0F) {
                memset(&LFD, 0, sizeof(LFD));
                memcpy(&LFD, &root[i], sizeof(LFD));
                
                string temp = "";
                for (int j = 0; j < 3; j++) {
                    if (j == 0) {
                        for (int k = 0; k < sizeof(LFD.Name1); k++)
                            if (32 <= (int)LFD.Name1[k] && (int)LFD.Name1[k] <= 126)
                                temp += LFD.Name1[k];
                    }
                    else if (j == 1) {
                        for (int k = 0; k < sizeof(LFD.Name2); k++)
                            if (32 <= (int)LFD.Name2[k] && (int)LFD.Name2[k] <= 126)
                                temp += LFD.Name2[k];
                    }
                    else {
                        for (int k = 0; k < sizeof(LFD.Name3); k++)
                            if (32 <= (int)LFD.Name3[k] && (int)LFD.Name3[k] <= 126)
                                temp += LFD.Name3[k];
                    }
                }

                SubName.insert(0, temp);
                continue;
                
            }

            if (SubName != "") {
                cout << SubName;
                SubName = "";
            }
            else {
                for (int j = 0; j < 8; j++)
                    cout << root[i].FileName[j];
                if ((root[i].FileAttributes & 0x10) != 0x10) {
                    cout << ".";
                    for (int j = 8; j < 11; j++)
                        cout << root[i].FileName[j];
                }
            }

            if (root[i].FileAttributes == 0x01)
                printf("\t<Read Only>\n");
            if (root[i].FileAttributes == 0x02)
                printf("\t<Hidden>\n");
            if (root[i].FileAttributes == 0x04)
                printf("\t<System>\n");
            if (root[i].FileAttributes == 0x08)
                printf("\t<Volume Label>\n");
            if (root[i].FileAttributes == 0x10)
                printf("\t<Directory>\n");
            if (root[i].FileAttributes == 0x20)
                printf("\t<Archive>\n");

            WORD nYear = (root[i].CreatedDate >> 9);
            WORD nMonth = (root[i].CreatedDate << 7);
            nMonth = nMonth >> 12;
            WORD nDay = (root[i].CreatedDate << 11);
            nDay = nDay >> 11;
            printf("\tCreate Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

            nYear = (root[i].LastModifiedDate >> 9);
            nMonth = (root[i].LastModifiedDate << 7);
            nMonth = nMonth >> 12;
            nDay = (root[i].LastModifiedDate << 11);
            nDay = nDay >> 11;
            printf("\tModification Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

            WORD nHour = (root[i].LastModifiedHour >> 11);
            WORD nMin = (root[i].LastModifiedHour << 5);
            nMin = nMin >> 10;
            WORD nSec = (root[i].LastModifiedHour << 11);
            nSec = nSec >> 11;
            printf("\tModification Hours: %02d:%02d:%02d\n", nHour, nMin, nSec/2);

            nYear = (root[i].LastAccessedDate >> 9);
            nMonth = (root[i].LastAccessedDate << 7);
            nMonth = nMonth >> 12;
            nDay = (root[i].LastAccessedDate << 11);
            nDay = nDay >> 11;
            printf("\tAccessed Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));
            
            DWORD clusterNumber = root[i].FirstClusterHigh << 16;
            clusterNumber |= root[i].FirstClusterLow;
            cout << "\t" << root[i].FileSize << "bytes\n" << "\t" << clusterNumber << "cluster\n" << endl;


            if (root[i].FileAttributes == 0x10) {
                std::cout << "Sub\n{\n" << std::endl;
                ReadSRDETFAT32(drive, device, clusterNumber);
                std::cout << "}\n" << std::endl;
            }
        }
    }
    delete[] root;
    CloseHandle(device);
    return 0;
}

int ReadSRDETFAT32(LPCWSTR drive, HANDLE device, DWORD cluster) {
    int retCode = 0;
    HANDLE CopyDevice = device;
    DWORD bytesRead;
    LongFileDir LFD;
    string SubName = "";    

    ULONG distance = bs32.ReservedSector + bs32.FatNum * bs32.SectorPerFat32 + (cluster - 2) * bs32.SectorPerCluster;
    distance *= bs32.BytePerSector; 

    int clusterSize = bs32.BytePerSector * bs32.SectorPerCluster;   // cluster size 
    int NumberOfEntries = clusterSize / sizeof(RDETFAT32);  // number of record inside cluster
    RDETFAT32* root = new RDETFAT32[NumberOfEntries];   // descripe the partition

    SetFilePointer(CopyDevice, distance, NULL, FILE_BEGIN);

    if (!ReadFile(CopyDevice, (BYTE*)root, clusterSize, &bytesRead, 0))
    {
        printf("ReadFile: %u\n", GetLastError());
        return 1;
    }
    else {
        for (int i = 0; i < NumberOfEntries; i++)
        {
            // Xet entry chinh
            if (root[i].FileName[0] == 0x00)
                break;
            if (root[i].FileName[0] == 0xE5)
                continue;
            if (root[i].FileName[0] == 0x2E)
                continue;

            // Xet entry phu
            if (root[i].FileAttributes == 0x0F) {
                memset(&LFD, 0, sizeof(LFD));
                memcpy(&LFD, &root[i], sizeof(LFD));

                string temp = "";
                for (int j = 0; j < 3; j++) {
                    if (j == 0) {
                        for (int k = 0; k < sizeof(LFD.Name1); k++)
                            if (32 <= (int)LFD.Name1[k] && (int)LFD.Name1[k] <= 126)
                                temp += LFD.Name1[k];
                    }
                    else if (j == 1) {
                        for (int k = 0; k < sizeof(LFD.Name2); k++)
                            if (32 <= (int)LFD.Name2[k] && (int)LFD.Name2[k] <= 126)
                                temp += LFD.Name2[k];
                    }
                    else {
                        for (int k = 0; k < sizeof(LFD.Name3); k++)
                            if (32 <= (int)LFD.Name3[k] && (int)LFD.Name3[k] <= 126)
                                temp += LFD.Name3[k];
                    }
                }

                SubName.insert(0, temp);
                continue;

            }

            if (SubName != "") {
                cout << SubName;
                SubName = "";
            }
            else {
                for (int j = 0; j < 8; j++)
                    cout << root[i].FileName[j];
                if ((root[i].FileAttributes & 0x10) != 0x10) {
                    cout << ".";
                    for (int j = 8; j < 11; j++)
                        cout << root[i].FileName[j];
                }
            }

            if (root[i].FileAttributes == 0x01)
                printf("\t<Read Only>\n");
            if (root[i].FileAttributes == 0x02)
                printf("\t<Hidden>\n");
            if (root[i].FileAttributes == 0x04)
                printf("\t<System>\n");
            if (root[i].FileAttributes == 0x08)
                printf("\t<Volume Label>\n");
            if (root[i].FileAttributes == 0x10)
                printf("\t<Directory>\n");
            if (root[i].FileAttributes == 0x20)
                printf("\t<Archive>\n");

            WORD nYear = (root[i].CreatedDate >> 9);
            WORD nMonth = (root[i].CreatedDate << 7);
            nMonth = nMonth >> 12;
            WORD nDay = (root[i].CreatedDate << 11);
            nDay = nDay >> 11;
            printf("\tCreate Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

            nYear = (root[i].LastModifiedDate >> 9);
            nMonth = (root[i].LastModifiedDate << 7);
            nMonth = nMonth >> 12;
            nDay = (root[i].LastModifiedDate << 11);
            nDay = nDay >> 11;
            printf("\tModification Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

            WORD nHour = (root[i].LastModifiedHour >> 11);
            WORD nMin = (root[i].LastModifiedHour << 5);
            nMin = nMin >> 10;
            WORD nSec = (root[i].LastModifiedHour << 11);
            nSec = nSec >> 11;
            printf("\tModification Hours: %02d:%02d:%02d\n", nHour, nMin, nSec / 2);

            nYear = (root[i].LastAccessedDate >> 9);
            nMonth = (root[i].LastAccessedDate << 7);
            nMonth = nMonth >> 12;
            nDay = (root[i].LastAccessedDate << 11);
            nDay = nDay >> 11;
            printf("\tAccessed Date: %d/%d/%d\n", nDay, nMonth, (nYear + 1980));

            DWORD clusterNumber = root[i].FirstClusterHigh << 16;
            clusterNumber |= root[i].FirstClusterLow;
            cout << "\t" << root[i].FileSize << "bytes\n" << "\t" << clusterNumber << "cluster\n" << endl;

            if (root[i].FileAttributes == 0x10) {
                std::cout << "Sub\n{\n" << std::endl;
                ReadSRDETFAT32(drive, CopyDevice, clusterNumber);
                std::cout << "}\n" << std::endl;
            }
        }
    }

    delete[] root;
    return 0;
}