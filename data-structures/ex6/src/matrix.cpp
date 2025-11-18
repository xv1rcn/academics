#include <algorithm>
#include <stdexcept>

#include "matrix.h"

Matrix::Matrix(std::size_t rows, std::size_t cols) : rows_(rows), cols_(cols), data_(rows * cols) {}

double &Matrix::operator()(std::size_t i, std::size_t j) noexcept { return data_[i * cols_ + j]; }
const double &Matrix::operator()(std::size_t i, std::size_t j) const noexcept { return data_[i * cols_ + j]; }

double *Matrix::operator[](std::size_t i) noexcept { return &data_[i * cols_]; }
const double *Matrix::operator[](std::size_t i) const noexcept { return &data_[i * cols_]; }

Matrix Matrix::operator*(const Matrix &B) const {
    if (this->ncols() != B.nrows()) {
        throw std::invalid_argument("Incompatible matrix dimensions for multiplication");
    }
    std::size_t R = this->nrows();
    std::size_t K = this->ncols();
    std::size_t Cc = B.ncols();

    Matrix C(R, Cc);
    std::size_t total = R * Cc;
    for (std::size_t i = 0; i < total; ++i)
        C.data()[i] = 0.0;
    for (std::size_t i = 0; i < R; ++i) {
        for (std::size_t k = 0; k < K; ++k) {
            double aik = (*this)(i, k);
            const double *Brow = &B.data()[k * Cc];
            double *Crow = &C.data()[i * Cc];
            for (std::size_t j = 0; j < Cc; ++j) {
                Crow[j] += aik * Brow[j];
            }
        }
    }

    return C;
}

std::size_t Matrix::nrows() const noexcept { return rows_; }
std::size_t Matrix::ncols() const noexcept { return cols_; }

double *Matrix::data() noexcept { return data_.data(); }
const double *Matrix::data() const noexcept { return data_.data(); }

void Matrix::fill(double v) noexcept { std::fill(data_.begin(), data_.end(), v); }
