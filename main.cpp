/*------------------------------------------------------
 *                  Trabajo 1
 *
 * Asignatura: Computación paralela y distribuida
 * Sección: 411
 * Profesor: Sebastian Salazar Molina
 *
 * Integrantes: - Jorge Luis Gallardo Contreras
 *              - Fernando Tomás Maldonado Rodríguez
 *              - Felipe Ignacio Vera Andrade
 *
 * ----------------------------------------------------*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include "libxl-4.3.0.14/include_cpp/libxl.h"
#include <set>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace libxl;

/*--- Prototipo de funciones ---*/
void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth);
void insertValueInMap(std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth, std::map<std::pair<int,int>,float>& solesToPesos);
void sequentialParseCsv(std::string& filename, std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& MapaProductos);
std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);
std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal);
void calculateIntermensualVariation(std::map <std::pair<int,int>, double>& ValorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);
void calculateInteranualVariation(std::map <std::pair<int,int>, double>& ValorCanastaMensual, std::map<std::pair<int,int>,float>& solesToPesos);

int main(int argc, char *argv[]) {
    
    /*--- Iniciamos cronómetro ---*/
    auto start = std::chrono::high_resolution_clock::now(); //Iniciamos cronómetro
    /*--- Fin ---*/
    
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <ruta_al_archivo_csv> <ruta_al_archivo_xlsx>" << std::endl;
        return 1;
    }
    
    /*--- Variables ---*/
    std::map<std::pair<int,int>,double> penToClp;  //pair(anho,mes),pesos
    std::map<std::pair<int,int>,int> daysPerMonth; //pair(anho,mes),dias
    std::map<std::pair<int,int>,float> solesToPesos;
    std::unordered_map<std::string, std::map<int, std::map<int, std::vector<float>>>> MapaProductos;
    
#pragma omp parallel sections
    {
#pragma omp section
        {
            /*--- Lectura archivo xlsx ---*/
            std::cout << "-- Lectura excel --" << std::endl;
//            std::string filename = "/home/jorge/Escritorio/Proyectos/TrabajoParalela/PEN_CLP.xlsx";
            std::string filename = argv[1];
            int startRow = 7;
            int chunkSize = 100;    //Leemos de 100 en 100 para evitar errores en la lectura
            
            while(true){
                readExcelChunk(filename, startRow, chunkSize, penToClp, daysPerMonth);
                if(startRow==3734){                                                                 //@todo: !!!VER ESTO!!!
                    break; // Termina si no hay más filas por leer
                }
            }
            /*--- Guardamos la transformación de moneda en un map ---*/
            
            insertValueInMap(penToClp, daysPerMonth, solesToPesos);
            std::cout << "-- Excel listo --" << std::endl;
            /*--- Fin ---*/
        }
#pragma omp section
        {
            /*--- Parseo CSV ---*/
            /*Utilizamos un mapa anidado que nos permita almacenar el SKU, y para cada SKU almacenar los años, y para cada fecha de cada SKU almacenar los meses, finalmente, se tiene un contador y la suma mensual del sku*/
//            std::string archivo = "/home/jorge/Escritorio/Proyectos/Datos/pd.csv";
            std::string archivo = argv[2];
            sequentialParseCsv(archivo,MapaProductos);
            /*--- Fin ---*/
        }
    }
#pragma omp barrier
#pragma omp parallel sections
    {
#pragma omp section
        {
            /*--- Obtención de la canasta básica para la variación intermensual ---*/
            std::map <std::pair<int,int>, double> ValorCanastaMensual = filterBasicBasketForIntermensualVariation(MapaProductos);
            /*--- Fin ---*/
            /*--- Calculamos la variación intermensual de la canasta básica ---*/
            calculateIntermensualVariation(ValorCanastaMensual, solesToPesos);
            /*--- Fin ---*/
        }
#pragma omp section
        {
            /*--- Obtención de la canasta básica para la variación interanual ---*/
            std::map <std::pair<int,int>, double> ValorCanastaAnual = filterBasicBasketForInteranualVariation(MapaProductos);
            /*--- Fin ---*/
            /*--- Calculamos la variación interanual de la canasta básica ---*/
            calculateInteranualVariation(ValorCanastaAnual, solesToPesos);
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
        std::pair<int,int> fecha(pesos->first.first,pesos->first.second);   //Asignamos el promedio de pesos a cada mes
        solesToPesos[fecha] = pesos->second/dias->second;
    }
}

