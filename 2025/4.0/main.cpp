#include <iostream>

#include "common/cpp/file.hpp"
#include "common/cpp/matrix.hpp"

constexpr int dir_x[] = {-1, -1, -1, 0, 0, 1, 1, 1};
constexpr int dir_y[] = {-1, 0, 1, -1, 1, -1, 0, 1};

inline constexpr size_t calculate_adjacent(const aoc::matrix<uint8_t>& mat, size_t x, size_t y) {
    size_t count = 0;
    for(size_t i = 0; i < 8; ++i) {
        size_t nx = x + dir_x[i];
        size_t ny = y + dir_y[i];
        if(nx >= mat.columns() || ny >= mat.rows())
            continue;
        count += mat[ny, nx];
    }
    return count;
}

constexpr size_t count_accessible(const aoc::matrix<uint8_t>& mat) {
    size_t count = 0;
    for(size_t y = 0; y < mat.rows(); ++y) {
        for(size_t x = 0; x < mat.columns(); ++x) {
            if(mat[y, x] && calculate_adjacent(mat, x, y) < 4) {
                ++count;
            }
        }
    }
    return count;
}

int main(void) {
    aoc::file f("input.txt");
    // Using uint8_t instead of bool because std::vector<bool> sucks
    aoc::matrix<uint8_t> mat(10, 10);

    size_t y = 0;
    while(f.readline()) {
        size_t x = 0;
        for(char c : f.line) {
            mat(y, x) = c == '@';
            ++x;
        }
        ++y;
    }

    std::cout << count_accessible(mat) << '\n';

    return 0;
}
