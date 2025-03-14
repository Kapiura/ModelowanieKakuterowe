#include <array>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

// colors
struct Color
{
    int red, green, blue;
};

namespace colors
{
Color white = {255, 255, 255};
Color black = {0, 0, 0};
} // namespace colors

// terminal size
void getTerminalSize(int &width, int &height)
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
    {
        width = w.ws_row - 1;
        height = w.ws_col - 1;
    }
    else
    {
        width = 80;
        height = 80;
    }
}

// life
struct LifeGame
{
    int width{};
    int height{};
    std::vector<std::vector<bool>> tab;
    const std::vector<std::pair<int, int>> neighborOffsets = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1},
                                                              {0, 1},   {1, -1}, {1, 0},  {1, 1}};
    LifeGame()
    {
        std::cout << "\033[2J";
        std::cout << "\033[?25l";
        width = 100;
        height = 100;
        tab.resize(width, std::vector<bool>(height, false));
        randomizeTab(tab);
        std::system("clear");
    }

    void loop()
    {
        for (;;)
        {
            std::string frame = buildFrame(tab);
            printFrame(frame);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            checkLife(tab);
        }
    }

    void randomizeTab(std::vector<std::vector<bool>> &tab)
    {
        for (auto &row : tab)
        {
            for (int i = 0; i < row.size(); i++)
            {
                row[i] = rand() % 2;
            }
        }
    }

    int coutnNeighbourNum(const std::vector<std::vector<bool>> &tab, int x, int y)
    {
        int count = 0;
        int numRow = tab.size();
        int numCol = tab[0].size();

        for (const auto &offset : neighborOffsets)
        {
            int newX = (x + offset.first + numRow) % numRow;
            int newY = (y + offset.second + numCol) % numCol;
            if (tab[newX][newY])
                count++;
        }
        return count;
    }

    void checkLife(std::vector<std::vector<bool>> &tab)
    {
        std::vector<std::vector<bool>> tmp = tab;
        for (int row = 0; row < tab.size(); row++)
        {
            for (int col = 0; col < tab[row].size(); col++)
            {
                int neighbourCount = coutnNeighbourNum(tab, row, col);
                if (tab[row][col])
                {
                    tmp[row][col] = !(neighbourCount == 0 || neighbourCount == 1 || neighbourCount >= 4);
                }
                else
                {
                    tmp[row][col] = (neighbourCount == 3);
                }
            }
        }
        tab = tmp;
    }

    std::string buildFrame(const std::vector<std::vector<bool>> &tab)
    {
        std::ostringstream oss;
        for (const auto &row : tab)
        {
            for (bool cell : row)
            {
                Color tmp = cell ? colors::white : colors::black;
                oss << "\x1b[48;2;" << tmp.red << ";" << tmp.green << ";" << tmp.blue << "m   \x1b[0m";
            }
            oss << "\n";
        }
        return oss.str();
    }

    void printFrame(const std::string &frame)
    {
        std::cout << "\033[H" << frame << std::flush;
    }
};

int main()
{
    LifeGame life;
    life.loop();
    std::cout << "\033[?25h";
    return 0;
}
