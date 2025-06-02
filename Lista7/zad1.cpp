#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <utility>
#include <fstream>
#include <cmath>

std::vector<std::pair<int, int>> directions =
{
             {0, 1},
    {-1, 0},         {1, 0},
             {0, -1},
};

std::pair<int, int> operator+(const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
{
    return std::make_pair(lhs.first + rhs.first, lhs.second + rhs.second);
}

void simulate(std::set<std::pair<int, int>>& pos, int steps)
{
    for (int i = 0; i < steps; i++)
    {
        auto it = pos.begin();
        std::advance(it, std::rand() % pos.size());
        auto cellPos = *it;
        auto dir = directions[std::rand() % directions.size()];
        auto newPos = cellPos + dir;

        if (pos.find(newPos) != pos.end())
            continue;

        bool neigh = true;
        for (const auto& d : directions)
        {
            if (pos.find(newPos + d) == pos.end())
            {
                neigh = false;
                break;
            }
        }

        if (!neigh)
        {
            pos.insert(newPos);
        }
    }
}

double compute_radius(const std::set<std::pair<int, int>>& pos)
{
    double maxR = 0.0;
    for (const auto& p : pos)
    {
        double r = std::sqrt(p.first * p.first + p.second * p.second);
        if (r > maxR)
            maxR = r;
    }
    return maxR;
}

void save_to_file(const std::set<std::pair<int, int>>& pos, const std::string& filename)
{
    std::ofstream file(filename);
    for (const auto& el : pos)
        file << el.first << " " << el.second << "\n";
    file.close();
}

void draw_with_gnuplot()
{
    FILE* gp = popen("gnuplot -persistent", "w");
    if (!gp) {
        std::cerr << "Nie udało się uruchomić gnuplota\n";
        return;
    }

    fprintf(gp, "set terminal pngcairo size 800,800\n");
    fprintf(gp, "set output 'eden.png'\n");
    fprintf(gp, "set title 'Klaster Edena'\n");
    fprintf(gp, "unset key\n");
    fprintf(gp, "set size ratio -1\n");
    fprintf(gp, "plot 'eden_data.dat' with points pt 7 ps 0.5 lc rgb 'red'\n");
    fflush(gp);
    pclose(gp);
}

void plot_scaling()
{
    FILE* gp = popen("gnuplot -persistent", "w");
    if (!gp) {
        std::cerr << "Nie udało się uruchomić gnuplota do rysowania skali\n";
        return;
    }

    fprintf(gp,
        "set terminal pngcairo size 800,600\n"
        "set output 'eden_scaling.png'\n"
        "set title 'Skalowanie promienia klastra'\n"
        "set xlabel 'log(N)'\n"
        "set ylabel 'log(R)'\n"
        "set logscale xy\n"
        "set grid\n"
        "plot 'eden_skala.csv' every ::1 using (log($1)):(log($2)) with linespoints title 'Eden scaling'\n"
    );
    fflush(gp);
    pclose(gp);
}


int main()
{
    std::srand(std::time(nullptr));

    // Nadpisujemy plik na nowo z nagłówkiem
    std::ofstream f("eden_skala.csv");
    f << "N,R\n";
    f.close();

    for (int steps = 1000; steps <= 10000; steps += 1000)
    {
        std::set<std::pair<int, int>> pos;
        pos.insert({0, 0});

        simulate(pos, steps);

        int N = (int)pos.size();
        double R = compute_radius(pos);

        std::cout << "Steps: " << steps << " -> N = " << N << ", R = " << R << "\n";

        // Dopisujemy do pliku skali
        std::ofstream f("eden_skala.csv", std::ios::app);
        f << N << "," << R << "\n";
        f.close();

        // Dla ostatniej iteracji zapisz dane do pliku i narysuj klaster
        if (steps == 10000)
        {
            save_to_file(pos, "eden_data.dat");
            draw_with_gnuplot();
        }
    }

    // Rysujemy wykres skalowania
    plot_scaling();

    return 0;
}
