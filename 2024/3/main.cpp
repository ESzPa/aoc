#include <iostream>
#include <fstream>
#include <regex>

int main(void){
    std::string input;
    std::ifstream file("input.txt");
    
    std::string tmp;
    while(file >> tmp){
        input.append(tmp);
    }

    std::regex pattern(R"(mul\(\d\d?\d?,\d\d?\d?\))");
    std::sregex_iterator begin(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    std::vector<std::string> muls;
    for(std::sregex_iterator i = begin; i != end; i++){
        muls.push_back(i->str());
    }

    int count = 0;
    for(std::string str : muls){
        std::regex num_pattern(R"(\d\d?\d?)");
        std::sregex_iterator num_begin(str.begin(), str.end(), num_pattern);
        std::sregex_iterator end;
        std::vector<int> nums;
        for(std::sregex_iterator i = num_begin; i != end; i++){
            nums.push_back(std::stoi(i->str()));
        }
        count += nums[0]*nums[1];
    }
    std::cout << count << "\n";
}