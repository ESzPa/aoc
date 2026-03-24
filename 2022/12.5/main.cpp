#include "common/cpp/file.hpp"
#include "common/cpp/matrix.hpp"
#include <print>
#include <queue>

size_t steps_to_lowest(const aoc::matrix<int>& mat, int sx, int sy) {
    auto index = [&](int x, int y) { return y * mat.columns() + x; };
    std::vector<bool> visited(mat.columns() * mat.rows(), false);
    std::vector<size_t> dist(mat.columns() * mat.rows(), 0);
    std::queue<std::pair<int, int>> q;

    static constexpr int dir_x[] = {0, 0, -1, +1};
    static constexpr int dir_y[] = {-1, +1, 0, 0};

    q.push({sx, sy});
    visited[index(sx, sy)] = true;
    dist[index(sx, sy)] = 0;

    int lastx, lasty;
    while (!q.empty()) {
        auto [x, y] = q.front();
        lastx = x;
        lasty = y;
        q.pop();

        if (mat(y, x) == 1) break;

        for (size_t i = 0; i < 4; ++i) {
            int nx = x + dir_x[i];
            int ny = y + dir_y[i];
            if (nx < 0 || ny < 0 || nx >= mat.columns() ||
                ny >= mat.rows()) {
                continue;
            } else if (mat(y, x) > mat(ny, nx) + 1) {
                continue;
            } else if (visited[index(nx, ny)]) {
                continue;
            }

            q.push({nx, ny});
            visited[index(nx, ny)] = true;
            dist[index(nx, ny)] = dist[index(x, y)] + 1;
        }
    }

    return dist[index(lastx, lasty)];
}

int main(void) {
    aoc::file f("input.txt");
    aoc::matrix<int> mat(10, 10);

    std::pair<int, int> start;

    size_t y = 0;
    while (f.readline()) {
        size_t x = 0;
        for (char c : f.line) {
            mat(y, x) = c - 96;
            if (mat(y, x) == -13) {
                mat(y, x) = 1;
            } else if (mat(y, x) == -27) {
                start = {x, y};
                mat(y, x) = 26;
            }
            ++x;
        }
        ++y;
    }

    size_t steps = steps_to_lowest(mat, start.first, start.second);
    std::print("{}\n", steps);

    return 0;
}
