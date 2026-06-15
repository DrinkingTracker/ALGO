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
    {  4,  0,  0,  0, 50,  0, 40,  0, 30, 11, 11 }, // row12 : SP4, ION2, PLA2
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
{"SP4 ", "    ", "    ", "    ", "WET2", "    ", "ION2", "    ", "PLA2", "WALL", "WALL"},
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
    cout << "                    [SP1]\n";
    cout << "                      |\n";
    cout << "            ----------+----------\n";
    cout << "            |         |         |\n";
    cout << "          [CVD1]---[ION1]---[CVD2]\n";
    cout << "            |         |         |\n";
    cout << "            +---------+---------+\n";
    cout << "            |         |         |\n";
    cout << "          [PLA1]---[PHO1]---[WET1]---[SP2]\n";
    cout << "            |         |         |\n";
    cout << "       +----+---------+---------+\n";
    cout << "       |    |         |         |\n";
    cout << "[SP4]--+--[WET2]---[ION2]---[PLA2]\n";
    cout << "       |    |         |         |\n";
    cout << "       +----+---------+---------+\n";
    cout << "            |         |         |\n";
    cout << "          [PLA3]---[CVD3]---[WET3]---[END]\n";
    cout << "            |         |         |\n";
    cout << "            +---------+---------+\n";
    cout << "            |         |         |\n";
    cout << "          [PHO2]---[ION3]---[PHO3]\n";
    cout << "            |         |         |\n";
    cout << "            ----------+----------\n";
    cout << "                      |\n";
    cout << "                    [SP3]\n";
    cout << "\nUrutan mesin: PHO -> WET -> CVD -> PLA -> WET -> ION -> END\n";
    cout << "Setiap node terhubung ke atas/bawah/kiri/kanan\n";
}

// Helper: trim spasi dari string
static string trimStr(string s) {
    while (!s.empty() && s.back() == ' ') s.pop_back();
    while (!s.empty() && s.front() == ' ') s.erase(s.begin());
    return s;
}

// Helper: apakah node ini bermakna (bukan kosong/wall)
static bool nodeBermakna(int r, int c) {
    int v = gridMap[r][c];
    return (v != 0 && v != 11);
}

// BFS dengan globalVisited untuk mencegah node dikunjungi ulang lintas segmen
// Mengembalikan {jalurString, foundRow, foundCol}
struct BFSResult {
    string jalur;
    int foundR, foundC;
};

BFSResult bfsCariJalurLengkap(int startRow, int startCol, NodeType target,
                               bool globalVisited[GRID_ROWS][GRID_COLS]) {
    int targetVal = 0;
    if (target == PHO) targetVal = 10;
    else if (target == CVD) targetVal = 20;
    else if (target == PLA) targetVal = 30;
    else if (target == ION) targetVal = 40;
    else if (target == WET) targetVal = 50;
    else if (target == END_NODE) targetVal = 99;

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
            foundR = r; foundC = c;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < GRID_ROWS && nc >= 0 && nc < GRID_COLS
                && !visited[nr][nc]
                && gridMap[nr][nc] != 11
                && !globalVisited[nr][nc]) {
                visited[nr][nc] = true;
                prevRow[nr][nc] = r;
                prevCol[nr][nc] = c;
                q.push({nr, nc});
            }
        }
    }

    if (foundR == -1) return {"[TIDAK DITEMUKAN]", -1, -1};

    // Rekonstruksi jalur
    vector<pair<int,int>> jalurPos;
    int cr = foundR, cc = foundC;
    while (cr != -1 && cc != -1) {
        jalurPos.push_back({cr, cc});
        int pr = prevRow[cr][cc];
        int pc = prevCol[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(jalurPos.begin(), jalurPos.end());

    // Tandai HANYA node tujuan (node terakhir = foundR,foundC) sebagai globalVisited
    // Node mesin yang dilewati sebagai koridor tetap bisa digunakan segmen berikutnya
    globalVisited[foundR][foundC] = true;

    // Bangun string jalur — hanya tampilkan node bermakna
    string hasil = "";
    bool first = true;
    for (auto [r, c] : jalurPos) {
        if (!nodeBermakna(r, c)) continue;
        string lbl = trimStr(gridLabel[r][c]);
        if (lbl.empty()) continue;
        if (!first) hasil += " -> ";
        hasil += "[" + lbl + "]";
        first = false;
    }

    return {hasil, foundR, foundC};
}

// Wrapper lama untuk kompatibilitas (tanpa globalVisited)
string bfsCariJalur(int startRow, int startCol, NodeType target, int targetIdx) {
    bool dummy[GRID_ROWS][GRID_COLS] = {};
    return bfsCariJalurLengkap(startRow, startCol, target, dummy).jalur;
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
        for (int c = 0; c < GRID_ROWS; c++)
            if (gridMap[r][c] == targetVal)
                return {r, c};
    return {-1, -1};
}

void jalankanBfsRouting(vector<Wafer>& w) {
    cout << "=== PROSES ROUTING BFS ===\n";
    cout << "Urutan mesin: PHO, WET, CVD, PLA, WET, ION, END_NODE\n\n";
    delayAnimasi(1000);

    vector<NodeType> urutanMesin = {PHO, WET, CVD, PLA, WET, ION, END_NODE};
    vector<string> namaMesin = {"Photolithography", "Wet Bench", "CVD", "Plasma Etcher", "Wet Bench", "Ion Implanter", "END"};

    for (auto& wafer : w) {
        cout << ">> Routing wafer: " << wafer.labelAwal << "\n";
        delayAnimasi(400);

        auto [curR, curC] = posisiStartPoint(wafer.startPoint);
        string ruteTotal = "[SP" + to_string(wafer.startPoint) + "]";

        // globalVisited mencegah node dikunjungi ulang lintas segmen
        bool globalVisited[GRID_ROWS][GRID_COLS] = {};
        globalVisited[curR][curC] = true;

        for (size_t i = 0; i < urutanMesin.size(); i++) {
            BFSResult hasil = bfsCariJalurLengkap(curR, curC, urutanMesin[i], globalVisited);

            // Tambahkan ke rute total, skip node pertama (sudah ada)
            size_t pos = hasil.jalur.find(" -> ");
            if (pos != string::npos)
                ruteTotal += " -> " + hasil.jalur.substr(pos + 4);
            else if (hasil.jalur != "[TIDAK DITEMUKAN]" && hasil.jalur != "")
                ruteTotal += " -> " + hasil.jalur;

            cout << "   Menuju " << namaMesin[i] << "... " << hasil.jalur << "\n";
            delayAnimasi(300);

            if (hasil.foundR != -1) {
                curR = hasil.foundR;
                curC = hasil.foundC;
            }
        }

        wafer.ruteBFS = ruteTotal;
        cout << "   Rute final: " << ruteTotal << "\n\n";
        delayAnimasi(500);
    }
    cout << "Routing BFS selesai!\n";
}