cmake_minimum_required(VERSION 3.28)
project(TrabajoParalela)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

add_executable(
        TrabajoParalela
        src/main.cpp
        src/read.cpp
        include/read.h
        src/filterBasicBasket.cpp
        include/filterBasicBasket.h
        src/calculateVariation.cpp
        include/calculateVariation.h
)

set(LIBXL_BASE_DIR "${CMAKE_SOURCE_DIR}/include/libxl-4.3.0.14")
include_directories(${LIBXL_BASE_DIR}/include_cpp)

target_link_libraries(
        TrabajoParalela
        ${LIBXL_BASE_DIR}/lib64/libxl.so
        OpenMP::OpenMP_CXX
)
