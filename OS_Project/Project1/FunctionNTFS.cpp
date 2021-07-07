#include "NTFS.h"
#include "GUI.h"

#define STB_DS_IMPLEMENTATION
#include "./Sublib/stb_ds.h"

File* files = 0;

BOOTSECTORNTFS bootSector;

uint8_t mftFile[MFT_FILE_SIZE];
uint8_t mftBuffer[MFT_FILES_PER_BUFFER * MFT_FILE_SIZE];

int Read(LPCWSTR driveLabel, void* buffer, uint64_t from, uint64_t count)
{
    int retCode = 0;
    DWORD bytesRead;
    HANDLE device = NULL;

    device = CreateFile(driveLabel,    // Drive to open
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

    LONG high = from >> 32;
    SetFilePointer(device, from & 0xFFFFFFFF, &high, FILE_BEGIN);
    ReadFile(device, buffer, count, &bytesRead, NULL);
    assert(bytesRead == count);

    CloseHandle(device);
    return 0;
}

char* DuplicateName(wchar_t* name, size_t nameLength) {
    static char* allocationBlock = nullptr;
    static size_t bytesRemaining = 0;

    size_t bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, name, nameLength, NULL, 0, NULL, NULL) + 1;

    if (bytesRemaining < bytesNeeded) {
        allocationBlock = (char*)malloc((bytesRemaining = 16 * 1024 * 1024));
    }

    char* buffer = allocationBlock;
    buffer[bytesNeeded - 1] = 0;
    WideCharToMultiByte(CP_UTF8, 0, name, nameLength, allocationBlock, bytesNeeded, NULL, NULL);

    bytesRemaining -= bytesNeeded;
    allocationBlock += bytesNeeded;

    return buffer;
}

