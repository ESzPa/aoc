#include "../../common/cpp/file.hpp"
#include <iostream>

int main(void) {
    aoc::file f("input.txt");
    auto lines = f.read_lines();

    int point = 50;
    int counter = 0;

    for (const auto& line : lines) {
        char dir = line[0];
        int count = std::stoi(line.substr(1));

        for (int i = 0; i < count; i++) {
            if (dir == 'L') {
                point = (point - 1 + 100) % 100;
            } else {
                point = (point + 1) % 100;
            }

            if (point == 0) { ++counter; }
        }
    }

    std::cout << counter << '\n';

    return 0;
}
