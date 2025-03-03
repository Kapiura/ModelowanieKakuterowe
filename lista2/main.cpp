#include <iostream>
#include <mutex>
#include <thread>
#include <cstdlib> 
#include <vector>
#include <sys/ioctl.h>
#include <unistd.h>

std::mutex mtx;

void getTerminalSize(int& width, int& height)
{
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0)
  {
    width = w.ws_col - 10;
    height = w.ws_row - 10;
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

void printRow(const std::vector<bool>& row)
{
    for(bool&& el: row)
    {
      Color tmp = el ? colors::white : colors::black;
      std::cout<<"\x1b[48;2;" << tmp.red << ";" << tmp.green << ";" << tmp.blue << "m \x1b[0m";
    }
    std::cout << "\n"; 
}

void printTable(std::vector<std::vector<bool>>& tab)
{
  std::vector<std::thread> threads;
  for(const auto& row: tab)
  {
        threads.push_back(std::thread([&](){
            std::lock_guard<std::mutex> lock(mtx);
            printRow(row);
        }));
  }
  for(auto& t: threads)
    t.join();
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

int coutnNeighbourNum(const std::vector<std::vector<bool>>& tab, int x, int y)
{
  return 0;
}

std::vector<std::vector<bool>> checkLife(const std::vector<std::vector<bool>>& tab)
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
  return tmp;
}

int main()
{
  std::system("clear");
  
  // tab size
  int width{};
  int height{};
  getTerminalSize(height, width);

  std::cout << "Terminal size: " << width << "x" << height << "\n";
  
  sleep(2);
  std::vector<std::vector<bool>> tab(10, std::vector<bool>(10, false));
  /* std::vector<std::vector<bool>> tab(width, std::vector<bool>(height, false)); */
  randomizeTab(tab);
  std::system("clear");
  for(;;)
  {
    printTable(tab);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    checkLife(tab);
    std::system("clear");
  }
  return 0;
}
