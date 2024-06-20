#include <iostream>
#include "Producto.h"
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include <omp.h>
#include <map>
#include "libxl-4.3.0.14/include_cpp/libxl.h"

using namespace std;
using namespace libxl;

void readExcelChunk(const std::string& filename, int& startRow, int chunkSize) {
    Book* book = xlCreateXMLBook();
    if(book){
        if(book->load(filename.c_str())){ // Carga el archivo Excel
            Sheet* sheet = book->getSheet(0); // Obtiene la primera hoja
            if(sheet){
                int rowCount = sheet->lastRow(); // Obtiene el número de filas
                int endRow = std::min(startRow + chunkSize, rowCount); // Determina la última fila a leer en este chunk
                for(int row = startRow; row <= endRow; ++row){ // Itera sobre las filas
                    double dateValue = sheet->readNum(row, 0);
                    int year, month, day;
                    book->dateUnpack(dateValue, &year, &month, &day);
                    double num = sheet->readNum(row, 1); // Lee el contenido de la celda en la segunda columna como número
                    // Formatea e imprime la fecha
                    std::cout << "actual: " << row << " final: " << endRow << " - ";
                    std::cout << std::setfill('0') << std::setw(4) << year << "-"
                              << std::setfill('0') << std::setw(2) << month << "-"
                              << std::setfill('0') << std::setw(2) << day << " "
                              << num << std::endl;
                }
                startRow = endRow;
            }
            else{
                std::cerr << "Error: No se pudo abrir la hoja 0." << std::endl;
            }
        }
        else{
            std::cerr << "Error: No se pudo cargar el archivo " << filename << "." << std::endl;
        }
        book->release();
    }
    else{
        std::cerr << "Error: No se pudo crear el libro." << std::endl;
    }
}

