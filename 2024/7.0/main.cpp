#include "../../common/cpp/string.hpp"
#include "../../common/cpp/expression.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

bool test_equation(int64_t goal, std::vector<std::string>& tokens) {
    std::vector<std::string> expr;
    expr.reserve(tokens.size()*2-1);

    int all_combs = 1 << (tokens.size()-1);
    for(int mask = 0; mask < all_combs; ++mask) {
        expr.push_back(tokens[0]);

        for(size_t i = 1; i < tokens.size(); ++i) {
            bool mul = (mask >> (i-1)) & 1;
            std::string op = mul ? "*" : "+";
            expr.push_back(tokens[i]);
            expr.push_back(op);
        }

        if(goal == aoc::utils::evaluate_postfix(expr)) {
            return true;
        }

        expr.clear();
    }

    return false;
}

int64_t try_equations(const std::vector<std::string>& eqs) {
    int64_t goalsum = 0;
    for(const auto& eq : eqs) {
        auto splitted = aoc::str::split(eq, ':');
        int64_t goal = std::stoll(splitted[0]);
        auto tokens = aoc::str::split(splitted[1], ' ');
        tokens.erase(tokens.begin());
        if(test_equation(goal, tokens)) goalsum += goal;
    }

    return goalsum;
}

int main(void) {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream file("input.txt");
    while(std::getline(file, line)) lines.push_back(line);

    std::cout << try_equations(lines) << '\n';

    return 0;
}
