#pragma once

#include<windows.h>
#include<stdio.h>
#include <vector>
#include <string>

using namespace System;

std::vector<std::string> getDrive() {

    DWORD cchBuffer;
    WCHAR* driveStrings;
    UINT driveType;
    LPCWSTR driveTypeString;
    ULARGE_INTEGER freeSpace;
    std::vector<std::string> ok;
    array<String^>^ ok2;

    // Find out how big a buffer we need
    cchBuffer = GetLogicalDriveStrings(0, NULL);

    driveStrings = (WCHAR*)malloc((cchBuffer + 1) * sizeof(TCHAR));
    if (driveStrings == NULL)
    {
        return;
    }

    // Fetch all drive strings
    GetLogicalDriveStrings(cchBuffer, driveStrings);

    //ok2 = gcnew array<String^>();
    //int i = 0;

    // Loop until we find the final '\0'
    // driveStrings is a double null terminated list of null terminated strings)
    while (*driveStrings)
    {
        // Dump drive information
        driveType = GetDriveType(driveStrings);
        GetDiskFreeSpaceEx(driveStrings, &freeSpace, NULL, NULL);

        switch (driveType)
        {
        case DRIVE_FIXED:
            driveTypeString = L"Hard disk";
            break;

        case DRIVE_CDROM:
            driveTypeString = L"CD/DVD";
            break;

        case DRIVE_REMOVABLE:
            driveTypeString = L"Removable";
            break;

        case DRIVE_REMOTE:
            driveTypeString = L"Network";
            break;

        default:
            driveTypeString = L"Unknown";
            break;
        }

        /*printf("%S - %S - %I64u GB free\n", driveStrings, driveTypeString,        //_int64
            freeSpace.QuadPart / 1024 / 1024 / 1024);*/

        std::wstring ws(driveStrings);
        std::string temp(ws.begin(), ws.end());
        ok.push_back(temp);

        /*ok2[i] = ;
        ++i;*/

        driveStrings += lstrlen(driveStrings) + 1;
    }

    return ok;
}
