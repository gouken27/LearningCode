// greeter.h — объявление класса Greeter
// Заголовочный файл описывает ИНТЕРФЕЙС: что умеет класс.
// Реализация (КАК он это делает) — в greeter.cpp.

#pragma once          // защита от двойного включения
#include <string>

class Greeter {
public:
    // Конструктор принимает имя, которое нужно поприветствовать
    explicit Greeter(const std::string& name);

    // Выводит приветствие в стандартный поток вывода
    void sayHello() const;

private:
    std::string m_name;   // имя хранится внутри объекта
};