int NTFSParse(LPCWSTR DriveLabel) {
    Read(DriveLabel, &bootSector, 0, 512);

    uint64_t bytesPerCluster = bootSector.BytePerSector * bootSector.SectorPerCluster;

    Read(DriveLabel, &mftFile, bootSector.$MFTCluster * bytesPerCluster, MFT_FILE_SIZE);

    FileRecordHeader* fileRecord = (FileRecordHeader*)mftFile;
    AttributeHeader* attribute = (AttributeHeader*)(mftFile + fileRecord->firstAttributeOffset);
    NonResidentAttributeHeader* dataAttribute = nullptr;
    uint64_t approximateRecordCount = 0;
    assert(fileRecord->magic == 0x454C4946);

    while (true) {
        if (attribute->attributeType == 0x80) {
            dataAttribute = (NonResidentAttributeHeader*)attribute;
        }
        else if (attribute->attributeType == 0xB0) {
            approximateRecordCount = ((NonResidentAttributeHeader*)attribute)->attributeSize * 8;
        }
        else if (attribute->attributeType == 0xFFFFFFFF) {
            break;
        }

        attribute = (AttributeHeader*)((uint8_t*)attribute + attribute->length);
    }

    assert(dataAttribute);
    RunHeader* dataRun = (RunHeader*)((uint8_t*)dataAttribute + dataAttribute->dataRunsOffset);
    uint64_t clusterNumber = 0, recordsProcessed = 0;

    while (((uint8_t*)dataRun - (uint8_t*)dataAttribute) < dataAttribute->length && dataRun->lengthFieldBytes) {
        uint64_t length = 0, offset = 0;

        for (int i = 0; i < dataRun->lengthFieldBytes; i++) {
            length |= (uint64_t)(((uint8_t*)dataRun)[1 + i]) << (i * 8);
        }

        for (int i = 0; i < dataRun->offsetFieldBytes; i++) {
            offset |= (uint64_t)(((uint8_t*)dataRun)[1 + dataRun->lengthFieldBytes + i]) << (i * 8);
        }

        if (offset & ((uint64_t)1 << (dataRun->offsetFieldBytes * 8 - 1))) {
            for (int i = dataRun->offsetFieldBytes; i < 8; i++) {
                offset |= (uint64_t)0xFF << (i * 8);
            }
        }

        clusterNumber += offset;
        dataRun = (RunHeader*)((uint8_t*)dataRun + 1 + dataRun->lengthFieldBytes + dataRun->offsetFieldBytes);

        uint64_t filesRemaining = length * bytesPerCluster / MFT_FILE_SIZE;
        uint64_t positionInBlock = 0;

        while (filesRemaining) {
            //fprintf(stderr, "%d%% ", (int)(recordsProcessed * 100 / approximateRecordCount));

            uint64_t filesToLoad = MFT_FILES_PER_BUFFER;
            if (filesRemaining < MFT_FILES_PER_BUFFER) filesToLoad = filesRemaining;
            Read(DriveLabel, &mftBuffer, clusterNumber * bytesPerCluster + positionInBlock, filesToLoad * MFT_FILE_SIZE);
            positionInBlock += filesToLoad * MFT_FILE_SIZE;
            filesRemaining -= filesToLoad;

            for (int i = 0; i < filesToLoad; i++) {
                // Even on an SSD, processing the file records takes only a fraction of the time to read the data,
                // so there's not much point in multithreading this.

                FileRecordHeader* fileRecord = (FileRecordHeader*)(mftBuffer + MFT_FILE_SIZE * i);
                recordsProcessed++;

                if (!fileRecord->inUse) continue;

                AttributeHeader* attribute = (AttributeHeader*)((uint8_t*)fileRecord + fileRecord->firstAttributeOffset);
                assert(fileRecord->magic == 0x454C4946);

                while ((uint8_t*)attribute - (uint8_t*)fileRecord < MFT_FILE_SIZE) {
                    if (attribute->attributeType == 0x30) {
                        FileNameAttributeHeader* fileNameAttribute = (FileNameAttributeHeader*)attribute;

                        if (fileNameAttribute->namespaceType != 2 && !fileNameAttribute->nonResident) {
                            File file = {};
                            file.parent = fileNameAttribute->parentRecordNumber;
                            file.name = DuplicateName(fileNameAttribute->fileName, fileNameAttribute->fileNameLength);

                            if (file.name[0] != '$' && file.name[0] != '.') {
                                std::cout << file.name << std::endl;

                                SYSTEMTIME stSystemTime, stLocalTime;
                                FILETIME fileTime;
                                TIME_ZONE_INFORMATION tZone;
                                GetTimeZoneInformation(&tZone);

                                fileTime.dwHighDateTime = fileNameAttribute->creationTime >> 32;
                                fileTime.dwLowDateTime = fileNameAttribute->creationTime & 0xFFFFFFFF;
                                if (FileTimeToSystemTime(&fileTime, &stSystemTime) && SystemTimeToTzSpecificLocalTime(&tZone, &stSystemTime, &stLocalTime))
                                {
                                    std::cout << "Creation Time: " << dayOfWeek(stSystemTime.wDayOfWeek);
                                    printf(", %d/%d/%d, %02d:%02d:%d\n", stLocalTime.wDay, stLocalTime.wMonth, stLocalTime.wYear, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wMilliseconds);
                                }
                                fileTime.dwHighDateTime = fileNameAttribute->modificationTime >> 32;
                                fileTime.dwLowDateTime = fileNameAttribute->modificationTime & 0xFFFFFFFF;
                                if (FileTimeToSystemTime(&fileTime, &stSystemTime) && SystemTimeToTzSpecificLocalTime(&tZone, &stSystemTime, &stLocalTime))
                                {
                                    std::cout << "Modification Time: " << dayOfWeek(stSystemTime.wDayOfWeek);
                                    printf(", %d/%d/%d, %02d:%02d:%d\n", stLocalTime.wDay, stLocalTime.wMonth, stLocalTime.wYear, stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wMilliseconds);
                                }

                                std::cout << "Size on disk: " << fileNameAttribute->allocatedSize << " bytes\n";
                                std::cout << std::endl;
                            }

                            uint64_t oldLength = arrlenu(files);

                            if (fileRecord->recordNumber >= oldLength) {
                                arrsetlen(files, fileRecord->recordNumber + 1);
                                memset(files + oldLength, 0, sizeof(File) * (fileRecord->recordNumber - oldLength));
                            }

                            files[fileRecord->recordNumber] = file;
                        }
                    }
                    else if (attribute->attributeType == 0xFFFFFFFF) {
                        break;
                    }

                    attribute = (AttributeHeader*)((uint8_t*)attribute + attribute->length);
                }
            }
        }
    }

    //fprintf(stderr, "\nFound %lld files.\n", arrlen(files));

    return 0;
}

