#ifndef SORTING_HPP
#define SORTING_HPP

#include "wafer.h"
#include <vector>

void selectionSortLabel(std::vector<Wafer>& w);
void sortDanKelompokkanGrade(std::vector<Wafer>& w);
void sortBerdasarkanNomorUrut(std::vector<Wafer>& w);
int binarySearchWafer(const std::vector<Wafer>& w, int targetNomor);

#endif