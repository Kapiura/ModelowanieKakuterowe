#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>

const double g = 9.81;
const double L1 = 1.0;
const double L2 = 1.0;
const double m1 = 1.0;
const double m2 = 1.0;

const double dt = 0.01;
const int steps = 2000;
const int num_pendulums = 100;

struct StanWahadla {
    double theta1;
    double p1;
    double theta2;
    double p2;
};

struct Pozycja {
    double x1, y1;
    double x2, y2;
};

struct DaneFazowe {
    double theta2;
    double dtheta2;
};

std::pair<std::vector<Pozycja>, std::vector<DaneFazowe>> symuluj(const StanWahadla& init) {
    std::vector<Pozycja> pozycje;
    std::vector<DaneFazowe> dane_fazowe;

    double theta1 = init.theta1;
    double p1 = init.p1;
    double theta2 = init.theta2;
    double p2 = init.p2;

    for (int i = 0; i < steps; ++i) {
        double delta = theta2 - theta1;
        double den1 = (m1 + m2) * L1 - m2 * L1 * std::cos(delta) * std::cos(delta);

        double dtheta1 = p1;
        double dtheta2 = p2;

        double dp1 = (
            (m2 * L1 * p1 * p1 * std::sin(delta) * std::cos(delta))
            + (m2 * g * std::sin(theta2) * std::cos(delta))
            + (m2 * L2 * p2 * p2 * std::sin(delta))
            - ((m1 + m2) * g * std::sin(theta1))
            / den1);

        double dp2 = (
            (-m2 * L2 * p2 * p2 * std::sin(delta) * std::cos(delta))
            + ((m1 + m2) * g * std::sin(theta1) * std::cos(delta))
            - ((m1 + m2) * L1 * p1 * p1 * std::sin(delta))
            - ((m1 + m2) * g * std::sin(theta2)))
            / (L2 * ((m1 + m2) - m2 * std::cos(delta) * std::cos(delta)));

        theta1 += dtheta1 * dt;
        p1 += dp1 * dt;
        theta2 += dtheta2 * dt;
        p2 += dp2 * dt;

        double x1 = L1 * std::sin(theta1);
        double y1 = -L1 * std::cos(theta1);
        double x2 = x1 + L2 * std::sin(theta2);
        double y2 = y1 - L2 * std::cos(theta2);

        pozycje.push_back({x1, y1, x2, y2});
        dane_fazowe.push_back({theta2, dtheta2});
    }

    return {pozycje, dane_fazowe};
}

int main() {
    std::vector<StanWahadla> warunki_poczatkowe;
    double theta1 = M_PI / 2;
    double theta2 = M_PI / 2;

    for (int i = 0; i < num_pendulums; ++i) {
        double delta = i * 1e-5;
        warunki_poczatkowe.push_back({theta1 + delta, 0.0, theta2, 0.0});
    }

    std::vector<std::vector<Pozycja>> wszystkie_pozycje;
    std::vector<std::vector<DaneFazowe>> przestrzenie_fazowe;

    for (const auto& init : warunki_poczatkowe) {
        auto [pos, phase] = symuluj(init);
        wszystkie_pozycje.push_back(pos);
        przestrzenie_fazowe.push_back(phase);
    }

    std::ofstream plik_animacja("wyniki_zadanie4_animacja.txt");
    plik_animacja << std::fixed << std::setprecision(6);
    for (int frame = 0; frame < steps; ++frame) {
        for (int i = 0; i < num_pendulums; ++i) {
            const auto& pos = wszystkie_pozycje[i][frame];
            plik_animacja << frame << " " << i << " "
                         << 0.0 << " " << 0.0 << " "
                         << pos.x1 << " " << pos.y1 << " "
                         << pos.x2 << " " << pos.y2 << "\n";
        }
    }
    plik_animacja.close();

    std::vector<int> wybrane = {1, 99};
    for (int idx : wybrane) {
        std::ofstream plik_faza("wyniki_zadanie4_faza_" + std::to_string(idx) + ".txt");
        plik_faza << std::fixed << std::setprecision(6);
        for (const auto& phase : przestrzenie_fazowe[idx]) {
            plik_faza << phase.theta2 << " " << phase.dtheta2 << "\n";
        }
        plik_faza.close();
    }

    return 0;
}