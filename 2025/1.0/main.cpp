#include "../../common/cpp/file.hpp"
#include <iostream>

int main(void) {
    aoc::file f("input.txt");
    auto lines = f.read_lines();

    int point = 50;

    auto turn_L = [&point](int count) {
        point = (point - count) % 100;
        if (point < 0) point += 100;
    };

    auto turn_R = [&point](int count) {
        point = (point + count) % 100;
    };

    int counter = 0;
    for (const auto& line : lines) {
        if (line[0] == 'L') {
            turn_L(std::stoi(line.substr(1, line.size() - 1)));
        } else {
            turn_R(std::stoi(line.substr(1, line.size() - 1)));
        }
        if (!point) ++counter;
    }

    std::cout << counter << '\n';

    return 0;
}
