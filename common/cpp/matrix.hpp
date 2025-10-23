#pragma once
#include <vector>
#include <stdexcept>

namespace aoc {
    template <typename T>
    class matrix {
    private:
        size_t rows_, cols_;
        std::vector<std::vector<T>> matrix_;

        inline void resize(size_t new_rows, size_t new_cols) {
            for (auto& row : matrix_) {
                row.resize(new_cols, T());
            }
            matrix_.resize(new_rows, std::vector<T>(new_cols, T()));

            rows_ = new_rows;
            cols_ = new_cols;
        }

    public:
        matrix(size_t rows, size_t cols) 
            : rows_(rows), cols_(cols), matrix_(rows, std::vector<T>(cols, T())) {}

        ~matrix() = default;

        matrix(const matrix& other) = default;

        size_t rows()   const   { return rows_; }
        size_t columns() const   { return cols_; }

        T& at(size_t x, size_t y) {
            return matrix_[x][y];
        }

        const T& at(size_t x, size_t y) const {
            return matrix_[x][y];
        }

        T& operator()(size_t x, size_t y) {
            if (x >= rows_ || y >= cols_) {
                resize(std::max(rows_, x + 1), std::max(cols_, y + 1));
            }

            return matrix_[x][y];
        }

        const T& operator()(size_t x, size_t y) const {
            if (x >= rows_ || y >= cols_) {
                throw std::out_of_range("Matrix index out of bounds");
            }

            return matrix_[x][y];
        }

        template <typename Predicate>
        size_t count(Predicate predicate) {
            size_t count = 0;
            for(auto& row : matrix_) {
                for(T& e : row) {
                    if(predicate(e)) ++count;
                }
            }
            return count;
        }
    };
}
