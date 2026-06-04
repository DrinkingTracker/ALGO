#include "heading/bfs.h"
#include "heading/utils.h"
#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

/*
  Grid berdasarkan draw.io (7 baris x 5 kolom):

=== PETA STASIUN PRODUKSI ===
                    [SP1]
                      |
            ----------+----------
            |         |         |
          [CVD]-----[ION]-----[CVD]
            |         |         |
            |---------|---------|
            |         |         |
          [PLA]-----[PHO]-----[WET]-----[SP2]
            |         |         |
       +----|---------|---------|
       |    |         |         |
[SP4]--+--[WET]-----[ION]-----[PLA]
       |    |         |         |
       +----|---------|---------|    
            |         |         |
          [PLA]-----[CVD]-----[WET]-----[END]
            |         |         |
            |---------|---------|    
            |         |         |
          [PHO]-----[ION]-----[PHO]
            |         |         |
            ----------+----------
                      |       
                    [SP3]

  Nilai grid:
  0=EMPTY, 1=SP1, 2=SP2, 3=SP3, 4=SP4,
  10=PHO, 20=CVD, 30=PLA, 40=ION, 50=WET, 99=END
  11 = WALL
  (indeks mesin dibedakan lewat gridLabel)
*/

int gridMap[GRID_ROWS][GRID_COLS] = {
    { 11, 11, 11, 11, 11, 11,  1, 11, 11, 11, 11 }, // row 0  : SP1
    { 11, 11, 11, 11, 11, 11,  0, 11, 11, 11, 11 }, // row 1
    { 11, 11, 11, 11,  0,  0,  0,  0,  0, 11, 11 }, // row 2
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row 3
    { 11, 11, 11, 11, 20, 11, 40, 11, 20, 11, 11 }, // row 4  : CVD1, ION1, CVD2
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row 5
    { 11, 11, 11, 11,  0,  0,  0,  0,  0, 11, 11 }, // row 6
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row 7
    { 11, 11, 11, 11, 30, 11, 10, 11, 50,  0,  2 }, // row 8  : PLA1, PHO1, WET1, SP2
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row 9
    { 11, 11,  0,  0,  0,  0,  0,  0,  0, 11, 11 }, // row10
    { 11, 11,  0, 11,  0, 11,  0, 11,  0, 11, 11 }, // row11
    {  4,  0,  0,  0,  0,  0, 40,  0, 30, 11, 11 }, // row12 : SP4, ION2, PLA2
    { 11, 11,  0, 11,  0, 11,  0, 11,  0, 11, 11 }, // row13
    { 11, 11,  0,  0,  0,  0,  0,  0,  0, 11, 11 }, // row14
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row15
    { 11, 11, 11, 11, 30, 11, 20, 11, 50,  0, 99 }, // row16 : PLA3, CVD3, WET3, END
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row17
    { 11, 11, 11, 11,  0,  0,  0,  0,  0, 11, 11 }, // row18
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row19
    { 11, 11, 11, 11, 10, 11, 40, 11, 10, 11, 11 }, // row20 : PHO2, ION3, PHO3
    { 11, 11, 11, 11,  0, 11,  0, 11,  0, 11, 11 }, // row21
    { 11, 11, 11, 11,  0,  0,  0,  0,  0, 11, 11 }, // row22
    { 11, 11, 11, 11, 11, 11,  0, 11, 11, 11, 11 }, // row23
    { 11, 11, 11, 11, 11, 11,  3,  3, 11, 11, 11 }, // row24 : SP3
};

