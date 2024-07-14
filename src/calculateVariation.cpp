//
// Created by jorge on 14/07/24.
//

#include "../include/calculateVariation.h"

/*--- Función que permite calcular la variación intermensual de cada año ---*/
void calculateIntermensualVariation(std::map <std::pair<int,int>, double>& valorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos){
/*--- Calculamos la variación intermensual de la canasta básica ---*/
    std::cout << "--- Variación intermensual ---" << std::endl;
    //Inicializamos algunas variables que utilizaremos
    double mesBase;
    double mesBaseClp;
    bool flag = false;
    double varAcumulado = 0;
    double varAcumuladoClp = 0;
    int anhoActual;
    //En el siguiente for, lo que hacemos es recorrer todos los meses y años del valor de canasta mensual
    for(auto& vcm : valorCanastaMensual){
        /*Si es el primer mes, lo usamos como mes base inicial*/
        if(!flag){                                                                                    //Si el flag es falso, entonces es el primer mes del primer año
            anhoActual = vcm.first.first;                                                             //El año actual será el mes base
            mesBase = vcm.second;                                                                     //El mes base será el primer mes
            mesBaseClp = mesBase * solesToPesos[std::pair{vcm.first.first,vcm.first.second}];   //El mes base será el primer mes
            flag = true;                                                                              //Cambiamos el flag para que no volvamos a entrar a la condición
        }
        /*Verificamos si cambiamos de año*/
        if(anhoActual != vcm.first.first){                                                              //Si el año actual (anterior) es distinto al nuevo, tuvimos un cambio de año, por lo que reseteamos las variables
            mesBase = vcm.second;                                                                       //Mes base será igual nuevo mes base
            mesBaseClp = mesBase * solesToPesos[std::pair{vcm.first.first,vcm.first.second}];     //Mes base será igual nuevo mes base
            anhoActual = vcm.first.first;                                                               //Año actual será igual al nuevo año
            varAcumulado = 0;                                                                           //Reseteamos la variación acumulada
            varAcumuladoClp = 0;                                                                        //Reseteamos la variación acumulada
            std::cout << "--- Nuevo año ---" << std::endl;
        }
        double mesActual = vcm.second;                                          //Asignamos el mes actual
        double mesActualClp = mesActual * solesToPesos[std::pair{vcm.first.first,vcm.first.second}];
        double variacionIntermensual = ((mesActual/mesBase) - 1)*100;           //Calculamos la variación intermensual entre ambos meses
        double variacionIntermensualClp = ((mesActualClp/mesBaseClp) - 1)*100;  //Calculamos la variación intermensual entre ambos meses
        varAcumulado += variacionIntermensual;                                  //Sumamos la variación
        varAcumuladoClp += variacionIntermensualClp;                            //Sumamos la variación
        std::cout << vcm.first.first << "/" << vcm.first.second << "\tVariación intermensual: " << variacionIntermensual << "%\tVariación acumulada: " << varAcumulado << "%\tCLP: " << solesToPesos[std::pair{vcm.first.first,vcm.first.second}] << "\tVariación intermensual CLP: " << variacionIntermensualClp << "%\tVariación acumulada: " << varAcumuladoClp << "%" << std::endl;
        mesBase = mesActual;                                                    //Actualizamos el mes base con el mes que acabamos de usar
        mesBaseClp = mesActualClp;
    }
    /*--- Fin ---*/
    std::cout << std::endl;
}

/*--- Función que permite calcular la variación interanual de los años en total ---*/
void calculateInteranualVariation(std::map <std::pair<int,int>, double>& valorCanastaAnual, std::map<std::pair<int,int>,float>& solesToPesos){
/*--- Calculamos la variación intermensual de la canasta básica ---*/
    std::cout << "--- Variación interanual ---" << std::endl;
    //Inicializamos algunas variables que utilizaremos
    double mesBase;
    double mesBaseClp;
    bool flag = false;
    double varAcumulado = 0;
    double varAcumuladoClp = 0;
    //En el siguiente for, lo que hacemos es recorrer todos los meses y años del valor de canasta mensual
    for(auto& vcm : valorCanastaAnual){
        /*Si es el primer mes, lo usamos como mes base inicial*/
        if(!flag){                                                                                    //Si el flag es falso, entonces es el primer mes del primer año
            mesBase = vcm.second;                                                                     //El mes base será el primer mes
            mesBaseClp = mesBase * solesToPesos[std::pair{vcm.first.first,vcm.first.second}];   //El mes base será el primer mes
            flag = true;                                                                              //Cambiamos el flag para que no volvamos a entrar a la condición
        }
        double mesActual = vcm.second;                                          //Asignamos el mes actual
        double mesActualClp = mesActual * solesToPesos[std::pair{vcm.first.first,vcm.first.second}];
        double variacionIntermensual = ((mesActual/mesBase) - 1)*100;           //Calculamos la variación intermensual entre ambos meses
        double variacionIntermensualClp = ((mesActualClp/mesBaseClp) - 1)*100;  //Calculamos la variación intermensual entre ambos meses
        varAcumulado += variacionIntermensual;                                  //Sumamos la variación
        varAcumuladoClp += variacionIntermensualClp;                            //Sumamos la variación
        std::cout << vcm.first.first << "/" << vcm.first.second << "\tVariación intermensual: " << variacionIntermensual << "%\tVariación acumulada: " << varAcumulado << "%\tCLP: " << solesToPesos[std::pair{vcm.first.first,vcm.first.second}] << "\tVariación intermensual CLP: " << variacionIntermensualClp << "%\tVariación acumulada: " << varAcumuladoClp << "%" << std::endl;
        mesBase = mesActual;                                                    //Actualizamos el mes base con el mes que acabamos de usar
        mesBaseClp = mesActualClp;                                              //Actualizamos el mes base con el mes que acabamos de usar
    }
    std::cout << std::endl;
}