#pragma once
#include <string>
#include <map>
#include "TStack.h"

class TPostfix {
private:
    std::string infix;     // исходное выражение
    std::string postfix;   // постфиксная форма

    bool IsLetter(char c) const;
    bool IsDigit(char c) const;
    bool IsOperator(char c) const;
    int  Priority(const std::string& op) const;

    void CheckExpression() const;   // проверка корректности
    void ToPostfixInternal();       // преобразование в постфикс

    void AddToken(const std::string& tok); //добавление элементов в постфикс (операторы и операнды)

public:
    TPostfix(const std::string& expr = "");

    const std::string& GetInfix() const;
    const std::string& GetPostfix() const;

    void ToPostfix();               // публичный вызов
    double Calculate();             // вычисление постфиксного выражения
};