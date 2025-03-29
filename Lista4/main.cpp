#include "matplotlibcpp.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace plt = matplotlibcpp;

struct ZipfLaw
{
    std::vector<std::pair<int, std::string>> zipfVec;
    std::string filename;
    std::vector<int> x;
    std::vector<int> y;

    ZipfLaw(std::string& filename): filename(filename)
    {
        std::string txtFilename = filename + ".txt";
        std::fstream file(txtFilename, std::ios::in | std::ios::out);
        removePunctuationMarksFromFile(txtFilename);
        std::map<std::string, int> zipfMap = zipfLaw(file);
        for(auto& el: zipfMap)
        {
            zipfVec.push_back(std::make_pair(el.second, el.first));
        }
        std::sort(begin(zipfVec), end(zipfVec), [](std::pair<int, std::string>& a, std::pair<int, std::string>& b){ return a.first > b.first; });
        file.close();
    }

    std::map<std::string, int> zipfLaw(std::fstream& file) {
        std::map<std::string, int> result;
        std::string word;
        while (file >> word) {
            result[word]++;
        }
        file.clear();
        file.seekg(0);
        return result;
    }

    void removePunctuationMarksFromLine(std::string& line)
    {
        line.erase(std::remove_if(line.begin(), line.end(),
            [](unsigned char c) { return std::ispunct(c); }),
            line.end());
    }

    void lineToLowercase(std::string& line)
    {
        std::transform(line.begin(), line.end(), line.begin(),
            [](unsigned char c){ return std::tolower(c); });
    }

    void removePunctuationMarksFromFile(std::string& filename)
    {
        std::ofstream tempFile("temp.txt");
        std::fstream file(filename, std::ios::in | std::ios::out);
        std::string line;

        while (std::getline(file, line)) {
            removePunctuationMarksFromLine(line);
            lineToLowercase(line);
            tempFile << line << "\n";
        }

        file.close();
        tempFile.close();

        std::remove(filename.c_str());
        std::rename("temp.txt", filename.c_str());
    }

    void parseVecIntoTxt(const std::string& filename)
    {
        std::string filenameResult = filename + "_data";
        std::ofstream file(filenameResult + ".txt");
        for(auto& el: zipfVec)
        {
            file << el.first << " " << el.second << "\n";
        }
        file.close();
    }

    void printFile(std::fstream& file)
    {
        std::string line;
        while(std::getline(file, line))
            std::cout << line << "\n";
        file.clear();
        file.seekg(0);
    }

    void viewGraph()
    {
        for(int i = 1; i <= zipfVec.size(); i++)
        {
            x.push_back(i);
            y.push_back(zipfVec[i].first);
        }
        double s = calculateZipfExponent();
        plt::clf();
        plt::loglog(x, y);
        plt::named_plot("Prawo Zipfa", x, y, "b-");

        plt::title("Prawo Zipfa - " + filename + " (s = " + std::to_string(s) + ")");
        plt::xlabel("Ranga");
        plt::ylabel("Czestotliwosc");
        plt::legend();
        plt::grid(true);

        plt::save("wykres" + filename + ".png");
    }
    double calculateZipfExponent()
    {
        std::vector<double> logRank{};
        std::vector<double> logFreq{};

        for(int i = 0; i < zipfVec.size(); i++)
        {
            logRank.push_back(std::log(i+1));
            logFreq.push_back(std::log(zipfVec[i].first));
        }
        double logRankSum = std::accumulate(logRank.begin(), logRank.end(), 0.0);
        double logFreqSum = std::accumulate(logFreq.begin(), logFreq.end(), 0.0);

        double avgLogRank = logRankSum / (double)zipfVec.size();
        double avgLogFreq = logFreqSum / (double)zipfVec.size();

        double licznik{};
        double mianownik{};

        for(int i = 0; i < zipfVec.size(); i++)
        {
            double diffLogRank = logRank[i] - avgLogRank;
            double diffLogFreq = logFreq[i] - avgLogFreq;
            licznik += diffLogRank * diffLogFreq;
            mianownik += diffLogRank * diffLogRank;
        }

        double s = - (licznik / mianownik);
        return s;
    }
};

int main()
{
    std::vector<std::string> books = {"RomeoJulia", "PanTadeusz", "Dracula", "bible", "warAndPeace"};
    for(auto& el: books)
    {
        ZipfLaw* zipf = new ZipfLaw(el);
        zipf->parseVecIntoTxt(el);
        zipf->viewGraph();
        delete zipf;
    }
    return 0;
}