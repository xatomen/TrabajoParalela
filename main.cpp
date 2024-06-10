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
    getline(archivo, linea);

    std::vector<Producto> Productos;
    clock_t t;
    t = clock();
    long i = 0;
#pragma omp parallel sections num_threads(8)
    {
        //std::string linea2;
#pragma omp section

        {
            while (getline(archivo, linea)){
                //std::cout << linea << std::endl;
                //std::stringstream stream(linea2);
                /*
                std::string created;
                getline(stream, created, delimitador);  //Obtener la fecha (created) de la transacción
                std::cout << "1" << std::endl;
                std::cout << created << std::endl;
                int anho = stoi(created.substr(1,4)); //Extraer el año del string created
                int mes = stoi(created.substr(6,2));  //Extraer el mes del string created
                int dia = stoi(created.substr(9,2));  //Extraer el día del string created
                std::cout << "2" << std::endl;

                std::string saltar;
                getline(stream,saltar,delimitador);
                getline(stream,saltar,delimitador);
                getline(stream,saltar,delimitador);
                getline(stream,saltar,delimitador);
                getline(stream,saltar,delimitador);
                std::string sku;
                getline(stream,sku,delimitador);
                getline(stream,saltar,delimitador);
                getline(stream,saltar,delimitador);
                std::string str_amount;
                getline(stream,str_amount,delimitador);
                */
                //float amount = stof(str_amount);

                //Producto producto;
                //producto.setCreated(anho, mes, dia);
                //producto.setPayment(payment);
                //producto.setStore_fk(store_fk);
                //producto.setSeller(seller);
                //producto.setCategory(category);
                //producto.setDelivery_type(delivery_type);
                //producto.setSku(sku);
                //producto.setQuantity(quantity);
                //producto.setName(name);
                //producto.setAmount(amount);
                //Productos[i] = producto;

                i++;
            }
        }

    }


    t = clock() - t;
    std::cout << "clock t = " << t << std::endl;
    std::cout << "length = " << i << std::endl;
    archivo.close();
    return 0;
}
