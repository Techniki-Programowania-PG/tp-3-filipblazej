import Projekt3

sygnal4 = [1, 2, 3]
filtr = [1, 0, -1]
wynik = Projekt3.filtracja_1D(sygnal4, filtr)
print(wynik)
Projekt3.wizualizacja_sygnalu(sygnal = wynik)

sygnal = Projekt3.generuj_sin(czestotliwosc=100, czestotliwosc_probek=1000, czas_trwania=1.0)
Projekt3.wizualizacja_sygnalu(sygnal = sygnal)
transformata = Projekt3.transformata(sygnal = sygnal)
Projekt3.wizualizacja_transformaty(transformata = transformata, czestotliwosc_probek = 1000)
otransformata = Projekt3.odwrotna_transformata(transformata = transformata)
Projekt3.wizualizacja_sygnalu(sygnal = otransformata)

sygnal1 = Projekt3.generuj_cos(czestotliwosc=10, czestotliwosc_probek=1000, czas_trwania=1.0)
Projekt3.wizualizacja_sygnalu(sygnal = sygnal1)

sygnal2 = Projekt3.generuj_prostokat(czestotliwosc=10, czestotliwosc_probek=1000, czas_trwania=1.0)
Projekt3.wizualizacja_sygnalu(sygnal = sygnal2)

sygnal3 = Projekt3.generuj_pila(czestotliwosc=10, czestotliwosc_probek=1000, czas_trwania=1.0)
Projekt3.wizualizacja_sygnalu(sygnal = sygnal3)

