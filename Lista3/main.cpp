#include "matplotlibcpp.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
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
  std::vector<std::vector<bool>> tab;
  const std::vector<std::pair<int, int>> neighborOffsets = {
      {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

  LifeGame(double p0) {
    std::cout << "\033[2J";
    std::cout << "\033[?25l";
    tab.resize(width, std::vector<bool>(height, false));
    randomizeTab(tab, p0);
    std::system("clear");
  }

  std::vector<std::pair<int, int>> loop(int n = 10) {
    std::vector<std::pair<int, int>> tmp;
    int allCells = tab[0].size() * tab.size();
    for (int i = 0; i < n; i++) {
      // Konwersja tablicy booli na wartości float
      std::vector<std::vector<float>> data(width,
                                           std::vector<float>(height, 0.0));
      for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
          data[i][j] = tab[i][j] ? 1.0f : 0.0f;
        }
      }

      std::vector<float> flat_data;
      for (const auto &row : data) {
        flat_data.insert(flat_data.end(), row.begin(), row.end());
      }

      plt::clf(); // Czyści poprzedni wykres
      plt::imshow(flat_data.data(), width, height, 1);
      plt::pause(0.1); // Opóźnienie dla płynności animacji

      checkLife(tab); // Aktualizacja stanu gry
      int liveCells = 0;
      for (auto &row : tab) {
        liveCells +=
            std::count_if(row.begin(), row.end(), [](bool val) { return val; });
      }
      tmp.push_back(std::make_pair(i, liveCells));
    }
    return tmp;
  }
  /* void loop(int n = 10) { */
  /*   for (int i = 0; i < n; i++) { */
  /*     std::string frame = buildFrame(tab); */
  /*     printFrame(frame); */
  /*     std::this_thread::sleep_for(std::chrono::milliseconds(100)); */
  /*     checkLife(tab); */
  /*   } */
  /* } */

  void randomizeTab(std::vector<std::vector<bool>> &tab, double p0) {
    for (auto &row : tab) {
      for (int i = 0; i < row.size(); i++) {
        row[i] = ((double)rand() / RAND_MAX) < p0 ? 1 : 0;
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
        if (tab[row][col]) {
          tmp[row][col] = !(neighbourCount == 0 || neighbourCount == 1 ||
                            neighbourCount >= 4);
        } else {
          tmp[row][col] = (neighbourCount == 3);
        }
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
            << "m   \x1b[0m";
      }
      oss << "\n";
    }
    return oss.str();
  }

  void printFrame(const std::string &frame) {
    std::cout << "\033[H" << frame << std::flush;
  }

  void printGrahp(std::vector<std::pair<int, int>> &data) {
    std::vector<int> x;
    std::vector<int> y;

    for (const auto &point : data) {
      x.push_back(point.first);
      y.push_back(point.second);
    }

    plt::plot(x, y);
    plt::title("Wykres gestosci zywych komorek w czasie");
    plt::xlabel("Czas [Iteracja]");
    plt::ylabel("Liczba zywych komorek");
    plt::grid(true);

    plt::save("wykres.png"); // Zapis wykresu do pliku
    plt::show();             // Wyświetlenie wykresu
  }
};

int main() {
  LifeGame life(0.2);
  life.loop();
  std::cout << "\033[?25h";
  return 0;
}
