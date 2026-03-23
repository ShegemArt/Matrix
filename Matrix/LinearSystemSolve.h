#include <cmath>
#include <iostream>
#include "MatrixAndVectorOperations.h"

using namespace std;

// L1-норма матрицы (максимальная сумма модулей по столбцам)
double L1MatrixNorm(const Matrix& C) {
    double max_sum = 0.0;

    for (int j = 0; j < C.Dimension; j++) {
        double sum = 0.0;
        for (int i = 0; i < C.Dimension; i++) {
            sum += abs(C.numbersMatrix[i][j]);
        }
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

// Бесконечная норма матрицы (максимальная сумма модулей по строкам)
double InfMatrixNorm(const Matrix& C) {
    double max_sum = 0.0;

    for (int i = 0; i < C.Dimension; i++) {
        double sum = 0.0;
        for (int j = 0; j < C.Dimension; j++) {
            sum += abs(C.numbersMatrix[i][j]);
        }
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

// L1-норма вектора (сумма модулей элементов)
double L1VectorNorm(const Vector& v) {
    double sum = 0.0;
    for (int i = 0; i < v.Dimension; i++) {
        sum += abs(v.numbersVector[i]);
    }
    return sum;
}

// Бесконечная норма вектора (максимальный по модулю элемент)
double InfVectorNorm(const Vector& v) {
    double max_val = 0.0;
    for (int i = 0; i < v.Dimension; i++) {
        max_val = max(max_val, abs(v.numbersVector[i]));
    }
    return max_val;
}

// Евклидова (L2) норма вектора
double L2VectorNorm(const Vector& v) {
    double sum = 0.0;
    for (int i = 0; i < v.Dimension; i++) {
        sum += v.numbersVector[i] * v.numbersVector[i];
    }
    return sqrt(sum);
}

// Решает систему Ax = b методом простых итераций
Vector MPI(Matrix A, Vector b, double e, int n) {
    Matrix E = Matrix::MakeIdentityMatrix(n);

    // Выбор параметра релаксации
    double u = 1.0 / InfMatrixNorm(A);

    // Формируем матрицу итераций: B = E - uA
    Matrix B = E - u * A;

    // Если условие сходимости не выполняется
    if (InfMatrixNorm(B) >= 1) {
        // Переход к нормальной системе A^T A x = A^T b
        Matrix A_n = A;
        A = A.Transpose();
        b = A * b;
        A = A * A_n;

        u = 1.0 / InfMatrixNorm(A);
        B = E - u * A;
    }

    Vector c = u * b;

    double normB = InfMatrixNorm(B);

    Vector x0 = c;
    Vector x1 = B * x0 + c;

    int iter = 1;

    // Оценка ошибки через норму B
    if (normB < 1) {
        while ((normB / (1 - normB)) * InfVectorNorm(x1 - x0) > e) {
            x0 = x1;
            x1 = B * x0 + c;
            iter++;
        }
    }
    else {
        // Альтернативный критерий (через невязку)
        while (InfVectorNorm(A * x1 - b) > e) {
            x0 = x1;
            x1 = B * x0 + c;
            iter++;
        }
    }

    cout << "Count of iterations: " << iter << "\n";
    return x1;
}

// Решает систему Ax = b методом Зейделя 
Vector Seidel(Matrix A, Vector b, double e, int n) {
    bool need_transform = false;

    // Проверка диагонального преобладания
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += abs(A.numbersMatrix[i][j]);
        }
        if (sum > 2 * abs(A.numbersMatrix[i][i])) {
            need_transform = true;
            break;
        }
    }

    // Приведение к A^T A x = A^T b
    if (need_transform) {
        Matrix A_n = A;
        A = A.Transpose();
        b = A * b;
        A = A * A_n;
    }

    Matrix C = A;
    Vector d = b;

    // Приведение к итерационному виду x = Cx + d
    for (int i = 0; i < n; i++) {
        d.numbersVector[i] /= A.numbersMatrix[i][i];
        for (int j = 0; j < n; j++) {
            if (i != j)
                C.numbersMatrix[i][j] = -C.numbersMatrix[i][j] / A.numbersMatrix[i][i];
            else
                C.numbersMatrix[i][j] = 0;
        }
    }

    Vector x0 = d;
    Vector x1 = d;

    for (int i = 0; i < n; i++) {
        for (int g = 0; g < i; g++)
            x1.numbersVector[i] += C.numbersMatrix[i][g] * x1.numbersVector[g];

        for (int j = i + 1; j < n; j++)
            x1.numbersVector[i] += C.numbersMatrix[i][j] * x0.numbersVector[j];
    }

    int iter = 1;

    while (InfVectorNorm(A * x1 - b) > e) {
        x0 = x1;
        x1 = d;

        for (int i = 0; i < n; i++) {
            for (int g = 0; g < i; g++)
                x1.numbersVector[i] += C.numbersMatrix[i][g] * x1.numbersVector[g];

            for (int j = i + 1; j < n; j++)
                x1.numbersVector[i] += C.numbersMatrix[i][j] * x0.numbersVector[j];
        }
        iter++;
    }

    cout << "Count of iterations: " << iter << "\n";
    return x1;
}

// Решение Ax = b через LU-разложение с перестановками
Vector PLU(Matrix A, Vector b, int n) {
    Matrix P = Matrix::MakeIdentityMatrix(n);

    // Прямой ход (LU-разложение)
    for (int k = 0; k < n - 1; k++) {

        for (int j = k; j < n; j++) {
            double max_row = 0;
            int max_i = j;

            for (int i = j; i < n; i++) {
                if (abs(A.numbersMatrix[i][j]) > max_row) {
                    max_row = abs(A.numbersMatrix[i][j]);
                    max_i = i;
                }
            }

            swap(A.numbersMatrix[j], A.numbersMatrix[max_i]);
            swap(P.numbersMatrix[j], P.numbersMatrix[max_i]);
        }

        for (int j = k + 1; j < n; j++) {
            A.numbersMatrix[j][k] /= A.numbersMatrix[k][k];
            for (int d = k + 1; d < n; d++) {
                A.numbersMatrix[j][d] -= A.numbersMatrix[j][k] * A.numbersMatrix[k][d];
            }
        }
    }

    Matrix L = A;
    Matrix U = A;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) L.numbersMatrix[i][j] = 1;
            if (i < j) L.numbersMatrix[i][j] = 0;
            if (i > j) U.numbersMatrix[i][j] = 0;
        }
    }

    b = P * b;

    // Решение Ly = b (прямой ход)
    Vector y = b;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            y.numbersVector[i] -= L.numbersMatrix[i][j] * y.numbersVector[j];
        }
    }

    // Решение Ux = y (обратный ход)
    Vector x = y;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            x.numbersVector[n - 1 - i] -= U.numbersMatrix[n - 1 - i][n - 1 - j] * x.numbersVector[n - 1 - j];
        }
        x.numbersVector[n - 1 - i] /= U.numbersMatrix[n - 1 - i][n - 1 - i];
    }

    return x;
}

