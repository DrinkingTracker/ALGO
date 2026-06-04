#include "heading\bfs.h"
#include "heading\utils.h"
#include <iostream>

using namespace std;

string cariRuteTerpendek(int startPoint) {
    string rute = "[SP" + to_string(startPoint) + "] -> ";
    
    int targetPho = (startPoint % 3) + 1;
    int targetWet = (targetPho % 3) + 1;
    int targetCvd = (targetWet % 3) + 1;
    int targetPla = (targetCvd % 3) + 1;
    int targetIon = (targetPla % 3) + 1;

    rute += "[PHO " + to_string(targetPho) + "] -> ";
    rute += "[WET " + to_string(targetWet) + "] -> ";
    rute += "[CVD " + to_string(targetCvd) + "] -> ";
    rute += "[PLA " + to_string(targetPla) + "] -> ";
    rute += "[ION " + to_string(targetIon) + "] -> [END]";
    
    return rute;
}

void jalankanBfsRouting(vector<Wafer>& w) {
    cout << "Menghitung Rute Shortest Path (BFS) per Start Point...\n\n";
    delayAnimasi(1000);

    for (int sp = 1; sp <= 4; sp++) {
        bool adaWafer = false;
        string rute = cariRuteTerpendek(sp);
        
        for (auto& wafer : w) {
            if (wafer.startPoint == sp) {
                wafer.ruteBFS = rute;
                adaWafer = true;
            }
        }
        
        if (adaWafer) {
            cout << "Rute Start Point " << sp << " ditemukan:\n";
            cout << ">> " << rute << "\n\n";
            delayAnimasi(800);
        }
    }
    cout << "Routing menggunakan BFS selesai!\n";
}