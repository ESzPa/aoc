#pragma once
#include <algorithm>
#include <stdexcept>
#include <vector>

namespace aoc {
template <typename T>
class matrix {
  private:
    size_t rows_, cols_;
    std::vector<T> matrix_;

    constexpr inline void resize(size_t new_rows, size_t new_cols) {
        if (new_rows == rows_ && new_cols == cols_) return;

        std::vector<T> new_matrix(new_rows * new_cols, T());

        size_t min_rows = std::min(rows_, new_rows);
        size_t min_cols = std::min(cols_, new_cols);

        for (size_t y = 0; y < min_rows; ++y) {
            for (size_t x = 0; x < min_cols; ++x) {
                new_matrix[y * new_cols + x] =
                    std::move(matrix_[index_(y, x)]);
            }
        }

        matrix_ = std::move(new_matrix);
        rows_ = new_rows;
        cols_ = new_cols;
    }

    constexpr size_t index_(size_t y, size_t x) const noexcept {
        return y * cols_ + x;
    }

  public:
    matrix(size_t rows, size_t cols)
        : rows_(rows), cols_(cols), matrix_(cols_ * rows_) {}

    ~matrix() = default;

    matrix(const matrix& other) = default;

    constexpr size_t rows() const noexcept { return rows_; }
    constexpr size_t columns() const noexcept { return cols_; }
    constexpr size_t size() const noexcept { return cols_ * rows_; }

    constexpr T& at(size_t y, size_t x) noexcept {
        return matrix_[index_(y, x)];
    }

    constexpr const T& at(size_t y, size_t x) const noexcept {
        return matrix_[index_(y, x)];
    }

    constexpr T& operator()(size_t y, size_t x) {
        if (y >= rows_ || x >= cols_) {
            resize(std::max(rows_, y + 1), std::max(cols_, x + 1));
        }

        return matrix_[index_(y, x)];
    }

    constexpr const T& operator()(size_t y, size_t x) const {
        if (y >= rows_ || x >= cols_) {
            throw std::out_of_range("Matrix index out of bounds");
        }

        return matrix_[index_(y, x)];
    }

    constexpr T& operator[](size_t y, size_t x) {
        return (*this)(y, x);
    }

    constexpr const T& operator[](size_t y, size_t x) const {
        return (*this)(y, x);
    }

    template <typename Predicate>
    size_t count(Predicate predicate) {
        return std::count_if(matrix_.begin(), matrix_.end(),
                             predicate);
    }
};
} // namespace aoc
