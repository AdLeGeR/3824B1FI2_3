#pragma once
#include <string>
#include <cctype>
#include <stdexcept>
#include <map>
#include <cmath>
#include "TStack.h"

using std::string;
using std::isdigit;
using std::isalpha;
using std::runtime_error;
using std::to_string;

bool IsHigherPrecedence(char op1, char op2);

bool IsOperator(char c);

enum Lexems {
    NUMBER = 1,
    OPERATOR
};

class TPostfix {
    string infix;
    string postfix;

public:
    TPostfix(string infix_);

    const string& GetPostfix() const;

    void ToPostfix();

    double Evaluate(const std::map<char, double>& vars) const;
};