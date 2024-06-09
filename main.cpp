#include <iostream>
#include "Producto.h"
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>

int main() {
    std::ifstream archivo("/home/jorge/Escritorio/Proyectos/Datos/pd.csv");
    std::string linea;
    char delimitador = ';';
    getline(archivo, linea);

    //std::vector<Producto> Productos;
    clock_t t;
    t = clock();

    while(getline(archivo,linea)){
        //std::cout << linea << std::endl;
        /*
        std::vector<std::string> fila = csv.GetRow<std::string> (i);

        std::string created = fila[0];  //Obtener la fecha (created) de la transacción

        int anho = stoi(created.substr(0,4)); //Extraer el año del string created
        int mes = stoi(created.substr(5,2));  //Extraer el mes del string created
        int dia = stoi(created.substr(8,2));  //Extraer el día del string created

        int payment = fila[1].empty() ? 0 : std::stoi(fila[1]);  //Convertir número de boleta de string a int, si está vacío, asignar 0, en caso contrario, realizar la conversión.
        int store_fk = fila[2].empty() ? 0 : std::stoi(fila[2]);  //Convertir número de tienda de string a int, si está vacío, asignar 0, en caso contrario, realizar la con
        std::string seller = fila[3];  //Extraer el
        std::string category = fila[4];
        std::string delivery_type = fila[5];
        std::string sku = fila[6];
        int quantity = fila[7].empty() ? 0 : std::stoi(fila[7]);
        std::string name = fila[8];
        float amount = fila[9].empty() ? 0 : std::stof(fila[9]);

        Producto producto;
        producto.setCreated(anho, mes, dia);
        producto.setPayment(payment);
        producto.setStore_fk(store_fk);
        producto.setSeller(seller);
        producto.setCategory(category);
        producto.setDelivery_type(delivery_type);
        producto.setSku(sku);
        producto.setQuantity(quantity);
        producto.setName(name);
        producto.setAmount(amount);

        Productos[i] = producto;
        */
    }
    t = clock() - t;
    std::cout << "clock t = " << t << std::endl;
    archivo.close();
    return 0;
}
