//
// Created by jorge on 14/07/24.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <chrono>
#include <unordered_map>
#include "../include/libxl-4.3.0.14/include_cpp/libxl.h"

#ifndef TRABAJOPARALELA_CALCULATEVARIATION_H
#define TRABAJOPARALELA_CALCULATEVARIATION_H

void calculateIntermensualVariation(std::map <std::pair<int,int>, double>& valorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);
void calculateInteranualVariation(std::map <std::pair<int,int>, double>& valorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);

#endif //TRABAJOPARALELA_CALCULATEVARIATION_H
