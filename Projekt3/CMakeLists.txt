cmake_minimum_required(VERSION 3.14)
project(Projekt3)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(pybind11)
add_subdirectory(matplotplusplus)

include_directories(include)
file(GLOB SOURCES src/*.cpp)

pybind11_add_module(Projekt3 ${SOURCES} python_bindings/pythonbiblioteka.cpp)

target_link_libraries(Projekt3 PRIVATE matplot)