#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>

/*
zapis
    std::fstream file("dane.txt");
    file << "dupa";
    file.close();
odczyt
    std::string line;
    while(std::getline(file, line))
    {
        std::cout << line << "\n";
    }
    file.close();
*/

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
        [](unsigned char c) { return std::ispunct(c)|| std::isdigit(c); }),
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

void printFile(std::fstream& file)
{
    std::string line;
    while(std::getline(file, line))
        std::cout << line << "\n";
    file.clear();
    file.seekg(0);
}

void parseMapIntoTxt(const std::map<std::string, int>& zipfMap, const std::string& filename)
{
    std::string filenameResult = filename + "_data";
    std::ofstream file(filenameResult + ".txt");
    for(auto& el: zipfMap)
    {
        file << el.second << " " << el.first << "\n";
    }
    file.close();
}

int main()
{
    std::string filename = "RomeoJulia";

    std::fstream file(filename + ".txt", std::ios::in | std::ios::out);
    removePunctuationMarksFromFile(filename);
    auto zipfMap = zipfLaw(file);
    parseMapIntoTxt(zipfMap, filename);

    file.close();
    return 0;
}