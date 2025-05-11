#include <iostream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

class SymulacjaTrzechCial {
public:
    int liczbaCial;
    double krokCzasowy, masa, stalaGrawitacji;
    int krok;
    std::vector<double> pozycjeX, pozycjeY, predkosciX, predkosciY;
    std::vector<double> pozycjeX_nastepne, pozycjeY_nastepne;
    std::vector<double> pozycjeX_poprzednie, pozycjeY_poprzednie;

    SymulacjaTrzechCial(std::vector<double>& poczatkoweX, std::vector<double>& poczatkoweY,
                        std::vector<double>& poczatkoweVX, std::vector<double>& poczatkoweVY,
                        double krokCzasowy, double masa, double stalaGrawitacji)
        : liczbaCial(poczatkoweX.size()), krokCzasowy(krokCzasowy), masa(masa), stalaGrawitacji(stalaGrawitacji), krok(0),
          pozycjeX(poczatkoweX), pozycjeY(poczatkoweY), predkosciX(poczatkoweVX), predkosciY(poczatkoweVY),
          pozycjeX_nastepne(liczbaCial), pozycjeY_nastepne(liczbaCial),
          pozycjeX_poprzednie(liczbaCial), pozycjeY_poprzednie(liczbaCial) {}

    std::pair<std::vector<double>, std::vector<double>> wykonajKrokSymulacji() {
        krok++;
        std::vector<double> silyX(liczbaCial, 0.0), silyY(liczbaCial, 0.0);

        for (int i = 0; i < liczbaCial; i++) {
            for (int j = i + 1; j < liczbaCial; j++) {
                double roznicaX = pozycjeX[i] - pozycjeX[j];
                double roznicaY = pozycjeY[i] - pozycjeY[j];
                double odleglosc = std::sqrt(roznicaX * roznicaX + roznicaY * roznicaY);
                if (odleglosc != 0) {
                    double kierunekX = roznicaX / odleglosc;
                    double kierunekY = roznicaY / odleglosc;
                    double sila = stalaGrawitacji * masa * masa / (odleglosc * odleglosc);
                    double silaX = kierunekX * sila;
                    double silaY = kierunekY * sila;
                    silyX[i] -= silaX;
                    silyY[i] -= silaY;
                    silyX[j] += silaX;
                    silyY[j] += silaY;
                }
            }
        }

        if (krok == 1) {
            for (int cialo = 0; cialo < liczbaCial; cialo++) {
                predkosciX[cialo] += (silyX[cialo] / masa) * krokCzasowy;
                predkosciY[cialo] += (silyY[cialo] / masa) * krokCzasowy;
                pozycjeX_nastepne[cialo] = pozycjeX[cialo] + predkosciX[cialo] * krokCzasowy;
                pozycjeY_nastepne[cialo] = pozycjeY[cialo] + predkosciY[cialo] * krokCzasowy;
            }
        } else {
            for (int cialo = 0; cialo < liczbaCial; cialo++) {
                pozycjeX_nastepne[cialo] = 2 * pozycjeX[cialo] - pozycjeX_poprzednie[cialo] + (krokCzasowy * krokCzasowy * silyX[cialo] / masa);
                pozycjeY_nastepne[cialo] = 2 * pozycjeY[cialo] - pozycjeY_poprzednie[cialo] + (krokCzasowy * krokCzasowy * silyY[cialo] / masa);
            }
        }

        for (int cialo = 0; cialo < liczbaCial; cialo++) {
            pozycjeX_poprzednie[cialo] = pozycjeX[cialo];
            pozycjeY_poprzednie[cialo] = pozycjeY[cialo];
            pozycjeX[cialo] = pozycjeX_nastepne[cialo];
            pozycjeY[cialo] = pozycjeY_nastepne[cialo];
            predkosciX[cialo] = (pozycjeX[cialo] - pozycjeX_poprzednie[cialo]) / (2 * krokCzasowy);
            predkosciY[cialo] = (pozycjeY[cialo] - pozycjeY_poprzednie[cialo]) / (2 * krokCzasowy);
        }

        return {pozycjeX, pozycjeY};
    }
};

void rysujWykres(const std::vector<int>& kroki, const std::vector<double>& trajektoriaX1,
                const std::vector<double>& trajektoriaY1, const std::vector<double>& trajektoriaX2,
                const std::vector<double>& trajektoriaY2, const std::vector<double>& trajektoriaX3,
                const std::vector<double>& trajektoriaY3) {
    plt::figure_size(1130, 800);

    plt::plot(trajektoriaX1, trajektoriaY1, {{"label", "Ciało 1"}, {"color", "red"}});
    plt::plot(trajektoriaX2, trajektoriaY2, {{"label", "Ciało 2"}, {"color", "green"}});
    plt::plot(trajektoriaX3, trajektoriaY3, {{"label", "Ciało 3"}, {"color", "blue"}});

    plt::xlabel("X (skala)");
    plt::ylabel("Y (skala)");
    plt::title("Trajektorie trzech ciał");
    plt::legend();
    plt::grid(true);

    plt::save("zad2.png");
}

void rysujPojedynczeCialo(const std::vector<double>& trajektoriaX, const std::vector<double>& trajektoriaY, const std::string& etykieta, const std::string& kolor, const std::string& nazwaPliku) {
    plt::figure_size(1130, 800);

    plt::plot(trajektoriaX, trajektoriaY, {{"label", etykieta}, {"color", kolor}});

    plt::xlabel("X (skala)");
    plt::ylabel("Y (skala)");
    plt::title("Trajektoria " + etykieta);
    plt::legend();
    plt::grid(true);

    plt::save(nazwaPliku);
}

int main() {
    double krokCzasowy = 0.0015, masa = 1.0, stalaGrawitacji = 1.0;
    int liczbaKrokow = 3000;

    std::vector<double> poczatkoweX = {0.97000436, -0.97000436, 0.0};
    std::vector<double> poczatkoweY = {-0.24308753, 0.24308753, 0.0};
    std::vector<double> poczatkoweVX = {0.466203685, 0.466203685, -0.93240737};
    std::vector<double> poczatkoweVY = {0.432365730, 0.432365730, -0.86473146};

    SymulacjaTrzechCial symulacja(poczatkoweX, poczatkoweY, poczatkoweVX, poczatkoweVY, krokCzasowy, masa, stalaGrawitacji);

    std::vector<double> trajektoriaX1, trajektoriaY1, trajektoriaX2, trajektoriaY2, trajektoriaX3, trajektoriaY3;

    for (int i = 0; i < liczbaKrokow; i++) {
        auto [x, y] = symulacja.wykonajKrokSymulacji();

        trajektoriaX1.push_back(x[0]);
        trajektoriaY1.push_back(y[0]);

        trajektoriaX2.push_back(x[1]);
        trajektoriaY2.push_back(y[1]);

        trajektoriaX3.push_back(x[2]);
        trajektoriaY3.push_back(y[2]);

        std::cout << "Krok: " << symulacja.krok << std::endl;
    }

    rysujWykres({0, 1, 2, 3}, trajektoriaX1, trajektoriaY1, trajektoriaX2, trajektoriaY2, trajektoriaX3, trajektoriaY3);
    rysujPojedynczeCialo(trajektoriaX1, trajektoriaY1, "Ciało 1", "red", "cialo1.png");
    rysujPojedynczeCialo(trajektoriaX2, trajektoriaY2, "Ciało 2", "green", "cialo2.png");
    rysujPojedynczeCialo(trajektoriaX3, trajektoriaY3, "Ciało 3", "blue", "cialo3.png");

    return 0;
}