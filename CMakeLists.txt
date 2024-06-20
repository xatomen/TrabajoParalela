cmake_minimum_required(VERSION 3.28)
project(TrabajoParalela)

set(CMAKE_CXX_STANDARD 17)

#find_package(OpenMP REQUIRED)

add_executable(TrabajoParalela main.cpp
        Producto.cpp
        Producto.h
)

include_directories(libxl-4.3.0.14/include_cpp)
target_link_libraries(TrabajoParalela
        /home/jorge/Escritorio/Proyectos/TrabajoParalela/libxl-4.3.0.14/lib64/libxl.so
)

#if(OpenMP_CXX_FOUND)
#    target_link_libraries(TrabajoParalela PUBLIC OpenMP::OpenMP_CXX libxl-4.3.0.14/lib64/libxl.so)
#endif()
