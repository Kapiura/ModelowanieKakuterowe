#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

int main() {
    const double krok_czasowy = 0.01;
    const int liczba_krokow = 10000;

    double x = 1.0, y = 1.0, z = 1.0;
    std::vector<double> trajektoria_x, trajektoria_y, trajektoria_z;

    for (int i = 0; i < liczba_krokow; ++i) {
        double dx = 10.0 * (y - x);
        double dy = -x * z + 28.0 * x - y;
        double dz = x * y - (8.0 / 3.0) * z;

        x += dx * krok_czasowy;
        y += dy * krok_czasowy;
        z += dz * krok_czasowy;

        trajektoria_x.push_back(x);
        trajektoria_y.push_back(y);
        trajektoria_z.push_back(z);
    }

    std::ofstream plik_wyjsciowy("wyniki_zadanie3.txt");
    plik_wyjsciowy << std::fixed << std::setprecision(6);
    for (size_t i = 0; i < trajektoria_x.size(); ++i) {
        plik_wyjsciowy << trajektoria_x[i] << " " << trajektoria_y[i] << " " << trajektoria_z[i] << "\n";
    }
    plik_wyjsciowy.close();

    return 0;
}