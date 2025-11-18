#ifndef MATRIX_H
#define MATRIX_H

#include <cstddef>
#include <vector>

class Matrix {
public:
    Matrix(std::size_t rows, std::size_t cols);

    Matrix(const Matrix &) = default;
    Matrix(Matrix &&) noexcept = default;

    ~Matrix() = default;

    Matrix &operator=(const Matrix &) = default;
    Matrix &operator=(Matrix &&) noexcept = default;

    double &operator()(std::size_t i, std::size_t j) noexcept;
    const double &operator()(std::size_t i, std::size_t j) const noexcept;

    double *operator[](std::size_t i) noexcept;
    const double *operator[](std::size_t i) const noexcept;

    Matrix operator*(const Matrix &other) const;

    std::size_t nrows() const noexcept;
    std::size_t ncols() const noexcept;

    double *data() noexcept;
    const double *data() const noexcept;

    void fill(double v) noexcept;

    double *begin() noexcept { return data(); }
    double *end() noexcept { return data() + (nrows() * ncols()); }
    const double *begin() const noexcept { return data(); }
    const double *end() const noexcept { return data() + (nrows() * ncols()); }

private:
    std::size_t rows_;
    std::size_t cols_;
    std::vector<double> data_;
};

#endif // MATRIX_H
