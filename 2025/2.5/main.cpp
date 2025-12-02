#include "../../common/cpp/file.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>

bool is_repeated(const std::string& str) {
    return (str + str).substr(1, 2 * str.size() - 2).find(str) !=
           std::string::npos;
}

int main(void) {
    aoc::file f("input.txt");
    auto line = f.read_lines()[0];

    uint64_t invalid_sum = 0;

    std::stringstream ss(line);
    std::string id;

    while (std::getline(ss, id, ',')) {
        size_t pos = id.find('-');

        uint64_t id_start;
        id_start = stoul(id.substr(0, pos));

        uint64_t id_end;
        id_end = stoul(id.substr(pos + 1));

        for (; id_start <= id_end; ++id_start) {
            std::string cur_id = std::to_string(id_start);

            if (is_repeated(cur_id)) invalid_sum += id_start;
        }
    }

    std::cout << invalid_sum << '\n';

    return 0;
}
