#include <iostream>
#include <array>
#include <sstream>
#include <thread>
#include <cstdlib> 
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>


void getTerminalSize(int& width, int& height)
{
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
  {
    width = w.ws_col - 1;
    height = w.ws_row - 1;
  }
  else
  {
    width = 80;
    height = 80;
  }
}

struct Color 
{
  int red, green, blue;
};

namespace colors
{
  Color white = {255, 255, 255};
  Color black = {0,0,0};
}

void randomizeTab(std::vector<std::vector<bool>>& tab)
{
  for(auto& row: tab)
  {
    for(int i = 0; i < row.size(); i++)
    {
      row[i] = rand() % 2;
    }
  }
}

const std::vector<std::pair<int, int>> neighborOffsets = 
{
    {-1, -1}, {-1, 0}, {-1, 1},
    {0, -1},           {0, 1},
    {1, -1},  {1, 0},  {1, 1}
};

int coutnNeighbourNum(const std::vector<std::vector<bool>>& tab, int x, int y)
{
  int count{};
  int numRow = tab.size();
  int numCol = tab[0].size();

  for(const auto& off: neighborOffsets)
  {
    int newX = x + off.first;
    int newY = y + off.second;
    if(newX >= 0 && newY >= 0 && newY && newX < numRow && newY < numCol)
    {
      if(tab[newX][newY])
        count++;
    }
  }
  return count;
}

void checkLife(std::vector<std::vector<bool>>& tab)
{
  std::vector<std::vector<bool>> tmp = tab;
  for(int row = 0; row < tab.size(); row++)
  {
      for(int col = 0; col < tab[row].size(); col++)
      {
        int neighbourCount = coutnNeighbourNum(tab, row, col);
        if(tab[row][col] == true && (neighbourCount == 3 || neighbourCount == 2))
            tmp[row][col] = true;
        else if(tab[row][col] == false && neighbourCount == 3)
            tmp[row][col] = true;
        else
            tmp[row][col] = false;
      }
  }
  tab = tmp;
}


std::string buildFrame(const std::vector<std::vector<bool>>& tab)
{
  std::ostringstream oss;
  for(const auto& row: tab)
  {
    for(bool cell: row)
    {
      Color tmp = cell ? colors::white : colors::black;
      oss <<"\x1b[48;2;" << tmp.red << ";" << tmp.green << ";" << tmp.blue << "m \x1b[0m";
    }
    oss << "\n";
  }
  return oss.str();
}

void printFrame(const std::string& frame)
{
  std::cout << "\033[H" << frame << std::flush; 
}

int main()
{
  std::cout << "\033[2J";
  std::cout << "\033[?25l";
  // tab size
  int width{};
  int height{};
  getTerminalSize(height, width);

  std::cout << "Terminal size: " << width << "x" << height << "\n";
  
  sleep(2);
  std::vector<std::vector<bool>> tab(width, std::vector<bool>(height, false));
  randomizeTab(tab);
  std::system("clear");
  for(;;)
  {
    std::string frame = buildFrame(tab);
    printFrame(frame);
    /* std::this_thread::sleep_for(std::chrono::milliseconds(100)); */
    checkLife(tab);
  }
  std::cout << "\033[?25h";
  return 0;
}
