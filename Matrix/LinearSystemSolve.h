#include <cmath>
#include <iostream>
#include "MatrixAndVectorOperations.h"

using namespace std;

double norma_1_matrix(const Matrix& C) {
    double max_sum = 0.0;

    for (int j = 0; j < C.len; j++) {
        double sum = 0.0;
        for (int i = 0; i < C.len; i++) {
            sum += abs(C.a[i][j]);
        }
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

double norma_inf_matrix(const Matrix& C) {
    double max_sum = 0.0;

    for (int i = 0; i < C.len; i++) {
        double sum = 0.0;
        for (int j = 0; j < C.len; j++) {
            sum += abs(C.a[i][j]);
        }
        max_sum = max(max_sum, sum);
    }
    return max_sum;
}

double norma_1_vector(const Vector& v) {
    double sum = 0.0;
    for (int i = 0; i < v.n; i++) {
        sum += abs(v.v[i]);
    }
    return sum;
}

double norma_inf_vector(const Vector& v) {
    double max_val = 0.0;
    for (int i = 0; i < v.n; i++) {
        max_val = max(max_val, abs(v.v[i]));
    }
    return max_val;
}

double norma_2_vector(const Vector& v) {
    double sum = 0.0;
    for (int i = 0; i < v.n; i++) {
        sum += v.v[i] * v.v[i];
    }
    return sqrt(sum);
}



Vector MPI(Matrix A, Vector b, double e, int n) {
    Matrix E = Matrix::identity(n);

    double u = 1.0 / norma_inf_matrix(A);
    Matrix B = E - u * A;

    if (norma_inf_matrix(B) >= 1) {
        Matrix A_n = A;
        A = A.transpose();
        b = A * b;
        A = A * A_n;

        u = 1.0 / norma_inf_matrix(A);
        B = E - u * A;
    }

    Vector c = u * b;

    double normB = norma_inf_matrix(B);

    Vector x0 = c;
    Vector x1 = B * x0 + c;

    int iter = 1;

    if (normB < 1) {
        while ((normB / (1 - normB)) * norma_inf_vector(x1 - x0) > e) {
            x0 = x1;
            x1 = B * x0 + c;
            iter++;
        }
    }
    else {
        while (norma_inf_vector(A * x1 - b) > e) {
            x0 = x1;
            x1 = B * x0 + c;
            iter++;
        }
    }

    cout << "Count of iterations: " << iter << "\n";
    return x1;
}


Vector method_Seidel(Matrix A, Vector b, double e, int n) {
    bool need_transform = false;

    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++) {
            sum += abs(A.a[i][j]);
        }
        if (sum > 2 * abs(A.a[i][i])) {
            need_transform = true;
            break;
        }
    }

    if (need_transform) {
        Matrix A_n = A;
        A = A.transpose();
        b = A * b;
        A = A * A_n;
    }

    Matrix C = A;
    Vector d = b;

    for (int i = 0; i < n; i++) {
        d.v[i] /= A.a[i][i];
        for (int j = 0; j < n; j++) {
            if (i != j)
                C.a[i][j] = -C.a[i][j] / A.a[i][i];
            else
                C.a[i][j] = 0;
        }
    }

    Vector x0 = d;
    Vector x1 = d;

    for (int i = 0; i < n; i++) {
        for (int g = 0; g < i; g++)
            x1.v[i] += C.a[i][g] * x1.v[g];

        for (int j = i + 1; j < n; j++)
            x1.v[i] += C.a[i][j] * x0.v[j];
    }

    int iter = 1;

    while (norma_inf_vector(A * x1 - b) > e) {
        x0 = x1;
        x1 = d;

        for (int i = 0; i < n; i++) {
            for (int g = 0; g < i; g++)
                x1.v[i] += C.a[i][g] * x1.v[g];

            for (int j = i + 1; j < n; j++)
                x1.v[i] += C.a[i][j] * x0.v[j];
        }
        iter++;
    }

    cout << "Count of iterations: " << iter << "\n";
    return x1;
}


Vector PLU(Matrix A, Vector b, int n) {
    Matrix P = Matrix::identity(n);

    for (int k = 0; k < n - 1; k++) {

        for (int j = k; j < n; j++) {
            double max_row = 0;
            int max_i = j;

            for (int i = j; i < n; i++) {
                if (abs(A.a[i][j]) > max_row) {
                    max_row = abs(A.a[i][j]);
                    max_i = i;
                }
            }

            swap(A.a[j], A.a[max_i]);
            swap(P.a[j], P.a[max_i]);
        }

        for (int j = k + 1; j < n; j++) {
            A.a[j][k] /= A.a[k][k];
            for (int d = k + 1; d < n; d++) {
                A.a[j][d] -= A.a[j][k] * A.a[k][d];
            }
        }
    }

    Matrix L = A;
    Matrix U = A;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) L.a[i][j] = 1;
            if (i < j) L.a[i][j] = 0;
            if (i > j) U.a[i][j] = 0;
        }
    }

    b = P * b;

    Vector y = b;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            y.v[i] -= L.a[i][j] * y.v[j];
        }
    }

    Vector x = y;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            x.v[n - 1 - i] -= U.a[n - 1 - i][n - 1 - j] * x.v[n - 1 - j];
        }
        x.v[n - 1 - i] /= U.a[n - 1 - i][n - 1 - i];
    }

    return x;
}


Vector QR(Matrix A, Vector b, int n) {

    Matrix Q = Matrix::identity(n);
    Matrix R = A;

    int k = 0;

    while (k < n - 1) {

        Matrix R_x(n - k);
        for (int i = 0; i < n - k; i++) {
            for (int j = 0; j < n - k; j++) {
                R_x.a[i][j] = R.a[i + k][j + k];
            }
        }

        Vector z(n - k);
        z.v[0] = 1;

        Vector y(n - k);
        for (int i = 0; i < y.n; i++) {
            y.v[i] = R.a[i + k][k];
        }

        Vector w = y - norma_2_vector(y) * z;
        w = (1.0 / norma_2_vector(w)) * w;

        Matrix E = Matrix::identity(n - k);
        Matrix Q_x = E - 2 * (w * w);

        R_x = Q_x * R_x;

        for (int i = 0; i < n - k; i++) {
            for (int j = 0; j < n - k; j++) {
                R.a[i + k][j + k] = R_x.a[i][j];
            }
        }

        Matrix Q_dop = Matrix::identity(n);
        for (int i = 0; i < n - k; i++) {
            for (int j = 0; j < n - k; j++) {
                Q_dop.a[i + k][j + k] = Q_x.a[i][j];
            }
        }

        Q = Q * Q_dop;

        k++;
    }

    Q = Q.transpose();

    Vector f = Q * b;

    Vector x = f;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            x.v[n - 1 - i] -= R.a[n - 1 - i][n - 1 - j] * x.v[n - 1 - j];
        }
        x.v[n - 1 - i] /= R.a[n - 1 - i][n - 1 - i];
    }

    return x;
}