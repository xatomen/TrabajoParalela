//
// Created by jorge on 8/06/24.
//

#ifndef TRABAJOPARALELA_PRODUCTO_H
#define TRABAJOPARALELA_PRODUCTO_H

#include <string>
#include <iostream>

class Producto {

private:
    int anho;
    int mes;
    int dia;
    int payment;
    int store_fk;
    std::string seller;
    std::string category;
    std::string delivery_type;
    std::string sku;
    int quantity;
    std::string name;
    float amount;
public:
    void setCreated(int anho, int mes, int dia);
    void setPayment(int payment);
    void setStore_fk(int store_fk);
    void setSeller(std::string seller);
    void setCategory(std::string category);
    void setDelivery_type(std::string delivery_type);
    void setSku(std::string sku);
    void setQuantity(int quantity);
    void setName(std::string name);
    void setAmount(float amount);
    std::string getCreated();
    int getPayment();
    int getStore_fk();
    std::string getSeller();
    std::string getCategory();
    std::string getDelivery_type();
    std::string getSku();
    int getQuantity();
    std::string getName();
    float getAmount();
};


#endif //TRABAJOPARALELA_PRODUCTO_H
