#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

// Точное решение для варианта 7: u(x) = x / (2 - ln(x))
double exactSolution(double x) {
    return x / (2 - std::log(x));
}

// Функция f(x, u) для варианта 7: u' = (u^2 + ux) / x^2
double f(double x, double u) {
    return (u * u + u * x) / (x * x);
}

// Частная производная f по u, df/du, для варианта 7
// f(x, u) = u^2/x^2 + u/x
// df/du = 2u/x^2 + 1/x
double df_du(double x, double u) {
    return 2 * u / (x * x) + 1.0 / x;
}

// Метод Ньютона для решения неявного уравнения в неявном методе трапеций
double newtonMethod(double x_prev, double u_prev, double h) {
    double u_curr = u_prev; // Начальное предположение для u_curr
    const double tol = 1e-8; // Допуск (точность)
    const int max_iter = 100; // Максимальное количество итераций

    for (int iter = 0; iter < max_iter; ++iter) {
        double x_curr = x_prev + h;
        double f_prev = f(x_prev, u_prev);
        double f_curr_guess = f(x_curr, u_curr);
        
        // Неявное уравнение, которое нужно решить: F(u_curr) = u_curr - u_prev - h/2 * (f(x_prev, u_prev) + f(x_curr, u_curr)) = 0
        double F_u_curr = u_curr - u_prev - h / 2.0 * (f_prev + f_curr_guess);
        
        // Производная F(u_curr) по u_curr: dF/du_curr = 1 - h/2 * df/du(x_curr, u_curr)
        double dF_du_curr = 1.0 - h / 2.0 * df_du(x_curr, u_curr);

        double delta_u = F_u_curr / dF_du_curr;
        u_curr -= delta_u;

        if (std::abs(delta_u) < tol) {
            break; // Если изменение достаточно мало, выходим из цикла
        }
    }
    return u_curr;
}

// Неявный метод трапеций
std::vector<double> implicitTrapezoidal(double a, double b, double h, double u0) {
    int n = static_cast<int>((b - a) / h); // Количество шагов
    std::vector<double> u(n + 1);
    u[0] = u0; // Начальное условие
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        u[i + 1] = newtonMethod(x, u[i], h); // Вычисляем следующее значение u с помощью метода Ньютона
    }
    return u;
}

// Метод Рунге-Кутты 3-го порядка (на основе таблицы для варианта 7)
// Коэффициенты для метода Рунге-Кутты из таблицы (p=3, вариант 7):
// k1 = f(x_n, u_n)
// k2 = f(x_n + h/2, u_n + h/2 * k1)
// k3 = f(x_n + 3/4*h, u_n + 3/4*h*k2)
// u_{n+1} = u_n + h * (2/9 * k1 + 1/3 * k2 + 4/9 * k3)
std::vector<double> rungeKutta3(double a, double b, double h, double u0) {
    int n = static_cast<int>((b - a) / h); // Количество шагов
    std::vector<double> u(n + 1);
    u[0] = u0; // Начальное условие
    for (int i = 0; i < n; ++i) {
        double x = a + i * h;
        double k1 = f(x, u[i]);
        double k2 = f(x + h * 0.5, u[i] + h * 0.5 * k1);
        double k3 = f(x + h * 0.75, u[i] + h * 0.75 * k2);
                                                         
        u[i + 1] = u[i] + h * (2.0 / 9.0 * k1 + 1.0 / 3.0 * k2 + 4.0 / 9.0 * k3);
    }
    return u;
}

// Вычисление максимальной ошибки (разности между точным и численным решениями)
double computeMaxError(const std::vector<double>& x_values, const std::vector<double>& y_values) {
    double max_error = 0.0;
    for (size_t i = 0; i < x_values.size(); ++i) {
        double error = std::abs(exactSolution(x_values[i]) - y_values[i]);
        if (error > max_error) {
            max_error = error;
        }
    }
    return max_error;
}

// Функция для печати результатов метода
void printResults(const std::string& methodName, double h1, double h2, double error_h1, double error_h2, double p) {
    double R = std::abs(error_h1 - error_h2) / (std::pow(2, p) - 1.0);
    std::cout << "Результаты для " << methodName << ":" << std::endl;
    std::cout << "Шаг h1 = " << h1 << ", Максимальная ошибка = " << error_h1 << std::endl;
    std::cout << "Шаг h2 = " << h2 << ", Максимальная ошибка = " << error_h2 << std::endl;
    std::cout << "Оценка по правилу Рунге (для h2): " << R << std::endl;
    std::cout << std::endl;
}

int main() {
    setlocale(LC_ALL, "ru"); // Установка локали для корректного вывода русских символов
    
    // Параметры для варианта 7
    double a = 1.0, b = 2.0; // Диапазон x
    double h1 = 0.1; // Первый размер шага
    double h2 = h1 / 2.0; // Второй размер шага (в два раза меньше)
    double u0 = 0.5; // Начальное условие u(1) = 0.5

    // Создание векторов значений x для обоих размеров шага
    int n1 = static_cast<int>((b - a) / h1);
    int n2 = static_cast<int>((b - a) / h2);

    std::vector<double> x_values_h1(n1 + 1);
    std::vector<double> x_values_h2(n2 + 1);
    for (int i = 0; i <= n1; ++i) {
        x_values_h1[i] = a + i * h1;
    }
    for (int i = 0; i <= n2; ++i) {
        x_values_h2[i] = a + i * h2;
    }

    std::cout << std::fixed << std::setprecision(10); // Установка точности вывода

    // Расчеты и вывод для неявного метода трапеций
    std::vector<double> u_trap_h1 = implicitTrapezoidal(a, b, h1, u0);
    std::vector<double> u_trap_h2 = implicitTrapezoidal(a, b, h2, u0);
    double error_trap_h1 = computeMaxError(x_values_h1, u_trap_h1);
    double error_trap_h2 = computeMaxError(x_values_h2, u_trap_h2);
    printResults("неявного метода трапеций", h1, h2, error_trap_h1, error_trap_h2, 2.0);

    // Расчеты и вывод для метода Рунге-Кутты 3-го порядка
    std::vector<double> u_rk_h1 = rungeKutta3(a, b, h1, u0);
    std::vector<double> u_rk_h2 = rungeKutta3(a, b, h2, u0);
    double error_rk_h1 = computeMaxError(x_values_h1, u_rk_h1);
    double error_rk_h2 = computeMaxError(x_values_h2, u_rk_h2);
    printResults("метода Рунге-Кутты (порядок 3)", h1, h2, error_rk_h1, error_rk_h2, 3.0);

    return 0;
}
