#include <iostream>
#include <ctime>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

std::vector<std::pair<int, int>> klaster;
std::vector<std::pair<int, int>> czastki;
std::vector<std::pair<int, int>> directions =
{
            {0, 1},
    {-1, 0},         {1, 0},
            {0, -1},
};

void init(int ile_czastek)
{
    klaster.clear();
    czastki.clear();

    klaster.emplace_back(0, 0);
    for(int i = 0; i < ile_czastek; i++)
    {
        int x = std::rand() % 200 - 100;
        int y = std::rand() % 200 - 100;
        czastki.emplace_back(x, y);
    }
}

double oblicz_promien()
{
    double r2 = 0.0;
    for (const auto& p : klaster)
        r2 += p.first * p.first + p.second * p.second;

    return std::sqrt(r2 / klaster.size());
}

void simulate(int max_krokow)
{
    for (int krok = 0; krok < max_krokow; krok++)
    {
        for (auto it = czastki.begin(); it != czastki.end(); )
        {
            auto& el = *it;
            auto dir = directions[std::rand() % directions.size()];
            el.first += dir.first;
            el.second += dir.second;

            bool dotknelo = false;
            for (const auto& k : klaster)
            {
                if (std::abs(k.first - el.first) + std::abs(k.second - el.second) == 1)
                {
                    klaster.emplace_back(el);
                    it = czastki.erase(it);
                    dotknelo = true;
                    break;
                }
            }

            if (!dotknelo)
                ++it;
        }

        if (czastki.empty()) break;
    }
}

int main()
{
    std::srand(std::time(nullptr));

    std::ofstream dane("skala.csv");
    dane << "N,R\n";

    for (int N = 1000; N <= 10000; N += 1000)
    {
        init(N);
        simulate(1000);
        double R = oblicz_promien();
        dane << klaster.size() << "," << R << "\n";
        std::cout << "N=" << N << " -> klaster=" << klaster.size() << " R=" << R << "\n";

        std::string filename = "dla_" + std::to_string(N) + ".png";
        FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
        fprintf(gnuplotPipe, "set terminal pngcairo size 800,800 enhanced font ',10'\n");
        fprintf(gnuplotPipe, "set output '%s'\n", filename.c_str());
        fprintf(gnuplotPipe, "set xrange [-100:100]\n");
        fprintf(gnuplotPipe, "set yrange [-100:100]\n");
        fprintf(gnuplotPipe, "set size square\n");
        fprintf(gnuplotPipe, "unset key\n");
        fprintf(gnuplotPipe, "set title 'DLA dla N = %d'\n", N);
        fprintf(gnuplotPipe, "plot '-' with points pt 7 ps 0.8 lc rgb 'blue'\n");

        for (const auto& p : klaster)
            fprintf(gnuplotPipe, "%d %d\n", p.first, p.second);
        fprintf(gnuplotPipe, "e\n");

        fflush(gnuplotPipe);
        pclose(gnuplotPipe);
    }

    dane.close();

    return 0;
}