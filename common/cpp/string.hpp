#pragma once
#include <vector>
#include <string>
#include <sstream>

namespace aoc::str {
    inline std::vector<std::string> split(const std::string& str, char delim) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string item;
        while(getline(ss, item, delim)) {
            tokens.push_back(item);
        }
        return tokens;
    }
}
