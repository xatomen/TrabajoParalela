#include <iostream>
//#include "Producto.h"
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <omp.h>
#include <map>
#include "Moneda.h"
#include "libxl-4.3.0.14/include_cpp/libxl.h"
//xlsxio
//ANOTAR PREGUNTAS CUANDO LAS TENGA!!!!!!!

using namespace std;
using namespace libxl;

std::map<std::pair<int,int>,double> PEN_CLP;  //pair(anho,mes),pesos
std::map<std::pair<int,int>,int> FECHAS; //pair(anho,mes),dias
std::map<std::pair<int,int>,float> SolesToPesos;

/*--- Función que permite leer el archivo xlsx por chunks (para evitar errores) ---*/
void readExcelChunk(const std::string& filename, int& startRow, int chunkSize);

int main() {
    /*--- Lectura archivo xlsx ---*/
    std::cout << "-- Lectura excel --" << std::endl;
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
    /*--- Guardamos la transformación de moneda en un map*/
    auto pesos = PEN_CLP.begin();
    auto dias = FECHAS.begin();
    for(; pesos!=PEN_CLP.end() && dias!=FECHAS.end(); pesos++, dias++){
//        std::cout << pesos->first.first << "/" << pesos->first.second << "\t- dias: " << dias->second << " suma: " << pesos->second << std::endl;
        std::pair<int,int> fecha(pesos->first.first,pesos->first.second);   //Asignamos el promedio de pesos a cada mes
        SolesToPesos[fecha] = pesos->second/dias->second;
    }
    /*--- Imprimimos la transformación de moneda por mes ---*/
    for(auto& map: SolesToPesos){
        std::cout << map.first.first << "/" << map.first.second << "\t- " << map.second << std::endl;
    }
    std::cout << "-- Excel listo --" << std::endl;
//    getchar();
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
    std::map<std::string, std::map<int, std::map<int, std::vector<float> > > > MapaProductos;
    //mapa fecha-pesos para excel
    //usar mediana para los precios (criterio) y la variacion de precios (mediana)
    
    /*Utilizaremos este while para obtener la cantidad de líneas, para luego paralelizar la lectura*/
    std::cout << "-- Cargado de líneas --" << std::endl;
    int k=0;
    std::ifstream arch_csv("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string line;
    while(getline(arch_csv,line)){
        k++;
    }
    std::cout << "k: " << k << std::endl;
    std::cout << "-- Cargado de lineas listo --" << std::endl;
//    getchar();
    /*--- Fin ---*/
    
    /*Parseo del archivo csv, utilizando únicamente los campos relevantes*/
    std::cout << "-- Parseo csv --" << std::endl;
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
        int quantity = stoi(str_quantity);                                //Obtenemos el quantity en int
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
        for(int qty=0; qty<quantity; qty++){    //Sumamos la cantidad de veces que se adquirió el producto/sku
            MapaProductos[sku][anho][mes][0]++;
            MapaProductos[sku][anho][mes][1]+= amount;
        }
        i++;    //Incrementamos el contador
        if(i==10000000)break;
    }
//    t = clock() - t;
//    std::cout << "clock t = " << t << std::endl;
//    std::cout << "length = " << i << std::endl;
    archivo.close();
    std::cout << "--- Parseo listo ---" << std::endl;
//    getchar();
    /*--- Fin ---*/
    
    /*--- Obtención canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos antes de obtener la canasta básica para verificar ---*/
    std::map<std::pair<int,int>,std::map<std::string,int>> fechaSku1; //Mapa que contiene fecha (año, mes) y cada fecha tiene el sku
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);
                // Si la clave ya existe, agregar la cantidad al valor existente
                fechaSku1[fecha][Sku.first]++;
            }
        }
    }
    /*--- Fin ---*/
    
    /*--- Guardamos en un mapa, la cantidad de meses de cada año ---*/
    std::map<int, int> mesesPorAnho;    // mesesPorAnho {año,mes}
    for (const auto& entry : fechaSku1) {
        mesesPorAnho[entry.first.first]++;
    }
    /*--- Fin ---*/

    getchar();
    
    /*Recorrer y eliminar años con menos de 12 meses*/
    int mesesAnho;
    for (auto& Sku : MapaProductos) {
//        std::cout << Sku.first << " | " << std::endl;
        auto it = Sku.second.begin();   //Declaramos e inicializamos un iterador sobre el año
        /*--- En este bloque buscamos encontrar la cantidad de meses ---*/
        int anho = it->first;   //it->first corresponde al año actual
        for(auto& entry: mesesPorAnho){  //Recorremos el map, de los meses por año, completo
            if(entry.first == anho){                        //Buscamos el año actual que es igual al año del mapa
                mesesAnho = entry.second;                   //Recuperamos la cantidad de meses del año buscado en el mapa
            }
        }
        /*--- Fin ---*/
        /*--- En este while buscamos los SKUs que no se encuentren en todos los meses del año y los eliminamos ---*/
        while(it != Sku.second.end()){              //Iteramos mientras no lleguemos al año final
            if(it->second.size() < mesesAnho){      //Si el año no tiene a lo menos 4 meses, lo eliminamos ///REEEVISARRRR!!! que sean 12 meses
                it = Sku.second.erase(it);  // Eliminar solo el año y avanzar el iterador
            }
            else{
                ++it; // Solo incrementamos el iterador
            }
        }
        /*--- Fin ---*/
    }
    /*--- Fin ---*/

    /*--- Recorrer y eliminar SKU sin años (ya que anteriormente solo eliminamos los años) ---*/
    auto skuIt = MapaProductos.begin();     //Inicializamos un nuevo iterador
    while(skuIt != MapaProductos.end()){                //Iteramos mientras no lleguemos al final
        if(skuIt->second.empty()){                      //Si el SKU no tiene un elemento "par", que es el/los años, entonces lo eliminamos
            skuIt = MapaProductos.erase(skuIt); // Eliminar SKU y avanzar el iterador
        }
        else{
            ++skuIt;                                    // Solo incrementamos el iterador
        }
    }
    /*--- Fin ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos después de obtener la canasta básica para verificar ---*/
    /*--- Con esto obtenemos la canasta básica por año ---*/
    std::map<std::pair<int,int>,std::map<std::string,int>> fechaSku2; //Mapa que contiene fecha año, mes y cada fecha tiene el sku
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);
                fechaSku2[fecha][Sku.first]++;
            }
        }
    }
    /*--- Fin ---*/
    
    /*--- Verificación temporal ---*/
    int contador;
    contador=0;
    for(const auto& Fecha: fechaSku1){
        std::cout << Fecha.first.first << "/" << Fecha.first.second << std::endl;
        for(const auto& Sku : Fecha.second){
//            std::cout << "\t" << Sku.first << "\t- cantidad:" << Sku.second << std::endl;
        }
        contador++;
    }
    std::cout << "Contador 1: " << contador << std::endl;
