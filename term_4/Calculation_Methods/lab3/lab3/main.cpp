#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <locale> // For setlocale

using namespace std;

const double A = -3;
const double B = 3;
const int N = 16;


double f(double x)
{
    return pow(x, 2) * cos(2 * x);
}

// f'(x) = 2x*cos(2x) - 2x^2*sin(2x)
double f_prime(double x)
{
    return 2 * x * cos(2 * x) - 2 * pow(x, 2) * sin(2 * x);
}

// f''(x) = 2*cos(2x) - 4x*sin(2x) - 4x*sin(2x) - 4x^2*cos(2x)
// f''(x) = 2*cos(2x) - 8x*sin(2x) - 4x^2*cos(2x)
double f_double_prime(double x)
{
    return 2 * cos(2 * x) - 8 * x * sin(2 * x) - 4 * pow(x, 2) * cos(2 * x);
}

// Решение трёхдиагональной системы методом прогонки
// a - поддиагональ (a[0] не используется)
// c - главная диагональ
// b - наддиагональ (b[N-1] не используется)
// d - правая часть
vector<double> solveTridiagonal(const vector<double>& a,
                                const vector<double>& b,
                                const vector<double>& c,
                                const vector<double>& d)
{
    int n = d.size();
    vector<double> cp(n, 0), dp(n, 0), x(n, 0);

    // Прямая прогонка
    cp[0] = b[0] / c[0];
    dp[0] = d[0] / c[0];
    for (int i = 1; i < n; i++)
    {
        double m = c[i] - a[i] * cp[i - 1];
        if (fabs(m) < 1e-18) { // Check for near-zero pivot to prevent division by zero
            cerr << "Error: Division by zero or near-zero in tridiagonal solver at index " << i << endl;
            exit(1); // Exit with an error code
        }
        if (i < n - 1)
            cp[i] = b[i] / m;
        dp[i] = (d[i] - a[i] * dp[i - 1]) / m;
    }

    // Обратная прогонка
    x[n - 1] = dp[n - 1];
    for (int i = n - 2; i >= 0; i--)
        x[i] = dp[i] - cp[i] * x[i + 1];

    return x;
}

// Вычисление значения кубического сплайна S(x) в точке x
// Используется стандартная формула сплайна с узлами xm, значениями f_vals и вторыми производными M
double S(double x,
         double h,
         const vector<double>& M,
         const vector<double>& f_vals,
         const vector<double>& xm)
{
    // Находим интервал [xm[i], xm[i+1]], которому принадлежит x
    int i = 0;
    while (i < N - 1 && x > xm[i + 1])
        i++;

    // Ensure 'i' is within bounds for array access (especially for x very close to B)
    if (i == N - 1 && x == xm[N-1]) {
        i = N - 2; // If x is exactly the last point, use the previous interval
    }
    if (i < 0) i = 0; // Should not happen with current logic, but as a safeguard
    if (i >= N - 1) i = N - 2; // For x values that might slightly exceed B due to floating point

    // Standard formula for cubic spline on a uniform grid
    double A_term = M[i] * pow(xm[i + 1] - x, 3) / (6 * h);
    double B_term = M[i + 1] * pow(x - xm[i], 3) / (6 * h);
    double C_term = (f_vals[i] - M[i] * h * h / 6) * (xm[i + 1] - x) / h;
    double D_term = (f_vals[i + 1] - M[i + 1] * h * h / 6) * (x - xm[i]) / h;
    return A_term + B_term + C_term + D_term;
}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");

    // 1) Формируем узлы xm и значения функции f_vals
    double h = (B - A) / (N - 1);
    vector<double> xm(N), f_vals(N);
    for (int i = 0; i < N; i++)
    {
        xm[i] = A + i * h;
        f_vals[i] = f(xm[i]);
    }

    // 2) Составляем СЛАУ для нахождения вторых производных M[i] = S''(x_i)
    //    Граничные условия для варианта 7: S'(A) = f'(A) и S''(B) = f''(B)
    vector<double> a(N, 1), b(N, 1), c(N, 4), d(N, 0);

    // Левая граница (i = 0): S'(A) = f'(A)
    // Equation: 2*M[0] + M[1] = (6/h^2) * (f_vals[1] - f_vals[0] - h * f_prime(A))
    a[0] = 0; // No M[-1] term
    c[0] = 2; // Coefficient for M[0]
    b[0] = 1; // Coefficient for M[1]
    d[0] = (6.0 / (h * h)) * (f_vals[1] - f_vals[0] - h * f_prime(A));

    // Правая граница (i = N-1): S''(B) = f''(B)
    a[N - 1] = 0; // No M[N] term
    b[N - 1] = 0; // No M[N] term
    c[N - 1] = 1; // Coefficient for M[N-1]
    d[N - 1] = f_double_prime(B);

    // Внутренние узлы i = 1..N-2:
    // M[i-1] + 4*M[i] + M[i+1] = 6/h^2 * (f_vals[i+1] - 2*f_vals[i] + f_vals[i-1])
    // The coefficients a[i], b[i], c[i] for i = 1..N-2 are already 1, 1, 4 respectively from initialization
    for (int i = 1; i < N - 1; i++)
    {
        d[i] = 6 * ((f_vals[i + 1] - 2 * f_vals[i] + f_vals[i - 1]) / (h * h));
    }

    // 3) Решаем систему методом прогонки -> вектор M
    vector<double> M = solveTridiagonal(a, b, c, d);

    // 4) Проверим, что в узлах x_i сплайн действительно совпадает с f(x_i)
    //    (В идеале разница должна быть ~ 1e-14 для double)
    //    Выведем для наглядности в файл "nodes_check.txt"
    ofstream outNodes("nodes_check.txt");
    outNodes << fixed << setprecision(20);
    outNodes << "# i    x_i                  f(x_i)                   S(x_i)                   |S(x_i)-f(x_i)|\n";
    for (int i = 0; i < N; i++)
    {
        double s_val = S(xm[i], h, M, f_vals, xm);
        outNodes << i << " " << xm[i] << " " << f_vals[i] << " " << s_val << " "
                 << fabs(s_val - f_vals[i]) << "\n";
    }
    outNodes.close();

    // 5) Вычислим значения сплайна и функции в 101 точке для оценки max ошибки
    double maxError = 0;
    int imax = -1;
    ofstream outSpline("spline.txt");
    ofstream outFunc("function.txt");
    outSpline << fixed << setprecision(20);
    outFunc << fixed << setprecision(20);

    for (int i = 0; i <= 100; i++)
    {
        double x = A + (B - A) * i / 100.0;
        double spline_val = S(x, h, M, f_vals, xm);
        double func_val = f(x);
        double error = fabs(spline_val - func_val);

        if (error > maxError)
        {
            maxError = error;
            imax = i;
        }
        outSpline << x << " " << spline_val << "\n";
        outFunc << x << " " << func_val << "\n";
    }
    outSpline.close();
    outFunc.close();

    // 6) Выводим итоговую информацию
    cout << "Максимальная ошибка на сетке из 101 точки: " << maxError
         << " в точке x = " << A + (B - A) * imax / 100.0 << endl;
    cout << "Проверка совпадения сплайна и функции в узлах см. в файле nodes_check.txt\n";
    cout << "Значения сплайна и функции на сетке (101 точка) см. в файлах spline.txt и function.txt\n";

    return 0;
}
