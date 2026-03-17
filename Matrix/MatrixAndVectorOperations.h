#include "Vector.h"
#include "Matrix.h"
#pragma once

Vector operator*(const Matrix& A, const Vector& x) {
    if (A.len != x.n)
        throw invalid_argument("Size mismatch");

    Vector r(x.n);

    for (int i = 0; i < x.n; i++) {
        for (int j = 0; j < x.n; j++) {
            r.v[i] += A.a[i][j] * x.v[j];
        }
    }

    return r;
}

Matrix operator*(const Vector& a, const Vector& b) {
    Matrix A;
    A.len = a.n;

    // создаём матрицу n x n
    A.a.resize(A.len, std::vector<double>(A.len));

    for (int i = 0; i < a.n; i++) {
        for (int j = 0; j < a.n; j++) {
            A.a[i][j] = a.v[i] * b.v[j];
        }
    }

    return A;
}