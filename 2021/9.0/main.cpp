#include "../../common/cpp/file.hpp"
#include "../../common/cpp/matrix.hpp"
#include <print>

int risk_sum(const aoc::matrix<int>& mat) {
    auto valid = [&](size_t y, size_t x) {
        return x < mat.columns() && y < mat.rows();
    };

    int sum = 0;

    for (size_t y = 0; y < mat.rows(); ++y) {
        for (size_t x = 0; x < mat.columns(); ++x) {
            int current = mat(y, x);
            bool is_lowest = true;

            if (y > 0 && mat(y - 1, x) <= current) is_lowest = false;
            if (y + 1 < mat.rows() && mat(y + 1, x) <= current)
                is_lowest = false;
            if (x > 0 && mat(y, x - 1) <= current) is_lowest = false;
            if (x + 1 < mat.columns() && mat(y, x + 1) <= current)
                is_lowest = false;

            if (is_lowest) { sum += current + 1; }
        }
    }

    return sum;
}

int main(void) {
    aoc::file f("input.txt");
    aoc::matrix<int> mat(10, 10);

    size_t y = 0;
    while (f.readline()) {
        size_t x = 0;
        for (char c : f.line) {
            mat(y, x) = c - '0';
            ++x;
        }
        ++y;
    }

    int sum = risk_sum(mat);
    std::print("{}\n", sum);
}
