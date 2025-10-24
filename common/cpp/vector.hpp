#pragma once
#include <vector>

namespace aoc::vec {
    template <typename T>
    inline std::vector<T> intersperse(const std::vector<T>& vec, const T& separator) {
        if (vec.empty()) return {};
        std::vector<T> result;
        result.reserve(vec.size() * 2 - 1);

        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) result.push_back(separator);
            result.push_back(vec[i]);
        }
        return result;
    }
}