void sequentialParseCsv(std::string& filename, std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& MapaProductos){
    std::cout << "-- Parseo csv --" << std::endl;
    /*--- Lectura archivo csv ---*/
    std::fstream archivo(filename);
    std::string linea;
    char delimitador = ';';
    char comilla = '\"';
    getline(archivo, linea);    //Leemos el "encabezado" del texto
    
    /*Parseo del archivo csv, utilizando únicamente los campos relevantes*/
    
    /*--- Declaramos las variables ---*/
    int anho, mes;
    int quantity;
    float amount;
    std::string sku;
    std::string name;
    std::string str_quantity;
    std::string fecha;
    std::string str_amount;
    /*--- Fin ---*/
    /*---- Cargamos la línea en memoria como stream ----*/
    std::stringstream stream;     //Creamos una variable stringstream con el contenido de la línea
    
    int i = 0;
    while(getline(archivo,linea)){
        stream.clear();
        stream.str(linea);
        /*--- Extraemos los datos de la línea leída ---*/
        getline(stream,fecha,delimitador);                     //Obtenemos la fecha en ISO de la compra
        /*Nos saltamos los siguientes cinco campos que no utilizaremos por el momento*/
        getline(stream,linea,delimitador);
        getline(stream,linea,delimitador);
        getline(stream,linea,delimitador);
        getline(stream,linea,delimitador);
        getline(stream,linea,delimitador);
        getline(stream,sku,delimitador);                       //Obtenemos el Sku del producto comprado
        getline(stream,str_quantity,delimitador);              //Cantidad
        /*Para el campo nombre:
         * Si no tiene nombre, no tiene comillas, por lo tanto, solo buscamos el siguiente delimitador
         * Si el siguiente carácter es una comilla, tiene nombre, por lo que buscamos la siguiente comilla y luego, buscamos el delimitador (para evitar tomar un ";" dentro del nombre del producto)*/
        if(linea[0]==comilla){
            getline(stream,name, comilla);                     //Buscamos las primeras comillas
            getline(stream,name, comilla);                     //Buscamos las segundas comillas
            /*Si no se encuentran las segundas comillas, entonces deberemos saltar de línea y continuar*/
            while(!stream.good()){                                         //Mientras el stream no sea correcto, no saldremos del while
                i++;                                                       //Aumentamos el contador de filas porque saltaremos a la siguiente línea (para evitar inconsistencias con el while anterior)
                getline(archivo,linea);                              //Saltamos a la siguiente línea
                stream.clear();                                            //Limpiamos el stream
                stream.str(linea);                                      //Asignamos la nueva línea
                getline(stream,name, comilla);                 //Volvemos a buscar la comilla faltante
            }
        }
        getline(stream,name,delimitador);                      //Obtenemos el nombre del producto
        getline(stream,str_amount,delimitador);                //Obtenemos el amount del producto
        /*--- Fin ---*/
        
        /*--- Asignamos la información extraída ---*/
        anho = stoi(fecha.substr(1,4));                                                 //Extraer el año
        mes = stoi(fecha.substr(6,2));                                                  //Extraer el mes
        quantity = stoi(str_quantity.substr(1,str_quantity.length()-2)); //Debemos eliminar las comillas del string para transformarlo en float
        amount = stof(str_amount.substr(1,str_amount.length()-2));                      //Debemos eliminar las comillas del string para transformarlo en float
        /*--- Fin ---*/
        
        /*--- Ponemos los datos en el map anidado ---*/
        if(MapaProductos[sku][anho][mes].empty()){      //Si el campo está vacío, debemos inicializarlo en 0 para evitar errores después al incrementar y sumar
            MapaProductos[sku][anho][mes] = {0, 0};
        }
        MapaProductos[sku][anho][mes][0] += quantity;           //Sumamos la cantidad de veces que se compró el producto en ese mes
        MapaProductos[sku][anho][mes][1] += (amount*quantity);  //Sumamos el precio total de la compra (cantidad de veces que se compró por el precio unitario
        /*--- Con lo anterior, podemos obtener el precio promedio del sku en ese año y en ese mes al realizar la división sin la necesidad de tener la cantidad de días ---*/
        i++;                                            //Incrementamos el contador
    }
    archivo.close();
    std::cout << std::endl;
}

