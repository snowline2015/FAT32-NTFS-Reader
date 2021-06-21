////use FAT32   
//#undef UNICODE        // to flag window deactive unicode
//#include<Windows.h>  //to use windows api
//#include<iostream>
//#include<iomanip>
//#include<cstring>
//#include<string>
//#include<algorithm>
//using namespace std;
//#pragma pack(1)  //tell compiler do'nt do prag
//struct BPB
//{
//    BYTE            JMP[3];
//    BYTE            OEM[8];
//    WORD            NumberOfBytesPerSector;
//    BYTE            NumberOfSectorsPerCluster;
//    WORD            NumberOfReservedSectors;
//    BYTE            NumberOfFATs;
//    WORD            NumberOfRootEntries16;
//    WORD            LowNumbferOfSectors;
//    BYTE            MediaDescriptor;
//    WORD            NumberOfSectorsPerFAT16;
//    WORD            NumberOfSectorsPerTrack;
//    WORD            NumberOfHeads;
//    DWORD           NumberOfHiddenSectors;
//    DWORD           HighNumberOfSectors;
//
//    DWORD           NumberOfSectorsPerFAT32;
//    WORD            Flags;
//    WORD            FATVersionNumber;
//    DWORD           RootDirectoryClusterNumber;
//    WORD            FSInfoSector;
//    WORD            BackupSector;
//    BYTE            Reserver[12];
//    BYTE            BiosDrive;
//    BYTE            WindowsNTFlag;
//    BYTE            Signature;
//    DWORD           VolumeSerial;
//    BYTE            VolumeLabel[11];
//    BYTE            SystemID[8];
//    BYTE            CODE[420];
//    WORD            BPBSignature;
//};
////-----------------------------------------------------------
//struct DirectoryEntry
//{
//    BYTE Name[11];
//    BYTE Attributes;
//    BYTE Reserved;
//    BYTE CreationTimeTenth;
//    WORD CreationTime;
//    WORD CreationDate;
//    WORD LastAccessTime;
//    WORD HiClusterNumber;
//    WORD WriteTime;
//    WORD WriteDate;
//    WORD LowClusterNumber;
//    DWORD FileSize;   //acual size of file
//};
////---------------------------------------------------
//void dirFunction(string s) {
//    string path = "\\\\.\\" + s + ":";
//    HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE,
//        FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);//open partition 
//    BPB bootSector;//var from bootSector structure
//    DWORD readBytes = 0;
//    if (hFile == INVALID_HANDLE_VALUE)
//    {
//        cout << "Error " << GetLastError() << endl;
//        return;
//    }
//    ReadFile(hFile, (BYTE*)&bootSector, sizeof(bootSector), &readBytes, 0);//read partition and load bootSector information inside our structure
//    LONG t = 0;
//    ULONG distance = bootSector.NumberOfReservedSectors +
//        bootSector.NumberOfFATs * bootSector.NumberOfSectorsPerFAT32;//distance from begine until Root Directory or content of partetion
//    distance *= bootSector.NumberOfBytesPerSector;//convert distance number to bytes value
//
//    SetFilePointer(hFile, distance, &t, FILE_BEGIN);//set pointer to root directory begine or begine of data
//    int clusterSize = bootSector.NumberOfBytesPerSector * bootSector.NumberOfSectorsPerCluster; //cluster size 
//    int NumberOfEntries = clusterSize / sizeof(DirectoryEntry); //number of record inside cluster
//    DirectoryEntry* root = new DirectoryEntry[NumberOfEntries];//descripe the partetion
//    ReadFile(hFile, (BYTE*)root, clusterSize, &readBytes, 0);
//    DWORD clusterNumber;
//    for (int i = 0; i < NumberOfEntries; i++)
//    {
//        if (root[i].Name[0] == 0)//there no entery after this
//            break;
//        if (root[i].Name[0] == 0xE5)
//            continue;
//        if ((root[i].Attributes & 0xF) == 0xF)
//            continue;
//        for (int j = 0; j < 8; j++)
//            cout << root[i].Name[j];
//        if ((root[i].Attributes & 0x10) != 0x10) {
//            cout << ".";
//            for (int j = 8; j < 11; j++)
//                cout << root[i].Name[j];
//        }
//        if ((root[i].Attributes & 0x10) == 0x10) {
//            cout << "\t<Folder>";
//        }
//        else {
//
//            cout << "\t<File>";
//        }
//        clusterNumber = root[i].HiClusterNumber << 16;
//        clusterNumber |= root[i].LowClusterNumber;
//        cout << "\t" << root[i].FileSize << "bytes" << "\t" << clusterNumber << "cluster" << endl;
//    }
//
//    CloseHandle(hFile);
//}
////---------------------------------------------------------------
//string convertLowerToUpper(string f) {
//
//    string temp = "";
//    for (int i = 0; i < f.size(); i++) {
//
//        temp += toupper(f[i]);
//    }
//
//    return temp;
//}
////---------------------------------------------------------------
//string getFileName(BYTE filename[11]) {
//    string name = "";
//    for (int i = 0; i < 8; i++) {
//        if (filename[i] != ' ')
//            name += filename[i];
//    }
//    return (name);
//
//}
////------------------------------------------------------------------
//int findEntryNumber(DirectoryEntry* root, int NumberOfEntries, string required) {
//    string n;
//    int j = 0;
//    for (int i = 0; i < NumberOfEntries; i++) {
//
//        if (strcmp((getFileName(root[i].Name).c_str()), convertLowerToUpper(required).c_str()) == 0) {
//
//            return i;
//        }
//
//
//    }
//
//    return -1;
//}
////---------------------------------------------------------------
//void typeFunction(string fileName, string s) {
//    string path = "\\\\.\\" + s + ":";
//    HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE,
//        FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);//open partition 
//    BPB bootSector;//var from bootSector structure
//    DWORD readBytes = 0;
//    if (hFile == INVALID_HANDLE_VALUE)
//    {
//        cout << "Error " << GetLastError() << endl;
//        return;
//    }
//    ReadFile(hFile, (BYTE*)&bootSector, sizeof(bootSector), &readBytes, 0);//read partition and load bootSector information inside our structure
//    LONG t = 0;
//    ULONG distance = bootSector.NumberOfReservedSectors +
//        bootSector.NumberOfFATs * bootSector.NumberOfSectorsPerFAT32;//distance from begine until Root Directory or content of partetion
//    distance *= bootSector.NumberOfBytesPerSector;//convert distance number to bytes value
//
//    SetFilePointer(hFile, distance, &t, FILE_BEGIN);//set pointer to root directory begine or begine of data
//    int clusterSize = bootSector.NumberOfBytesPerSector * bootSector.NumberOfSectorsPerCluster; //cluster size 
//    int NumberOfEntries = clusterSize / sizeof(DirectoryEntry); //number of record inside cluster
//    DirectoryEntry* root = new DirectoryEntry[NumberOfEntries];//descripe the partetion
//    ReadFile(hFile, (BYTE*)root, clusterSize, &readBytes, 0);
//    DWORD clusterNumber;
//    int index = findEntryNumber(root, NumberOfEntries, fileName);
//    if (index == -1) {
//
//        cout << "File is not found" << endl;
//        return;
//    }
//    if (((root[index].Attributes & 0x10) == 0x10)) {
//
//        cout << "Is not file name" << endl;
//        return;
//    }
//    clusterNumber = root[index].HiClusterNumber << 16;
//    clusterNumber |= root[index].LowClusterNumber;
//    ULONG temp = (clusterNumber - 2) * clusterSize;
//    distance += temp;
//    t = 0;
//    SetFilePointer(hFile, distance, &t, FILE_BEGIN);
//    BYTE* buffer = new BYTE[clusterSize];
//    readBytes = 0;
//
//
//    ReadFile(hFile, (BYTE*)buffer, clusterSize, &readBytes, 0);
//
//    for (int i = 0; i < root[index].FileSize; i++) {
//
//        cout << buffer[i];
//    }
//
//    cout << endl;
//    CloseHandle(hFile);
//
//}
////----------------------------------------------------------------------
//void delFunction(string filename, string s) {
//    string path = "\\\\.\\" + s + ":";
//    HANDLE hFile = CreateFile(path.c_str(), GENERIC_READ | GENERIC_WRITE,
//        FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);//open partition 
//    BPB bootSector;//var from bootSector structure
//    DWORD readBytes = 0;
//    if (hFile == INVALID_HANDLE_VALUE)
//    {
//        cout << "Error " << GetLastError() << endl;
//        return;
//    }
//    ReadFile(hFile, (BYTE*)&bootSector, sizeof(bootSector), &readBytes, 0);//read partition and load bootSector information inside our structure
//    LONG t = 0;
//    ULONG distance = bootSector.NumberOfReservedSectors +
//        bootSector.NumberOfFATs * bootSector.NumberOfSectorsPerFAT32;//distance from begine until Root Directory or content of partetion
//    distance *= bootSector.NumberOfBytesPerSector;//convert distance number to bytes value
//
//    SetFilePointer(hFile, distance, &t, FILE_BEGIN);//set pointer to root directory begine or begine of data
//    int clusterSize = bootSector.NumberOfBytesPerSector * bootSector.NumberOfSectorsPerCluster; //cluster size 
//    int NumberOfEntries = clusterSize / sizeof(DirectoryEntry); //number of record inside cluster
//    DirectoryEntry* root = new DirectoryEntry[NumberOfEntries];//descripe the partetion
//    ReadFile(hFile, (BYTE*)root, clusterSize, &readBytes, 0);
//    DWORD clusterNumber;
//    readBytes = 0;
//    t = 0;
//    int index = findEntryNumber(root, NumberOfEntries, filename);
//    if (index == -1) {
//
//        cout << "FIle is not found" << endl;
//        return;
//    }
//    if ((root[index].Attributes & 0x10) == 0x10) {
//
//        cout << "Is not file name" << endl;
//        return;
//    }
//
//    //delete file
//    root[index].Name[0] = 0xE5;
//    SetFilePointer(hFile, distance, &t, FILE_BEGIN);
//    WriteFile(hFile, (BYTE*)root, clusterSize, &readBytes, 0);
//    cout << filename << " is deleted\n";
//
//    CloseHandle(hFile);
//
//}
////----------------------------------------------------------------------
//string removeExtention(string s) {
//
//    string t = "";
//
//    for (int i = 0; i < s.size(); i++) {
//
//        if (s[i] == '.')break;
//        t += s[i];
//    }
//
//    return t;
//
//}
////-------------------------------------------------------------------
//void main()
//{
//    string swich_value;
//    string directory;
//    string file_name;
//    //dirFunction("G");
//    cout << "plz, Enter single Partition character ------> example E or G\n\n";
//    cin >> directory;
//    string path = "\\\\.\\" + directory + ":";
//    cout << "current directory is " << path << endl;
//    cout << "Enter Options: \n1- dir \n2- type file_name.extention \n3- del file_name.extention\n\n";
//again:
//    cin >> swich_value;
//
//    if (swich_value.at(1) != 'i')
//        cin >> file_name;
//    string answer;
//    switch (swich_value.at(1)) {
//    case 'i':
//        dirFunction(directory);
//        cout << "\nare  you want to do another process: y or n?";
//        cin >> answer;
//        if (answer.at(0) == 'y')
//            goto again;
//        break;
//    case 'y':
//        typeFunction(removeExtention(file_name), directory);
//        cout << "\nare  you want to do another process: y or n?";
//        cin >> answer;
//        if (answer.at(0) == 'y')
//            goto again;
//        break;
//    case 'e':
//        delFunction(removeExtention(file_name), directory);
//        cout << "\nare  you want to do another process: y or n?";
//        cin >> answer;
//        if (answer.at(0) == 'y')
//            goto again;
//        break;
//
//
//    }
//}