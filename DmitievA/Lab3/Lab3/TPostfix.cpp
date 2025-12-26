#pragma once
#include <string>
#include <cctype>
#include <stdexcept>
#include <map>
#include <cmath>
#include <vector>
#include "TStack.h"
#include "TPostfix.h"

using std::string;
using std::isdigit;
using std::isalpha;
using std::runtime_error;
using std::to_string;
using std::map;
using std::cin;
using std::ostream;
using std::cout;

string GetStrLexem(int lexem) {
    switch (lexem) {
	case NONE: return "NONE";
	case NUMBER: return "NUMBER";
	case OPERATOR: return "OPERATOR";
	case VARIABLE: return "VARIABLE";
	case OPBRACKET: return "OPENING BRACKET";
	case CLBRACKET: return "CLOSING BRACKET";
    }
}


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
        string current_lexem;
        TStack<char> ops(100);
        bool expectOperand = true;
		//int current_lexem = NONE;
		int last_lexem = NONE;

        for (size_t i = 0; i < infix.size();) {

            // пропуск пробелов
            if (infix[i] == ' ') {
                i++; continue; 
            }

            // обработка цифр
            if (isdigit(infix[i])) {
                if (last_lexem != OPERATOR && last_lexem!=OPBRACKET && last_lexem!=NONE) {
                    throw runtime_error(GetStrLexem(last_lexem) + " before NUMBER at position " + to_string(i));
                }
                current_lexem = infix[i++];
                while (i < infix.size() &&
                    (isdigit(infix[i]) || infix[i] == '.')) {
                    current_lexem += infix[i++];
                }
                postfix += current_lexem + ' ';
				last_lexem = NUMBER;
                continue;
            }

            // переменна€
            if (isalpha(infix[i])) {

                if (i + 1 < infix.size() && infix.substr(i, 3) == "ln(") {
                    ops.Push('#');      // специальный маркер функции ln
                    ops.Push('(');
                    i += 3;
                    last_lexem = OPBRACKET;
                    continue;
                }
                if (last_lexem != OPERATOR && last_lexem!= OPBRACKET && last_lexem!=NONE) {
                    throw runtime_error(GetStrLexem(last_lexem) + " before VARIABLE at position " + to_string(i));
                }

                current_lexem.clear();
                while (i < infix.size() && isalpha(infix[i])){
					current_lexem += infix[i++];
                }
                last_lexem = VARIABLE;
                vars[current_lexem] = 0;
				postfix += current_lexem + ' ';
                continue;
            }

            // открывающа€ скобка
            if (infix[i] == '(') {
                if (last_lexem == VARIABLE || last_lexem == NUMBER)
					throw runtime_error(GetStrLexem(last_lexem)+"before OPENING BRACKET at position " + to_string(i));
                ops.Push('(');
				last_lexem = OPBRACKET;
                i++;
                continue;
            }

            // закрывающа€ скобка
            if (infix[i] == ')') {
                if (last_lexem == OPERATOR || last_lexem == NONE)
                    throw runtime_error("OPERATOR before CLOSING BRACKET at position " + to_string(i));
                while (!ops.IsEmpty() && ops.Top() != '(') {
                    postfix += ops.Pop();
                    postfix += ' ';
                }
                if (ops.IsEmpty())
                    throw runtime_error("Inconsistent parentheses");

                ops.Pop(); // убрать '('

                // если сверху функци€ ln Ч вывести еЄ
                if (!ops.IsEmpty() && ops.Top() == '#') {
                    postfix += "ln() ";
                    ops.Pop();
                }
				last_lexem = CLBRACKET;
                i++;
                continue;
            }

            //  оператор
            if (IsOperator(infix[i])) {
                if (last_lexem==OPBRACKET || last_lexem == OPERATOR || last_lexem==NONE)
                    throw runtime_error(GetStrLexem(last_lexem) + " before OPERATOR at position " + to_string(i));
                while (!ops.IsEmpty() &&
                    ops.Top() != '(' &&
                    !IsHigherPrecedence(infix[i], ops.Top())) {
                    if (ops.Top() == '#') {
                        postfix += "ln() ";
                    }
                    else {
                        postfix += ops.Pop();
                    }
                    postfix += ' ';
                }
                ops.Push(infix[i]);
				last_lexem = OPERATOR;
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
        if (vars.empty()) {
			readVariables = true;
        }
    }

double TPostfix::Evaluate() {
        if (!readVariables)
			ReadVariables();
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
                token.clear();
				while (i < postfix.size() && postfix[i] != ' ') {
                    token += postfix[i++];
				}
                if (token == "ln()") {
                    if (st.IsEmpty()) throw runtime_error("Ln error");
                    double a = st.Pop();
                    st.Push(std::log(a));
                    continue;
                }
                char v = postfix[i++];
                auto it = vars.find(token);
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

void TPostfix::ReadVariables(istream& in, ostream& out) {
        for (auto& var : vars) {
            out << "Enter value for variable " << var.first << ": ";
            in >> var.second;
			out << std::endl;
        }
		readVariables = true;
}
