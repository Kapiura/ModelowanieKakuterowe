#include "matplotlibcpp.h"
#include <algorithm>
#include <array>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>
#include <vector>

namespace plt = matplotlibcpp;

// color
struct Color {
  int red, green, blue;
};

namespace colors {
Color white = {255, 255, 255};
Color black = {0, 0, 0};
} // namespace colors

// height and width
void getTerminalSize(int &width, int &height) {
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
    width = w.ws_col - 1;
    height = w.ws_row - 1;
  } else {
    width = 80;
    height = 80;
  }
}

// plama
struct Plama {
  int width{};
  int height{};
  std::vector<std::vector<bool>> tab;
  std::string frame;

  const std::vector<std::pair<int, int>> neighborOffsets = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0},
      {0, 1},   {1, -1}, {1, 0},  {1, 1}};

  Plama() {
    height = 100;
    width = 100;
    std::cout << "Terminal size: " << width << "x" << height << "\n";
    sleep(2);

    std::cout << "\033[2J";
    std::cout << "\033[?25l";

    tab.resize(height, std::vector<bool>(width, false));
    randomizeTab(tab);
    frame = buildFrame(tab);
  }

  void printStart() { printFrame(frame); }
  void loop() {
    for (;;) {
      frame = buildFrame(tab);
      printFrame(frame);
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
      checkLife(tab);
    }
  }
  void loop(int n) {
    for (int i = 0; i < n; i++) {
      std::string frame = buildFrame(tab);
      printFrame(frame);
      checkLife(tab);
    }
  }

  std::vector<std::pair<int, int>> loopWithSave(int n) {
    std::vector<std::pair<int, int>> tmp;
    int allCells = tab[0].size() * tab.size();
    for (int i = 0; i < n; i++) {
      /* std::string frame = buildFrame(tab); */
      /* printFrame(frame); */
      checkLife(tab);
      std::cout << "i = " << i << "\n";
      int liveCells = 0;
      for (auto &row : tab) {
        liveCells +=
            std::count_if(row.begin(), row.end(), [](bool val) { return val; });
      }
      tmp.push_back(std::make_pair(i, liveCells));
    }
    return tmp;
  }

  void randomizeTab(std::vector<std::vector<bool>> &tab) {
    for (auto &row : tab) {
      for (int i = 0; i < row.size(); i++) {
        row[i] = rand() % 2;
      }
    }
  }

  int coutnNeighbourNum(const std::vector<std::vector<bool>> &tab, int x,
                        int y) {
    int count = 0;
    int numRow = tab.size();
    int numCol = tab[0].size();

    for (const auto &offset : neighborOffsets) {
      int newX = (x + offset.first + numRow) % numRow;
      int newY = (y + offset.second + numCol) % numCol;
      if (tab[newX][newY])
        count++;
    }
    return count;
  }

  void checkLife(std::vector<std::vector<bool>> &tab) {
    std::vector<std::vector<bool>> tmp = tab;
    for (int row = 0; row < tab.size(); row++) {
      for (int col = 0; col < tab[row].size(); col++) {
        int neighbourCount = coutnNeighbourNum(tab, row, col);
        if (neighbourCount >= 6 && neighbourCount <= 9 || neighbourCount == 4)
          tmp[row][col] = true;
        else if (neighbourCount >= 0 && neighbourCount <= 3 ||
                 neighbourCount == 5)
          tmp[row][col] = false;
      }
    }
    tab = tmp;
  }

  std::string buildFrame(const std::vector<std::vector<bool>> &tab) {
    std::ostringstream oss;
    for (const auto &row : tab) {
      for (bool cell : row) {
        Color tmp = cell ? colors::white : colors::black;
        oss << "\x1b[48;2;" << tmp.red << ";" << tmp.green << ";" << tmp.blue
            << "m \x1b[0m";
      }
      oss << "\n";
    }
    return oss.str();
  }

  void printFrame(const std::string &frame) {
    std::cout << "\033[H" << frame << std::flush;
  }
};

int main() {
  Plama plama{};
  std::system("clear");
  int iterations = 2000;

  /* auto data = plama.loopWithSave(iterations); */
  /**/
  /* std::vector<int> x; */
  /* std::vector<double> y; */
  /* for (const auto &point : data) { */
  /*   x.push_back(point.first); */
  /*   y.push_back(point.second / 10000.0); */
  /* } */
  /**/
  /* plt::plot(x, y); */
  /**/
  /* plt::title("Wykres gestosci zywych komorek w czasie"); */
  /* plt::xlabel("Czas [Iteracja]"); */
  /* plt::ylabel("Zywe komorek [%]"); */
  /**/
  /* plt::grid(true); */
  /* plt::save("wykres.png"); */

  std::cout << "\033[?25h";
  plama.loop(100);

  return 0;
}
