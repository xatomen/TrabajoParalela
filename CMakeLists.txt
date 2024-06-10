cmake_minimum_required(VERSION 3.28)
project(TrabajoParalela)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(TrabajoParalela main.cpp
        Producto.cpp
        Producto.h
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(TrabajoParalela PUBLIC OpenMP::OpenMP_CXX)
endif()
