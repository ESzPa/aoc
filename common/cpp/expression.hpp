#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>
#include <cstdint>
#include <cmath>
#include <stack>
#include <stdexcept>

namespace aoc::utils {
    namespace {
        inline int64_t calc_factorial(int64_t a) {
            if(a < 2) return 1;
            int64_t result = 2;
            for(int64_t i = 3; i <= a; ++i)
                result *= i;
            return result;
        }

        inline const std::unordered_map<char, std::function<int64_t(int64_t, int64_t)>> binary_ops_map = {
            {'+', [](int64_t a, int64_t b){ return a + b; }},
            {'-', [](int64_t a, int64_t b){ return a - b; }},
            {'*', [](int64_t a, int64_t b){ return a * b; }},
            {'/', [](int64_t a, int64_t b){ return a / b; }},
            {'%', [](int64_t a, int64_t b){ return a % b; }},
            {'^', [](int64_t a, int64_t b){ return std::pow(a, b); }},
            {'&', [](int64_t a, int64_t b){ return a & b; }},
            {'|', [](int64_t a, int64_t b){ return a | b; }}
        };

        inline const std::unordered_map<char, std::function<int64_t(int64_t)>> unary_ops_map = {
            {'!', [](int64_t a){ return calc_factorial(a); }}
        };
    }

    inline int64_t evaluate_postfix(const std::vector<std::string>& tokens) {
        std::stack<int64_t> operands;

        for (const std::string& t : tokens) {
            if (t.size() == 1 && binary_ops_map.count(t[0])) {
                int64_t b = operands.top(); operands.pop();
                int64_t a = operands.top(); operands.pop();
                operands.push(binary_ops_map.at(t[0])(a, b));
            }
            else if (t.size() == 1 && unary_ops_map.count(t[0])) {
                int64_t a = operands.top(); operands.pop();
                operands.push(unary_ops_map.at(t[0])(a));
            }
            else {
                try {
                    operands.push(std::stoll(t));
                } catch (...) {
                    throw std::invalid_argument("Invalid token: " + t);
                }
            }
        }

        if (operands.size() != 1) {
            throw std::invalid_argument("Invalid postfix expression");
        }

        return operands.top();
    }
}
