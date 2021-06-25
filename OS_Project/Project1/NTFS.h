#ifndef NTFS_H
#define NTFS_H

#define magicNTFS const_cpu_to_le64(0x202020205346544e)
#define NTFS_SB_MAGIC 0x5346544e

#include "FAT32.h"

struct BOOTSECTORNTFS {
    BYTE JUMP[3];
    BYTE OEM[8];

    // BPB
    WORD BytePerSector;
    BYTE SectorPerCluster;
    WORD ReservedSector;
    BYTE Unknown1[3];             
    WORD Unknown2;           
    BYTE MediaDescriptor;
    WORD Unknown3;        
    WORD SectorPerTrack;
    WORD HeadPerDisk;
    DWORD NumberHiddenSectors;
    DWORD Unknown4;    

    // Extended BPB
    DWORD Unknown5;       
    BYTE TotalSectors[8];
    BYTE $MFTCluster[8];
    BYTE $MFTMirrCluster[8];        
    BYTE ClustersPerFileRecordSegment;      // > 0 -> cluster || < 0 : Ex: -10 -> 2^10 bytes
    BYTE Unknown6[3];
    BYTE ClustersPerIndexBuffer;            // Like above
    BYTE Unknown7[3];            
    BYTE VolumeSerial[8];
    DWORD Checksum;

    BYTE Bootstrap[426];
    WORD EndOfSectorMarker;
};

struct Metafiles {
    int $MFT = 0;
    int $MFTMirr = 1;
    int $LogFile = 2;
    int $Volume = 3;
    int $AttrDef = 4;
    int RDET = 5;
    int $Bitmap = 6;
    int $Boot = 7;
    int $BadClus = 8;
    int $Secure = 9;
    int $UpCase = 10;
    int $Extend = 11;
    int Reserved[12] = { 12,13,14,15,16,17,18,19,20,21,22,23 };
    int $Extend$Quota = 24;
    int $Extend$ObjId = 25;
    int $Extend$Reparse = 26;
};

struct NTFS_SYSTEM_FILES
{

};

int ReadSectorNTFS(LPCWSTR  drive, int readPoint, BYTE sector[512]);

#endif