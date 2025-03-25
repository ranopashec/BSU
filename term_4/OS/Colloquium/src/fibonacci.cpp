//
// Created by Simon on 25/03/2025.
//

#include "../include/fibonacci.h"
#include <stdexcept>

std::vector<int> generateFibonacci(int n) {
    if (n <= 0) {
        throw std::invalid_argument("n должно быть натуральным числом");
    }

    std::vector<int> fibonacci;
    fibonacci.reserve(n);

    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        fibonacci.push_back(a);
        int next = a + b;
        a = b;
        b = next;
    }

    return fibonacci;
}

