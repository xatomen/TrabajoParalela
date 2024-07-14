//
// Created by jorge on 14/07/24.
//

#include "../include/read.h"

void readExcelChunk(const std::string& filename, int& startRow, int chunkSize, std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth){
    Book* book = xlCreateXMLBook();
    bool flag = false;  //Avisa si hemos terminado de leer el excel
    if(book){
        if(book->load(filename.c_str())){ // Carga el archivo Excel
            Sheet* sheet = book->getSheet(0); // Obtiene la primera hoja
            if(sheet){
                int rowCount = sheet->lastRow(); // Obtiene el número de filas
                int endRow = std::min(startRow + chunkSize, rowCount); // Determina la última fila a leer en este chunk
                /*--- Leemos las filas del excel desde donde indiquemos ---*/
                for(int row = startRow; row <= endRow; ++row){      //Itera sobre las filas
                    double dateValue = sheet->readNum(row, 0);  //Obtenemos la fecha
                    int year, month, day;                           //Declaramos las variables a utilizar en la fecha
                    book->dateUnpack(dateValue, &year, &month, &day);   //Cambiamos el tipo de fecha excel a tres variables independientes
                    double num = sheet->readNum(row, 1);    //Lee el contenido de la celda en la segunda columna como número
                    if(num==0){                                 //Si el número a leer es igual a cero, indica que hemos terminado de leer el excel
                        flag = true;                            //Cambiamos a true el flag
                        break;                                  //Salimos del for
                    }
                    //Ingresamos el dato en el mapa PEN_CLP
                    std::pair<int,int> fecha(year,month); //Declaramos un par {año,mes} para insertarlo en los mapas
                    if(penToClp.count(fecha) > 0){           //Si la clave ya existe, solo sumamos los datos
                        penToClp[fecha] += num;                 //Sumamos la transformación de cada día del mes
                        daysPerMonth[fecha]+=1;                 //Aumentamos la cantidad de días del mes
                    }
                    else{
                        // Si la clave no existe, crear una nueva entrada con la cantidad
                        penToClp[fecha] = num;                  //Asignamos la primera transformación del mes
                        daysPerMonth[fecha]+=1;                 //Aumentamos la cantidad de días
                    }
                }
                if(flag){                                       //Si el flag es true, quiere decir que hemos terminado de leer
                    startRow = 0;                               //Dejamos el startRow en 0 para avisar en el main que debemos dejar de iterar
                }
                else{
                    startRow = endRow;                          //En caso contrario, no hemos finalizado, por lo que indicamos que debemos seguir leyendo en la última fila que quedamos
                }
                
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

/*--- Función que permite obtener el promedio mensual de la transformación de soles a pesos chilenos en un mapa ---*/
void insertValueInMap(std::map<std::pair<int,int>,double>& penToClp, std::map<std::pair<int,int>,int>& daysPerMonth, std::map<std::pair<int,int>,float>& solesToPesos){
    auto pesos = penToClp.begin();
    auto dias = daysPerMonth.begin();
    /*--- En este for, iteramos sobre ambos mapas al mismo tiempo para obtener el promedio de la transformación en cada mes ---*/
    for(; pesos!=penToClp.end() && dias!=daysPerMonth.end(); pesos++, dias++){    //Iteramos sobre ambos mapas
        std::pair<int,int> fecha(pesos->first.first,pesos->first.second);   //Asignamos el promedio de pesos a cada mes en el mapa
        solesToPesos[fecha] = pesos->second/dias->second;
    }
}

/*--- Función que permite parsear línea a línea el archivo csv y guardar estos datos en un mapa ---*/
void sequentialParseCsv(std::string& filename, std::unordered_map<std::string,std::map<int,std::map<int,std::vector<float>>>>& mapaProductos){
    std::cout << "-- Parseo csv --" << std::endl;
    /*--- Lectura archivo csv ---*/
    std::fstream archivo(filename);
    if(!archivo){
        std::cout << "--- No se encontró la ruta del archivo ---" << std::endl;
    }
    else{
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
        std::string strQuantity;
        std::string fecha;
        std::string strAmount;
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
            getline(stream,strQuantity,delimitador);              //Cantidad
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
            getline(stream,strAmount,delimitador);                //Obtenemos el amount del producto
            /*--- Fin ---*/
            
            /*--- Asignamos la información extraída ---*/
            anho = stoi(fecha.substr(1,4));                                                 //Extraer el año
            mes = stoi(fecha.substr(6,2));                                                  //Extraer el mes
            quantity = stoi(strQuantity.substr(1,strQuantity.length()-2));                  //Debemos eliminar las comillas del string para transformarlo en float
            amount = stof(strAmount.substr(1,strAmount.length()-2));                        //Debemos eliminar las comillas del string para transformarlo en float
            /*--- Fin ---*/
            
            /*--- Ponemos los datos en el map anidado ---*/
            if(mapaProductos[sku][anho][mes].empty()){              //Si el campo está vacío, debemos inicializarlo en 0 para evitar errores después al incrementar y sumar
            mapaProductos[sku][anho][mes] = {0, 0};
            }
            mapaProductos[sku][anho][mes][0] += quantity;           //Sumamos la cantidad de veces que se compró el producto en ese mes
            mapaProductos[sku][anho][mes][1] += (amount*quantity);  //Sumamos el precio total de la compra (cantidad de veces que se compró por el precio unitario
            /*--- Con lo anterior, podemos obtener el precio promedio del sku en ese año y en ese mes al realizar la división sin la necesidad de tener la cantidad de días ---*/
            i++;                                                    //Incrementamos el contador
        }
        archivo.close();
        std::cout << std::endl;
    }
}