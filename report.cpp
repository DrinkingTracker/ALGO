#include "report.h"
#include "utils.h"
#include <iostream>
#include <fstream>

using namespace std;

void exportReport(const vector<Wafer>& w, int batch) {
    cout << "\nMengekspor laporan ke report.txt...\n";
    delayAnimasi(1000);

    ofstream file("report.txt", ios::app); 
    if (file.is_open()) {
        file << "=======================================\n";
        file << "REPORT BATCH PRODUKSI: " << batch << "\n";
        file << "=======================================\n\n";

        for (const auto& wafer : w) {
            file << "Label Akhir  : " << wafer.labelAkhir << "\n";
            file << "Rute (BFS)   : " << wafer.ruteBFS << "\n";
            file << "Total Defect : " << wafer.totalDefect << "% (" << wafer.grade << ")\n";
            file << "Matriks 10x10 (1=OK, 0=Defect):\n";
            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    file << wafer.gridDefect[i][j] << " ";
                }
                file << "\n";
            }
            file << "---------------------------------------\n";
        }
        file.close();
        cout << "✓ Export report.txt berhasil!\n";
    } else {
        cout << "Gagal membuka file report.txt!\n";
    }
}

void bacaReport() {
    ifstream file("report.txt");
    if (!file.is_open()) {
        cout << "[INFO] Belum ada report.txt yang dibuat.\n";
    } else {
        string line;
        while (getline(file, line)) {
            cout << line << "\n";
        }
        file.close();
    }
}