#define M_PI 3.14159265358979323846
#include <cmath>
#include <matplot/matplot.h>
#include <vector>
#include <complex>

std::vector<double> generuj_sin(double czestotliwosc, double czestotliwosc_probek, double czas_trwania)
{
	int liczba_probek = (czestotliwosc_probek * czas_trwania);
	
	std::vector<double> sinus(liczba_probek);

	for (int i = 0; i < liczba_probek; ++i)
	{
		sinus[i] = sin(M_PI * 2 * czestotliwosc * (i / czestotliwosc_probek));
	}

	return sinus;
}

std::vector<double> generuj_cos(double czestotliwosc, double czestotliwosc_probek, double czas_trwania)
{
	int liczba_probek = (czestotliwosc_probek * czas_trwania);

	std::vector<double> cosinus(liczba_probek);

	for (int i = 0; i < liczba_probek; ++i)
	{
		cosinus[i] = cos(M_PI * 2 * czestotliwosc * (i / czestotliwosc_probek));
	}

	return cosinus;
}

std::vector<double> generuj_prostokat(double czestotliwosc, double czestotliwosc_probek, double czas_trwania)
{
	int liczba_probek = (czestotliwosc_probek * czas_trwania);

	double okres = (1/czestotliwosc);
	double czas_probki = (czas_trwania / liczba_probek);

	std::vector<double> prostokat(liczba_probek);

	for (int i=0; i < liczba_probek; ++i)
	{
		double czas = i*czas_probki;
		double czesc_okresu = std::fmod(czas , okres);

		prostokat[i] = (czesc_okresu < okres / 2) ? 1 : (-1);
	}

	return prostokat;
}

std::vector<double> generuj_pila(double czestotliwosc, double czestotliwosc_probek, double czas_trwania)
{
	int liczba_probek = (czestotliwosc_probek * czas_trwania);
	double czas_probki = (czas_trwania / liczba_probek);

	std::vector<double> pila(liczba_probek);

	for(int i =0; i < liczba_probek; ++i)
	{
		double czas = i*czas_probki;
		pila[i] = 2* ((czas * czestotliwosc) - std::floor((czas*czestotliwosc) +0.5));
	}

	return pila;
}

/*
std::vector<std::complex<double>> transformata(const std::vector<double>& sygnal)
{
	int liczba_probek = sygnal.size();

	std::vector<std::complex<double>> transformata(liczba_probek);

	for(size_t i = 0; i < liczba_probek; ++i)
	{
		std::complex<double> suma = 0;
		for (size_t j = 0; j < liczba_probek; ++j)
		{
			suma = suma + (sygnal[j] * std::exp(std::complex<double>(0, (-2*M_PI*(static_cast<double>(i)/static_cast<double>(liczba_probek)) * j))));
		}
		transformata[i] = suma;
	}

	return transformata;
}
	*/

std::vector<std::complex<double>> transformata(const std::vector<double>& sygnal)
{
	int liczba_probek = sygnal.size();

	std::vector<std::complex<double>> transformata(liczba_probek);

	for(size_t i = 0; i < liczba_probek; ++i)
	{
		std::complex<double> suma = (0,0);
		for (size_t j = 0; j < liczba_probek; ++j)
		{
			double kat = -2*M_PI*i*j/liczba_probek;
			suma = suma + std::polar(sygnal[j], kat);
		}
		transformata[i] = suma;
	}

	return transformata;
}

std::vector<double> odwrotna_transformata(const std::vector<std::complex<double>>& transformata)
{
	int liczba_probek = transformata.size();

	std::vector<double> sygnal(liczba_probek);

	for(size_t i = 0; i < liczba_probek; ++i)
	{
		std::complex<double> suma = 0;
		for (size_t j = 0; j < liczba_probek; ++j)
		{
			suma = suma + (transformata[j] * std::exp(std::complex<double>(0, (2*M_PI*(static_cast<double>(i)/static_cast<double>(liczba_probek)) * j))));
		}
		sygnal[i] = suma.real() / static_cast<double>(liczba_probek);
	}

	return sygnal;
}

std::vector<double> filtracja_1D(const std::vector<double>& sygnal, const std::vector<double>& filtr)
{
	int dlugosc_sygnalu = sygnal.size();
	int dlugosc_filtra = filtr.size();
	int dlugosc_wyniku = dlugosc_sygnalu + dlugosc_filtra - 1;

	std::vector<double> filtracja(dlugosc_wyniku);

	for(int i = 0; i < dlugosc_wyniku; ++i)
	{
		double suma = 0;
		for(int j = 0; j < dlugosc_sygnalu; ++j)
		{
			if((i -j) >=0 && (i-j) < dlugosc_filtra)
			 {
				suma = suma + (sygnal[j] * filtr[i - j]);
			 }
		}
		filtracja[i] = suma;
	}
	return filtracja;
}

void wizualizacja_sygnalu(const std::vector<double>& sygnal)
{
	std::vector<int> x(sygnal.size());
	for (int i = 0; i < sygnal.size(); ++i)
	 x[i] = i;

	auto fig = matplot::figure(true);

	matplot::plot(x, sygnal);
	matplot::xlabel("Probka");
	matplot::ylabel("Amplituda");
	matplot::title("Rysunek sygnalu");
	matplot::show();
}
/*void wizualizacja_transformaty(const std::vector<std::complex<double>>& transformata)
{
	std::vector<int> x(transformata.size());
	std::vector<double> y(transformata.size());
	for (int i = 0; i < transformata.size(); ++i)
	{
	 x[i] = i;
	 y[i] = std::abs(transformata[i]);
	}

	auto fig = matplot::figure(true);

	matplot::stem(x, y);
	matplot::xlabel("Probka");
	matplot::ylabel("Amplituda");
	matplot::title("Rysunek transformaty");
	matplot::show();
}*/

void wizualizacja_transformaty(const std::vector<std::complex<double>>& transformata, double czestotliwosc_probek)
{
	std::vector<int> x(transformata.size());
	std::vector<double> y(transformata.size());
	for (int i = 0; i < (transformata.size()/2); ++i)
	{
	 x[i] = (i*(czestotliwosc_probek/ static_cast<double>(transformata.size())));
	 y[i] = (std::abs(transformata[i])*2/transformata.size());
	}

	auto fig = matplot::figure(true);

	matplot::stem(x, y);
	matplot::xlabel("Czestotliwość");
	matplot::ylabel("Amplituda");
	matplot::title("Rysunek transformaty");
	matplot::show();
}





