#pragma once
#include <string>
#include <cctype>
#include <stdexcept>
#include <map>
#include <cmath>
#include "TStack.h"
#include "TPostfix.h"

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


bool IsOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

TPostfix::TPostfix(string infix_) : infix(infix_) {}

const string& TPostfix::GetPostfix() const { return postfix; }

void TPostfix::ToPostfix() {
        postfix.clear();
        string current_number;
        TStack<char> ops(100);
        bool expectOperand = true;

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
                expectOperand = false;
                continue;
            }

            // переменна€
            if (isalpha(infix[i])) {

                // функци€ ln
                if (i + 1 < infix.size() && infix.substr(i, 2) == "ln") {
                    ops.Push('#');      // специальный маркер функции ln
                    i += 2;
                    continue;
                }

                postfix += infix[i++];
                postfix += ' ';
                expectOperand = false;
                continue;
            }

            // открывающа€ скобка
            if (infix[i] == '(') {
                ops.Push('(');
                expectOperand = true;
                i++;
                continue;
            }

            // закрывающа€ скобка
            if (infix[i] == ')') {
                while (!ops.IsEmpty() && ops.Top() != '(') {
                    postfix += ops.Pop();
                    postfix += ' ';
                }
                if (ops.IsEmpty())
                    throw runtime_error("Inconsistent parentheses");

                ops.Pop(); // убрать '('

                // если сверху функци€ ln Ч вывести еЄ
                if (!ops.IsEmpty() && ops.Top() == '#') {
                    postfix += "ln ";
                    ops.Pop();
                }
                expectOperand = false;
                i++;
                continue;
            }

            //  оператор
            if (IsOperator(infix[i])) {
                if (expectOperand)
                    throw runtime_error("ќжидалс€ операнд, найден оператор");
                while (!ops.IsEmpty() &&
                    ops.Top() != '(' &&
                    !IsHigherPrecedence(infix[i], ops.Top())) {
                    postfix += ops.Pop();
                    postfix += ' ';
                }
                ops.Push(infix[i]);
                expectOperand = true;
                i++;
                continue;
            }

            throw runtime_error("Unknown symbol: " + string(1, infix[i]));
        }

        // сбрасываем остаток операторов
        while (!ops.IsEmpty()) {
            if (ops.Top() == '(')
                throw runtime_error("Inconsistent parentheses");
            postfix += ops.Pop();
            postfix += ' ';
        }
    }

double TPostfix::Evaluate(const std::map<char, double>& vars) const {
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

            // переменна€
            if (isalpha(postfix[i])) {
                // функци€ ln
                if (i + 1 < postfix.size() && postfix.substr(i, 2) == "ln") {
                    i += 2;
                    if (st.IsEmpty()) throw runtime_error("Ln error");
                    double a = st.Pop();
                    st.Push(std::log(a));
                    continue;
                }
                char v = postfix[i++];
                auto it = vars.find(v);
                if (it == vars.end())
                    throw runtime_error(string("The variable is not defined: ") + v);
                st.Push(it->second);
                continue;
            }

            

            // оператор
            char op = postfix[i++];
            if (st.Size() < 2)
                throw runtime_error("Not enough operands");

            double b = st.Pop();
            double a = st.Pop();

            switch (op) {
            case '+': st.Push(a + b); break;
            case '-': st.Push(a - b); break;
            case '*': st.Push(a * b); break;
            case '/': st.Push(a / b); break;
            default: throw runtime_error("Unknown operator");
            }
        }

        if (st.Size() != 1)
            throw runtime_error("Calculation error");

        return st.Pop();
    }
