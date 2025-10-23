#include "../../common/cpp/matrix.hpp"
#include <iostream>
#include <fstream>

void mark_guard_route(aoc::matrix<char>& mat) {
    int gx = 0, gy = 0;
    int heading = 0;
    bool found = false;
    for (int y = 0; y < mat.rows(); ++y) {
        for (int x = 0; x < mat.columns(); ++x) {
            if (mat.at(y, x) == '^') {
                gy = y;
                gx = x;
                found = true;
                break;
            }
        }
        if (found) break;
    }

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    
    bool exited = false;
    while(!exited) {
        int nx = gx + dx[heading];
        int ny = gy + dy[heading];

        if (nx < 0 || ny < 0 || nx >= mat.columns() || ny >= mat.rows()) {
            exited = true;
        }
        else {
            while(mat.at(ny, nx) == '#') {
                heading = (heading+1) % 4;
                nx = gx + dx[heading];
                ny = gy + dy[heading];
            }
        }
        mat.at(gy, gx) = 'x';
        gx = nx;
        gy = ny;
    }
}

int main(void) {
    aoc::matrix<char> mat(1, 1);

    std::string line;
    std::ifstream file("input.txt");
    int y = 0;
    while(file >> line) {
        int x = 0;
        for(char c : line) {
            mat(y, x++) = c;
        }
        ++y;
    }

    mark_guard_route(mat);

    size_t count = mat.count([](char c){
        return c == 'x';
    });
    std::cout << count << '\n';

    return 0;
}
