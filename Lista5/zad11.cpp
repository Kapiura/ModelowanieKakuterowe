#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

constexpr double krokCzasowy = 0.0015;
constexpr double masa = 1.0;
constexpr double stalaGrawitacji = 1.0;
constexpr int liczbaKrokow = 2000;
constexpr int liczbaCial = 3;
constexpr int szerokoscOkna = 800;
constexpr int wysokoscOkna = 800;

double sredniaOdleglosc(const std::vector<double>& pozycjeX, const std::vector<double>& pozycjeY) {
    double odleglosc01 = std::hypot(pozycjeX[0] - pozycjeX[1], pozycjeY[0] - pozycjeY[1]);
    double odleglosc02 = std::hypot(pozycjeX[0] - pozycjeX[2], pozycjeY[0] - pozycjeY[2]);
    double odleglosc12 = std::hypot(pozycjeX[1] - pozycjeX[2], pozycjeY[1] - pozycjeY[2]);
    return (odleglosc01 + odleglosc02 + odleglosc12) / 3.0;
}

class SymulacjaTrzechCial {
public:
    SymulacjaTrzechCial(
        const std::vector<double>& poczatkoweX,
        const std::vector<double>& poczatkoweY,
        const std::vector<double>& poczatkoweVX,
        const std::vector<double>& poczatkoweVY,
        double krokCzasowy, double masa, double stalaGrawitacji
    ) : liczbaCial(poczatkoweX.size()), krokCzasowy(krokCzasowy), masa(masa), stalaGrawitacji(stalaGrawitacji), krok(0),
        pozycjeX(poczatkoweX), pozycjeY(poczatkoweY), predkosciX(poczatkoweVX), predkosciY(poczatkoweVY),
        pozycjeX_nastepne(liczbaCial, 0.0), pozycjeY_nastepne(liczbaCial, 0.0),
        pozycjeX_poprzednie(liczbaCial, 0.0), pozycjeY_poprzednie(liczbaCial, 0.0) {}

