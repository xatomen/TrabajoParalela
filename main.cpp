#include <iostream>
#include "Producto.h"
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <omp.h>
#include <map>

int main() {
    std::ifstream archivo("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string linea;
    char delimitador = ';';
    char comilla = '\"';
    
    getline(archivo, linea);    //Leemos el "encabezado" del texto
    
//    clock_t t;
//    t = clock();
    int i = 0;
    
    /*Utilizamos un mapa anidado que nos permita almacenar el SKU, y para cada SKU almacenar los años, y para cada fecha de cada SKU almacenar los meses*/
    std::map<std::string, std::map<int,std::map<int,int>>> MapaProductos;
    
    while(getline(archivo,linea)){
        //Si en la línea, el primer campo a leer posee el número "2" perteneciente a la fecha, entonces leemos; en caso contrario, saltamos la línea
        if(linea[1]=='2'){
            /*---- Cargamos la línea en memoria como stream ----*/
//            std::string original = linea; //String para realizar trazas
            std::stringstream stream(linea);    //Creamos una variable stringstream con el contenido de la línea
            //        std::cout << "created " << linea << std::endl;
            
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
            //        std::cout << "sku" << linea << std::endl;
            std::string sku;                                //Declaramos la variable sku que contendrá el sku del producto
            getline(stream,sku,delimitador);  //Obtenemos el Sku del producto comprado
            
            /*Nos saltamos los siguientes dos campos que no utilizaremos por el momento*/
            getline(stream,linea,delimitador);  //Cantidad
            std::string name;
            /*
             * Para el campo nombre:
             * Si no tiene nombre, no tiene comillas, por lo tanto, solo buscamos el siguiente delimitador
             * Si el siguiente carácter es una comilla, tiene nombre, por lo que buscamos la siguiente comilla y luego, buscamos el delimitador (para evitar tomar un ";" dentro del nombre del producto)
            */
            if(linea[0]==comilla){
                getline(stream,name, comilla);     //Buscamos las primeras comillas
                getline(stream,name, comilla);     //Saltamos a las siguientes comillas
            }
            getline(stream,name,delimitador);  //Nombre
            
            /*---- Leer amount/costo del producto ----*/
            //        std::cout << "amount" << linea << std::endl;
            std::string str_amount;                                             //Declaramos la variable str_amount, que corresponde al string del amount del producto
            getline(stream,str_amount,delimitador);                //Obtenemos el amount del producto
            /*
             * Si la línea no tiene problemas en su estructura, entonces todo ok
             * Si la línea tiene problemas de estructura (ejemplo: tiene un enter en el nombre), descartamos la línea
             * */
            if(str_amount[0]!=comilla){
//                std::cout << "linea: " << i+2 << " original -> " << original << std::endl;
//                getchar();
            }
            //Si todo esta bien, pasamos al else y guardamos el producto en el map
            else{
                //Por ahora no utilizamos el amount
                str_amount = str_amount.substr(1,str_amount.length()-2);    //Debemos eliminar las comillas del string para transformarlo en float
                float amount = stof(str_amount);                                //Obtenemos el amount en float
                
                MapaProductos[sku][anho][mes]++;
//                std::cout << "sku: " << producto->getSku() << " - amount: " << producto->getAmount() << std::endl;
            }
        }
        //Caso contrario, saltamos la línea (primer if)
        else{
//            std::cout << "sale" << std::endl;
        }
        i++;    //Incrementamos el contador
    }

//    t = clock() - t;
//    std::cout << "clock t = " << t << std::endl;
//    std::cout << "length = " << i << std::endl;
    archivo.close();

    getchar();
    
    /*Recorrer y eliminar años con menos de 12 meses*/
    for (auto& Sku : MapaProductos) {
//        std::cout << Sku.first << " | " << std::endl;
        auto it = Sku.second.begin();
        while (it != Sku.second.end()) {
            if (it->second.size() != 12) {
                it = Sku.second.erase(it); // Eliminar año y avanzar el iterador
            } else {
                ++it; // Solo avanzar el iterador
            }
        }
    }
    
    /*Recorrer y eliminar SKU sin años*/
    auto skuIt = MapaProductos.begin();
    while (skuIt != MapaProductos.end()) {
        if (skuIt->second.empty()) {
            skuIt = MapaProductos.erase(skuIt); // Eliminar SKU y avanzar el iterador
        } else {
            ++skuIt; // Solo avanzar el iterador
        }
    }
    
    /*Productos de la canasta básica*/
    for(const auto& Sku : MapaProductos){
        std::cout << Sku.first << " | " << std::endl;
        for(const auto& Anho : Sku.second){
            std::cout << "\tAnho: " << Anho.first << " | -> meses: " << Anho.second.size() << std::endl;
            for(const auto& Mes : Anho.second){
                    std::cout << "\t\tMes: " << Mes.first << " -> " << Mes.second << std::endl;
            }
        }
    }
    
    getchar();

    return 0;
}

