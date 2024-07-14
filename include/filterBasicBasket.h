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

#ifndef TRABAJOPARALELA_FILTERBASICBASKET_H
#define TRABAJOPARALELA_FILTERBASICBASKET_H

std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);
std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);

#endif //TRABAJOPARALELA_FILTERBASICBASKET_H
