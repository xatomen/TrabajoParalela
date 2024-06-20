//
// Created by jorge on 20/06/24.
//

#ifndef TRABAJOPARALELA_MONEDA_H
#define TRABAJOPARALELA_MONEDA_H

#include <iostream>

class Moneda {
private:
    int dia;
    int mes;
    int anho;
    float CLP;
public:
    Moneda();
    virtual ~Moneda();
    void setPEN(int dia, int mes, int anho, double CLPtoPEN);
    double getCLP();
    void printMoneda();
};


#endif //TRABAJOPARALELA_MONEDA_H
