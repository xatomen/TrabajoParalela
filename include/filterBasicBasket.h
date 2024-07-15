/*
 Header que contiene las funciones de filtrado de la canasta básica intermensual e interanual
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>

#ifndef TRABAJOPARALELA_FILTERBASICBASKET_H
#define TRABAJOPARALELA_FILTERBASICBASKET_H

std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);
std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);

#endif //TRABAJOPARALELA_FILTERBASICBASKET_H
