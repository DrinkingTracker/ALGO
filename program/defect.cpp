#include "heading/defect.h"
#include "heading/utils.h"
#include <iostream>
#include <random>

using namespace std;

void prosesQualityControl(vector<Wafer>& w) {
    random_device rd;
    mt19937 gen(rd());
    bernoulli_distribution d(0.3); // 30% kemungkinan defect per core

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

        // Grade berdasarkan persentase core OK (totalOK dari 100)
        if (wafer.totalOK >= 75)      wafer.grade = "i9"; // >=75% OK -> terbaik
        else if (wafer.totalOK >= 50) wafer.grade = "i7"; // 50-74% OK
        else if (wafer.totalOK >= 25) wafer.grade = "i5"; // 25-49% OK
        else                          wafer.grade = "i3"; // <25% OK -> terburuk

        wafer.labelAkhir = wafer.labelAwal + "-" + wafer.grade;
    }
}

void pelabelanUlangAnimasi(vector<Wafer>& w) {
    cout << "\n=== PROSES GRADING & PELABELAN ULANG ===\n";
    delayAnimasi(1000);

    for (size_t i = 0; i < w.size(); i++) {
        int persen = w[i].totalOK; // sudah dari 100
        cout << "Inspeksi " << w[i].labelAwal 
             << " ... Core OK: " << persen << "/100 (" << persen << "%) ";
        delayAnimasi(400);
        cout << "-> Grade: " << w[i].grade 
             << "  =>  " << w[i].labelAkhir << " [DONE]\n";
        delayAnimasi(300);
    }
    cout << "\nPelabelan akhir selesai.\n";
}