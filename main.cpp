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

/*--- Función que permite leer el archivo xlsx por chunks (para evitar errores) ---*/
void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth);
void insertValueInMap(std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth, std::map<std::pair<int,int>,float>& solesToPesos);
void sequentialParseCsv(std::string& filename, std::map<std::string,std::map<int,std::map<int,std::vector<float>>>>& MapaProductos);
std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);
std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);
void calculateIntermensualVariation(std::map <std::pair<int,int>, double>& ValorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);
void calculateInteranualVariation(std::map <std::pair<int,int>, double>& ValorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);


int main() {
    
    /*--- Lectura archivo xlsx ---*/
    std::cout << "-- Lectura excel --" << std::endl;
    std::map<std::pair<int,int>,double> penToClp;  //pair(anho,mes),pesos
    std::map<std::pair<int,int>,int> daysPerMonth; //pair(anho,mes),dias
    std::string filename = "/home/jorge/Escritorio/Proyectos/TrabajoParalela/PEN_CLP.xlsx";
    int startRow = 7;
    int chunkSize = 100;    //Leemos de 100 en 100 para evitar errores en la lectura
    
    while(true){
        readExcelChunk(filename, startRow, chunkSize, penToClp, daysPerMonth);
        if(startRow==3734){                                                                 //@todo: !!!VER ESTO!!!
            break; // Termina si no hay más filas por leer
        }
    }
    /*--- Guardamos la transformación de moneda en un map*/
    std::map<std::pair<int,int>,float> solesToPesos;
    insertValueInMap(penToClp, daysPerMonth, solesToPesos);
    /*--- Imprimimos la transformación de moneda por mes ---*/
//    for(auto& map: solesToPesos){
//        std::cout << map.first.first << "/" << map.first.second << "\t- " << map.second << std::endl;
//    }
    std::cout << "-- Excel listo --" << std::endl;
    /*--- Fin ---*/
    
    /*--- Parseo CSV ---*/
    /*Utilizamos un mapa anidado que nos permita almacenar el SKU, y para cada SKU almacenar los años, y para cada fecha de cada SKU almacenar los meses, finalmente, se tiene un contador y la suma mensual del sku*/
    std::map<std::string, std::map<int, std::map<int, std::vector<float>>>> MapaProductos;
    std::string archivo = "/home/jorge/Escritorio/Proyectos/Datos/pd.csv";
    sequentialParseCsv(archivo,MapaProductos);
    /*--- Fin ---*/
    /*--- Obtención de la canasta básica para la variación intermensual ---*/
    std::map <std::pair<int,int>, double> ValorCanastaMensual = filterBasicBasketForIntermensualVariation(MapaProductos);
    /*--- Fin ---*/
    
    /*--- Obtención de la canasta básica para la variación interanual ---*/
    std::map <std::pair<int,int>, double> ValorCanastaAnual = filterBasicBasketForInteranualVariation(MapaProductos);
    /*--- Fin ---*/
    
    /*--- Calculamos la variación intermensual de la canasta básica ---*/
    calculateIntermensualVariation(ValorCanastaMensual, solesToPesos);
    /*--- Fin ---*/
    
    /*--- Calculamos la variación interanual de la canasta básica ---*/
    calculateInteranualVariation(ValorCanastaAnual, solesToPesos);
    /*--- Fin ---*/
    
    return 0;
}

void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth){
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
                    if (penToClp.count(fecha) > 0) {
                        penToClp[fecha] += num; // Cantidad de productos
                        daysPerMonth[fecha]+=1;
                    }
                    else {
                        // Si la clave no existe, crear una nueva entrada con la cantidad
                        penToClp[fecha] = num;
                        daysPerMonth[fecha]+=1;
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

void insertValueInMap(std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth, std::map<std::pair<int,int>,float>& solesToPesos){
    auto pesos = penToClp.begin();
    auto dias = daysPerMonth.begin();
    for(; pesos!=penToClp.end() && dias!=daysPerMonth.end(); pesos++, dias++){
//        std::cout << pesos->first.first << "/" << pesos->first.second << "\t- dias: " << dias->second << " suma: " << pesos->second << std::endl;
        std::pair<int,int> fecha(pesos->first.first,pesos->first.second);   //Asignamos el promedio de pesos a cada mes
        solesToPesos[fecha] = pesos->second/dias->second;
    }
}

void sequentialParseCsv(std::string& filename, std::map<std::string,std::map<int,std::map<int,std::vector<float>>>>& MapaProductos){
    std::cout << "-- Parseo csv --" << std::endl;
    /*--- Lectura archivo csv ---*/
    std::fstream archivo(filename);
    std::string linea;
    char delimitador = ';';
    char comilla = '\"';
    getline(archivo, linea);    //Leemos el "encabezado" del texto
    
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
        if(i==5000000)break;
    }
    archivo.close();
    
}

std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal){
    std::cout << "--- Filtrado canasta básica intermensual ---" << std::endl;
    std::map<std::string, std::map<int, std::map<int, std::vector<float>>>> MapaProductos = mapaProductosOriginal;
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
    for (const auto& entry : fechaSku1){    //Recorremos el mapa fechaSku1
        mesesPorAnho[entry.first.first]++;                  //Cada vez que pasemos por un mes, aumentamos el contador
    }
    /*--- Fin ---*/
    
    /*Recorrer y eliminar años con menos de 12 meses*/
    int mesesAnho;
    for (auto& Sku : MapaProductos) {
        auto it = Sku.second.begin();                   //Declaramos e inicializamos un iterador sobre el año actual
        mesesAnho = mesesPorAnho[it->first];                        //Recuperamos la cantidad de meses en el año actual
        /*--- En este while buscamos los SKUs que no se encuentren en todos los meses del año y los eliminamos ---*/
        while(it != Sku.second.end()){                              //Iteramos mientras no lleguemos al año final
            if(it->second.size() < mesesAnho){                      //Si el sku, no se encuentra presenta en todos los meses del año, lo eliminamos
                it = Sku.second.erase(it);                  //Eliminar solo el año y avanzar el iterador
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
    std::map<std::pair<int,int>,std::map<std::string,double>> fechaSku2; //Mapa que contiene fecha año, mes y cada fecha tiene el sku, el sku posee el valor promedio de ese sku en esa fecha
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);         //Generamos un par valor que representa la fecha {año,mes}
                fechaSku2[fecha][Sku.first] = Mes.second[1]/Mes.second[0];    //En cada SKU de cada año colocamos el valor promedio del SKU en esa fecha (año-mes)
            }
        }
    }
    /*--- Fin ---*/
    
    /*--- Mapa del valor de la canasta mensual ---*/
    std::map <std::pair<int,int>, double> ValorCanastaMensual;
    
    /*--- Obtenemos el total de productos por fecha ---*/
    for(auto& fecha : fechaSku2){
        for(auto& sku : fecha.second){
            ValorCanastaMensual[fecha.first] += sku.second;
        }
    }
    /*--- Fin ---*/
    /*--- Imprimimos el precio promedio de la canasta mensual*/
    for(auto& map : ValorCanastaMensual){
        std::cout << map.first.first << "/" << map.first.second << "\t- Valor canasta: " << map.second << std::endl;
    }
    /*--- Fin ---*/
    
    return ValorCanastaMensual;
}

