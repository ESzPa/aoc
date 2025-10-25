#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <iterator>

namespace aoc {
    class file {
    private:
        std::ifstream file_; 

    public:
        std::string line;

        explicit file(const std::string& name) : file_(name) {
            if(!file_.is_open()) {
                std::runtime_error("Failed to open file: " + name);
            }
        }

        bool readline() {
            return static_cast<bool>(std::getline(file_, line));
        }

        std::string read_all(bool auto_reset = true) {
            std::string content(
                (std::istreambuf_iterator<char>(file_)),
                std::istreambuf_iterator<char>()
            );

            if(auto_reset) reset();

            return content;
        }

        std::vector<std::string> read_lines(bool auto_reset = true) {
            std::vector<std::string> lines;
            std::string l;
            while(std::getline(file_, l)) lines.push_back(l);

            if(auto_reset) reset();

            return lines;
        }

        void reset() {
            file_.clear();
            file_.seekg(0, std::ios::beg);
        }
    };
}
