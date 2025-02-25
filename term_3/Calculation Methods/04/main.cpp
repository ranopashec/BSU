#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

int m = 4;
int n = 1000;

void generateMatrix(vector<vector<double>> &A) {
    srand(time(0));
    double sum = 0; //накопитель суммы модулей недиагональных элементов
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            A[i][j] = (rand() % 201) - 100; // Элементы в диапазоне [-100, 100]
            A[j][i] = A[i][j]; // Симметричное присваивание
            sum += abs(A[i][j]);
        }
        for (int j = 0;  j < i; ++j) {
            sum += abs(A[i][j]);
        }
        int end = sum + 10 * m;
        int start = sum + m;
        A[i][i] = (rand() % (end - start + 1) ) + start;
    }
    /*int end = sum + 10 * m;
    int start = sum + m;
    for (int i = 0; i < n; ++i) {
        A[i][i] = (rand() % (end - start + 1) ) + start;
    }*/
}

double computeRelativeError(const vector<double> &x, const vector<double> &x_approx) {
    double max_norm_diff = 0.0, max_norm_x = 0.0;

    for (int i = 0; i < x.size(); ++i) {
        max_norm_diff = max(max_norm_diff, abs(x[i] - x_approx[i]));
        max_norm_x = max(max_norm_x, abs(x[i]));
    }

    return max_norm_diff / max_norm_x;
}

double computeAbsoluteError(const vector<double> &x, const vector<double> &x_approx) {
    double max_norm_diff = 0.0;

    for (int i = 0; i < x.size(); ++i) {
        max_norm_diff = max(max_norm_diff, abs(x[i] - x_approx[i]));
    }

    return max_norm_diff;
}

void jacobi_method(vector<vector<double>>& A, vector<double>& b, vector<double>& x, double eps, int k_max,
    const vector<double>& x_acc) {
    int n = A.size();
    vector<double> x_new(n);
    int k = 0;
    double norm;
    cout << "_____________________________________________\n";
    cout << "Метод Якоби\n";
    cout << "Первые 5 координат приближенного x0: ";
    for (int i = 0; i < 5; ++i) cout << x[i] << " ";
    cout << endl;
    cout << "Последние 5 координат приближенного x0: ";
    for (int i = n - 5; i < n; ++i) cout << x[i] << " ";
    cout << endl;
    cout << "Первые 5 координат точного решения: ";
    for (int i = 0; i < 5; ++i) cout << x_acc[i] << " ";
    cout << endl;
    cout << "Последние 5 координат точного решения: ";
    for (int i = n - 5; i < n; ++i) cout << x_acc[i] << " ";
    cout << endl;
    while (k < k_max) {
        k++;
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < n; j++) {
                if (j != i) {
                    sum += A[i][j] * x[j];
                }
            }
            x_new[i] = (b[i] - sum) / A[i][i];

        }
        norm = computeRelativeError(x_new, x);
        if (norm <= eps) {
            break;
        }

        x = x_new;

    }

    cout << "Первые 5 координат приближенного решения: ";
    for (int i = 0; i < 5; ++i) cout << std::setprecision(16) << x[i] << " ";
    cout << endl;
    cout << "Последние 5 координат приближенного решения: ";
    for (int i = n - 5; i < n; ++i) cout << std::setprecision(16) << x[i] << " ";
    cout << endl;
    cout << "Количество итераций: " << k << endl;
    cout << "Относительная неточность: " << std::setprecision(16) << norm << endl;
    cout << "Абсолютная неточность: " << std::setprecision(16) <<
        computeAbsoluteError(x_acc, x) << endl;
}

// Метод релаксации
void gauss_seidel_method(vector<vector<double>>& A, vector<double>& b, vector<double>& x, double omega, double eps,
    int k_max, vector<double> x_acc) {
    int n = A.size();
    vector<double> x_new(n);
    int k = 0;
    double norm;
    cout << "_____________________________________________\n";
    cout << "Метод релаксации с omega = " << omega << ":\n";
    cout << "Первые 5 координат приближенного x0: ";
    for (int i = 0; i < 5; ++i) cout << x[i] << " ";
    cout << endl;
    cout << "Последние 5 координат приближенного x0: ";
    for (int i = n - 5; i < n; ++i) cout << x[i] << " ";
    cout << endl;
    cout << "Первые 5 координат точного решения: ";
    for (int i = 0; i < 5; ++i) cout << x_acc[i] << " ";
    cout << endl;
    cout << "Последние 5 координат точного решения: ";
    for (int i = n - 5; i < n; ++i) cout << x_acc[i] << " ";
    cout << endl;
    while (k < k_max) {
        k++;
        for (int i = 0; i < n; i++) {
            double sum1 = 0, sum2 = 0;
            for (int j = 0; j < i; j++) {
                sum1 += A[i][j] * x_new[j];
            }
            for (int j = i + 1; j < n; j++) {
                sum2 += A[i][j] * x[j];
            }
            x_new[i] = (1 - omega) * x[i] + (omega / A[i][i]) * (b[i] - sum1 - sum2);
        }

        norm = computeRelativeError(x_new, x);
        if (norm <= eps) {
            break;
        }

        x = x_new;

    }

    cout << "Первые 5 координат приближенного решения: ";
    for (int i = 0; i < 5; ++i) cout << std::setprecision(16) << x[i] << " ";
    cout << endl;

    cout << "Последние 5 координат приближенного решения: ";
    for (int i = n - 5; i < n; ++i) cout << std::setprecision(16) << x[i] << " ";
    cout << endl;
    cout << "Количество итераций: " << k << endl;
    cout << "Относительная неточность: " << std::setprecision(16) << norm << endl;
    cout << "Абсолютная неточность: " << std::setprecision(16) <<
        computeAbsoluteError(x_acc, x) << endl;
}

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

int main() {
    double eps = 1e-7;
    int k_max = 1000;

    vector<vector<double>> a(n, std::vector<double>(n)); //генерация решений и матрицы
    generateMatrix(a);
    vector<double> x(n); //точное решение
    for (int i = 0; i < n; ++i) {
        x[i] = m + i;
    }
    vector<double> b = multiplyMatrixVector(a, x);
    std::vector<double> x01(n, 0.0), x02(n, 0.0),
    x03(n, 0.0), x04(n, 0.0);

    jacobi_method(a, b, x01, eps, k_max, x);
    gauss_seidel_method(a, b, x02, 0.5, eps, k_max, x);
    gauss_seidel_method(a, b, x03, 1, eps, k_max, x);
    gauss_seidel_method(a, b, x04, 1.5, eps, k_max, x);

    return 0;
}