std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal){
    std::cout << "--- Filtrado canasta básica interanual ---" << std::endl;
    std::map<std::string, std::map<int, std::map<int, std::vector<float>>>> MapaProductos = mapaProductosOriginal;
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
    for (const auto& entry : fechaSku1){    //Recorremos el mapa fechaSku1
        mesesPorAnho[entry.first.first]++;                  //Cada vez que pasemos por un mes, aumentamos el contador
    }
    /*--- Fin ---*/
    
    // Verificar cada SKU y eliminar los que no estén presentes en todas las fechas
    for (auto skuIt = MapaProductos.begin(); skuIt != MapaProductos.end(); ) {
        bool estaPresenteEnTodasLasFechas = true;
        for (const auto& fecha : mesesPorAnho) {
            int anho = fecha.first;
            int mes = fecha.second;
//            std::cout << "Año: " << anho << " Mes: " << mes << std::endl;
            if (skuIt->second.count(anho) == 0 || skuIt->second[anho].count(mes) == 0) {
                estaPresenteEnTodasLasFechas = false;
                break;
            }
        }
        if (!estaPresenteEnTodasLasFechas) {
            skuIt = MapaProductos.erase(skuIt);
        } else {
            std::cout << "Está presente en todas: " << skuIt->first << std::endl;
            for(const auto& fecha: skuIt->second){
                std::cout << "\tAño: " << fecha.first << "\tCantidad: " << std::endl;
                for(const auto& mes: fecha.second){
                    std::cout << "\t\tMes: " << mes.first << std::endl;
                }
            }
            getchar();
            ++skuIt;
        }
    }
//    getchar();
    //Fin
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos después de obtener la canasta básica para verificar ---*/
    /*--- Con esto obtenemos la canasta básica por año ---*/
    std::map<std::pair<int,int>,std::map<std::string,double>> fechaSku2; //Mapa que contiene fecha año, mes y cada fecha tiene el sku, el sku posee el valor promedio de ese sku en esa fecha
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);         //Generamos un par valor que representa la fecha {año,mes}
                fechaSku2[fecha][Sku.first] = Mes.second[1]/Mes.second[0];    //En cada SKU de cada año colocamos el valor promedio del SKU en esa fecha (año-mes)
            }
        }
    }
    /*--- Fin ---*/
    
    /*--- Mapa del valor de la canasta mensual ---*/
    std::map <std::pair<int,int>, double> ValorCanastaMensual;
    
    /*--- Obtenemos el total de productos por fecha ---*/
    for(auto& fecha : fechaSku2){
        for(auto& sku : fecha.second){
            ValorCanastaMensual[fecha.first] += sku.second;
        }
    }
    /*--- Fin ---*/
    /*--- Imprimimos el precio promedio de la canasta mensual*/
    for(auto& map : ValorCanastaMensual){
        std::cout << map.first.first << "/" << map.first.second << "\t- Valor canasta: " << map.second << std::endl;
    }
    /*--- Fin ---*/
    
    return ValorCanastaMensual;
}

void calculateIntermensualVariation(std::map <std::pair<int,int>, double>& ValorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos){
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
    for(auto& vcm : ValorCanastaMensual){
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
//    getchar();
    /*--- Fin ---*/
}

void calculateInteranualVariation(std::map <std::pair<int,int>, double>& ValorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos){
/*--- Calculamos la variación intermensual de la canasta básica ---*/
    std::cout << "--- Variación interanual ---" << std::endl;
    //Inicializamos algunas variables que utilizaremos
    double mesBase;
    double mesBaseClp;
    bool flag = false;
    double varAcumulado = 0;
    double varAcumuladoClp = 0;
    int anhoActual;
    //En el siguiente for, lo que hacemos es recorrer todos los meses y años del valor de canasta mensual
    for(auto& vcm : ValorCanastaMensual){
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
//    getchar();
    /*--- Fin ---*/
}