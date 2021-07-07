#ifndef NTFS_H
#define NTFS_H
#define NDEBUG

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <windows.h>
#include <string>
#include <chrono>
#include "atlstr.h"
#include "atlbase.h"
#include "comutil.h"

using namespace std::chrono;

#pragma pack(push,1)

struct BOOTSECTORNTFS {
    uint8_t     JUMP[3];
    char        OEM[8];

    // BPB
    uint16_t    BytePerSector;
    uint8_t     SectorPerCluster;
    uint16_t    ReservedSector;
    uint8_t     Unknown1[3];
    uint16_t    Unknown2;
    uint8_t     MediaDescriptor;
    uint16_t    Unknown3;
    uint16_t    SectorPerTrack;
    uint16_t    HeadPerDisk;
    uint32_t    NumberHiddenSectors;
    uint32_t    Unknown4;

    // Extended BPB
    uint32_t    Unknown5;
    uint64_t    TotalSectors;
    uint64_t    $MFTCluster;
    uint64_t    $MFTMirrCluster;
    uint8_t     BytesPerFileRecordSegment;      // > 0 -> cluster || < 0 : Ex: -10 -> 2^10 bytes = 1024 bytes
    uint8_t     Unknown6[3];
    uint8_t     BytesPerIndexBuffer;            // Like above           
    uint8_t     Unknown7[3];
    uint64_t    VolumeSerial;
    uint32_t    Checksum;

    uint8_t     Bootstrap[426];
    uint16_t    EndOfSectorMarker;
};

struct FileRecordHeader {
    uint32_t    signature;
    uint16_t    updateSequenceOffset;
    uint16_t    updateSequenceSize;
    uint64_t    logSequence;
    uint16_t    sequenceNumber;
    uint16_t    hardLinkCount;
    uint16_t    firstAttributeOffset;
    uint16_t    inUse : 1;
    uint16_t    isDirectory : 1;
    uint32_t    usedSize;
    uint32_t    allocatedSize;
    uint64_t    fileReference;
    uint16_t    nextAttributeID;
    uint16_t    unused;
    uint32_t    recordNumber;
};

struct AttributeHeader {
    uint32_t    attributeType;
    uint32_t    length;
    uint8_t     nonResident;
    uint8_t     nameLength;
    uint16_t    nameOffset;
    uint16_t    flags;
    uint16_t    attributeID;
};

struct ResidentAttributeHeader : AttributeHeader {
    uint32_t    attributeLength;
    uint16_t    attributeOffset;
    uint8_t     indexed;
    uint8_t     unused;
};

struct FileNameAttributeHeader : ResidentAttributeHeader {
    uint64_t    parentRecordNumber : 48;
    uint64_t    sequenceNumber : 16;
    uint64_t    creationTime;
    uint64_t    modificationTime;
    uint64_t    metadataModificationTime;
    uint64_t    readTime;
    uint64_t    allocatedSize;
    uint64_t    realSize;
    uint32_t    flags;
    uint32_t    repase;
    uint8_t     fileNameLength;
    uint8_t     namespaceType;
    wchar_t     fileName[1];
};

struct NonResidentAttributeHeader : AttributeHeader {
    uint64_t    firstCluster;
    uint64_t    lastCluster;
    uint16_t    dataRunsOffset;
    uint16_t    compressionUnit;
    uint32_t    unused;
    uint64_t    attributeAllocated;
    uint64_t    attributeSize;
    uint64_t    streamDataSize;
};

struct RunHeader {
    uint8_t     lengthFieldBytes : 4;
    uint8_t     offsetFieldBytes : 4;
};

#pragma pack(pop)

struct File {
    uint64_t parent;
    char* name;
};

extern File* files;

extern BOOTSECTORNTFS bootSector;

#define MFT_FILE_SIZE (1024)
extern uint8_t mftFile[MFT_FILE_SIZE];

#define MFT_FILES_PER_BUFFER (65536)
extern uint8_t mftBuffer[MFT_FILES_PER_BUFFER * MFT_FILE_SIZE];

int Read(LPCWSTR driveLabel, void* buffer, uint64_t from, uint64_t count);
char* DuplicateName(wchar_t* name, size_t nameLength);
int NTFSParse(LPCWSTR DriveLabel);

#endif