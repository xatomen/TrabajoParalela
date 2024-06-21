#include <iostream>
//#include "Producto.h"
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include <omp.h>
#include <map>
#include "Moneda.h"
#include "libxl-4.3.0.14/include_cpp/libxl.h"

using namespace std;
using namespace libxl;

//vector<Moneda> Monedas;

/*--- Función que permite leer el archivo xlsx por chunks (para evitar errores) ---*/
void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, vector<Moneda> &Monedas);

int main() {
    /*--- Lectura archivo xlsx ---*/
    vector<Moneda> Monedas;
    std::string filename = "/home/jorge/Escritorio/Proyectos/TrabajoParalela/PEN_CLP.xlsx";
    int startRow = 7;
//    int endRow = 0;
    int chunkSize = 100;    //Leemos de 100 en 100 para evitar errores en la lectura
    
    while(true){
        readExcelChunk(filename, startRow, chunkSize, Monedas);
        if(startRow==3734){
            break; // Termina si no hay más filas por leer
        }
    }
    //Imprimir conversión de monedas
    for(auto & Moneda : Monedas){
        Moneda.printMoneda();
    }
    std::cout << "-- Excel listo --" << std::endl;
    /*--- Fin ---*/
    
    /*--- Lectura archivo csv ---*/
    std::ifstream archivo("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string linea;
    char delimitador = ';';
    char comilla = '\"';
    getline(archivo, linea);    //Leemos el "encabezado" del texto
    
//    clock_t t;
//    t = clock();
    
    /*Utilizamos un mapa anidado que nos permita almacenar el SKU, y para cada SKU almacenar los años, y para cada fecha de cada SKU almacenar los meses, finalmente, se tiene un contador y la suma mensual del sku*/
    std::map<std::string, std::map<int,std::map<int,std::vector<float>>>> MapaProductos;
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
    /*--- Fin ---*/
    
    /*Parseo del archivo csv, utilizando únicamente los campos relevantes*/
    int i = 0;
    while(getline(archivo,linea)){
        //Si en la línea, el primer campo a leer posee el número "2" perteneciente a la fecha, entonces leemos; en caso contrario, saltamos la línea
        
        /*---- Cargamos la línea en memoria como stream ----*/
        std::stringstream stream(linea);    //Creamos una variable stringstream con el contenido de la línea
        /*--- Fin ---*/
        
        /*---- Leer Fecha ----*/
        getline(stream,linea,delimitador);  //Obtenemos la fecha en ISO de la compra
        //Solo utilizamos año y mes
        int anho = stoi(linea.substr(1,4)); //Extraer el año
        int mes = stoi(linea.substr(6,2));  //Extraer el mes
        /*--- Fin ---*/
        
        /*Nos saltamos los siguientes cinco campos que no utilizaremos por el momento*/
        for(int j=0; j<5; j++){
            getline(stream,linea,delimitador);
        }
        /*--- Fin ---*/
        
        /*---- Leer SKU del producto ----*/
        std::string sku;                                //Declaramos la variable sku que contendrá el sku del producto
        getline(stream,sku,delimitador);  //Obtenemos el Sku del producto comprado
        /*--- Fin ---*/
        
        /*--- Leemos quantity ---*/
        getline(stream,linea,delimitador);  //Cantidad
        std::string str_quantity = linea;
        str_quantity = str_quantity.substr(1,str_quantity.length()-2);    //Debemos eliminar las comillas del string para transformarlo en float
        float quantity = stof(str_quantity);                                //Obtenemos el quantity en int
        /*--- Fin ---*/
        
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
        /*--- Fin ---*/
        
        /*---- Leer amount/costo del producto ----*/
        std::string str_amount;                                             //Declaramos la variable str_amount, que corresponde al string del amount del producto
        getline(stream,str_amount,delimitador);                //Obtenemos el amount del producto
        str_amount = str_amount.substr(1,str_amount.length()-2);    //Debemos eliminar las comillas del string para transformarlo en float
        float amount = stof(str_amount);                                //Obtenemos el amount en float
        
        /*--- Ponemos los datos en el map anidado ---*/
        if(MapaProductos[sku][anho][mes].empty()){      //Si el campo está vacío, debemos inicializarlo en 0 para evitar errores después al incrementar y sumar
            MapaProductos[sku][anho][mes] = {0, 0};
        }
        MapaProductos[sku][anho][mes][0]++;
        MapaProductos[sku][anho][mes][1]+= (amount*quantity);
        i++;    //Incrementamos el contador
    }
//    t = clock() - t;
//    std::cout << "clock t = " << t << std::endl;
//    std::cout << "length = " << i << std::endl;
    archivo.close();
    std::cout << "--- Parseo listo ---" << std::endl;
    getchar();
    /*--- Fin ---*/
    
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
    /*--- Fin ---*/

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
    /*--- Fin ---*/

    /*Productos de la canasta básica*/
    int r = 0;
    for(const auto& Sku : MapaProductos){
        std::cout << Sku.first << " | " << std::endl;
        for(const auto& Anho : Sku.second){
            std::cout << "\tAnho: " << Anho.first << " | -> meses: " << Anho.second.size() << std::endl;
            for(const auto& Mes : Anho.second){
                    std::cout << "\t\tMes: " << Mes.first << " -> contador: " << Mes.second[0] << " - suma: " << Mes.second[1] << std::endl;
            }
        }
        r++;
    }
    std::cout << "r: " << r << std::endl;   //Contador de productos distintos que pertenecen a la canasta básica
    getchar();
    /*--- Fin ---*/
    /*--- Fin ---*/
    
    /*Cálculo de la inflación*/
    
    
    
    /*--- Fin ---*/
    
    
    return 0;
}

void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, vector<Moneda> &Monedas){
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
                    
                    //Creamos un objeto y lo colocamos en el vector
                    Moneda moneda;
                    moneda.setPEN(day,month,year,num);
                    Monedas.push_back(moneda);
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