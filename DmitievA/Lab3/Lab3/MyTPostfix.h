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

bool IsHigherPrecedence(char op1, char op2) {
    if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-'))
        return true;
    return false;
}

enum Lexems {
    NUMBER = 1,
    OPERATOR
};

class MyTPostfix {
    string infix;
    string postfix;

    bool IsOperator(char c) const {
        return c == '+' || c == '-' || c == '*' || c == '/';
    }

public:
    MyTPostfix(string infix_) : infix(infix_) {}

    const string& GetPostfix() const { return postfix; }

    void ToPostfix() {
        postfix.clear();
        string current_number;
        TStack<char> ops(100);

        for (size_t i = 0; i < infix.size();) {

            // пропуск пробелов
            if (infix[i] == ' ') { i++; continue; }

            // обработка цифр
            if (isdigit(infix[i])) {
                current_number.clear();
                while (i < infix.size() &&
                    (isdigit(infix[i]) || infix[i] == '.')) {
                    current_number += infix[i++];
                }
                postfix += current_number + ' ';
                continue;
            }

            // переменная
            if (isalpha(infix[i])) {

                // функция ln(...)
                if (i + 1 < infix.size() && infix.substr(i, 2) == "ln") {
                    ops.Push('#');      // специальный маркер функции ln
                    i += 2;
                    continue;
                }

                postfix += infix[i++];
                postfix += ' ';
                continue;
            }

            // ---- открывающая скобка ----
            if (infix[i] == '(') {
                ops.Push('(');
                i++;
                continue;
            }

            // ---- закрывающая скобка ----
            if (infix[i] == ')') {
                while (!ops.IsEmpty() && ops.Top() != '(') {
                    postfix += ops.Pop();
                    postfix += ' ';
                }
                if (ops.IsEmpty())
                    throw runtime_error("Несогласованные скобки");

                ops.Pop(); // убрать '('

                // если сверху функция ln — вывести её
                if (!ops.IsEmpty() && ops.Top() == '#') {
                    postfix += "ln ";
                    ops.Pop();
                }

                i++;
                continue;
            }

            //  оператор
            if (IsOperator(infix[i])) {
                while (!ops.IsEmpty() &&
                    ops.Top() != '(' &&
                    !IsHigherPrecedence(infix[i], ops.Top())) {
                    postfix += ops.Pop();
                    postfix += ' ';
                }
                ops.Push(infix[i]);
                i++;
                continue;
            }

            throw runtime_error("Неизвестный символ: " + string(1, infix[i]));
        }

        // сбрасываем остаток операторов
        while (!ops.IsEmpty()) {
            if (ops.Top() == '(')
                throw runtime_error("Несогласованные скобки");
            postfix += ops.Pop();
            postfix += ' ';
        }
    }

    double Evaluate(const std::map<char, double>& vars) const {
        TStack<double> st(100);
        string token;

        for (size_t i = 0; i < postfix.size();) {

            if (postfix[i] == ' ') { i++; continue; }

            // число
            if (isdigit(postfix[i])) {
                token.clear();
                while (i < postfix.size() &&
                    (isdigit(postfix[i]) || postfix[i] == '.')) {
                    token += postfix[i++];
                }
                st.Push(std::stod(token));
                continue;
            }

            // переменная
            if (isalpha(postfix[i])) {
                char v = postfix[i++];
                auto it = vars.find(v);
                if (it == vars.end())
                    throw runtime_error(string("Переменная не определена: ") + v);
                st.Push(it->second);
                continue;
            }

            // функция ln
            if (postfix.substr(i, 2) == "ln") {
                i += 2;
                if (st.IsEmpty()) throw runtime_error("Ошибка ln");
                double a = st.Pop();
                st.Push(std::log(a));
                continue;
            }

            // оператор
            char op = postfix[i++];
            if (st.Size() < 2)
                throw runtime_error("Недостаточно операндов");

            double b = st.Pop();
            double a = st.Pop();

            switch (op) {
            case '+': st.Push(a + b); break;
            case '-': st.Push(a - b); break;
            case '*': st.Push(a * b); break;
            case '/': st.Push(a / b); break;
            default: throw runtime_error("Неизвестный оператор");
            }
        }

        if (st.Size() != 1)
            throw runtime_error("Ошибка вычисления");

        return st.Pop();
    }
};
