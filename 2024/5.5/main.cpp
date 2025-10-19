#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <algorithm>

std::unordered_map<int, std::unordered_set<int>> rules;

bool is_correct(const std::vector<int>& update) {
    for(int i = 0; i < update.size(); ++i) {
        int cur_p = update[i];
        for(int j = i+1; j < update.size(); ++j) {
            int comp_p = update[j];
            auto it = rules.find(comp_p);
            if(it != rules.end() && it->second.count(cur_p)) {
                return false;
            }
        }
    }
    return true;
}

bool is_before(int a, int b) {
    auto it = rules.find(a);
    return it != rules.end() && it->second.count(b);
}

int main(void) {
    std::string tmp;
    std::ifstream file("input.txt");

    while(std::getline(file, tmp)) {
        if(tmp.empty()) break;

        int first  = std::stoi(tmp.substr(0, 2));
        int second = std::stoi(tmp.substr(3, 2)); 
        rules[first].insert(second);
    }
    
    int result = 0;
    while(std::getline(file, tmp)) {
        std::vector<int> update;
        for(int i = 0; i < tmp.size(); i+=3) {
            update.push_back(std::stoi(tmp.substr(i, 2)));
        }

        if(is_correct(update)) continue;

        std::sort(update.begin(), update.end(), [](int a, int b){
            if(is_before(a, b)) return true;
            return false;
        });

        result += update.at((int)update.size()/2);
    }

    std::cout << result << '\n';

    return 0;
}
