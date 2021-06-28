#ifndef NTFS_H
#define NTFS_H

#include "FAT32.h"
#include "../Project1/Sublib/stb_ds.h"
#include <assert.h>
#include <stdint.h>

struct BOOTSECTORNTFS {
    BYTE     JUMP[3];
    BYTE     OEM[8];

    // BPB
    WORD     BytePerSector;
    BYTE     SectorPerCluster;
    WORD     ReservedSector;
    BYTE     Unknown1[3];
    WORD     Unknown2;
    BYTE     MediaDescriptor;
    WORD     Unknown3;
    WORD     SectorPerTrack;
    WORD     HeadPerDisk;
    DWORD    NumberHiddenSectors;
    DWORD    Unknown4;

    // Extended BPB
    DWORD   Unknown5;
    uint64_t    TotalSectors;
    uint64_t    $MFTCluster;
    uint64_t    $MFTMirrCluster;
    DWORD   ClustersPerFileRecordSegment;      // > 0 -> cluster || < 0 : Ex: -10 -> 2^10 bytes
    DWORD   ClustersPerIndexBuffer;            // Like above           
    BYTE    VolumeSerial[8];
    DWORD   Checksum;

    BYTE    Bootstrap[426];
    WORD    EndOfSectorMarker;
};

struct FileRecordHeader {
    uint32_t    magic;
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

struct File {
    uint64_t    parent;
    char* name;
};

extern File files;
extern DWORD bytesAccessed;
extern HANDLE drive;
extern BOOTSECTORNTFS ntfs;

#define MFT_FILE_SIZE (1024)
extern uint8_t mftFile[MFT_FILE_SIZE];

#define MFT_FILES_PER_BUFFER (65536)
extern uint8_t mftBuffer[MFT_FILES_PER_BUFFER * MFT_FILE_SIZE];

int ReadBootSectorNTFS(LPCWSTR  drive, int readPoint, BYTE sector[512]);
char* DuplicateName(wchar_t* name, size_t nameLength);
void Read(void* buffer, uint64_t from, uint64_t count);
int NTFSParse(LPCWSTR  drive);

#endif