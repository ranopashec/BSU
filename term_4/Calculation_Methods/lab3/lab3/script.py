import numpy as np
import matplotlib.pyplot as plt

# Считываем данные из файлов
data_func = np.loadtxt('function.txt')
data_spline = np.loadtxt('spline.txt')

# Разделяем их на координаты X и Y
x_func, y_func = data_func[:, 0], data_func[:, 1]
x_spline, y_spline = data_spline[:, 0], data_spline[:, 1]

# Строим график исходной функции и график сплайна
plt.plot(x_func, y_func, label='f(x) = x^3 * cos(3x - 1)')
plt.plot(x_spline, y_spline, '--', label='Кубический сплайн')

# Настройка оформления
plt.title('Сравнение функции и сплайна')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.grid()

# Отображаем график
plt.show()
