#include <iostream>
#include <ctime>
#include <cstdio>
#include <array>
#include <vector>

std::vector<std::tuple<int, int, int>> directions_3d = {
    {-1,-1,-1}, {-1,-1,0}, {-1,-1,1},
    {-1,0,-1},  {-1,0,0},  {-1,0,1},
    {-1,1,-1},  {-1,1,0},  {-1,1,1},
    {0,-1,-1},  {0,-1,0},  {0,-1,1},
    {0,0,-1},             {0,0,1},
    {0,1,-1},   {0,1,0},  {0,1,1},
    {1,-1,-1},  {1,-1,0},  {1,-1,1},
    {1,0,-1},   {1,0,0},   {1,0,1},
    {1,1,-1},   {1,1,0},   {1,1,1}
};

int main()
{
    std::srand(std::time(nullptr));
    int steps = 1000000;

    std::array<std::string, 9> colors =
        {"black", "red", "green", "blue", "cyan", "magenta", "yellow", "gray", "white"};

    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set terminal pngcairo size 29.7cm,21cm enhanced font ',10'\n");
    fprintf(gnuplotPipe, "set output 'random_walk_3d_1000000.png'\n");
    fprintf(gnuplotPipe, "set title 'Trójwymiarowy spacer losowy'\n");
    fprintf(gnuplotPipe, "set view 60, 30, 1, 1\n");
    fprintf(gnuplotPipe, "set xyplane at 0\n");
    fprintf(gnuplotPipe, "splot ");

    for (size_t i = 0; i < colors.size(); ++i) {
        if (i != 0) fprintf(gnuplotPipe, ", ");
        fprintf(gnuplotPipe, "'-' with lines lc rgb '%s' title 'Path %zu'", colors[i].c_str(), i+1);
    }
    fprintf(gnuplotPipe, "\n");

    for (const auto& color : colors) {
        std::tuple<int, int, int> position = {0, 0, 0};

        for (int i = 0; i < steps; i++) {
            fprintf(gnuplotPipe, "%d %d %d\n",
                   std::get<0>(position),
                   std::get<1>(position),
                   std::get<2>(position));

            auto dir = directions_3d[std::rand() % directions_3d.size()];
            position = {
                std::get<0>(position) + std::get<0>(dir),
                std::get<1>(position) + std::get<1>(dir),
                std::get<2>(position) + std::get<2>(dir)
            };
        }
        fprintf(gnuplotPipe, "e\n");
    }

    fprintf(gnuplotPipe, "set output\n");
    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
    return 0;
}