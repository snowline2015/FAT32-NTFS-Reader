#ifndef FAT32_H
#define FAT32_H
#define _CRT_SECURE_NO_WARNINGS
#define INVALID_SET_FILE_POINTER ((DWORD)-1)

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <stringapiset.h>

struct BOOTSECTORFAT32
{
    BYTE JUMP[3];
    BYTE OEM[8];
    WORD BytePerSector;
    BYTE SectorPerCluster;
    WORD ReservedSector;
    BYTE FatNum;
    WORD EntryRDET;
    WORD LowNumberSectors;
    BYTE DeviceType;
    WORD SectorPerFat16;
    WORD SectorPerTrack;
    WORD HeadPerDisk;

    DWORD NumberHiddenSectors;
    DWORD HighNumberSectors;
    DWORD SectorPerFat32;
    WORD Bit8Flag;             
    WORD FAT32Ver;
    DWORD FirstRDETCluster;
    WORD AddiInfoSector;
    WORD BackupSector;
    BYTE LaterVerReserved[12];       

    BYTE PhysicDisk;    
    BYTE Reserved;
    BYTE Signature;
    DWORD VolumeSerial;
    BYTE VolumeLabel[11];
    BYTE FATID[8];
    BYTE BootProgram[420];
    WORD EndSignature;    
};

struct RDETFAT32 {
    // Main entry
    BYTE FileName[11];
    BYTE FileAttributes;
    BYTE Reserved;
    BYTE CreatedHour[3];
    WORD CreatedDate;
    WORD LastAccessedDate;
    WORD FirstClusterHigh;
    WORD LastModifiedHour;
    WORD LastModifiedDate;
    WORD FirstClusterLow;
    DWORD FileSize;
};

struct LongFileDir {
    BYTE Flag;
    BYTE Name1[10];
    BYTE Attribute; // always 0F
    BYTE Reserved;
    BYTE Checksum;
    BYTE Name2[12];
    BYTE RelativeCluster[2];
    BYTE Name3[4];
};

extern BOOTSECTORFAT32 bs32;

int ReadBootSectorFAT32(LPCWSTR  drive, int readPoint, BYTE sector[512]);
int ReadRDETFAT32(LPCWSTR drive);
int ReadSRDETFAT32(LPCWSTR drive, HANDLE device, DWORD cluster, int depth);

unsigned int reversedWORD(WORD buffer);
unsigned int reversedDWORD(DWORD buffer);

int ReadTextFile(LPCWSTR drive, HANDLE device, DWORD cluster);


#endif