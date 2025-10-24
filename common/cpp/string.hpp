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

    inline std::string join(const std::vector<std::string>& parts, const std::string& delim) {
        std::ostringstream oss;
        for(size_t i = 0; i < parts.size(); ++i) {
            if(i > 0) oss << delim;
            oss << parts[i];
        } 
        return oss.str();
    }
}
