#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>

using namespace std;

struct Vector {
    int n;
    vector<double> v;

    Vector(int n = 0) : n(n), v(n, 0.0) {}


    void input() {
        for (int i = 0; i < n; i++)
            cin >> v[i];
    }


    void print() const {
        for (double x : v)
            cout << x << " ";
        cout << "\n\n";
    }


    static Vector zero(int n) {
        return Vector(n);
    }

    static Vector test0() {
        Vector x(3);
        x.v = { 13, 17, 32 };
        return x;
    }

    static Vector test1(int n) {
        int s = 12;
        Vector x(n);
        for (int i = 0; i < n; i++)
            x.v[i] = s + 4 + 2 * i;
        return x;
    }

    static Vector test2(int n) {
        int s = 12;
        Vector x(n);
        for (int i = 0; i < n; i++)
            x.v[i] = -(s + 4 + 2 * i);
        return x;
    }

    static Vector test5(int n) {
        Vector x(n);
        for (int i = 0; i < n; i++)
            x.v[i] = -1;
        x.v[n - 1] = 1;
        return x;
    }

    static Vector answer0(int n) {
        Vector x(n);
        for (int i = 0; i < n; i++)
            x.v[i] = i + 1;
        return x;
    }

    static Vector answer1(int n) {
        Vector x(n);
        for (int i = 0; i < n; i++)
            x.v[i] = 1;
        return x;
    }

    static Vector answer2() {
        Vector x(3);
        x.v = { 2637.0 / 1991, 2561.0 / 1991, 2501.0 / 1991 };
        return x;
    }

    static Vector answer5(int n, double e) {
        Vector x(n);
        x.v[n - 1] = 1.0 / (1.0 + 12 * e);
        return x;
    }
};

Vector operator+(const Vector& a, const Vector& b) {
    if (a.n != b.n)
        throw invalid_argument("Different sizes");

    Vector r(a.n);
    for (int i = 0; i < a.n; i++)
        r.v[i] = a.v[i] + b.v[i];

    return r;
}

Vector operator-(const Vector& a, const Vector& b) {
    if (a.n != b.n)
        throw invalid_argument("Different sizes");

    Vector r(a.n);
    for (int i = 0; i < a.n; i++)
        r.v[i] = a.v[i] - b.v[i];

    return r;
}

Vector operator*(double k, const Vector& a) {
    Vector r(a.n);
    for (int i = 0; i < a.n; i++)
        r.v[i] = k * a.v[i];
    return r;
}