#include <iostream>
#include "Matrix.h"
#include "Vector.h"
#include "LinearSystemSolve.h"

using namespace std;

int main() {

    while (true) {
        cout << "\nChoose method:\n";
        cout << "1 ---> MPI\n";
        cout << "2 ---> method_Seidel\n";
        cout << "3 ---> PLU\n";
        cout << "4 ---> QR\n";
        cout << "0 ---> Exit\n";

        int inputNumber;
        cin >> inputNumber;

        if (inputNumber == 0) break;

        int n;
        cout << "Enter matrix size n:\n";
        cin >> n;

        Matrix A(n);
        cout << "Enter matrix A (" << n << "x" << n << "):\n";
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cin >> A.numbersMatrix[i][j];
            }
        }

        Vector b(n);
        cout << "Enter vector b:\n";
        for (int i = 0; i < n; i++) {
            cin >> b.numbersVector[i];
        }

        double e;
        cout << "Enter epsilon:\n";
        cin >> e;

        Vector result;

        if (inputNumber == 1) {
            result = MPI(A, b, e, n);
        }
        else if (inputNumber == 2) {
            result = Seidel(A, b, e, n);
        }
        else if (inputNumber == 3) {
            result = PLU(A, b, n);
        }
        else if (inputNumber == 4) {
            result = QR(A, b, n);
        }
        else {
            cout << "Wrong choice\n";
            continue;
        }

        cout << "\nSolution x:\n";
        result.Print();
    }

    return 0;
}