int main() {
    
    /*Lectura archivo xlsx*/
    
    std::string filename = "/home/jorge/Escritorio/Proyectos/TrabajoParalela/PEN_CLP.xlsx";
    int startRow = 7;
//    int endRow = 0;
    int chunkSize = 100;    //Leemos de 100 en 100 para evitar errores en la lectura
    
    while(true){
        readExcelChunk(filename, startRow, chunkSize);
        if(startRow==3734){
            break; // Termina si no hay más filas por leer
        }
    }
    
    std::cout << "-- Excel listo --" << std::endl;
    
    /*Lectura archivo csv*/
    std::ifstream archivo("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string linea;
    char delimitador = ';';
    char comilla = '\"';
    
    getline(archivo, linea);    //Leemos el "encabezado" del texto
    
//    clock_t t;
//    t = clock();
    
    /*Utilizamos un mapa anidado que nos permita almacenar el SKU, y para cada SKU almacenar los años, y para cada fecha de cada SKU almacenar los meses*/
    std::map<std::string, std::map<int,std::map<int,int>>> MapaProductos;
    
    //mapa fecha-pesos para excel
    //usar mediana para los precios (criterio) y la variacion de precios (mediana)
    
    /*Utilizaremos este while para obtener la cantidad de líneas, para luego paralelizar la lectura*/
    int k=0;
    std::ifstream arch_csv("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string line;
    while(getline(arch_csv,line)){
        k++;
    }
    std::cout << "k: " << k << std::endl;
    getchar();
    
    /*Parseo del archivo csv, utilizando únicamente los campos relevantes*/
    int i = 0;
    while(getline(archivo,linea)){
        //Si en la línea, el primer campo a leer posee el número "2" perteneciente a la fecha, entonces leemos; en caso contrario, saltamos la línea
        /*---- Cargamos la línea en memoria como stream ----*/
        std::stringstream stream(linea);    //Creamos una variable stringstream con el contenido de la línea
        
        /*---- Leer Fecha ----*/
        getline(stream,linea,delimitador);  //Obtenemos la fecha en ISO de la compra
        //Solo utilizamos año y mes
        int anho = stoi(linea.substr(1,4)); //Extraer el año
        int mes = stoi(linea.substr(6,2));  //Extraer el mes
        
        /*Nos saltamos los siguientes cinco campos que no utilizaremos por el momento*/
        for(int j=0; j<5; j++){
            getline(stream,linea,delimitador);
        }
        
        /*---- Leer SKU del producto ----*/
        std::string sku;                                //Declaramos la variable sku que contendrá el sku del producto
        getline(stream,sku,delimitador);  //Obtenemos el Sku del producto comprado
        
        /*Nos saltamos los siguientes dos campos que no utilizaremos por el momento*/
        getline(stream,linea,delimitador);  //Cantidad
        
        /*
         * Para el campo nombre:
         * Si no tiene nombre, no tiene comillas, por lo tanto, solo buscamos el siguiente delimitador
         * Si el siguiente carácter es una comilla, tiene nombre, por lo que buscamos la siguiente comilla y luego, buscamos el delimitador (para evitar tomar un ";" dentro del nombre del producto)
        */
        std::string name;
        if(linea[0]==comilla){
            getline(stream,name, comilla);     //Buscamos las primeras comillas
            getline(stream,name, comilla);      //Buscamos las segundas comillas
            /*
             *Si no se encuentran las segundas comillas, entonces deberemos saltar de línea y continuar
             */
            while(!stream.good()){  //Mientras el stream no sea correcto, no saldremos del while
                i++;    //Aumentamos el contador de filas porque saltaremos a la siguiente línea (para evitar inconsistencias con el while anterior)
                getline(archivo,linea); //Saltamos a la siguiente línea
                stream.clear();                             //Limpiamos el stream
                stream.str(linea);                       //Asignamos la nueva línea
                getline(stream,name, comilla);  //Volvemos a buscar la comilla faltante
            }
        }
        getline(stream,name,delimitador);  //Nombre
        
        /*---- Leer amount/costo del producto ----*/
        std::string str_amount;                                             //Declaramos la variable str_amount, que corresponde al string del amount del producto
        getline(stream,str_amount,delimitador);                //Obtenemos el amount del producto
        //Por ahora no utilizamos el amount
        str_amount = str_amount.substr(1,str_amount.length()-2);    //Debemos eliminar las comillas del string para transformarlo en float
        float amount = stof(str_amount);                                //Obtenemos el amount en float
        
        MapaProductos[sku][anho][mes]++;
        i++;    //Incrementamos el contador
    }
    std::cout << "listo" << std::endl;
//    t = clock() - t;
//    std::cout << "clock t = " << t << std::endl;
//    std::cout << "length = " << i << std::endl;
    archivo.close();

    getchar();
    
    /*Obtención canasta básica*/
    
    /*Recorrer y eliminar años con menos de 4 meses*/
    for (auto& Sku : MapaProductos) {
//        std::cout << Sku.first << " | " << std::endl;
        auto it = Sku.second.begin();   //Iterador
        while(it != Sku.second.end()){            //Iteramos mientras no lleguemos al final
            if(it->second.size() < 4){          //Si el año no tiene a lo menos 4 meses, lo eliminamos ///REEEVISARRRR!!! que sean 12 meses
                it = Sku.second.erase(it); // Eliminar año y avanzar el iterador
            }
            else{
                ++it; // Solo incrementamos el iterador
            }
        }
    }
    
    /*Recorrer y eliminar SKU sin años*/
    auto skuIt = MapaProductos.begin(); //Iterador
    while(skuIt != MapaProductos.end()){          //Iteramos mientras no lleguemos al final
        if(skuIt->second.empty()){                 //Si el SKU no tiene un elemento "par", entonces lo eliminamos
            skuIt = MapaProductos.erase(skuIt); // Eliminar SKU y avanzar el iterador
        }
        else{
            ++skuIt; // Solo incrementamos el iterador
        }
    }
    
    /*Productos de la canasta básica*/
    int r = 0;
    for(const auto& Sku : MapaProductos){
        std::cout << Sku.first << " | " << std::endl;
        for(const auto& Anho : Sku.second){
            std::cout << "\tAnho: " << Anho.first << " | -> meses: " << Anho.second.size() << std::endl;
            for(const auto& Mes : Anho.second){
                    std::cout << "\t\tMes: " << Mes.first << " -> " << Mes.second << std::endl;
            }
        }
        r++;
    }
    std::cout << "r: " << r << std::endl;
    getchar();

    /*Cálculo de la inflación*/
    
    
    
    
    return 0;
}

