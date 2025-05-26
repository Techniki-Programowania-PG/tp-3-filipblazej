#include <Projekt3Funkcje.hpp>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>

namespace py = pybind11;

PYBIND11_MODULE(Projekt3, m)
{
    m.doc() = "Biblioteka do wizualizacji sygnalow";

    m.def("generuj_sin", &generuj_sin, "Funkcja Generuje sygnal sinus", py::arg("czestotliwosc"), py::arg("czestotliwosc_probek"), py::arg("czas_trwania"));
    m.def("generuj_cos", &generuj_cos, "Funkcja Generuje sygnal cosinus", py::arg("czestotliwosc"), py::arg("czestotliwosc_probek"), py::arg("czas_trwania"));
    m.def("generuj_prostokat", &generuj_prostokat, "Funkcja Generuje sygnal prostokatny", py::arg("czestotliwosc"), py::arg("czestotliwosc_probek"), py::arg("czas_trwania"));
    m.def("generuj_pila", &generuj_pila, "Funkcja Generuje sygnal piloksztaltnym", py::arg("czestotliwosc"), py::arg("czestotliwosc_probek"), py::arg("czas_trwania"));

    m.def("transformata", &transformata, "Generuje transformate z podanego sygnalu", py::arg("sygnal"));
    m.def("odwrotna_transformata", &odwrotna_transformata, "Generuje transformate odwrotna", py::arg("transformata"));

    m.def("filtracja_1D", &filtracja_1D, "Wykonuje filtracje1D sygnalu korzystajac z podanego filtra", py::arg("sygnal") , py::arg("filtr"));

    m.def("wizualizacja_sygnalu", &wizualizacja_sygnalu, "Funkcja wizualizuje sygnal", py::arg("sygnal"));
    m.def("wizualizacja_transformaty", &wizualizacja_transformaty, "Funkcja wizualizuje transformate", py::arg("transformata"), py::arg("czestotliwosc_probek"));
}
