#include <cstdio>
#include <cstdlib>

int main() {
    FILE* gnuplotPipe = popen("gnuplot -persistent", "w");

    fprintf(gnuplotPipe, "set datafile separator ','\n");
    fprintf(gnuplotPipe, "set terminal pngcairo size 800,600 enhanced font ',10'\n");
    fprintf(gnuplotPipe, "set output 'log_wykres.png'\n");
    fprintf(gnuplotPipe, "set title 'log(R) vs log(N) dla DLA'\n");
    fprintf(gnuplotPipe, "set xlabel 'log(N)'\n");
    fprintf(gnuplotPipe, "set ylabel 'log(R)'\n");
    fprintf(gnuplotPipe, "set grid\n");

    fprintf(gnuplotPipe, "f(x) = a*x + b\n");
    fprintf(gnuplotPipe, "fit f(x) 'skala.csv' using (log($1)):(log($2)) every ::1 via a, b\n");

    fprintf(gnuplotPipe, "plot 'skala.csv' using (log($1)):(log($2)) every ::1 with points pt 7 lc rgb 'blue' title 'Dane', \\\n");

    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return 0;
}
