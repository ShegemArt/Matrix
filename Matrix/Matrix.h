#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

// Класс представляет квадратную матрицу размера Dimension x Dimension
struct Matrix {
    // Размерность матрицы
    int Dimension;

    // Двумерный вектор для хранения элементов матрицы
    vector<vector<double>> numbersMatrix;

    // Конструктор: создает матрицу размера n x n и заполняет её нулями
    Matrix(int n = 0) : Dimension(n), numbersMatrix(n, vector<double>(n, 0.0)) {}

    // Вводит элементов матрицы с консоли
    void InputFromConsole() {
        for (int i = 0; i < Dimension; i++)
            for (int j = 0; j < Dimension; j++)
                cin >> numbersMatrix[i][j];
    }

    // Выводит матрицы на экран
    void Print() const {
        for (int i = 0; i < Dimension; i++) {
            for (int j = 0; j < Dimension; j++) {
                cout << numbersMatrix[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    // Создает единичную матрицу размера n x n
    static Matrix MakeIdentityMatrix(int n) {
        Matrix I(n);
        for (int i = 0; i < n; i++)
            I.numbersMatrix[i][i] = 1;
        return I;
    }

    // Возвращает транспонированную матрицу
    Matrix Transpose() const {
        Matrix t(Dimension);
        for (int i = 0; i < Dimension; i++)
            for (int j = 0; j < Dimension; j++)
                t.numbersMatrix[j][i] = numbersMatrix[i][j];
        return t;
    }

    // Возвращает подматрицу заданной размерности ( оберзаются крайние столбцы/строки)
    Matrix SubMatrix(int m) const {
        if (m <= 0 || m > Dimension) {
            throw std::invalid_argument("Invalid subMatrix size");
        }

        Matrix result(m);

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < m; j++) {
                result.numbersMatrix[i][j] = numbersMatrix[i][j];
            }
        }

        return result;
    }
};

// Перегрузка оператора сложения матриц
Matrix operator+(const Matrix& A, const Matrix& B) {
    if (A.Dimension != B.Dimension)
        throw invalid_argument("Different sizes");

    Matrix C(A.Dimension);
    for (int i = 0; i < A.Dimension; i++)
        for (int j = 0; j < A.Dimension; j++)
            C.numbersMatrix[i][j] = A.numbersMatrix[i][j] + B.numbersMatrix[i][j];

    return C;
}

// Перегрузка оператора вычитания матриц
Matrix operator-(const Matrix& A, const Matrix& B) {
    if (A.Dimension != B.Dimension)
        throw invalid_argument("Different sizes");

    Matrix C(A.Dimension);
    for (int i = 0; i < A.Dimension; i++)
        for (int j = 0; j < A.Dimension; j++)
            C.numbersMatrix[i][j] = A.numbersMatrix[i][j] - B.numbersMatrix[i][j];

    return C;
}

// Перегрузка оператора умножения матриц
Matrix operator*(const Matrix& A, const Matrix& B) {
    if (A.Dimension != B.Dimension)
        throw invalid_argument("Different sizes");

    Matrix C(A.Dimension);
    for (int i = 0; i < A.Dimension; i++) {
        for (int j = 0; j < A.Dimension; j++) {
            for (int k = 0; k < A.Dimension; k++) {
                C.numbersMatrix[i][j] += A.numbersMatrix[i][k] * B.numbersMatrix[k][j];
            }
        }
    }
    return C;
}

// Перегрузка оператора умножения матрицы на число
Matrix operator*(double k, const Matrix& A) {
    Matrix C(A.Dimension);
    for (int i = 0; i < A.Dimension; i++)
        for (int j = 0; j < A.Dimension; j++)
            C.numbersMatrix[i][j] = k * A.numbersMatrix[i][j];

    return C;
}