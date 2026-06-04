#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
 
#include "heading/wafer.h"
#include "heading/utils.h"
#include "heading/sorting.h"
#include "heading/bfs.h"
#include "heading/defect.h"
#include "heading/report.h"
 
using namespace std;
 
int currentBatch = 1;
vector<Wafer> daftarWafer;
 
string generateLabelAwal(int batch, int nomor, int sp) {
    stringstream ss;
    ss << "W-" << batch << "-" << setfill('0') << setw(4) << nomor << "-" << sp;
    return ss.str();
}
 
void mulaiSimulasi() {
    clearScreen();
    cout << "=== INPUT PRODUKSI (Batch " << currentBatch << ") ===\n";
    int jumlahWafer;
    cout << "Masukkan jumlah wafer: ";
    cin >> jumlahWafer;
 
    if (jumlahWafer <= 0) {
        cout << "[ERROR] Jumlah wafer harus lebih dari 0.\n";
        delayAnimasi(1500);
        return;
    }
 
    daftarWafer.clear();
 
    cout << "\nMendistribusikan ke 4 Start Point...\n";
    delayAnimasi(1000);
 
    for (int i = 0; i < jumlahWafer; i++) {
        Wafer w;
        w.batch = currentBatch;
        w.nomorUrut = i + 1;
        w.startPoint = (i % 4) + 1;
        w.labelAwal = generateLabelAwal(w.batch, w.nomorUrut, w.startPoint);
        daftarWafer.push_back(w);
    }
 
    for (int sp = 1; sp <= 4; sp++) {
        cout << "Start Point " << sp << ": ";
        for (const auto& wafer : daftarWafer) {
            if (wafer.startPoint == sp) cout << "[" << wafer.labelAwal << "] ";
        }
        cout << "\n";
    }
 
    selectionSortLabel(daftarWafer);
    konfirmasiLanjut();
 
    clearScreen();
    jalankanBfsRouting(daftarWafer);
    konfirmasiLanjut();
 
    clearScreen();
    cout << "=== QUALITY CONTROL & DEFECT ===\n\n";
    prosesQualityControl(daftarWafer);
    pelabelanUlangAnimasi(daftarWafer);
 
    cout << "\n=== KLASIFIKASI AKHIR ===\n";
    sortDanKelompokkanGrade(daftarWafer);
    delayAnimasi(800);
 
    int c3=0, c5=0, c7=0, c9=0;
    for (const auto& w : daftarWafer) {
        if      (w.grade == "i3") c3++;
        else if (w.grade == "i5") c5++;
        else if (w.grade == "i7") c7++;
        else if (w.grade == "i9") c9++;
    }
    cout << "Grade i9 (>=75% core OK)  : " << c9 << " wafer\n";
    cout << "Grade i7 (50%-74% core OK): " << c7 << " wafer\n";
    cout << "Grade i5 (25%-49% core OK): " << c5 << " wafer\n";
    cout << "Grade i3 (<25% core OK)   : " << c3 << " wafer\n\n";
 
    exportReport(daftarWafer, currentBatch);
    currentBatch++;
 
    konfirmasiLanjut();
}
 
void cariWaferMenu() {
    clearScreen();
    cout << "=== CARI WAFER (BINARY SEARCH) ===\n";
    if (daftarWafer.empty()) {
        cout << "[INFO] Lakukan simulasi produksi terlebih dahulu.\n";
    } else {
        sortBerdasarkanNomorUrut(daftarWafer);
        int target;
        cout << "Masukkan Nomor Urut Wafer yang dicari: ";
        cin >> target;
 
        cout << "Mencari dengan Binary Search...\n";
        delayAnimasi(800);
 
        int hasilIdx = binarySearchWafer(daftarWafer, target);
        if (hasilIdx != -1) {
            const auto& w = daftarWafer[hasilIdx];
            cout << "\n[DITEMUKAN]\n";
            cout << "Label Awal  : " << w.labelAwal << "\n";
            cout << "Label Akhir : " << w.labelAkhir << "\n";
            cout << "Grade       : " << w.grade << "\n";
            cout << "Core OK     : " << w.totalOK << "/100\n";
            cout << "Rute        : " << w.ruteBFS << "\n";
        } else {
            cout << "\n[INFO] Wafer nomor " << target << " tidak ditemukan.\n";
        }
    }
    cout << "\nTekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}
 
void lihatPeta() {
    clearScreen();
    tampilkanPeta();
    cout << "\nTekan Enter untuk kembali ke Dashboard...";
    cin.ignore();
    cin.get();
}
 
void resetBatch() {
    currentBatch = 1;
    daftarWafer.clear();
    cout << "\n[NOTIFIKASI] Batch direset ke 1 dan data dibersihkan.\n";
    delayAnimasi(1500);
}
 
void bukaReportMenu() {
    clearScreen();
    cout << "=== REPORT.TXT ===\n\n";
    bacaReport();
    cout << "\nTekan Enter untuk kembali...";
    cin.ignore();
    cin.get();
}
 
int main() {
    inisialisasiGrid();
    int pilihan;
    do {
        clearScreen();
        cout << "+==========================================+\n";
        cout << "|   CPU WAFER PRODUCTION SIMULATION v1.0   |\n";
        cout << "+==========================================+\n";
        cout << "|  [1] Mulai Simulasi Produksi Baru        |\n";
        cout << "|  [2] Lihat Peta Stasiun Produksi         |\n";
        cout << "|  [3] Reset Batch                         |\n";
        cout << "|  [4] Buka report.txt                     |\n";
        cout << "|  [5] Cari Wafer (Binary Search)          |\n";
        cout << "|  [6] Keluar                              |\n";
        cout << "+==========================================+\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
 
        switch (pilihan) {
            case 1: mulaiSimulasi(); break;
            case 2: lihatPeta(); break;
            case 3: resetBatch(); break;
            case 4: bukaReportMenu(); break;
            case 5: cariWaferMenu(); break;
            case 6:
                cout << "Keluar. Terima kasih!\n";
                delayAnimasi(1000);
                break;
            default:
                cout << "Pilihan tidak valid!\n";
                delayAnimasi(1000);
        }
    } while (pilihan != 6);
 
    return 0;
}