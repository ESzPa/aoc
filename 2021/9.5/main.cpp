#include "../../common/cpp/file.hpp"
#include "../../common/cpp/matrix.hpp"
#include <algorithm>
#include <print>
#include <queue>

bool is_lowest(const aoc::matrix<int>& mat, int x, int y) {
    int current = mat(y, x);

    if (y > 0 && mat(y - 1, x) <= current) { return false; }
    if (y + 1 < mat.rows() && mat(y + 1, x) <= current) {
        return false;
    }
    if (x > 0 && mat(y, x - 1) <= current) { return false; }
    if (x + 1 < mat.columns() && mat(y, x + 1) <= current) {
        return false;
    }

    return true;
}

int get_basin_size(const aoc::matrix<int>& mat, int sx, int sy,
                   std::vector<bool>& visited) {
    int size = 0;
    std::queue<std::pair<int, int>> q;

    q.push({sx, sy});
    visited[sy * mat.columns() + sx] = true;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        ++size;

        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        for (size_t i = 0; i < 4; ++i) {
            size_t nx = x + dx[i];
            size_t ny = y + dy[i];
            if (ny >= mat.rows() || nx >= mat.columns()) continue;

            if (mat(ny, nx) < 9 &&
                !visited[ny * mat.columns() + nx]) {
                q.push({nx, ny});
                visited[ny * mat.columns() + nx] = true;
            }
        }
    }

    return size;
}

std::vector<int> basin_sizes(const aoc::matrix<int>& mat) {
    std::vector<int> sizes;
    std::vector<bool> visited(mat.size(), false);

    for (size_t y = 0; y < mat.rows(); ++y) {
        for (size_t x = 0; x < mat.columns(); ++x) {
            size_t idx = y * mat.columns() + x;
            if (!visited[idx] && is_lowest(mat, x, y)) {
                sizes.push_back(get_basin_size(mat, x, y, visited));
            }
        }
    }

    std::sort(sizes.rbegin(), sizes.rend());
    return sizes;
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

    std::vector<int> sizes = basin_sizes(mat);
    std::print("{}\n", (sizes[0] * sizes[1] * sizes[2]));

    return 0;
}
