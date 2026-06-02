// greeter.cpp — реализация класса Greeter
// Включаем собственный заголовок первым (хорошая практика).

#include "greeter.h"
#include <iostream>

Greeter::Greeter(const std::string& name)
    : m_name(name)          // инициализация через список инициализаторов
{}

void Greeter::sayHello() const {
    std::cout << "Привет, " << m_name << "!" << std::endl;
}
