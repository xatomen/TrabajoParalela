/*
 Header que contiene las funciones del cálculo de la variación intermensual e interanual en soles y en pesos chilenos
 */

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <omp.h>

#ifndef TRABAJOPARALELA_CALCULATEVARIATION_H
#define TRABAJOPARALELA_CALCULATEVARIATION_H

void calculateIntermensualVariation(std::map <std::pair<int,int>, double>& valorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);
void calculateInteranualVariation(std::map <std::pair<int,int>, double>& valorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);

#endif //TRABAJOPARALELA_CALCULATEVARIATION_H