//    getchar();
    
    contador=0;
    int contador2;
    std::vector<std::string> Vector4;
    std::vector<std::string> Vector5;
    for(const auto& Fecha: fechaSku2){
        contador2=0;
        std::cout << Fecha.first.first << "/" << Fecha.first.second << std::endl;
        for(const auto& Sku : Fecha.second){
//            std::cout << "\t" << Sku.first << "\t- cantidad:" << Sku.second << std::endl;
            if(Fecha.first.second==4)Vector4.push_back(Sku.first);
            if(Fecha.first.second==5)Vector5.push_back(Sku.first);
            contador2++;
        }
        std::cout << "contador2: " << contador2 << std::endl;
//        contador2=0;
        contador++;
    }
    std::cout << "Contador 2: " << contador << std::endl;
    if(Vector4==Vector5){std::cout<<"SON IGUALES"<<std::endl;}
    getchar();
    /*--- Fin ---*/
    
    //Guardar canasta mensual/anual en archivo
    
    /*Productos de la canasta básica*/
    int r = 0;
    std::map <std::pair<int,int>, int> total_productos_por_fecha;
    std::map <std::pair<int,int>, double> total_suma_por_fecha;
    std::map <std::pair<int,int>, double> IPC_fecha;
    
    for(const auto& Sku : MapaProductos){
//        std::cout << Sku.first << " | " << std::endl;
        for(const auto& Anho : Sku.second){
//            std::cout << "\tAnho: " << Anho.first << " | -> meses: " << Anho.second.size() << std::endl;
            for(const auto& Mes : Anho.second){
                float prom = Mes.second[1]/Mes.second[0];
//                std::cout << "\t\tMes: " << Mes.first << " -> contador: " << Mes.second[0] << " - suma: " << Mes.second[1] << " - precio promedio: " << prom << std::endl;
            }
        }
        r++;
    }
    std::cout << "Cantidad de SKUs diferentes: " << r << std::endl;   //Contador de productos distintos que pertenecen a la canasta básica
