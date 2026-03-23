#include "Vector.h"
#include "Matrix.h"
#pragma once

// Перегрузка оператора умножения матрицы на вектор r = A * x
Vector operator*(const Matrix& A, const Vector& x) {
    if (A.Dimension != x.Dimension)
        throw invalid_argument("Size mismatch");

    Vector r(x.Dimension);

    for (int i = 0; i < x.Dimension; i++) {
        for (int j = 0; j < x.Dimension; j++) {
            r.numbersVector[i] += A.numbersMatrix[i][j] * x.numbersVector[j];
        }
    }

    return r;
}

// Перегрузка оператора умножения двух векторов
Matrix operator*(const Vector& a, const Vector& b) {
    Matrix A;
    A.Dimension = a.Dimension;
        
    // создаём матрицу n x n
    A.numbersMatrix.resize(A.Dimension, std::vector<double>(A.Dimension));

    for (int i = 0; i < a.Dimension; i++) {
        for (int j = 0; j < a.Dimension; j++) {
            A.numbersMatrix[i][j] = a.numbersVector[i] * b.numbersVector[j];
        }
    }

    return A;
}