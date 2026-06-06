#include "linAlg.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <utility>

Vec operator+(const Vec& v1, const Vec& v2) {
	Vec v3(v1.size());

	for (size_t i = 0; i < v1.size(); i++) {
		v3[i] = v1[i] + v2[i];
	}

	return v3;
}

Vec& operator+=(Vec& v1, const Vec& v2) {
	v1 = v1 + v2;
	return v1;
}

Vec operator-(const Vec& v1, const Vec& v2) {
	Vec v3(v1.size());

	for (size_t i = 0; i < v1.size(); i++) {
		v3[i] = v1[i] - v2[i];
	}

	return v3;
}

Vec& operator-=(Vec& v1, const Vec& v2) {
	v1 = v1 - v2;
	return v1;
}

Vec operator*(const double& c, const Vec& v) {
	Vec r(v.size());

	for (size_t i = 0; i < v.size(); i++) {
		r[i] = c * v[i];
	}

	return r;
}

Vec operator*(const Vec& v, const double& c) {
	return c * v;
}

// dot product
double operator*(const Vec& v1, const Vec& v2) {
	double dot = 0;

	for (size_t i = 0; i < v1.size(); i++) {
		dot += v1[i] * v2[i];
	}

	return dot;
}

Vec operator/(const Vec& v, const double& c) {
	Vec r(v.size());

	for (size_t i = 0; i < v.size(); i++) {
		r[i] = v[i] / c;
	}

	return r;
}


Matrix operator*(const Matrix& m1, const Matrix& m2) {
	// assume that m1[0].size() == m2.size() "inner dimensions"
	// NOTE, matrix multiplication is not commutative!
	
	size_t rows = m1.rows();
	size_t cols = m2.cols();

	Matrix p(rows, cols);

	for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            p[i][j] = 0;

            for (size_t k = 0; k < m1.cols(); k++) {
                p[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

	return p;
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
	size_t rows = m1.rows();
	size_t cols = m2.cols();

	Matrix r(rows, cols);

	for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            r[i][j] = m1[i][j] + m2[i][j];
        }
    }

	return r;
}

Matrix& operator+=(Matrix& m1, const Matrix& m2) {
	m1 = m1 + m2;
	return m1;
}

Matrix operator-(const Matrix& m1, const Matrix& m2) {
	size_t rows = m1.rows();
	size_t cols = m2.cols();

	Matrix r(rows, cols);

	for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            r[i][j] = m1[i][j] - m2[i][j];
        }
    }

	return r;
}

Matrix& operator-=(Matrix& m1, const Matrix& m2) {
	m1 = m1 - m2;
	return m1;
}

Matrix operator+(const Matrix& m, const Vec& v) {
	// broadcasting ?
	Matrix r = m;
	for (Vec& row : r) {
		row += v;
	}

	return r;
}

Matrix operator+(const Matrix& m, const double& c) {
	Matrix r = m;
	for (Vec& row : r) {
		for (double& val : row) {
			val += c;
		}
	}

	return r;
}

Matrix operator*(const Matrix& m, const double& c) {
	Matrix r = m;
	size_t rows = m.rows();
	size_t cols = m.cols();

	for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            r[i][j] *= c;
        }
    }

	return r;
}

Matrix operator*(const double& c, const Matrix& m) {
	return m * c;
}


std::ostream& operator<<(std::ostream& os, const Vec& v) {
	for (const double& o : v) {
		os << std::fixed << std::setprecision(2) << o << "\t";
	}

	os << std::endl;

	return os;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
	for (const Vec& v : m) {
		os << v;
	}

	return os;
}

double sum(const Vec& v) {
	double vecSum = 0;
	for (const double& value : v) {
		vecSum += value;
	}

	return vecSum;
}

double average(const Vec& v) {
	return sum(v) / v.size();
}

Matrix transpose(const Matrix& m) {
    size_t rows = m.rows();
   	size_t cols = m.cols();

    Matrix t(cols, rows);

    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            t[j][i] = m[i][j];
        }
    }

    return t;
}

Vec GaussianElimination(const Matrix& A, const Vec& b) {
	Vec x(b.size());

	// + 1 col for the constant vector being augmented to A
	Matrix augmented(A.rows(), A.cols() + 1);

	// populate augmented matrix
	for (size_t i = 0; i < A.rows(); i++) {
		for (size_t j = 0; j < A.cols(); j++) {
			// first, the coefficent matrix
			augmented[i][j] = A[i][j];
		}
		// second, the constant vector
		augmented[i][A.cols()] = b[i];
	}

	// RREF
	for (size_t i = 0; i < A.rows(); i++) {
		size_t pivotRow = i;
		// start at diagonal, pivot must not be further to the left than this
		double maxValue = std::abs(augmented[i][i]);
		for (size_t j = i + 1; j < A.cols(); j++) {
			if (std::abs(augmented[j][i]) > maxValue) {
				maxValue = std::abs(augmented[j][i]);
				pivotRow = j;
			}
		}
		
		// no effect if i == pivot row
		std::swap(augmented[i], augmented[pivotRow]);

		if (std::abs(augmented[i][i]) < std::numeric_limits<double>::epsilon()) {
			throw std::runtime_error("system has no unique solution!");
		}

		double pivot = augmented[i][i];
		// ""normalise"" row (to ensure 1s across diagonals)
		augmented[i] = augmented[i] / pivot;

		for (size_t j = i + 1; j < A.cols(); j++) {
			augmented[j] -= augmented[i] * augmented[j][i];
		}
	}

	// back sub
	for (int i = A.rows() - 1; i >= 0; i--) {
		// get value from augmented part
		x[i] = augmented[i][A.rows()];
		for (size_t j = 0; j < i; j++) {
			augmented[j] -= augmented[i] * augmented[j][i];
		}
	}

	// std::cout << augmented << std::endl;
	return x;
}
