#include "../../common/cpp/matrix.hpp"
#include <iostream>
#include <fstream>
#include <set>
#include <tuple>
#include <thread>
#include <chrono>
#include <atomic>

size_t MAX_THREADS = 0;

// If the guard is stuck in a loop return true, otherwise return false
bool test_guard_route(const aoc::matrix<char>& mat) {
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

    std::set<std::tuple<int, int, int>> visited;

    int dx[4] = {0, 1, 0, -1};
    int dy[4] = {-1, 0, 1, 0};
    
    bool exited = false;
    bool stuck = false;
    while(!exited && !stuck) {
        auto state = std::make_tuple(gx, gy, heading);
        if(visited.count(state)) {
            stuck = true;
        }
        else {
            visited.insert(state);
        }

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
        gx = nx;
        gy = ny;
    }
    
    return stuck;
}

size_t bruteforce_test_obstructions_multi(const aoc::matrix<char>& mat) {
    std::atomic<size_t> count = 0;
    std::vector<std::thread> workers;
    workers.reserve(MAX_THREADS);

    auto tfunc = [&count](const aoc::matrix<char>&& lmat){
        bool ret = test_guard_route(lmat);
        count += ret;
    };

    for(int y = 0; y < mat.rows(); ++y) {
        for(int x = 0; x < mat.columns(); ++x) {
            if(mat.at(y, x) != '.' && mat.at(y, x) != '^') continue;
            
            while(workers.size() >= MAX_THREADS) {
                for(auto it = workers.begin(); it != workers.end(); ) {
                    if(it->joinable()) {
                        it->join();
                        it = workers.erase(it);
                    }
                    else {
                        ++it;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            aoc::matrix<char> localmat = mat;
            localmat.at(y, x) = '#';
            workers.emplace_back(tfunc, std::move(localmat));
        }
    }

    for(auto& thread : workers) {
        if(thread.joinable()) {
            thread.join();
        }
    }

    return count;
}

size_t bruteforce_test_obstructions_seq(const aoc::matrix<char>& mat) {
    size_t count = 0;
    for(int y = 0; y < mat.rows(); ++y) {
        for(int x = 0; x < mat.columns(); ++x) {
            if(mat.at(y, x) != '.' && mat.at(y, x) != '^') continue;
            
            aoc::matrix<char> localmat = mat;
            localmat.at(y, x) = '#';
            count += test_guard_route(localmat);
        }
    }
    return count;
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
    
    MAX_THREADS = std::thread::hardware_concurrency()-1;
    std::cout << bruteforce_test_obstructions_multi(mat) << '\n';

    return 0;
}
