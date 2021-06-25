#ifndef NTFS_H
#define NTFS_H

#define magicNTFS const_cpu_to_le64(0x202020205346544e)
#define NTFS_SB_MAGIC 0x5346544e

#include "FAT32.h"

using namespace std;

struct BOOTSECTORNTFS {
    BYTE JUMP[3];
    BYTE OEM[8];

    // BPB
    WORD BytePerSector;
    BYTE SectorPerCluster;
    WORD ReservedSector;
    BYTE Unknown1[3];             // chua bik ten
    WORD Unknown2;             // chua bik ten
    BYTE MediaDescriptor;
    WORD Unknown3;        // chua bik ten
    WORD SectorPerTrack;
    WORD HeadPerDisk;
    DWORD NumberHiddenSectors;
    DWORD Unknown4;    // chua bik ten

    // Extended BPB
    DWORD Unknown5;       // chua bik ten
    BYTE TotalSectors[8];
    BYTE LogicalClusterNumber$MFT[8];
    BYTE LogicalClusterNumber$MFTMirror[8];
    DWORD ClustersPerFileRecordSegment;
    BYTE ClustersPerIndexBuffer;
    BYTE Unknown6[3];             // chua bik ten
    BYTE VolumeSerial[8];
    DWORD Checksum;

    BYTE Bootstrap[426];
    WORD EndOfSectorMarker;
};
struct NTFS_SYSTEM_FILES
{

};

#endif