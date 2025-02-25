#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>

using namespace std;

int m = 7;

// Функция для генерации симметричной матрицы A
void generateMatrix(vector<vector<double>> &A, int n, int m) {
    srand(time(0));
    double sum = 0; //накопитель суммы модулей недиагональных элементов
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            A[i][j] = (rand() % 201) - 100; // Элементы в диапазоне [-100, 100]
            A[j][i] = A[i][j]; // Симметричное присваивание
            sum += abs(A[i][j]);
        }

    }
    int end = sum + 10 * m;
    int start = sum + m;
    for (int i = 0; i < n; ++i) {
        A[i][i] = (rand() % (end - start + 1) ) + start;
    }
}

// Функция для умножения матрицы A на вектор x
vector<double> multiplyMatrixVector(const vector<vector<double>> &A, const vector<double> &x) {
    int n = x.size();
    vector<double> b(n, 0.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            b[i] += A[i][j] * x[j];
        }
    }

    return b;
}

// LDL^T разложение матрицы A
bool LDLTDecomposition(vector<vector<double>> &A, vector<vector<double>> &L, vector<double> &D, vector<vector<double>> &Lt) {
    int n = A.size();

    for (int i = 0; i < n; ++i) {
        D[i] = A[i][i];
        for (int j = 0; j < i; ++j) {
            D[i] -= L[i][j] * L[i][j] * D[j];
        }
        if (abs(D[i]) <= 1e-12) return false; // Проверка на сингулярность

        for (int j = i + 1; j < n; ++j) {
            L[j][i] = A[j][i];
            for (int k = 0; k < i; ++k) {
                L[j][i] -= L[i][k] * L[j][k] * D[k];
            }
            L[j][i] /= D[i];
            Lt[i][j] = L[j][i];
        }
    }

    return true;
}

// Решение системы уравнений LDL^T x = b
vector<double> solveSystem(const vector<vector<double>> &L, const vector<double> &D, const vector<vector<double>> &Lt, const vector<double> &b) {
    int n = b.size();
    vector<double> y(n, 0.0), z(n, 0.0), x(n, 0.0);

    // Решение системы Ly = b
    for (int i = 0; i < n; ++i) {
        y[i] = b[i];
        for (int j = 0; j < i; ++j) {
            y[i] -= L[i][j] * y[j];
        }
    }

    // Решение системы Dz = y
    for (int i = 0; i < n; ++i) {
        z[i] = y[i] / D[i];
    }

    // Решение системы L^T x = z
    for (int i = n - 1; i >= 0; --i) {
        x[i] = z[i];
        for (int j = i + 1; j < n; ++j) {
            x[i] -= Lt[i][j] * x[j];
        }
    }

    return x;
}

// Вычисление относительной погрешности
double computeRelativeError(const vector<double> &x, const vector<double> &x_approx) {
    double max_norm_diff = 0.0, max_norm_x = 0.0;

    for (int i = 0; i < x.size(); ++i) {
        max_norm_diff = max(max_norm_diff, abs(x[i] - x_approx[i]));
        max_norm_x = max(max_norm_x, abs(x[i]));
    }

    return max_norm_diff / max_norm_x;
}

int main() {
    int n = 1000;
    unsigned int time1 = clock();

    vector<vector<double>> A(n, vector<double>(n, 0.0));
    generateMatrix(A, n, m);


    vector<double> x(n);
    for (int i = 0; i < n; ++i) {
        x[i] = m + i;
    }

    // Вычисление правой части b
    vector<double> b = multiplyMatrixVector(A, x);

    // Инициализация матриц L и вектора D для LDL^T разложения
    vector<vector<double>> L(n, vector<double>(n, 0.0));
    vector<vector<double>> Lt(n, vector<double>(n, 0.0));
    vector<double> D(n, 0.0);
    for (int i = 0; i < n; ++i) {
        L[i][i] = 1.0;
        Lt[i][i] = 1.0;
    }

    // Выполнение LDL^T разложения
    if (!LDLTDecomposition(A, L, D, Lt)) {
        cout << "Solution is not possible" << endl;
        return -1;
    }

    // Решение системы

    vector<double> x_approx = solveSystem(L, D, Lt, b);

    unsigned int time2 = clock();
    // Вычисление относительной погрешности
    double relative_error = computeRelativeError(x, x_approx);

    // Вывод результатов
    cout << "First 5 coordinates of accurate solution: ";
    for (int i = 0; i < 5; ++i) cout << x[i] << " ";
    cout << endl;

    cout << "last 5 coordinates of accurate solution: ";
    for (int i = n - 5; i < n; ++i) cout << x[i] << " ";
    cout << endl;

    cout << "First 5 coordinates of approximate solution: ";
    for (int i = 0; i < 5; ++i) cout << std::setprecision(16) << x_approx[i] << " ";
    cout << endl;

    cout << "Last 5 coordinates of approximate solution: ";
    for (int i = n - 5; i < n; ++i) cout << std::setprecision(16) << x_approx[i] << " ";
    cout << endl;

    cout << "Inaccuracy: " << relative_error << endl;

    cout << "Time: "  << (double) (time2 - time1)/1000 << " sec" << endl;

    return 0;
}