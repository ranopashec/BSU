#include <iostream>
#include <math.h>
#include <limits>
#include <string> // Added for string

using namespace std;

// Function for Вариант 7: integral of sqrt(x) / (2x - 21)
double f(double x) {
    if (x < 0) { // Handle cases where sqrt(x) is not real
        return numeric_limits<double>::quiet_NaN(); // Not a Number
    }
    return sqrt((9 - 2 * x) / (2.0 * x - 21.0));
}

double midpoint_rule(double a, double b, int N) {
    double h = (b - a) / N;
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
        double x_mid = a + (i + 0.5) * h;
        sum += f(x_mid);
    }
    return h * sum;
}

double simpson_rule(double a, double b, int N) {
    // Simpson's rule requires N to be even for the standard composite formula.
    // If N is odd, it's common to increase N by 1 to make it even, or use a combination
    // of Simpson's and another rule (e.g., trapezoidal) for the last interval.
    // For simplicity and standard implementation, we'll ensure N is even if it isn't.
    if (N % 2 != 0) {
        N += 1; // Make N even
    }

    double h = (b - a) / N;
    double sum = f(a) + f(b); // First and last terms

    for (int i = 1; i < N; i += 2) { // Odd indices (4*f(x_odd))
        sum += 4 * f(a + i * h);
    }
    for (int i = 2; i < N - 1; i += 2) { // Even indices (2*f(x_even))
        sum += 2 * f(a + i * h);
    }
    return (h / 3.0) * sum;
}

double runge_error(double Q_h, double Q_2h, int p) {
    int k = 2; // k is typically 2 for halving the step size
    return abs(Q_h - Q_2h) / (pow(k, p) - 1);
}

void integrate_with_accuracy(double a, double b,
                             double (*method)(double, double, int),
                             const string& method_name,
                             int p, double eps, double exact_value) { // Added exact_value
    int N = 2; // Start with N=2
    double prev_result, curr_result;
    double error_estimate;

    cout << "--- " << method_name << " ---\n";

    // Loop to refine the integral approximation until desired accuracy is met
    do {
        // Calculate integral with current N
        curr_result = method(a, b, N);

        // Calculate integral with N/2 (for Runge estimation), if N is not 2
        if (N > 2) {
            prev_result = method(a, b, N / 2);
            error_estimate = runge_error(curr_result, prev_result, p);
            cout << "N = " << N << ", Интеграл = " << curr_result << ", Ошибка по Рунге = " << error_estimate << "\n";
        } else { // For the first iteration, we don't have prev_result yet
            cout << "N = " << N << ", Интеграл = " << curr_result << "\n";
            // Initialize error_estimate to a value greater than eps to ensure the loop continues
            error_estimate = eps + 1.0;
        }

        if (N > numeric_limits<int>::max() / 2) { // Prevent overflow for N
            cout << "Превышен максимальный предел итераций, точность не достигнута.\n";
            break;
        }
        prev_result = curr_result;
        N *= 2; // Double N for the next iteration

    } while (error_estimate > eps);

    cout << "\nТребуемая точность достигнута для " << method_name << ".\n";
    cout << "Итоговое значение интеграла: " << prev_result << "\n"; // prev_result holds the last calculated curr_result
    cout << "Оценка погрешности: " << error_estimate << "\n";
    // Added the difference output
    cout << "Разность с точным значением: " << abs(prev_result - exact_value) << "\n\n";
}

int main() {
    setlocale(LC_ALL, "ru");
    // Parameters
    double a = 6.0;
    double b = 9.0;
    const double PI = acos(-1.0);
    double exact_value = PI; // Exact value
    double eps = 1e-7; // Desired accuracy

    cout << "Точное значение интеграла: " << exact_value << "\n\n";

    // The order of accuracy 'p' for midpoint rule is 2, for Simpson's rule it's 4.
    // The 'k' value in the table is 4, which refers to the order of accuracy for the methods,
    // not directly to the 'k' in the Runge error formula (which is usually 2 for halving step size).
    integrate_with_accuracy(a, b, midpoint_rule, "Метод средних прямоугольников", 2, eps, exact_value);
    integrate_with_accuracy(a, b, simpson_rule, "Метод Симпсона", 4, eps, exact_value);

    return 0;
}