std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal){
    std::cout << "--- Filtrado canasta básica intermensual ---" << std::endl;
    std::unordered_map<std::string, std::map<int, std::map<int, std::vector<float>>>> MapaProductos = mapaProductosOriginal;
    /*--- Obtención canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos antes de obtener la canasta básica para verificar ---*/
    /*--- Es "similar" a "invertir" el map, antes era: sku->fecha, con este cambia a: fecha->sku ---*/
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
    std::map<int, int> mesesPorAnho;                        // mesesPorAnho {año,mes}
    for (const auto& entry : fechaSku1){    //Recorremos el mapa fechaSku1
        mesesPorAnho[entry.first.first]++;                  //Cada vez que pasemos por un mes, aumentamos el contador
    }
    fechaSku1.clear();  //Liberamos memoria
    /*--- Fin ---*/
    
    /*-- Eliminamos los años que no tienen todos los meses correspondientes*/
    for(auto& sku : MapaProductos){                                              //Recorremos cada sku
        for(auto anhoIt = sku.second.begin(); anhoIt != sku.second.end(); ){    //Recorremos cada año
            int anho = anhoIt->first;                                                       //Creamos una variable año para facilitar la lectura del código
            if(anhoIt->second.size() < mesesPorAnho[anho]){                                 //Si el sku no está en todos los meses del año, debemos eliminar la fecha del sku
                anhoIt = sku.second.erase(anhoIt);                                  //Eliminamos el año del sku
            }
            else{
                ++anhoIt;                                                                   //Iteramos y pasamos al siguiente año
            }
        }
    }
    
    /*--- Eliminamos los SKUs que se hayan quedado sin años durante el filtrado de la canasta básica ---*/
    for(auto skuIt = MapaProductos.begin(); skuIt != MapaProductos.end(); ){    //Recorremos cada sku
        if(skuIt->second.empty()){                                                          //Si el sku no tiene registros, lo eliminamos
            skuIt = MapaProductos.erase(skuIt);                                     //Eliminamos el sku del map y apuntamos al siguiente sku
        }
        else{
            ++skuIt;                                                                        //Iteramos y pasamos al siguiente sku
        }
    }
    /*--- Con lo anterior hemos filtrado la canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos después de obtener la canasta básica para verificar ---*/
    /*--- Con esto obtenemos la canasta básica por año, es decír: fecha->sku, o: fecha->canasta ---*/
    std::map<std::pair<int,int>,std::map<std::string,double>> fechaSku2;      //Mapa que contiene fecha año, mes y cada fecha tiene el sku, el sku posee el valor promedio de ese sku en esa fecha
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);         //Generamos un par valor que representa la fecha {año,mes}
                fechaSku2[fecha][Sku.first] = Mes.second[1]/Mes.second[0];    //En cada SKU de cada año colocamos el valor promedio del SKU en esa fecha (año-mes)
                /*--- Con lo anterior, tenemos el valor promedio de cada sku en cada fecha (año,mes) ---*/
            }
        }
    }
    MapaProductos.clear(); //Liberamos memoria
    /*--- Fin ---*/
    
    /*--- Mapa del valor de la canasta mensual ---*/
    std::map <std::pair<int,int>, double> ValorCanastaMensual;
    
    /*--- Obtenemos el total de productos por fecha ---*/
    for(auto& fecha : fechaSku2){
        for(auto& sku : fecha.second){
            ValorCanastaMensual[fecha.first] += sku.second;
            /*--- Con esto obtenemos el valor de la canasta, en donde la canasta está formada por una unidad de cada sku ---*/
            /*--- En donde cada sku que sumamos corresponde al valor promedio de ese sku en cada mes ---*/
            /*--- Por lo tanto, cada fecha (año,mes) tendrá almacenado la misma cantidad de skus y la diferencia será la suma ---*/
        }
    }
    /*--- Fin ---*/
    std::cout << std::endl;
    
    return ValorCanastaMensual;
}

