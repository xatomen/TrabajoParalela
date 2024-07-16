Asignatura: Computación paralela y distribuida

Sección: 411

Profesor: Sebastián Salazar Molina

Desarrollado por:

  - Jorge Luis Gallardo Contreras
  - Fernando Tomás Maldonado Rodríguez
  - Felipe Ignacio Vera Andrade
    
Requisitos:
  1. OpenMP (se puede instalar con el comando: sudo apt-get install libomp-dev)
  2. CMake (se puede instalar con el comando: sudo apt-get install cmake)
  3. Make (se puede instalar con el comando: sudo apt-get install make)
  4. GCC (se puede instalar con el comando: sudo apt-get install gcc)
     
Para iniciar el programa se deben seguir los siguientes pasos:
  1. Abrir una terminal de linux en la ubicación del proyecto.
  2. Escribir los siguientes comandos:
     
     cmake .
     
     make
     
     ./TrabajoParalela <ubicación archivo .xlsx> <ubicación archivo .csv>

Probado en:
  1. Ubuntu 24.04
  2. Ubuntu 23.10 (Fue necesario cambiar la versión requerida de CMake de 3.28 a 3.27 en el archivo CMakeLists.txt)
  3. Cantidad de procesadores: 4
  4. Cantidad de RAM: 4 GB
