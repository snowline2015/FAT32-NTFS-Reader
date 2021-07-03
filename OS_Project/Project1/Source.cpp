#include "FAT32.h"
#include "NTFS.h"
#include "GUI.h"

int main(int argc, char** argv)
{
    int choice;
    std::cout << "1. FAT32\n2. NTFS\n" << "Please choose your drive format before reading: ";
    std::cin >> choice;
    std::cin.ignore(1);
        
    std::string drive;
    std::cout << "Please enter your removable disk letter: ";
    std::getline(std::cin, drive);
    std::cout << std::endl;

    BYTE sector[512];
    if (choice == 1) {
        ReadBootSectorFAT32(s2ws("\\\\.\\" + drive + ":").c_str(), 0, sector);
        ReadRDETFAT32(s2ws("\\\\.\\" + drive + ":").c_str());
    }

    else {
        //ReadBootSectorNTFS(s2ws("\\\\.\\" + drive + ":").c_str(), 0, sector);
        NTFSParse(s2ws("\\\\.\\" + drive + ":").c_str());
    }

    return 0;
}