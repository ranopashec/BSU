#include <iostream>
#include <cmath>
#include <vector>

// Функция для вычисления конечных разностей
double finiteDifference(const std::vector<double>& y, int order, int index) {
    if (order == 0) {
        return y[index];
    } else {
        if (index + order >= y.size()) {
            throw std::invalid_argument("Index out of range");
        }
        return finiteDifference(y, order - 1, index + 1) - finiteDifference(y, order - 1, index);
    }
}

// Функция для вычисления разделённых разностей
double dividedDifference(const std::vector<double>& x, const std::vector<double>& y, int order, int index) {
    if (order == 0) {
        return y[index];
    } else {
        if (index + order >= x.size() || index + order >= y.size()) {
            throw std::invalid_argument("Index out of range");
        }
        return (dividedDifference(x, y, order - 1, index + 1) - dividedDifference(x, y, order - 1, index)) / (x[index + order] - x[index]);
    }
}

// Функция для построения интерполяционного многочлена Ньютона на равноотстоящих узлах
double newtonPolynomialUniform(const std::vector<double>& x, const std::vector<double>& y, double xValue) {
    double result = y[0];
    double h = x[1] - x[0];
    for (int i = 1; i < y.size(); ++i) {
        double term = finiteDifference(y, i, 0) / (pow(h, i) * tgamma(i + 1));
        double product = 1.0;
        for (int j = 0; j < i; ++j) {
            product *= (xValue - x[j]);
        }
        result += term * product;
    }
    return result;
}

// Функция для построения интерполяционного многочлена Ньютона на чебышёвских узлах
double newtonPolynomialChebyshev(const std::vector<double>& x, const std::vector<double>& y, double xValue) {
    double result = dividedDifference(x, y, 0, 0);
    for (int i = 1; i < y.size(); ++i) {
        double term = dividedDifference(x, y, i, 0);
        double product = 1.0;
        for (int j = 0; j < i; ++j) {
            product *= (xValue - x[j]);
        }
        result += term * product;
    }
    return result;
}

// Функция для вычисления гамма-функции (для конечных разностей)
double tgamma(double x) {
    if (x == 1 || x == 2) {
        return 1.0;
    } else if (x == 0.5) {
        return sqrt(M_PI);
    } else {
        return (x - 1) * tgamma(x - 1);
    }
}

int main() {
    // Пример использования для равноотстоящих узлов
    std::vector<double> xUniform = {0.0, 1.0, 2.0, 3.0};
    std::vector<double> yUniform = {1.0, 2.0, 3.0, 4.0};
    double xValueUniform = 1.5;
    std::cout << "Интерполяция на равноотстоящих узлах: " << newtonPolynomialUniform(xUniform, yUniform, xValueUniform) << std::endl;

    // Пример использования для чебышёвских узлов
    std::vector<double> xChebyshev = {cos(M_PI / 4), cos(M_PI / 2), cos(3 * M_PI / 4)};
    std::vector<double> yChebyshev = {1.0, 0.0, -1.0};
    double xValueChebyshev = 0.0;
    std::cout << "Интерполяция на чебышёвских узлах: " << newtonPolynomialChebyshev(xChebyshev, yChebyshev, xValueChebyshev) << std::endl;

    return 0;
}
