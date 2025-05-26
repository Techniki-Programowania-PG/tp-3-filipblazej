#pragma once

#include <vector>
#include <complex>

std::vector<double> generuj_sin(double czestotliwosc, double czestotliwosc_probek, double czas_trwania);
std::vector<double> generuj_cos(double czestotliwosc, double czestotliwosc_probek, double czas_trwania);
std::vector<double> generuj_prostokat(double czestotliwosc, double czestotliwosc_probek, double czas_trwania);
std::vector<double> generuj_pila(double czestotliwosc, double czestotliwosc_probek, double czas_trwania);

std::vector<std::complex<double>> transformata(const std::vector<double>& sygnal);
std::vector<double> odwrotna_transformata(const std::vector<std::complex<double>>& transformata);

std::vector<double> filtracja_1D(const std::vector<double>& sygnal, const std::vector<double>& filtr);

void wizualizacja_sygnalu(const std::vector<double>& sygnal);
void wizualizacja_transformaty(const std::vector<std::complex<double>>& transformata, double czestotliwosc_probek);