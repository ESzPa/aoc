#include "../../common/cpp/file.hpp"
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>

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
            if (cur_id.length() % 2) continue;

            int idlen_half = cur_id.length() / 2;
            if (!strcmp(cur_id.substr(0, idlen_half).c_str(),
                        cur_id.substr(idlen_half).c_str())) {
                invalid_sum += id_start;
            }
        }
    }

    std::cout << invalid_sum << '\n';

    return 0;
}
