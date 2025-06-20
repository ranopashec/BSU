import numpy as np
from sympy import symbols, integrate, roots, sqrt, Poly
import math

def build_gauss_rule(f, a, b, k):
    # ... (rest of the build_gauss_rule function remains the same)
    x = symbols('x') 
    M = [] 
    B = [] 

    for m in range(k):
        row = []
        for i in range(k):
            term = integrate(x**m * x**(k - i - 1), (x, a, b)).evalf()
            row.append(float(term))
        
        rhs = -integrate(x**m * x**k, (x, a, b)).evalf()
        B.append(float(rhs))
        M.append(row)

    try:
        sol = np.linalg.solve(np.array(M, dtype=np.float64), np.array(B, dtype=np.float64))
    except np.linalg.LinAlgError:
        raise ValueError("Не удалось решить систему уравнений для коэффициентов многочлена.")

    w_k = x**k + sum(sol[i] * x**(k - i - 1) for i in range(k))
    
    node_roots = Poly(w_k, x).all_roots() 
    
    nodes = []
    for r in node_roots:
        if r.is_real and a <= r.evalf() <= b:
            nodes.append(float(r.evalf()))
    
    nodes = sorted(nodes) 

    if len(nodes) != k:
        raise ValueError(f"Не найдено ровно {k} узлов в интервале [{a}, {b}]. Найдено: {len(nodes)}. "
                         f"Корни: {[str(r.evalf()) for r in node_roots]}")

    A = []
    for i in range(k):
        L_i = 1 
        for j in range(k):
            if i != j:
                L_i *= (x - nodes[j]) / (nodes[i] - nodes[j])
        
        Ai = integrate(L_i, (x, a, b)).evalf()
        A.append(float(Ai))

    result = sum(A[i] * f(nodes[i]) for i in range(k))
    return result


if __name__ == "__main__":
    def integrand(x_val):
        numerator = 9.0 - 2.0 * x_val
        denominator = 2.0 * x_val - 21.0
        
        if abs(denominator) < 1e-15:
            return np.nan
        
        arg_sqrt = numerator / denominator
        if arg_sqrt < 0:
            return np.nan
            
        return np.sqrt(arg_sqrt)

    a, b = 6.0, 9.0
    k = 6 

    print(f"--- Вычисление интеграла с помощью КФ НАСТ (Гаусса) с {k} узлами ---")
    
    try:
        approx_value = build_gauss_rule(integrand, a, b, k)
        print(f"Приближенное значение интеграла (КФ НАСТ, k={k}): {approx_value:.10f}")

        # This line was causing the error because math was not imported
        exact_value = math.pi 
        print(f"Точное значение из таблицы (I = pi): {exact_value:.10f}")
        
        difference = abs(approx_value - exact_value)
        print(f"Разница между приближенным и точным значением: {difference:.10e}")

    except ValueError as e:
        print(f"Ошибка при вычислении КФ НАСТ: {e}")
    except Exception as e:
        print(f"Произошла непредвиденная ошибка: {e}")
