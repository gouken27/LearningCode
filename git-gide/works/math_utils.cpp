// math_utils.cpp — реализация математических утилит

#include "math_utils.h"

int multiply(int a, int b) {
    return a * b;
}

long long factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n - 1);   // рекурсия
}
