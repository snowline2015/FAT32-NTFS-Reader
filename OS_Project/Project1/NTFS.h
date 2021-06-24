#ifndef NTFS_H
#define NTFS_H
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#define magicNTFS	const_cpu_to_le64(0x202020205346544e)
#define NTFS_SB_MAGIC	0x5346544e	;
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <iomanip>
#include <ctype.h>

using namespace std;

struct BootSector_NTFS
{
    BYTE        jump[3];
    BYTE        oemID[8];
    WORD        bytePerSector;
    BYTE        sectorPerCluster;
    BYTE        reserved[2];
    BYTE        zero1[3];
    BYTE        unused1[2];
    BYTE        mediaDescriptor;
    BYTE        zeros2[2];
    WORD        sectorPerTrack;
    WORD        headNumber;
    DWORD       hiddenSector;
    BYTE        unused2[8];
    LONGLONG    totalSector;
    LONGLONG    MFTCluster;
    LONGLONG    MFTMirrCluster;
    signed char clusterPerRecord;
    BYTE        unused3[3];
    signed char clusterPerBlock;
    BYTE        unused4[3];
    LONGLONG    serialNumber;
    DWORD       checkSum;
    BYTE        bootCode[0x1aa];
    BYTE        endMarker[2];
};

struct NTFS_SYSTEM_FILES
{

};

#endif