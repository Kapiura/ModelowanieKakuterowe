#include <iostream>
#include <ctime>
#include <cstdio>


int main()
{
    int directions[2] = {-1, 1};

    std::srand(std::time(nullptr));

    int steps = 1000000;
    int kids = 10000;

    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
    fprintf(gnuplotPipe, "set terminal pngcairo size 800,600 enhanced font 'Verdana,10'\n");
    fprintf(gnuplotPipe, "set output 'histogram_dzieci.png'\n");
    fprintf(gnuplotPipe, "set title 'Rozkład końcowych pozycji (n=%d kroków, N=%d spacerów)'\n", steps, kids);
    fprintf(gnuplotPipe, "set xlabel 'Końcowa pozycja'\n");
    fprintf(gnuplotPipe, "set ylabel 'Liczba spacerów'\n");
    fprintf(gnuplotPipe, "set style data histogram\n");
    fprintf(gnuplotPipe, "set style fill solid\n");
    fprintf(gnuplotPipe, "set boxwidth 0.9\n");
    fprintf(gnuplotPipe, "binwidth = 20\n");
    fprintf(gnuplotPipe, "bin(x) = binwidth*floor(x/binwidth)\n");
    fprintf(gnuplotPipe, "plot '-' using (bin($1)):(1.0) smooth freq with boxes lc rgb 'blue' title 'Rozkład pozycji'\n");

    for(int i = 0; i < kids; i++)
    {
        int pos = 0;
        for(int j = 0; j < steps; j++)
        {
            int dir = directions[std::rand() % 2];
            pos += dir;
        }
        fprintf(gnuplotPipe, "%d\n", pos);
    }
    fprintf(gnuplotPipe, "e\n");
    fprintf(gnuplotPipe, "set output\n");
    fflush(gnuplotPipe);
    pclose(gnuplotPipe);
    return 0;
}