#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

int main() {
    const double r = 3.95;
    const double x_poczatkowe = 0.5;
    const int liczba_iteracji = 1000;
    const int liczba_do_wykresu = 80;

    double x = x_poczatkowe;
    std::vector<double> seria_x;

    for (int i = 0; i < liczba_iteracji; ++i) {
        x = r * x * (1 - x);
        seria_x.push_back(x);
    }

    std::ofstream plik_wyjsciowy("wyniki_zadanie2.txt");
    plik_wyjsciowy << std::fixed << std::setprecision(6);
    for (int i = liczba_iteracji - liczba_do_wykresu; i < liczba_iteracji; ++i) {
        plik_wyjsciowy << i << " " << seria_x[i] << "\n";
    }
    plik_wyjsciowy.close();

    return 0;
}