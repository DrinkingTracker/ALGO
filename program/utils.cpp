#include "heading\utils.h"
#include <iostream>
#include <cstdlib>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

void delayAnimasi(int milliseconds) {
#ifdef _WIN32
    Sleep(milliseconds); 
#else
    usleep(milliseconds * 1000); 
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void konfirmasiLanjut() {
    std::cout << "\n============================================\n";
    std::cout << "Tekan Enter untuk melanjutkan ke tahap berikutnya...";
    std::cin.ignore();
    std::cin.get();
}