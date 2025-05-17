#include <iostream>
#include <vector>
#include <ctime>

std::vector<std::pair<int, int>> direction4 =
{
            {0, 1},
    {-1, 0},       {1, 0},
            {0, -1}
};

std::vector<std::pair<int, int>> direction8 =
{
    {-1, 1},  {0, 1},  {1, 1},
    {-1, 0},           {1, 0},
    {-1, -1}, {0, -1}, {1, -1}
};

double dyfuszyn(int n, int nSim,int dym)
{
    const std::vector<std::pair<int, int>>& direkszyns = (dym == 4) ? direction4 : direction8;
    double r2sum = 0.0;

    for (int i = 0; i < nSim; ++i)
    {
        std::pair<int, int> pos = {0, 0};
        for (int j = 0; j < n; ++j)
        {
            std::pair<int, int> dir = direkszyns[std::rand() % direkszyns.size()];
            pos.first += dir.first;
            pos.second += dir.second;
        }
        int r2 = pos.first * pos.first + pos.second * pos.second;
        r2sum += r2;
    }

    double r2mean = r2sum / nSim;
    return r2mean / (4.0 * n);
}

int main()
{
    std::srand(std::time(nullptr));

    int n = 1000;
    int nSim = 1000;

    double d1 = dyfuszyn(n, nSim, 4);
    double d2 = dyfuszyn(n, nSim, 8);

    std::cout << "D 4: " << d1 << std::endl;
    std::cout << "D 8: " << d2 << std::endl;
}