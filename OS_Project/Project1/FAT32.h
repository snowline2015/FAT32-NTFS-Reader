#ifndef HEADER_H
#define HEADER_H
#define INVALID_SET_FILE_POINTER ((DWORD)-1)
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

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
    WORD Bit8Flag;             // bit 8 bat
    WORD FAT32Ver;
    DWORD FirstRDETCluster;
    WORD AddiInfoSector;
    WORD BackupSector;
    BYTE LaterVerReserved[12];         // Cho cac phien ban sau

    BYTE PhysicDisk;         // 0: mem, 80h : cung
    BYTE Reserved;
    BYTE Signature;
    DWORD VolumeSerial;
    BYTE VolumeLabel[11];
    BYTE FATID[8];
    BYTE BootProgram[420];
    WORD EndSignature;     // luon luon AA55h
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

    // Sub Entry
    //BYTE Entry;
    //BYTE Unicode5Char[10];      // UTF16
    //BYTE DauHieuNhanBiet;
    //BYTE Next6Char[12];
    //BYTE Next2Char[4];
};

int ReadSectorFAT32(LPCWSTR  drive, int readPoint, BYTE sector[512]);
int ReadRDETFAT32(LPCWSTR  drive, int readPoint, BYTE sector[512]);
int buffToInteger(byte* buffer);
unsigned int reversedBytes(uint8_t* byte);

#endif