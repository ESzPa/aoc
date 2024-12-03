#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

bool isSafe(std::vector<int>& levels){
    if(levels.size() < 2) return false;

    bool inc = true, desc = true;

    for(size_t i = 1; i < levels.size(); i++){
        int diff = levels[i] - levels[i-1];
        if(std::abs(diff) < 1 || std::abs(diff) > 3) return false;
        if(levels[i] < levels[i-1]) inc = false;
        else if(levels[i] > levels[i-1]) desc = false;
    }

    return inc || desc;
}

int main(void){
    std::vector<std::vector<int>> reports;
    std::ifstream file("input.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        int num;
        std::vector<int> levels;
        while (ss >> num) {
            levels.push_back(num);
        }
        reports.push_back(levels);
    }

    int count = 0;

    for(int i = 0; i < reports.size(); i++){
        if(isSafe(reports[i])) count++;
        else{
            bool foundSafe = false;
            for(int j = 0; j < reports[i].size() && !foundSafe; j++){
                std::vector<int> levels = reports[i];
                levels.erase(levels.begin()+j);
                if(isSafe(levels)){
                    count++;
                    foundSafe = true;   
                }
            }
        }
    }

    std::cout << count << "\n";
}