#include <iostream>
#include <ctime>
#include <cstdio>
#include <array>
#include <vector>

std::vector<std::pair<int, int>> directions =
{
    {-1, 1},  {0, 1},  {1, 1},
    {-1, 0},           {1, 0},
    {-1, -1}, {0, -1}, {1, -1}
};

int main()
{
    std::srand(std::time(nullptr));

    int steps = 1000000;

    std::array<std::string, 9> colors =
        {"black", "red", "green", "blue", "cyan", "magenta", "yellow", "gray", "white"};

    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set terminal pngcairo size 29.7cm,21cm enhanced font ',10'\n");
    fprintf(gnuplotPipe, "set output 'random_walk.png'\n");
    fprintf(gnuplotPipe, "set title 'Spacer losowy'\n");
    fprintf(gnuplotPipe, "set size ratio -1\n");
    fprintf(gnuplotPipe, "plot ");

    for (int i = 0; i < colors.size(); ++i)
    {
        if (i != 0)
            fprintf(gnuplotPipe, ", ");
        fprintf(gnuplotPipe, "'-' with lines lc rgb '%s'", colors[i].c_str());
    }
    fprintf(gnuplotPipe, "\n");

    for(auto& color : colors)
    {
        std::pair<int, int> atPosition = {0,0};

        for(int i = 0; i < steps; i++)
            {
                fprintf(gnuplotPipe, "%d %d\n", atPosition.first, atPosition.second);

                std::pair<int, int> dir = directions.at(std::rand() % 8);
                atPosition = {atPosition.first + dir.first, atPosition.second + dir.second};
            }
        fprintf(gnuplotPipe, "e\n");
    }

    fprintf(gnuplotPipe, "set output\n");
    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
    return 0;
}