//    getchar();
    /*--- Fin ---*/
    //cambiar estrategia
    
    //Revisar y reformular si es necesario!!!!!!11
    
    /*--- Obtenemos el total de productos por fecha ---*/
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                // Crear clave para el mapa de sumas de productos
                std::pair<int, int> fecha_clave1(Anho.first, Mes.first);
                // Si la clave ya existe, agregar la cantidad al valor existente
                if(total_productos_por_fecha.count(fecha_clave1) > 0){
                    total_productos_por_fecha[fecha_clave1] += Mes.second[0]; // Cantidad de productos
                }
                else{
                    // Si la clave no existe, crear una nueva entrada con la cantidad
                    total_productos_por_fecha[fecha_clave1] = Mes.second[0];
                }
            }
        }
    }
    
    /*--- Obtener la suma total de precios por fecha ---*/
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                // Crear clave para el mapa de sumas de productos
                std::pair<int, int> fecha_clave2(Anho.first, Mes.first);
                // Si la clave ya existe, agregar la cantidad al valor existente
                if(total_suma_por_fecha.count(fecha_clave2) > 0){
                    total_suma_por_fecha[fecha_clave2] += Mes.second[1]; // Suma de los precios
                }
                else{
                    // Si la clave no existe, crear una nueva entrada con la suma de los precios
                    total_suma_por_fecha[fecha_clave2] = Mes.second[1];
                }
            }
        }
    }
    getchar();
    
    /*--- Recorrer el mapa de sumas de suma precios ---*/
    auto it1 = total_productos_por_fecha.begin();
    auto it2 = total_suma_por_fecha.begin();
    int base = 0;
    double mes_base;
    double acum = 0;
    for (; it1!=total_productos_por_fecha.end() && it2!=total_suma_por_fecha.end() ; it1++, it2++) {
        //Definimos mes base
        if(base==0){
            mes_base = it2->second/it1->second;
            base++;
        }
        double mediacanastames = it2->second/it1->second;
        double variacion = mediacanastames/mes_base - 1;
        acum += variacion; //Si la variación es mayor que cero, sumamos; si es menor, restamos
        // Imprimir la fecha y la suma de productos
        std::cout << "Fecha: " << it1->first.first << "/" << it1->first.second << "\t - total cantidad: " << it1->second << " - total suma precio: " << it2->second << " MediaCanasta: " << mediacanastames << " IPCmensual: " << mediacanastames/mes_base << " Var: " << variacion << " Acum: " << acum << std::endl;
    }
    getchar();
    /*--- Fin ---*/
    //IPC máximo entre 10-15%
    return 0;
}

void readExcelChunk(const std::string& filename, int& startRow, int chunkSize){
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
                    
                    //Ingresamos el dato en el mapa PEN_CLP
                    std::pair<int,int> fecha(year,month);
                    if (PEN_CLP.count(fecha) > 0) {
                        PEN_CLP[fecha] += num; // Cantidad de productos
                        FECHAS[fecha]+=1;
                    }
                    else {
                        // Si la clave no existe, crear una nueva entrada con la cantidad
                        PEN_CLP[fecha] = num;
                        FECHAS[fecha]+=1;
                    }
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