    double wykonajKrokSymulacji() {
        krok++;
        std::vector<double> silyX(liczbaCial, 0.0);
        std::vector<double> silyY(liczbaCial, 0.0);

        for (int i = 0; i < liczbaCial; ++i) {
            for (int j = i + 1; j < liczbaCial; ++j) {
                double roznicaX = pozycjeX[i] - pozycjeX[j];
                double roznicaY = pozycjeY[i] - pozycjeY[j];
                double odleglosc = std::hypot(roznicaX, roznicaY);
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
            for (int cialo = 0; cialo < liczbaCial; ++cialo) {
                predkosciX[cialo] += (silyX[cialo] / masa) * krokCzasowy;
                predkosciY[cialo] += (silyY[cialo] / masa) * krokCzasowy;
                pozycjeX_nastepne[cialo] = pozycjeX[cialo] + predkosciX[cialo] * krokCzasowy;
                pozycjeY_nastepne[cialo] = pozycjeY[cialo] + predkosciY[cialo] * krokCzasowy;
            }
        } else {
            for (int cialo = 0; cialo < liczbaCial; ++cialo) {
                pozycjeX_nastepne[cialo] = 2 * pozycjeX[cialo] - pozycjeX_poprzednie[cialo] + (krokCzasowy * krokCzasowy * silyX[cialo] / masa);
                pozycjeY_nastepne[cialo] = 2 * pozycjeY[cialo] - pozycjeY_poprzednie[cialo] + (krokCzasowy * krokCzasowy * silyY[cialo] / masa);
            }
        }

        for (int cialo = 0; cialo < liczbaCial; ++cialo) {
            pozycjeX_poprzednie[cialo] = pozycjeX[cialo];
            pozycjeY_poprzednie[cialo] = pozycjeY[cialo];
            pozycjeX[cialo] = pozycjeX_nastepne[cialo];
            pozycjeY[cialo] = pozycjeY_nastepne[cialo];
            predkosciX[cialo] = (pozycjeX[cialo] - pozycjeX_poprzednie[cialo]) / (2 * krokCzasowy);
            predkosciY[cialo] = (pozycjeY[cialo] - pozycjeY_poprzednie[cialo]) / (2 * krokCzasowy);
        }

        return sredniaOdleglosc(pozycjeX, pozycjeY);
    }

    std::vector<double> pozycjeX, pozycjeY;
    int krok;

private:
    int liczbaCial;
    double krokCzasowy, masa, stalaGrawitacji;
    std::vector<double> predkosciX, predkosciY;
    std::vector<double> pozycjeX_nastepne, pozycjeY_nastepne;
    std::vector<double> pozycjeX_poprzednie, pozycjeY_poprzednie;
};

int main() {
    std::vector<double> poczatkoweX_bazowe = {0.5 - 0.2, 0.5 + 0.2, 0.5};
    std::vector<double> poczatkoweY_bazowe = {0.5 - 0.2, 0.5 - 0.2, 0.5};
    std::vector<double> poczatkoweVX_bazowe = {0.93240737 / 2.0, 0.93240737 / 2.0, -0.93240737};
    std::vector<double> poczatkoweVY_bazowe = {0.86473146 / 2.0, 0.86473146 / 2.0, -0.86473146};

    double perturbacja = 0.001;

    SymulacjaTrzechCial symulacja1(poczatkoweX_bazowe, poczatkoweY_bazowe, poczatkoweVX_bazowe, poczatkoweVY_bazowe, krokCzasowy, masa, stalaGrawitacji);

    auto poczatkoweX2 = poczatkoweX_bazowe;
    auto poczatkoweY2 = poczatkoweY_bazowe;
    poczatkoweX2[0] += perturbacja;
    SymulacjaTrzechCial symulacja2(poczatkoweX2, poczatkoweY2, poczatkoweVX_bazowe, poczatkoweVY_bazowe, krokCzasowy, masa, stalaGrawitacji);

    auto poczatkoweX3 = poczatkoweX_bazowe;
    auto poczatkoweY3 = poczatkoweY_bazowe;
    poczatkoweY3[1] += perturbacja;
    SymulacjaTrzechCial symulacja3(poczatkoweX3, poczatkoweY3, poczatkoweVX_bazowe, poczatkoweVY_bazowe, krokCzasowy, masa, stalaGrawitacji);

    sf::RenderWindow okno(sf::VideoMode(szerokoscOkna, wysokoscOkna), "Symulacja trzech ciał");
    okno.setFramerateLimit(60);

    sf::CircleShape punkty1(6), punkty2(6), punkty3(6);
    punkty1.setFillColor(sf::Color::Black);
    punkty2.setFillColor(sf::Color::Red);
    punkty3.setFillColor(sf::Color::Green);

    std::vector<double> kroki;
    std::vector<double> srednieOdleglosci_sym1, srednieOdleglosci_sym2, srednieOdleglosci_sym3;

    int klatka = 0;
    while (okno.isOpen() && klatka < liczbaKrokow) {
        sf::Event zdarzenie;
        while (okno.pollEvent(zdarzenie)) {
            if (zdarzenie.type == sf::Event::Closed)
                okno.close();
        }

        double odleglosc1 = symulacja1.wykonajKrokSymulacji();
        double odleglosc2 = symulacja2.wykonajKrokSymulacji();
        double odleglosc3 = symulacja3.wykonajKrokSymulacji();

        kroki.push_back(klatka);
        srednieOdleglosci_sym1.push_back(odleglosc1);
        srednieOdleglosci_sym2.push_back(odleglosc2);
        srednieOdleglosci_sym3.push_back(odleglosc3);

        std::cout << "Krok: " << klatka << std::endl;

        okno.clear(sf::Color::White);

        auto rysujCialo = [&](SymulacjaTrzechCial& symulacja, sf::CircleShape& ksztalt) {
            for (int i = 0; i < liczbaCial; ++i) {
                ksztalt.setPosition(98 * symulacja.pozycjeX[i] + szerokoscOkna / 2, 98 * symulacja.pozycjeY[i] + wysokoscOkna / 2);
                okno.draw(ksztalt);
            }
        };

        rysujCialo(symulacja1, punkty1);
        rysujCialo(symulacja2, punkty2);
        rysujCialo(symulacja3, punkty3);

        okno.display();
        klatka++;
    }

    plt::figure_size(1130, 800);
    plt::plot(kroki, srednieOdleglosci_sym1, {{"label", "Symulacja 1 (warunki bazowe)"}});
    plt::plot(kroki, srednieOdleglosci_sym2, {{"label", "Symulacja 2 (perturbacja ciała 0)"}});
    plt::plot(kroki, srednieOdleglosci_sym3, {{"label", "Symulacja 3 (perturbacja ciała 1)"}});

    plt::xlabel("Krok symulacji");
    plt::ylabel("Średnia odległość między ciałami");
    plt::title("Porównanie zachowania chaotycznego");
    plt::legend();
    plt::grid(true);

    plt::save("zad1.png");
    return 0;
}