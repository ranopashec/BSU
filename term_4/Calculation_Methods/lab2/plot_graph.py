import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

# Настройка шрифтов и стиля графиков
rcParams['font.family'] = 'serif'
rcParams['font.sans-serif'] = ['Times New Roman']
plt.style.use('seaborn-v0_8')  # Используем актуальное название стиля

def plot_functions():
    # Загрузка данных оригинальных функций
    f1_data = np.loadtxt('f1.txt')
    f2_data = np.loadtxt('f2.txt')
    
    # Создание графиков для f1(x) = x²cos(2x)
    plt.figure(figsize=(15, 10))
    ns = [3, 10, 20]
    
    for i, n in enumerate(ns, 1):
        # Загрузка данных интерполяции
        try:
            P1_data = np.loadtxt(f'P1_{n}.txt')
            C1_data = np.loadtxt(f'C1_{n}.txt')
        except FileNotFoundError as e:
            print(f"Ошибка: файл не найден - {e.filename}")
            continue
        
        # График для равноотстоящих узлов
        plt.subplot(2, 3, i)
        plt.plot(f1_data[:, 0], f1_data[:, 1], 'k-', label='f1(x) = x²cos(2x)', linewidth=2)
        plt.plot(P1_data[:, 0], P1_data[:, 1], 'r--', label=f'P1,{n} (равноуз.)', linewidth=1.5)
        plt.title(f'Равноотстоящие узлы, n={n}')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
        
        # График для чебышевских узлов
        plt.subplot(2, 3, i+3)
        plt.plot(f1_data[:, 0], f1_data[:, 1], 'k-', label='f1(x) = x²cos(2x)', linewidth=2)
        plt.plot(C1_data[:, 0], C1_data[:, 1], 'b-.', label=f'C1,{n} (Чебышёв)', linewidth=1.5)
        plt.title(f'Чебышёвские узлы, n={n}')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
    
    plt.tight_layout()
    plt.savefig('f1_interpolation.png', dpi=300)
    plt.show()
    
    # Создание графиков для f2(x) = 1 / (1 + 5x²)
    plt.figure(figsize=(15, 10))
    
    for i, n in enumerate(ns, 1):
        # Загрузка данных интерполяции
        try:
            P2_data = np.loadtxt(f'P2_{n}.txt')
            C2_data = np.loadtxt(f'C2_{n}.txt')
        except FileNotFoundError as e:
            print(f"Ошибка: файл не найден - {e.filename}")
            continue
        
        # График для равноотстоящих узлов
        plt.subplot(2, 3, i)
        plt.plot(f2_data[:, 0], f2_data[:, 1], 'k-', label='f2(x) = 1/(1+5x²)', linewidth=2)
        plt.plot(P2_data[:, 0], P2_data[:, 1], 'r--', label=f'P2,{n} (равноуз.)', linewidth=1.5)
        plt.title(f'Равноотстоящие узлы, n={n}')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
        
        # График для чебышевских узлов
        plt.subplot(2, 3, i+3)
        plt.plot(f2_data[:, 0], f2_data[:, 1], 'k-', label='f2(x) = 1/(1+5x²)', linewidth=2)
        plt.plot(C2_data[:, 0], C2_data[:, 1], 'b-.', label=f'C2,{n} (Чебышёв)', linewidth=1.5)
        plt.title(f'Чебышёвские узлы, n={n}')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.grid(True, linestyle='--', alpha=0.7)
        plt.legend()
    
    plt.tight_layout()
    plt.savefig('f2_interpolation.png', dpi=300)
    plt.show()

if __name__ == "__main__":
    plot_functions()