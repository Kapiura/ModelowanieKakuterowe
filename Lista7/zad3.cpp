#include <iostream>
#include <vector>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <string>

const int size = 800;
const int center = size / 2;
const int iterations = 50;

std::vector<std::vector<int>> stan(size, std::vector<int>(size, 0));
std::vector<std::pair<int, int>> directions = {
    {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}
};

enum RuleType { RULE_1, RULE_2, RULE_3, RULE_ALWAYS };

bool rule(int count, RuleType ruleType)
{
    switch(ruleType)
    {
        case RULE_1:
            return count == 1;
        case RULE_2:
            return count == 2;
        case RULE_3:
            return count >= 1;
    }
    return false;
}

void init()
{
    for(auto& row : stan)
        for(auto& el : row)
            el = 0;
}

void simulate(RuleType ruleType)
{
    stan[center][center] = 1;
    stan[center - 1][center] = 1;
    stan[center][center + 1] = 1;

    for(int iter = 0; iter < iterations; iter++)
    {
        auto newStan = stan;
        for(int k = 1; k < size - 1; k++)
        {
            for(int j = 1; j < size - 1; j++)
            {
                if(stan[k][j] == 1) continue;

                int count = 0;
                for(auto& d : directions)
                {
                    if(stan[k + d.first][j + d.second] == 1)
                        count++;
                }

                if(rule(count, ruleType))
                {
                    newStan[k][j] = 1;
                }
            }
        }
        stan = newStan;
    }
}

void save_to_file(const std::string& filename)
{
    FILE* f = std::fopen(filename.c_str(), "w");
    if (!f)
    {
        std::cerr << "Nie mozna otworzyc pliku do zapisu\n";
        return;
    }
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
            if(stan[i][j] == 1)
                fprintf(f, "%d %d\n", j, i);
    std::fclose(f);
}

void plot_with_gnuplot(const std::string& datafile, const std::string& output_png, const std::string& title)
{
    FILE* gp = popen("gnuplot", "w");
    if(!gp)
    {
        std::cerr << "Nie udalo sie uruchomic gnuplota\n";
        return;
    }

    fprintf(gp,
        "set terminal png size 800,800\n"
        "set output '%s'\n"
        "set title '%s'\n"
        "set size ratio -1\n"
        "unset key\n"
        "unset xtics\n"
        "unset ytics\n"
        "plot '%s' with points pt 7 ps 0.5 lc rgb 'blue'\n",
        output_png.c_str(), title.c_str(), datafile.c_str());

    fflush(gp);
    pclose(gp);
}

int main()
{
    std::vector<std::pair<RuleType, std::string>> rules = {
        {RULE_1, "platek_1_sasiad"},
        {RULE_2, "platek_2_sasiadow"},
        {RULE_3, "platek_min1_sasiad"}
    };

    for (const auto& [ruleType, name] : rules) {
        init();
        simulate(ruleType);

        std::string filename = name + ".dat";
        std::string output = name + ".png";
        save_to_file(filename);
        plot_with_gnuplot(filename, output, name);
        std::cout << "Zapisano: " << output << std::endl;
    }

    return 0;
}