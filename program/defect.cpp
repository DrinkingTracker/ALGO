#include "heading\defect.h"
#include "heading\utils.h"
#include <iostream>
#include <random>

using namespace std;

void prosesQualityControl(vector<Wafer>& w) {
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution d(0.4); // Probabilitas defect diatur natural

    for (auto& wafer : w) {
        wafer.totalOK = 0;
        wafer.totalDefect = 0;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                bool isDefect = d(gen);
                wafer.gridDefect[i][j] = isDefect ? 0 : 1;
                if (isDefect) wafer.totalDefect++;
                else wafer.totalOK++;
            }
        }

        if (wafer.totalDefect >= 75) wafer.grade = "i3";
        else if (wafer.totalDefect >= 50) wafer.grade = "i5";
        else if (wafer.totalDefect >= 25) wafer.grade = "i7";
        else wafer.grade = "i9";

        wafer.labelAkhir = wafer.labelAwal + "-" + wafer.grade;
    }
}

void pelabelanUlangAnimasi(vector<Wafer>& w) {
    cout << "=== PROSES GRADING & PELABELAN ULANG ===\n";
    delayAnimasi(1000);

    for (size_t i = 0; i < w.size(); i++) {
        cout << "Inspeksi " << w[i].labelAwal << " ... Defect: " << w[i].totalDefect << "% ";
        delayAnimasi(400); 
        cout << "-> Grade: " << w[i].grade << "  =>  " << w[i].labelAkhir << " [DONE]\n";
        delayAnimasi(300); 
    }
    cout << "\nPelabelan akhir selesai.\n";