string gridLabel[GRID_ROWS][GRID_COLS] = {
{"WALL", "WALL", "WALL", "WALL", "WALL", " WALL ", "SP1", "WALL", "WALL", "WALL","WALL"},
{"WALL", "WALL", "WALL", "WALL", "WALL", "WALL", "    ", "WALL", "WALL", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "    ", "    ", "    ", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "CVD1", "WALL", "ION1", "WALL", "CVD2", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "    ", "    ", "    ", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "PLA1", "WALL", "PHO1", "WALL", "WET1", "    ", "SP2 "},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "    ", "    ", "    ", "    ", "    ", "    ", "    ", "WALL", "WALL"},
{"WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"SP4 ", "    ", "    ", "    ", "    ", "    ", "ION2", "    ", "PLA2", "WALL", "WALL"},
{"WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "    ", "    ", "    ", "    ", "    ", "    ", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "PLA3", "WALL", "CVD3", "WALL", "WET3", "    ", "END "},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "    ", "    ", "    ", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "PHO2", "WALL", "ION3", "WALL", "PHO3", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "WALL", "    ", "WALL", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "    ", "    ", "    ", "    ", "    ", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "WALL", "WALL", "    ", "WALL", "WALL", "WALL", "WALL"},
{"WALL", "WALL", "WALL", "WALL", "WALL", "WALL", "SP3", "SP3 ", "WALL", "WALL", "WALL"}
};

void inisialisasiGrid() {
    // Grid sudah diinisialisasi secara statis
}

void tampilkanPeta() {
    cout << "\n=== PETA STASIUN PRODUKSI ===\n\n";
    // Header kolom
    cout << "     ";
    for (int c = 0; c < 4; c++) cout << "  C" << c << "  ";
    cout << "\n";
    cout << "     +-----+-----+-----+-----+\n";

    for (int r = 0; r < GRID_ROWS; r++) {
        cout << " R" << r << "  |";
        for (int c = 0; c < 4; c++) {
            string lbl = gridLabel[r][c];
            if (lbl == "   " || lbl == "    ") {
                cout << "     |";
            } else {
                // Pad ke 5 karakter
                while ((int)lbl.size() < 4) lbl += " ";
                cout << lbl << " |";
            }
        }
        cout << "\n";
        cout << "     +-----+-----+-----+-----+\n";
    }

    cout << "\nKoneksi: setiap node terhubung ke atas/bawah/kiri/kanan\n";
    cout << "Urutan mesin: PHO -> CVD -> PLA -> ION -> END\n";
}

// BFS mencari jalur terpendek dari (startRow,startCol) ke node dengan label target
// Mengembalikan string jalur
string bfsCariJalur(int startRow, int startCol, NodeType target, int targetIdx) {
    // Encode target ke nilai gridMap
    int targetVal = 0;
    if (target == PHO) targetVal = 10;
    else if (target == CVD) targetVal = 20;
    else if (target == PLA) targetVal = 30;
    else if (target == ION) targetVal = 40;
    else if (target == WET) targetVal = 50;
    else if (target == END_NODE) targetVal = 99;
    else if (target == WALL) targetVal = 11;

    // BFS
    bool visited[GRID_ROWS][GRID_COLS] = {};
    int prevRow[GRID_ROWS][GRID_COLS];
    int prevCol[GRID_ROWS][GRID_COLS];
    for (int i = 0; i < GRID_ROWS; i++)
        for (int j = 0; j < GRID_COLS; j++) {
            prevRow[i][j] = -1;
            prevCol[i][j] = -1;
        }

    queue<pair<int,int>> q;
    q.push({startRow, startCol});
    visited[startRow][startCol] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    int foundR = -1, foundC = -1;

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();

        if (gridMap[r][c] == targetVal) {
            // Cek apakah ini target yang kita cari
            // Jika targetIdx == 0 artinya ambil yang pertama ditemukan (terdekat)
            foundR = r; foundC = c;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS &&
                !visited[nr][nc] && gridMap[nr][nc] != 11) {
                visited[nr][nc] = true;
                prevRow[nr][nc] = r;
                prevCol[nr][nc] = c;
                q.push({nr, nc});
            }
        }
    }

    if (foundR == -1) return "[TIDAK DITEMUKAN]";

    // Rekonstruksi jalur
    vector<string> jalur;
    int cr = foundR, cc = foundC;
    while (cr != -1 && cc != -1) {
        jalur.push_back(gridLabel[cr][cc]);
        int pr = prevRow[cr][cc];
        int pc = prevCol[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(jalur.begin(), jalur.end());

    string hasil = "";
    for (size_t i = 0; i < jalur.size(); i++) {
        // Trim spasi
        string s = jalur[i];
        while (!s.empty() && s.back() == ' ') s.pop_back();
        hasil += "[" + s + "]";
        if (i < jalur.size() - 1) hasil += " -> ";
    }
    return hasil;
}

// Posisi start point di grid
pair<int,int> posisiStartPoint(int sp) {
    if (sp == 1) return {0, 6};
    if (sp == 2) return {8, 10};
    if (sp == 3) return {24, 6};
    if (sp == 4) return {12, 0};
    return {0, 0};
}

// Posisi akhir mesin tertentu setelah BFS (node yang ditemukan)
pair<int,int> cariPosisiNode(NodeType target) {
    int targetVal = 0;
    if (target == PHO) targetVal = 10;
    else if (target == CVD) targetVal = 20;
    else if (target == PLA) targetVal = 30;
    else if (target == ION) targetVal = 40;
    else if (target == END_NODE) targetVal = 99;

    // Kembalikan posisi pertama yang ditemukan dari BFS
    // (akan ditentukan per-wafer saat routing)
    for (int r = 0; r < GRID_ROWS; r++)
        for (int c = 0; c < 4; c++)
            if (gridMap[r][c] == targetVal)
                return {r, c};
    return {-1, -1};
}

void jalankanBfsRouting(vector<Wafer>& w) {
    cout << "=== PROSES ROUTING BFS ===\n";
    cout << "Urutan mesin: PHO -> CVD -> PLA -> ION -> END\n\n";
    delayAnimasi(1000);

    // Urutan mesin yang harus dilalui
    vector<NodeType> urutanMesin = {PHO, CVD, PLA, ION, END_NODE};
    vector<string> namaMesin = {"Photolithography", "CVD", "Plasma Etcher", "Ion Implanter", "END"};

    for (auto& wafer : w) {
        cout << ">> Routing wafer: " << wafer.labelAwal << "\n";
        delayAnimasi(400);

        auto [curR, curC] = posisiStartPoint(wafer.startPoint);
        string ruteTotal = "[SP" + to_string(wafer.startPoint) + "]";

        for (size_t i = 0; i < urutanMesin.size(); i++) {
            string segmen = bfsCariJalur(curR, curC, urutanMesin[i], 0);

            // Ambil posisi node tujuan untuk langkah berikutnya
            // Parse segmen untuk ambil node terakhir (tujuan)
            // Cari node tujuan di grid
            int targetVal = 0;
            if (urutanMesin[i] == PHO) targetVal = 10;
            else if (urutanMesin[i] == CVD) targetVal = 20;
            else if (urutanMesin[i] == PLA) targetVal = 30;
            else if (urutanMesin[i] == ION) targetVal = 40;
            else if (urutanMesin[i] == END_NODE) targetVal = 99;

            // BFS ulang untuk dapatkan posisi tujuan
            bool visited[GRID_ROWS][GRID_COLS] = {};
            queue<pair<int,int>> q;
            q.push({curR, curC});
            visited[curR][curC] = true;
            int dr[] = {-1,1,0,0};
            int dc[] = {0,0,-1,1};
            int foundR = curR, foundC = curC;
            while (!q.empty()) {
                auto [r,c] = q.front(); q.pop();
                if (gridMap[r][c] == targetVal) { foundR=r; foundC=c; break; }
                for (int d=0;d<4;d++) {
                    int nr=r+dr[d], nc=c+dc[d];
                    if (nr>=0&&nr<GRID_ROWS&&nc>=0&&nc<GRID_COLS&&!visited[nr][nc]&&gridMap[nr][nc]!=11) {
                        visited[nr][nc]=true;
                        q.push({nr,nc});
                    }
                }
            }

            // Tambahkan segmen ke rute (skip node awal agar tidak duplikat)
            // Ambil bagian setelah node pertama
            size_t pos = segmen.find(" -> ");
            if (pos != string::npos)
                ruteTotal += " -> " + segmen.substr(pos + 4);
            else if (segmen != "[TIDAK DITEMUKAN]")
                ruteTotal += " -> " + segmen;

            cout << "   Menuju " << namaMesin[i] << "... " << segmen << "\n";
            delayAnimasi(300);

            curR = foundR;
            curC = foundC;
        }

        wafer.ruteBFS = ruteTotal;
        cout << "   Rute final: " << ruteTotal << "\n\n";
        delayAnimasi(500);
    }
    cout << "Routing BFS selesai!\n";
}