#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

struct Matrix {
    int len;
    vector<vector<double>> a;

    Matrix(int n = 0) : len(n), a(n, vector<double>(n, 0.0)) {}

    void input() {
        for (int i = 0; i < len; i++)
            for (int j = 0; j < len; j++)
                cin >> a[i][j];
    }


    void print() const {
        for (int i = 0; i < len; i++) {
            for (int j = 0; j < len; j++) {
                cout << a[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }


    static Matrix identity(int n) {
        Matrix I(n);
        for (int i = 0; i < n; i++)
            I.a[i][i] = 1;
        return I;
    }

    Matrix transpose() const {
        Matrix t(len);
        for (int i = 0; i < len; i++)
            for (int j = 0; j < len; j++)
                t.a[j][i] = a[i][j];
        return t;
    }

    static Matrix test0() {
        Matrix m(3);
        m.a = {
            {0, 2, 3},
            {1, 2, 4},
            {4, 5, 6}
        };
        return m;
    }

    static Matrix test1(int n) {
        int s = 12;
        Matrix m(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                m.a[i][j] = (i == j) ? (s + 2 * (i + 1)) : 1;
        return m;
    }

    static Matrix test2(int n) {
        int s = 12;
        Matrix m(n);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                m.a[i][j] = (i == j) ? -(s + 2 * (i + 1)) : 1;
        return m;
    }

    static Matrix test5(int n, double e) {
        int s = 12;
        Matrix m(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j)
                    m.a[i][j] = 1.0 + s * e;
                else if (i < j)
                    m.a[i][j] = -1 - s * e;
                else
                    m.a[i][j] = s * e;
            }
        }
        return m;
    }
};


Matrix operator+(const Matrix& A, const Matrix& B) {
    if (A.len != B.len)
        throw invalid_argument("Different sizes");

    Matrix C(A.len);
    for (int i = 0; i < A.len; i++)
        for (int j = 0; j < A.len; j++)
            C.a[i][j] = A.a[i][j] + B.a[i][j];

    return C;
}

Matrix operator-(const Matrix& A, const Matrix& B) {
    if (A.len != B.len)
        throw invalid_argument("Different sizes");

    Matrix C(A.len);
    for (int i = 0; i < A.len; i++)
        for (int j = 0; j < A.len; j++)
            C.a[i][j] = A.a[i][j] - B.a[i][j];

    return C;
}

Matrix operator*(const Matrix& A, const Matrix& B) {
    if (A.len != B.len)
        throw invalid_argument("Different sizes");

    Matrix C(A.len);
    for (int i = 0; i < A.len; i++) {
        for (int j = 0; j < A.len; j++) {
            for (int k = 0; k < A.len; k++) {
                C.a[i][j] += A.a[i][k] * B.a[k][j];
            }
        }
    }
    return C;
}

Matrix operator*(double k, const Matrix& A) {
    Matrix C(A.len);
    for (int i = 0; i < A.len; i++)
        for (int j = 0; j < A.len; j++)
            C.a[i][j] = k * A.a[i][j];

    return C;
}