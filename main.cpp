#include <iostream>
#include "Producto.h"
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <omp.h>

int main() {
    std::ifstream archivo("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string linea;
    char delimitador = ';';
    char comilla = '\"';
    getline(archivo, linea);

    std::vector<Producto*> Productos;    //Declaramos el vector que alojará los objetos producto
    clock_t t;
    t = clock();
    int i = 0;
    
    while(getline(archivo,linea)/* && i<=1500000*/){
        //Si la línea posee un largo "típico", la analizamos
        if(linea[1]=='2'){
            //        if(i==1512408) std::cout << linea << std::endl;
            //        if(i==1512409) std::cout << linea << std::endl;
            std::string original = linea;
            std::stringstream stream(linea);    //Creamos una variable stringstream con el contenido de la línea
            //        std::cout << "created " << linea << std::endl;
            
            getline(stream,linea,delimitador);  //Obtenemos la fecha en ISO de la compra
            int anho = stoi(linea.substr(1,4)); //Extraer el año
            int mes = stoi(linea.substr(6,2));  //Extraer el mes
            int dia = stoi(linea.substr(9,2));  //Extraer el día
            
            /*Nos saltamos los siguientes cinco campos que no utilizaremos por el momento*/
            for(int j=0; j<5; j++){
                getline(stream,linea,delimitador);
            }

            //        std::cout << "sku" << linea << std::endl;
            std::string sku;                                //Declaramos la variable sku que contendrá el sku del producto
            getline(stream,sku,delimitador);  //Obtenemos el Sku del producto comprado
            
            /*Nos saltamos los siguientes dos campos que no utilizaremos por el momento*/
            getline(stream,linea,delimitador);  //Cantidad
            std::string name;
            /*Si no tiene nombre, no tiene comillas, por lo tanto, si el siguiente carácter es una comilla, por lo que buscamos comilla y buscamos el delimitador*/
            if(linea[0]==comilla){
                getline(stream,name, comilla);     //Buscamos las primeras comillas
                getline(stream,name, comilla);     //Saltamos a las siguientes comillas
            }
            getline(stream,name,delimitador);  //Nombre

            //        std::cout << "amount" << linea << std::endl;
            std::string str_amount;                                             //Declaramos la variable str_amount, que corresponde al string del amount del producto
            getline(stream,str_amount,delimitador);                //Obtenemos el amount del producto
            
            //Si la linea tiene problemas de estructura, descartamos la línea
            if(str_amount[0]!=comilla){
//                std::cout << "original " << original << std::endl;
//                std::cout << "i = " << i << " str_amount " << str_amount << std::endl;
//                std::cout << "amount nulo..." << std::endl;
//                getchar();
            }
                //Si todo sale bien, pasamos al else y guardamos el objeto en memoria
            else{
                str_amount = str_amount.substr(1,str_amount.length()-2);    //Debemos eliminar las comillas del string para transformarlo en float
                float amount = stof(str_amount);                                //Obtenemos el amount en float
                Producto* producto;                      //Declaramos el objeto producto
                producto = new Producto();
                producto->setCreated(anho, mes, dia);    //Setteamos la fecha
                //        producto.setPayment(payment);
                //        producto.setStore_fk(store_fk);
                                producto->setSku(sku);                   //Setteamos el sku
                //        producto.setQuantity(quantity);
                //        producto.setName(name);
                                producto->setAmount(amount);             //Setteamos el amount
                //        std::cout << "objeto listo" << std::endl;
                                Productos.push_back(producto);  //Guardamos el objeto en el vector Productos
                                free(producto);             //Liberamos la memoria utilizada por el objeto
                //        std::cout << producto->getAmount() << std::endl;
            }
        }
        //Caso contrario, saltamos la línea
        else{
//            std::cout << "sale" << std::endl;
        }

        i++;
    }

    t = clock() - t;
    std::cout << "clock t = " << t << std::endl;
    std::cout << "length = " << i << std::endl;
    archivo.close();

    getchar();
    
    std::cout << "Cantidad de objetos = " << Productos.size() << std::endl;
    
    getchar();

    return 0;
}