// Решение Ax = b через QR-разложение
Vector QR(Matrix A, Vector b, int n) {

    Matrix Q = Matrix::MakeIdentityMatrix(n);
    Matrix R = A;

    int k = 0;

    // Построение QR-разложения
    while (k < n - 1) {

        Matrix R_x(n - k);
        for (int i = 0; i < n - k; i++) {
            for (int j = 0; j < n - k; j++) {
                R_x.numbersMatrix[i][j] = R.numbersMatrix[i + k][j + k];
            }
        }

        // Вектор для отражения Хаусхолдера
        Vector z(n - k);
        z.numbersVector[0] = 1;

        Vector y(n - k);
        for (int i = 0; i < y.Dimension; i++) {
            y.numbersVector[i] = R.numbersMatrix[i + k][k];
        }

        Vector w = y - L2VectorNorm(y) * z;
        w = (1.0 / L2VectorNorm(w)) * w;

        Matrix E = Matrix::MakeIdentityMatrix(n - k);

        // Матрица Хаусхолдера
        Matrix Q_x = E - 2 * (w * w);

        R_x = Q_x * R_x;

        for (int i = 0; i < n - k; i++) {
            for (int j = 0; j < n - k; j++) {
                R.numbersMatrix[i + k][j + k] = R_x.numbersMatrix[i][j];
            }
        }

        // Расширяем Q_x до полной размерности
        Matrix Q_dop = Matrix::MakeIdentityMatrix(n);
        for (int i = 0; i < n - k; i++) {
            for (int j = 0; j < n - k; j++) {
                Q_dop.numbersMatrix[i + k][j + k] = Q_x.numbersMatrix[i][j];
            }
        }

        Q = Q * Q_dop;

        k++;
    }

    // Получаем ортогональную матрицу Q
    Q = Q.Transpose();

    // Решаем Q^T b
    Vector f = Q * b;

    // Решаем Rx = f (обратный ход)
    Vector x = f;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            x.numbersVector[n - 1 - i] -= R.numbersMatrix[n - 1 - i][n - 1 - j] * x.numbersVector[n - 1 - j];
        }
        x.numbersVector[n - 1 - i] /= R.numbersMatrix[n - 1 - i][n - 1 - i];
    }

    return x;
}