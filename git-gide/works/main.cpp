// main.cpp — точка входа в программу
// Этот файл только вызывает логику из других модулей.
// Принцип: main() должен быть коротким и читаемым.

#include "greeter.h"
#include "math_utils.h"
#include <iostream>

int main() {
    // Приветствие из модуля greeter
    Greeter greeter("Мир");
    greeter.sayHello();

    // Математические утилиты из модуля math_utils
    int a = 6, b = 7;
    std::cout << "Произведение " << a << " × " << b
              << " = " << multiply(a, b) << std::endl;

    return 0;
}
