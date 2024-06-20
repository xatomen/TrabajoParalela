//
// Created by jorge on 20/06/24.
//

#include "Moneda.h"

Moneda::Moneda() {

}

Moneda::~Moneda() {

}

void Moneda::setPEN(int dia, int mes, int anho, double CLPtoPEN) {
    this->dia = dia;
    this->mes = mes;
    this->anho = anho;
    this->CLP = CLPtoPEN;
}

double Moneda::getCLP() {
    return CLP;
}

void Moneda::printMoneda() {
    std::cout << dia << "-" << mes << "-" << anho << " | " << CLP << std::endl;
}
