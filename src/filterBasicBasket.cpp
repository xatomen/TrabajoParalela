/*
 Funciones de filtrado de la canasta básica intermensual e interanual
 */

#include "../include/filterBasicBasket.h"

/*--- Función que permite filtrar la canasta básica de cada año para calcular la variación intermensual ---*/
std::map <std::pair<int,int>, double> filterBasicBasketForIntermensualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal){
#pragma omp critical
    {
        std::cout << "--- Filtrado canasta básica intermensual ---" << std::endl;
    }
    std::unordered_map<std::string, std::map<int, std::map<int, std::vector<float>>>> mapaProductos = mapaProductosOriginal;    //Copiamos el mapa para no afectar el original
    /*--- Obtención canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos antes de obtener la canasta básica ---*/
    /*--- Es "similar" a "invertir" el map, antes era: sku->fecha, con este cambia a: fecha->sku ---*/
    /*--- De esta manera podemos trabajar con las fechas más fácilmente ---*/
    std::map<std::pair<int,int>,std::map<std::string,int>> fechaSku1; //Mapa que contiene fecha (año, mes) y cada fecha tiene el sku
    for(const auto& Sku : mapaProductos){
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
    
    /*--- Eliminamos los años que no tienen todos los meses correspondientes para cada SKU ---*/
    /*--- Ejemplo: si el SKU 124123 no tiene un producto en el mes 7 del 2022, entonces no pertenece a la canasta, por lo que borramos el año de ese SKU ---*/
    for(auto& sku : mapaProductos){                                              //Recorremos cada sku
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
    for(auto skuIt = mapaProductos.begin(); skuIt != mapaProductos.end(); ){    //Recorremos cada sku
        if(skuIt->second.empty()){                                                          //Si el sku no tiene registros, lo eliminamos
            skuIt = mapaProductos.erase(skuIt);                                     //Eliminamos el sku del map y apuntamos al siguiente sku
        }
        else{
            ++skuIt;                                                                        //Iteramos y pasamos al siguiente sku
        }
    }
    /*--- Con lo anterior hemos filtrado la canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos después de obtener la canasta básica ---*/
    /*--- Con esto obtenemos la canasta básica por año, es decír: fecha->sku, o: fecha->canasta ---*/
    std::map<std::pair<int,int>,std::map<std::string,double>> fechaSku2;      //Mapa que contiene fecha año, mes y cada fecha tiene el sku, el sku posee el valor promedio de ese sku en esa fecha
    for(const auto& Sku : mapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);         //Generamos un par valor que representa la fecha {año,mes}
                fechaSku2[fecha][Sku.first] = Mes.second[1]/Mes.second[0];    //En cada SKU de cada año colocamos el valor promedio del SKU en esa fecha (año-mes)
                /*--- Con lo anterior, tenemos el valor promedio de cada sku en cada fecha (año,mes) ---*/
            }
        }
    }
    mapaProductos.clear(); //Liberamos memoria
    /*--- Fin ---*/
    
    /*--- Mapa del valor de la canasta mensual ---*/
    std::map <std::pair<int,int>, double> valorCanastaMensual;
    
    /*--- Obtenemos el total de productos por fecha para fines estadísticos---*/
    for(auto& fecha : fechaSku2){
        for(auto& sku : fecha.second){
            valorCanastaMensual[fecha.first] += sku.second;
            /*--- Con esto obtenemos el valor de la canasta, en donde la canasta está formada por una unidad de cada sku ---*/
            /*--- En donde cada sku que sumamos corresponde al valor promedio de ese sku en cada mes ---*/
            /*--- Por lo tanto, cada fecha (año,mes) tendrá almacenado la misma cantidad de skus y la diferencia será la suma ---*/
        }
    }
    /*--- Fin ---*/
    std::cout << std::endl;
    
    return valorCanastaMensual;
}

/*--- Función que permite filtrar la canasta básica común de todos los años para calcular la variación interanual ---*/
std::map <std::pair<int,int>, double> filterBasicBasketForInteranualVariation(std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductosOriginal){
#pragma omp critical
    {
        std::cout << "--- Filtrado canasta básica interanual ---" << std::endl;
    }
    std::unordered_map<std::string, std::map<int, std::map<int, std::vector<float>>>> mapaProductos = mapaProductosOriginal;    //Copiamos el mapa para no afectar el original
    /*--- Obtención canasta básica ---*/
    
    /*--- Crear un conjunto de fechas (años y meses) que se encontraron durante la lectura ---*/
    std::set<std::pair<int, int>> todasLasFechas;
    /*--- Iteramos sobre el mapa de productos ---*/
    for(const auto& sku : mapaProductos){
        for(const auto& anho : sku.second){
            for(const auto& mes : anho.second){
                todasLasFechas.emplace(anho.first, mes.first);  //Ingresamos la fecha en el set
            }
        }
    }
    /*--- Fin ---*/
    
    /*--- Verificar cada SKU y eliminar los que no estén presentes en todas las fechas de cada año ---*/
    for(auto skuIt = mapaProductos.begin(); skuIt != mapaProductos.end(); ){        //Iteramos sobre cada sku
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
            skuIt = mapaProductos.erase(skuIt);                                         //Eliminamos el sku del map y apuntamos al siguiente sku
        }
        else{
            ++skuIt;                                                                            //Si está presente en todas, pasamos al siguiente sku
        }
    }
    /*--- Fin ---*/
    /*--- Con lo anterior hemos filtrado la canasta básica ---*/
    
    /*--- A cada fecha (año,mes), asignamos los SKUs obtenidos después de obtener la canasta básica ---*/
    /*--- Con esto obtenemos la canasta básica por año, es decír: fecha->sku, o: fecha->canasta ---*/
    std::map<std::pair<int,int>,std::map<std::string,double>> fechaSku2; //Mapa que contiene fecha año, mes y cada fecha tiene el sku, el sku posee el valor promedio de ese sku en esa fecha
    for(const auto& Sku : mapaProductos){
        for(const auto& Anho : Sku.second){
            for(const auto& Mes : Anho.second){
                std::pair<int,int> fecha(Anho.first,Mes.first);         //Generamos un par valor que representa la fecha {año,mes}
                fechaSku2[fecha][Sku.first] = Mes.second[1]/Mes.second[0];    //En cada SKU de cada año colocamos el valor promedio del SKU en esa fecha (año-mes)
            }
        }
    }
    mapaProductos.clear(); //Liberamos memoria
    /*--- Fin ---*/
    
    /*--- Mapa del valor de la canasta mensual ---*/
    std::map <std::pair<int,int>, double> valorCanastaMensual;
    /*--- Obtenemos el total de productos por fecha para fines estadísticos ---*/
    for(auto& fecha : fechaSku2){
        for(auto& sku : fecha.second){
            valorCanastaMensual[fecha.first] += sku.second;
            /*--- Con esto obtenemos el valor de la canasta, en donde la canasta está formada por una unidad de cada sku ---*/
            /*--- En donde cada sku que sumamos corresponde al valor promedio de ese sku en cada mes ---*/
            /*--- Por lo tanto, cada fecha (año,mes) tendrá almacenado la misma cantidad de skus y la diferencia será la suma ---*/
        }
    }
    /*--- Fin ---*/
    std::cout << std::endl;
    
    return valorCanastaMensual;
}