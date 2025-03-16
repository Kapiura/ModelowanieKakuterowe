#include "matplotlibcpp.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace plt = matplotlibcpp;

// colors
struct Color {
  int red, green, blue;
};

namespace colors {
Color white = {255, 255, 255};
Color black = {0, 0, 0};
} // namespace colors

// terminal size
void getTerminalSize(int &width, int &height) {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
    width = w.ws_row - 1;
    height = w.ws_col - 1;
  } else {
    width = 80;
    height = 80;
  }
}

// life
struct LifeGame {
  int width{100};
  int height{100};
  double p0;
  std::vector<std::vector<bool>> tab;
  const std::vector<std::pair<int, int>> neighborOffsets = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

  LifeGame(double p0): p0(p0) {
    std::cout << "\033[2J";
    std::cout << "\033[?25l";
    tab.resize(width, std::vector<bool>(height, false));
    randomizeTab(tab, p0);
    std::system("clear");
  }

  std::vector<std::pair<int, double>> loop(int n = 10) 
  {
    plt::clf(); 
    std::vector<std::pair<int, double>> tmp;
    int allCells = tab[0].size() * tab.size();
    for (int i = 0; i < n; i++) 
    {
      int liveCells = 0;
      for (auto &row : tab) 
        liveCells += std::count_if(row.begin(), row.end(), [](bool val) { return val; });
      tmp.push_back(std::make_pair(i, liveCells));  

      std::vector<std::vector<float>> data(width, std::vector<float>(height, 0.0));
      for (int i = 0; i < width; i++) 
        for (int j = 0; j < height; j++) 
          data[i][j] = tab[i][j] ? 1.0f : 0.0f;

      std::vector<float> flat_data;
      for (const auto &row : data)
        flat_data.insert(flat_data.end(), row.begin(), row.end());

      plt::clf(); 
      plt::imshow(flat_data.data(), width, height, 1);
        plt::title("Gra w zycie - p0 = " + std::to_string(p0) + ", iteracja = " + std::to_string(i));
      plt::pause(0.1); 

      checkLife(tab);
    }
    return tmp;
  }

  void randomizeTab(std::vector<std::vector<bool>> &tab, double p0) 
  {
    for (auto &row : tab)
      for (int i = 0; i < row.size(); i++)
        row[i] = ((double)rand() / RAND_MAX) < p0 ? 1 : 0;
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

  void checkLife(std::vector<std::vector<bool>> &tab) {
    std::vector<std::vector<bool>> tmp = tab;
    for (int row = 0; row < tab.size(); row++) 
    {
      for (int col = 0; col < tab[row].size(); col++) 
      {
        int neighbourCount = coutnNeighbourNum(tab, row, col);
        if (tab[row][col]) 
          tmp[row][col] = !(neighbourCount == 0 || neighbourCount == 1 || neighbourCount >= 4);
        else 
          tmp[row][col] = (neighbourCount == 3);
      }
    }
    tab = tmp;
  }

  static void printGraph(std::vector<std::pair<int, double>> &data, std::string name) {
    std::vector<int> x;
    std::vector<double> y;
    int allCells = 100 * 100;
    
    for (const auto &point : data) 
    {
        x.push_back(point.first);
        y.push_back(point.second / static_cast<double>(allCells));
    }
    
    plt::clf(); 
    plt::named_plot("Gęstość w czasie", x, y, "b-");
    
    plt::title("Wykres gęstości żywych komórek w czasie");
    plt::xlabel("Czas [Iteracja]");
    plt::ylabel("Gęstość żywych komórek");
    plt::legend();
    plt::grid(true);
    
    name += ".png";
    plt::save(name);
}

};

int main() {
  std::vector<double> p_values = {0.05, 0.1, 0.3, 0.6, 0.75, 0.8, 0.95};
  std::vector<int> iterations = {50, 200, 200, 200, 100, 50, 50};
  plt::show();

  for (int i = 0; i < iterations.size(); i++) 
  {
    LifeGame* life = new LifeGame(p_values[i]);
    auto data = life->loop(iterations[i]);
    LifeGame::printGraph(data, "wykres_p" + std::to_string(p_values[i]));
    delete life;
  }
  return 0;
}
