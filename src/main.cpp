/*------------------------------------------------------
 *                  Trabajo 1
 *
 * Asignatura: Computación paralela y distribuida
 * Sección: 411
 * Profesor: Sebastián Salazar Molina
 *
 * Integrantes: - Jorge Luis Gallardo Contreras
 *              - Fernando Tomás Maldonado Rodríguez
 *              - Felipe Ignacio Vera Andrade
 *
 * ----------------------------------------------------*/

#include <iostream>
#include <map>
#include <chrono>
#include <unordered_map>
#include <omp.h>

#include "../include/filterBasicBasket.h"
#include "../include/read.h"
#include "../include/calculateVariation.h"

int main(int argc, char *argv[]) {
    
    /*--- Iniciamos cronómetro ---*/
    auto start = std::chrono::high_resolution_clock::now(); //Iniciamos cronómetro
    /*--- Fin ---*/
    
    /*--- Mapas ---*/
    std::map<std::pair<int,int>,double> penToClp;       //Mapa que permite guardar la suma en pesos chilenos de cada día de cada mes de la transformación SOL->CLP <pair(anho,mes),pesos>
    std::map<std::pair<int,int>,int> daysPerMonth;      //Mapa que permite guardar la cantidad de días de cada mes de la transformación SOL->CLP <pair(anho,mes),dias>
    std::map<std::pair<int,int>,float> solesToPesos;    //Mapa que contiene la transformación SOL->CLP promedio de cada mes
    
    /*--- Este unordered map aloja cada sku, en donde cada sku está asociado a un mapa que contiene el año, y cada año tiene asociado un mapa que contiene los meses,   ---*/
    /*--- y cada mes tiene asociado un vector que aloja dos datos, el primero es la cantidad de veces que se compró el sku en el mes                                    ---*/
    /*--- y el segundo es la suma total en soles de cada mes, estos dos últimos valores son para obtener el precio promedio del sku de cada mes de cada año             ---*/
    std::unordered_map<std::string, std::map<int, std::map<int, std::vector<float>>>> mapaProductos;
    
#pragma omp parallel sections
    {
#pragma omp section
        {
            /*--- Lectura archivo xlsx ---*/
#pragma omp critical
            {
                std::cout << "-- Lectura excel --" << std::endl;
            }
            std::string filename = argv[1];
            int startRow = 7;
            int chunkSize = 100;    //Leemos de 100 en 100 para evitar errores en la lectura
            
            while(true){
                readExcelChunk(filename, startRow, chunkSize, penToClp, daysPerMonth);
                if(startRow==0){    //Si termina el startRow queda en 0 al iterar, entonces hemos finalizado de leer el excel
                    break;          // Termina si no hay más filas por leer
                }
            }
            
            /*--- Guardamos la transformación de moneda en un map ---*/
            insertValueInMap(penToClp, daysPerMonth, solesToPesos);
            /*--- Imprimimos la transformación de moneda por mes ---*/
            std::cout << "-- Excel listo --" << std::endl;
            /*--- Fin ---*/
        }
#pragma omp section
        {
            /*--- Parseo CSV ---*/
            /*Utilizamos un mapa anidado que nos permita almacenar el SKU, y para cada SKU almacenar los años, y para cada fecha de cada SKU almacenar los meses, finalmente, se tiene un contador y la suma mensual del sku*/
            std::string archivo = argv[2];
            sequentialParseCsv(archivo,mapaProductos);
            /*--- Fin ---*/
        }
    }
    /*--- Utilizamos un barrier para determinar que se deben terminar las secciones anteriores por completo para avanzar ---*/
#pragma omp barrier
#pragma omp parallel sections
    {
#pragma omp section
        {
            /*--- Obtención de la canasta básica para la variación intermensual ---*/
            /*--- Este mapa contiene la suma de los precios promedio de cada producto en cada año-mes ---*/
            std::map <std::pair<int,int>, double> valorCanastaMensual = filterBasicBasketForIntermensualVariation(mapaProductos);
            /*--- Fin ---*/
            /*--- Calculamos la variación intermensual de la canasta básica ---*/
            calculateIntermensualVariation(valorCanastaMensual, solesToPesos);
            /*--- Fin ---*/
        }
#pragma omp section
        {
            /*--- Obtención de la canasta básica para la variación interanual ---*/
            /*--- Este mapa contiene la suma de los precios promedio de cada producto en cada año-mes ---*/
            std::map <std::pair<int,int>, double> valorCanastaAnual = filterBasicBasketForInteranualVariation(mapaProductos);
            /*--- Fin ---*/
            /*--- Calculamos la variación interanual de la canasta básica ---*/
            calculateInteranualVariation(valorCanastaAnual, solesToPesos);
            /*--- Fin ---*/
        }
    }
    
    /*--- Finalizamos cronómetro ---*/
    auto end = std::chrono::high_resolution_clock::now();                             // Registrar el tiempo de finalización
    std::chrono::duration<double> duration = end - start;                                           // Calcular la duración
    std::cout << "--- Tiempo de ejecución: " << duration.count() << " segundos ---" << std::endl;   // Imprimir la duración en segundos
    /*--- Fin ---*/
    
    return 0;
}





