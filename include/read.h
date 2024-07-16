/*
 Header que contiene las funciones de lectura del programa, es decir, la lectura del archivo CSV y la lectura del archivo XLSX
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <omp.h>
#include <unordered_map>
#include "../include/libxl-4.3.0.14/include_cpp/libxl.h"

using namespace libxl;

#ifndef TRABAJOPARALELA_READ_H
#define TRABAJOPARALELA_READ_H

void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth);
void insertValueInMap(std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth, std::map<std::pair<int,int>,float>& solesToPesos);
void sequentialParseCsv(std::string& filename, std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductos);

#endif //TRABAJOPARALELA_READ_H
