#include "heading/sorting.h"
#include "heading/utils.h"
#include <iostream>

using namespace std;

void cetakKondisiArray(const vector<Wafer>& w) {
    cout << "[ ";
    for (size_t i = 0; i < w.size(); i++) {
        cout << w[i].labelAwal << (i == w.size() - 1 ? "" : " | ");
    }
    cout << " ]\n";
}

void selectionSortLabel(vector<Wafer>& w) {
    int n = w.size();
    cout << "\n=== PROSES SORTING DISTRIBUSI (SELECTION SORT) ===\n";
    delayAnimasi(800);
    
    cout << "Awal: ";
    cetakKondisiArray(w);
    delayAnimasi(1000);

    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (w[j].nomorUrut < w[min_idx].nomorUrut) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap(w[i], w[min_idx]);
            cout << "Step " << i + 1 << ": ";
            cetakKondisiArray(w);
            delayAnimasi(600); 
        }
    }
    cout << "\nSorting selesai! Data wafer terurut berdasarkan nomor urut.\n";
}

void sortDanKelompokkanGrade(vector<Wafer>& w) {
    int n = w.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (w[j].grade < w[min_idx].grade) {
                min_idx = j;
            } else if (w[j].grade == w[min_idx].grade && w[j].nomorUrut < w[min_idx].nomorUrut) {
                min_idx = j;
            }
        }
        swap(w[i], w[min_idx]);
    }
}

void sortBerdasarkanNomorUrut(vector<Wafer>& w) {
    int n = w.size();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (w[j].nomorUrut < w[min_idx].nomorUrut) {
                min_idx = j;
            }
        }
        swap(w[i], w[min_idx]);
    }
}

int binarySearchWafer(const vector<Wafer>& w, int targetNomor) {
    int low = 0;
    int high = w.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (w[mid].nomorUrut == targetNomor) {
            return mid;
        }
        if (w[mid].nomorUrut < targetNomor) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; 
}