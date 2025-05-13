#include <algorithm>
#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <ctime>
#include <cstdio>

template <typename T>
class AbstraktDymenszyn
{
    protected:
        T position;
        bool isSukcesful;
        const int steps = 1000;
    public:
        AbstraktDymenszyn(T start = {}): position(start), isSukcesful(false) {}
        virtual const bool getSukcesful() { return isSukcesful; }
        virtual void wolk() = 0;
};

class OneDimenszynWalk: public AbstraktDymenszyn<int>
{
    private:
        int direction[2] = {1, -1};
    public:
        OneDimenszynWalk(): AbstraktDymenszyn(0) {}

        void wolk() override
        {
            int startPosition = position;
            for(int i = 0; i < steps; i++)
            {
                int dir = direction[std::rand() % 2];
                position += dir;
                if(position == startPosition)
                    isSukcesful = true;
            }
        }
};

class TuDimenszynWalk: public AbstraktDymenszyn<std::pair<int, int>>
{
    private:
        std::vector<std::pair<int, int>> direction =
            {
                {-1, 1},  {0, 1},  {1, 1},
                {-1, 0},           {1, 0},
                {-1, -1}, {0, -1}, {1, -1}
            };
    public:
            TuDimenszynWalk(): AbstraktDymenszyn(std::make_pair(0,0)) {}

            void wolk() override
            {
                std::pair<int,int> startPosition = position;
                std::pair<int, int> dir;
                for(int i = 0; i < steps; i++)
                {
                    dir = direction[std::rand() % 8];
                    position.first += dir.first;
                    position.second += dir.second;
                    if(position == startPosition)
                        isSukcesful = true;
                }
            }
};

class TriDimenszynWalk: public AbstraktDymenszyn<std::tuple<int, int, int>>
{
    private:
        std::vector<std::tuple<int, int, int>> direction =
        {
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
    public:
        TriDimenszynWalk(): AbstraktDymenszyn(std::make_tuple(0,0,0)) {}

        void wolk() override
        {
            std::tuple<int,int, int> startPosition = position;
            std::tuple<int, int, int> dir;
            for(int i = 0; i < steps; i++)
            {
                dir = direction[std::rand() % direction.size()];
                position = {
                    std::get<0>(position) + std::get<0>(dir),
                    std::get<1>(position) + std::get<1>(dir),
                    std::get<2>(position) + std::get<2>(dir)
                };
                if(position == startPosition)
                    isSukcesful = true;
            }
        }
};

template <typename T>
void wolking(std::vector<T> dymenszyn, int& cum)
{
    cum = 0;
    for(auto& wolk: dymenszyn)
        wolk.wolk();
    cum = std::count_if(dymenszyn.begin(), dymenszyn.end(), [](T w){ return w.getSukcesful();});
    std::cout << "Cum: " << cum << "\n";
}

int main()
{
    std::srand(std::time(nullptr));

    int wolksNum = 1000;
    int cumulation;

    std::vector<OneDimenszynWalk> oneDymenszynWalki(wolksNum);
    std::vector<TuDimenszynWalk> tuDymenszynWalki(wolksNum);
    std::vector<TriDimenszynWalk> triDymenszynWalki(wolksNum);
    wolking(oneDymenszynWalki, cumulation);
    wolking(tuDymenszynWalki, cumulation);
    wolking(triDymenszynWalki, cumulation);

    return 0;
}