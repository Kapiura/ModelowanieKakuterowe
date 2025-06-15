#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

int main() {
    const int liczba_krokow = 1000;
    const int liczba_iteracji = 1000;
    const double lambda = 1.999;
    const double x_poczatkowe = 0.434;

    std::vector<double> poczatkowe_roznice;
    std::vector<double> skumulowane_odleglosci;

    for (int i = 1; i <= liczba_krokow; ++i) {
        double delta = i * 0.000001;
        double x = x_poczatkowe;
        double y = x_poczatkowe + delta;

        std::vector<double> seria_x;
        std::vector<double> seria_y;

        for (int j = 0; j < liczba_iteracji; ++j) {
            x *= lambda;
            y *= lambda;
            if (x > 1.0) x -= 1.0;
            if (y > 1.0) y -= 1.0;
            seria_x.push_back(x);
            seria_y.push_back(y);
        }

        double suma_odleglosci = 0.0;
        for (size_t k = 0; k < seria_x.size(); ++k) {
            suma_odleglosci += std::abs(seria_x[k] - seria_y[k]);
        }

        poczatkowe_roznice.push_back(delta);
        skumulowane_odleglosci.push_back(suma_odleglosci);
    }

    std::ofstream plik_wyjsciowy("wyniki_zadanie1.txt");
    plik_wyjsciowy << std::fixed << std::setprecision(6);
    for (size_t i = 0; i < poczatkowe_roznice.size(); ++i) {
        plik_wyjsciowy << poczatkowe_roznice[i] << " " << skumulowane_odleglosci[i] << "\n";
    }
    plik_wyjsciowy.close();

    return 0;
}