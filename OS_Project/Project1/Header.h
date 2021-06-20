#ifndef HEADER_H
#define HEADER_H
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sstream>

using namespace std;

struct FAT32
{
    byte OEM[3];
    byte Version[8];
    byte BytePerSector[2];
    byte SectorPerCluster[1];
    byte ReservedSector[2];
    byte FatNum[1];
    byte EntryRDET[2];
    byte SectorVol[2];
    byte DeviceType[1];
    byte SectorPerFat[2];
    byte SectorPerTrack[2];
    byte HeadPerDisk[2];

    byte Distance[4];
    byte VolSize[4];
    byte SizePerFAT[4];
    byte Bit8On[2];             // bit 8 bat
    byte FAT32Ver[2];
    byte FirstRDETCluster[4];
    byte AddiInfoSector[2];
    byte SaveBootSector[2];
    byte DanhRieng[12];         // Cho cac phien ban sau

    byte PhysicDisk[1];         // 0: mem, 80h : cung
    byte Danhrieng[1];
    byte KiHieuNhanDienOS[1];
    byte SerialNumber[4];
    byte VolumeLabel[11];
    byte FATType[8];
    byte BootProgram[420];
    byte DauHieuKetThuc[2];     // always AA55h
};

int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512]);
int buffToInteger(byte* buffer);
unsigned int reversedBytes(uint8_t* byte, unsigned int count);
#endif