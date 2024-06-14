//
// Created by jorge on 8/06/24.
//

#include "Producto.h"

Producto::Producto(){
    anho = 0;
    mes = 0;
    dia = 0;
//    payment = 0;
//    store_fk = 0;
//    seller = "";
//    category = "";
//    delivery_type = "";
    sku = "";
//    quantity = 0;
//    name = "";
    amount = 0;
}

Producto::~Producto(){}

/*Setters*/

void Producto::setCreated(int anho, int mes, int dia){
    this->anho = anho;
    this->mes = mes;
    this->dia = dia;
}

void Producto::setPayment(int payment){
//    this->payment = payment;
}

void Producto::setStore_fk(int store_fk){
//    this->store_fk = store_fk;
}

void Producto::setSeller(std::string seller){
//    this->seller = seller;
}

void Producto::setCategory(std::string category){
//    this->category = category;
}

void Producto::setDelivery_type(std::string delivery_type){
//    this->delivery_type = delivery_type;
}

void Producto::setSku(std::string sku){
    this->sku = sku;
}

void Producto::setQuantity(int quantity){
//    this->quantity = quantity;
}

void Producto::setName(std::string name){
//    this->name = name;
}

void Producto::setAmount(float amount){
    this->amount = amount;
}

/*Getters*/
std::string Producto::getCreated(){
    return "";
}

int Producto::getPayment(){
//    return payment;
}

int Producto::getStore_fk(){
//    return store_fk;
}

std::string Producto::getSeller(){
//    return seller;
}

std::string Producto::getCategory(){
//    return category;
}

std::string Producto::getDelivery_type(){
//    return delivery_type;
}

std::string Producto::getSku(){
    return sku;
}

int Producto::getQuantity(){
//    return quantity;
}

std::string Producto::getName(){
//    return name;
}

float Producto::getAmount(){
    return amount;
}