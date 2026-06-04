#ifndef BFS_HPP
#define BFS_HPP

#include "wafer.h"
#include <vector>
#include <string>

// Tipe node pada grid
enum NodeType {
    EMPTY, SP1, SP2, SP3, SP4,
    PHO, CVD, PLA, ION, WET, END_NODE
};

struct Node {
    int row, col;
    NodeType type;
    int index; // indeks mesin (1,2,3) untuk tipe yang sama
};

// Dimensi grid sesuai draw.io: 7 baris x 5 kolom
#define GRID_ROWS 7
#define GRID_COLS 5

extern int gridMap[GRID_ROWS][GRID_COLS];
extern std::string gridLabel[GRID_ROWS][GRID_COLS];

void inisialisasiGrid();
void tampilkanPeta();
std::string bfsCariJalur(int startRow, int startCol, NodeType target, int targetIdx);
void jalankanBfsRouting(std::vector<Wafer>& w);

#endif