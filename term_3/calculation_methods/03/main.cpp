#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

const int N = 999;
const int m = 4;

int random(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Метод прогонки для решения трёхдиагональной системы
std::vector<double> solve_tridiagonal(const std::vector<double>& a,
                                      const std::vector<double>& b,
                                      const std::vector<double>& c,
                                      const std::vector<double>& f) {
    std::vector<double> alpha(N+1), beta(N+1), y(N+1);

    // Прямой ход
    alpha[0] = -b[0] / c[0];
    beta[0] = f[0] / c[0];

    for (int i = 1; i <= N; ++i) {
        double denominator = c[i] + a[i] * alpha[i - 1];
        alpha[i] = -b[i] / denominator;
        beta[i] = (f[i] - a[i] * beta[i - 1]) / denominator;
    }

    // Обратный ход
    y[N] = beta[N];
    for (int i = N-1; i >= 0; --i) {
        y[i] = alpha[i] * y[i + 1] + beta[i];
    }

    return y;
}

int main() {
    srand(time(0));

    // Генерация случайных данных
    std::vector<double> a(N+1), b(N+1), c(N+1), f(N+1), y_exact(N+1);

    for (int i = 1; i <= N; ++i) {
        a[i] = random(-100, 100);
        b[i-1] = random(-100, 100);
    }

    // Задание точного решения
    for (int i = 0; i <= N; ++i) {
        y_exact[i] = i + 1;
    }

    // Генерация главной диагонали c и правой части f
    for (int i = 0; i < N; ++i) {
        c[i] = random(abs(a[i]) + abs(b[i]) + m, abs(a[i]) + abs(b[i]) + 2*m);
        f[i] = (i > 0 ? a[i] * y_exact[i-1] : 0) + c[i] * y_exact[i] + b[i] * y_exact[i+1];
    }

    c[0] = random(abs(b[0]) + m, abs(b[0]) + 2*m);
    f[0] = c[0] * y_exact[0] + b[0] * y_exact[1];

    c[N] = random(abs(a[N]) + m, abs(a[N]) + 2*m);
    f[N] = a[N] * y_exact[N-1] + c[N] * y_exact[N];

    // Решение системы
    std::vector<double> y = solve_tridiagonal(a, b, c, f);

    // Вывод решения
    std::cout << "Точное решение: ";
    for (int i = 0; i <= 5; ++i) {
        std::cout << y_exact[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Вычисленное решение: ";
    for (int i = 0; i <= 5; ++i) {
        std::cout << std::setprecision(16) << y[i] << " ";
    }
    std::cout << std::endl;

    //Норма векторов и вычисление погрешности
    double norm = fabs(y_exact[0]);
    double norm1 = fabs(y_exact[0] - y[0]);
    for (int i = 0; i <= N; ++i) {
        if (abs(y_exact[i]) > norm)
            norm = fabs(y_exact[i]);
        if (abs(y_exact[i] - y[i]) > norm1)
            norm1 = fabs(y_exact[i] - y[i]);
    }
    std::cout << "Inaccuracy: " << std::setprecision(20) << norm1/norm << "\n";

    return 0;
}
