#ifndef WAFER_HPP
#define WAFER_HPP

#include <string>
#include <vector>

struct Wafer {
    int batch;
    int nomorUrut;
    int startPoint;
    int gridDefect[10][10];
    int totalOK;
    int totalDefect;
    std::string grade;
    std::string labelAwal;   
    std::string labelAkhir;  
    std::string ruteBFS;     // Menyimpan history rute
};

#endif