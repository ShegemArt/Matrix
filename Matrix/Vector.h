#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

// Класс представляет математический вектор размерности Dimension
struct Vector {
    // Размерность вектора
    int Dimension;

    // Вектор для хранения значений
    vector<double> numbersVector;

    // Конструктор: создает вектор длины n и заполняет его нулями
    Vector(int n = 0) : Dimension(n), numbersVector(n, 0.0) {}

    // Вводит элементов вектора с консоли
    void InputFromConsole() {
        for (int i = 0; i < Dimension; i++)
            cin >> numbersVector[i];
    }

    // Выводит вектора на экран
    void Print() const {
        for (double x : numbersVector)
            cout << x << " ";
        cout << "\n\n";
    }

    // Статический метод: создает нулевой вектор размерности n
    static Vector MakeZeroVector(int n) {
        return Vector(n);
    }
};

// Перегрузка оператора сложения векторов
Vector operator+(const Vector& a, const Vector& b) {
    if (a.Dimension != b.Dimension)
        throw invalid_argument("Different sizes");

    Vector r(a.Dimension);
    for (int i = 0; i < a.Dimension; i++)
        r.numbersVector[i] = a.numbersVector[i] + b.numbersVector[i];

    return r;
}

// Перегрузка оператора вычитания векторов
Vector operator-(const Vector& a, const Vector& b) {
    if (a.Dimension != b.Dimension)
        throw invalid_argument("Different sizes");

    Vector r(a.Dimension);
    for (int i = 0; i < a.Dimension; i++)
        r.numbersVector[i] = a.numbersVector[i] - b.numbersVector[i];

    return r;
}

// Перегрузка оператора умножения вектора на число
Vector operator*(double k, const Vector& a) {
    Vector r(a.Dimension);
    for (int i = 0; i < a.Dimension; i++)
        r.numbersVector[i] = k * a.numbersVector[i];
    return r;
}