std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal){
    std::cout << "--- Filtrado canasta básica interanual ---" << std::endl;
    std::unordered_map<std::string, std::map<int, std::map<int, std::vector<float>>>> MapaProductos = mapaProductosOriginal;
    /*--- Obtención canasta básica ---*/
    
    /*--- Crear un conjunto de fechas (años y meses) que se encontraron durante la lectura ---*/
    std::set<std::pair<int, int>> todasLasFechas;
    for (const auto& sku : MapaProductos) {
        for (const auto& anho : sku.second) {
            for (const auto& mes : anho.second) {
                todasLasFechas.emplace(anho.first, mes.first);  //Ingresamos la fecha en el set
            }
        }
    }
    
    /*--- Verificar cada SKU y eliminar los que no estén presentes en todas las fechas de cada año ---*/
    for(auto skuIt = MapaProductos.begin(); skuIt != MapaProductos.end(); ){        //Iteramos sobre cada sku
        bool estaPresenteEnTodasLasFechas = true;                                               //Para cada sku, verificamos si se encuentra presente en todos los meses de todos los años
        for(const auto& fecha : todasLasFechas){                             //Iteramos sobre cada fecha
            int anho = fecha.first;                                                             //Asignamos el año a una variable para facilitar la lectura del código
            int mes = fecha.second;                                                             //Asignamos el mes a una variable para facilitar la lectura del código
            if(skuIt->second.count(anho) == 0 || skuIt->second[anho].count(mes) == 0){    //Si en el año o mes buscado no se encuentra presente el sku, deberá ser eliminado
                estaPresenteEnTodasLasFechas = false;                                           //Indicamos que el sku no se encuentra disponible en todas las fechas consultadas
                break;                                                                          //Salimos del for
            }
        }
        if(!estaPresenteEnTodasLasFechas){                                                      //Si el sku no se encuentra disponible en todas las fechas, lo eliminamos del map
            skuIt = MapaProductos.erase(skuIt);                                         //Eliminamos el sku del map y apuntamos al siguiente sku
        }
        else{
            ++skuIt;                                                                            //Si está presente en todas, pasamos al siguiente sku
        }
    }
    /*--- Fin ---*/
    /*--- Con lo anterior hemos filtrado la canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos después de obtener la canasta básica para verificar ---*/
    /*--- Con esto obtenemos la canasta básica por año, es decír: fecha->sku, o: fecha->canasta ---*/
    std::map<std::pair<int,int>,std::map<std::string,double>> fechaSku2; //Mapa que contiene fecha año, mes y cada fecha tiene el sku, el sku posee el valor promedio de ese sku en esa fecha
    for(const auto& Sku : MapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);         //Generamos un par valor que representa la fecha {año,mes}
                fechaSku2[fecha][Sku.first] = Mes.second[1]/Mes.second[0];    //En cada SKU de cada año colocamos el valor promedio del SKU en esa fecha (año-mes)
            }
        }
    }
    MapaProductos.clear(); //Liberamos memoria
    /*--- Fin ---*/
    
    /*--- Mapa del valor de la canasta mensual ---*/
    std::map <std::pair<int,int>, double> ValorCanastaMensual;
    int contador;
    /*--- Obtenemos el total de productos por fecha ---*/
    for(auto& fecha : fechaSku2){
        contador=0;
        for(auto& sku : fecha.second){
            ValorCanastaMensual[fecha.first] += sku.second;
            /*--- Con esto obtenemos el valor de la canasta, en donde la canasta está formada por una unidad de cada sku ---*/
            /*--- En donde cada sku que sumamos corresponde al valor promedio de ese sku en cada mes ---*/
            /*--- Por lo tanto, cada fecha (año,mes) tendrá almacenado la misma cantidad de skus y la diferencia será la suma ---*/
        }
    }
    /*--- Fin ---*/
    std::cout << std::endl;
    
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
    /*--- Fin ---*/
    std::cout << std::endl;
}

void calculateInteranualVariation(std::map <std::pair<int,int>, double>& ValorCanastaAnual, std::map<std::pair<int,int>,float>& solesToPesos){
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
    for(auto& vcm : ValorCanastaAnual){
        /*Si es el primer mes, lo usamos como mes base inicial*/
        if(!flag){                                                                                    //Si el flag es falso, entonces es el primer mes del primer año
            anhoActual = vcm.first.first;                                                             //El año actual será el mes base
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
        mesBaseClp = mesActualClp;
    }
    std::cout << std::endl;
}