#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

// Генерация случайной симметричной матрицы
void generateSymmetricMatrix(vector<vector<double>>& A, int n) {
    srand(time(0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            A[i][j] = (rand() % 201 - 100); // элементы в диапазоне от -100 до 100
            A[j][i] = A[i][j]; // симметрия
        }
    }
}

// Вычисление евклидовой нормы вектора
double euclideanNorm(const vector<double>& v) {
    double norm = 0.0;
    for (double val : v) {
        norm += val * val;
    }
    return sqrt(norm);
}

// Умножение матрицы на вектор
vector<double> multiplyMatrixVector(const vector<vector<double>>& A, const vector<double>& v) {
    int n = v.size();
    vector<double> result(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i] += A[i][j] * v[j];
        }
    }
    return result;
}

// Метод Якоби для нахождения всех собственных значений
void jacobiMethod(vector<vector<double>> A, int n, double epsilon) {
    int iteration = 0;
    double offA = 0.0;

    // Вычисление суммы квадратов внедиагональных элементов
    auto calculateOff = [&]() -> double {
        double sum = 0.0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    sum += A[i][j] * A[i][j];
                }
            }
        }
        return sum;
    };

    offA = calculateOff();

    while (offA > epsilon) {
        // Нахождение наибольшего внедиагонального элемента
        int p = 0, q = 1;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (fabs(A[i][j]) > fabs(A[p][q])) {
                    p = i;
                    q = j;
                }
            }
        }

        // Вычисление угла поворота
        double phi = 0.5 * atan2(2 * A[p][q], A[p][p] - A[q][q]);

        // Обновление матрицы
        vector<vector<double>> R = A;
        for (int i = 0; i < n; ++i) {
            if (i != p && i != q) {
                R[i][p] = A[i][p] * cos(phi) + A[i][q] * sin(phi);
                R[p][i] = R[i][p];
                R[i][q] = A[i][q] * cos(phi) - A[i][p] * sin(phi);
                R[q][i] = R[i][q];
            }
        }
        R[p][p] = A[p][p] * cos(phi) * cos(phi) + 2 * A[p][q] * cos(phi) * sin(phi) + A[q][q] * sin(phi) * sin(phi);
        R[q][q] = A[p][p] * sin(phi) * sin(phi) - 2 * A[p][q] * cos(phi) * sin(phi) + A[q][q] * cos(phi) * cos(phi);
        R[p][q] = 0.0;
        R[q][p] = 0.0;

        A = R;
        offA = calculateOff();
        ++iteration;
    }

    // Вывод результатов
    cout << "Метод Якоби завершен за " << iteration << " итераций." << endl;
    cout << "Приближенные собственные значения: ";
    for (int i = 0; i < n; ++i) {
        cout << A[i][i] << " ";
    }
    cout << endl;
    cout << "Сумма квадратов внедиагональных элементов: " << offA << endl;
}

// Скалярный метод для нахождения наибольшего собственного значения
void scalarProductMethod(vector<vector<double>> A, int n, double epsilon) {
    // Начальный ненулевой вектор y^(0)
    vector<double> y(n, 0.0); // любой ненулевой вектор может быть использован
    y[1] = 1;
    vector<double> w(n, 0.0);
    // Вывод результатов
    cout << "Начальный вектор y^(0): ";
    for (double val : y) {
        cout << val << " ";
    }
    double lambda = 0.0; // приближенное собственное значение
    double prevLambda = 0.0;
    int iteration = 0;

    // Итерационный процесс
    while (true) {
        // Умножение матрицы на вектор
        w = multiplyMatrixVector(A, y);

        // Вычисление приближенного собственного значения
        lambda = 0.0;
        for (int i = 0; i < n; ++i) {
            lambda += w[i] * y[i];
        }

        // Нормализация вектора
        double norm = euclideanNorm(w);
        for (int i = 0; i < n; ++i) {
            y[i] = w[i] / norm;
        }

        ++iteration;

        // Проверка критерия остановки
        vector<double> Au = multiplyMatrixVector(A, y);
        vector<double> residual(n, 0.0);
        for (int i = 0; i < n; ++i) {
            residual[i] = Au[i] - lambda * y[i];
        }
        double error = euclideanNorm(residual);
        if (error < epsilon)
            break;
    }

    // Вычисление ошибки ||Au - λu||
    vector<double> Au = multiplyMatrixVector(A, y);
    vector<double> residual(n, 0.0);
    for (int i = 0; i < n; ++i) {
        residual[i] = Au[i] - lambda * y[i];
    }
    double error = euclideanNorm(residual);

    cout << endl;

    cout << "Количество итераций: " << iteration << endl;
    cout << "Приближенное наибольшее собственное значение lambda1: " << lambda << endl;

    cout << "Вектор Au - λu: ";
    for (double val : residual) {
        cout << val << " ";
    }
    cout << endl;

    cout << "Норма || Au - λu||: " << error << endl;
}

int main() {
    const int n = 20; // размер матрицы
    const double epsilon = 1e-7; // требуемая точность

    // Инициализация симметричной матрицы
    vector<vector<double>> A(n, vector<double>(n, 0.0));
    generateSymmetricMatrix(A, n);
    scalarProductMethod(A, n, epsilon);
    cout << "\n_____________________________" << endl;
    jacobiMethod(A, n, epsilon);
    return 0;
}
