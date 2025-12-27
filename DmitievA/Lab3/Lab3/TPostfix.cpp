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
using std::invalid_argument;
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

int get_priority(char op) {
    if (op == '~') return 3;   // unary minus
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

bool IsHigherPrecedence(char op1, char op2) {
    
    return get_priority(op1) > get_priority(op2);
}


bool IsOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

Postfix::Postfix(string infix_, bool parse) : infix(infix_) { if (parse) ToPostfix(); }

const string& Postfix::GetPostfix() const { return postfix; }

void Postfix::ToPostfix() {
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
                    throw invalid_argument(GetStrLexem(last_lexem) + " before NUMBER at position " + to_string(i));
                }
                current_lexem = infix[i++];
                bool have_dot = false;
                while (i < infix.size() &&
                    (isdigit(infix[i]) || infix[i] == '.')) {
                    if (infix[i] == '.') {
                        if (have_dot)
                            throw invalid_argument("Double dot in number at position " + to_string(i));
                        have_dot = true;
                    }
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
                    throw invalid_argument(GetStrLexem(last_lexem) + " before VARIABLE at position " + to_string(i));
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
                if (last_lexem == VARIABLE || last_lexem == NUMBER || last_lexem == CLBRACKET)
					throw invalid_argument(GetStrLexem(last_lexem)+"before OPENING BRACKET at position " + to_string(i));
                ops.Push('(');
				last_lexem = OPBRACKET;
                i++;
                continue;
            }

            // закрывающа€ скобка
            if (infix[i] == ')') {
                if (last_lexem == OPERATOR || last_lexem == NONE)
                    throw invalid_argument("OPERATOR before CLOSING BRACKET at position " + to_string(i));
                while (!ops.IsEmpty() && ops.Top() != '(') {
                    postfix += ops.Pop();
                    postfix += ' ';
                }
                if (ops.IsEmpty())
                    throw invalid_argument("Inconsistent parentheses");

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
                if (infix[i] == '-' && (last_lexem == OPBRACKET || last_lexem == NONE)) {
                    char op = '~';                 // наш унарный минус
                    // права€ ассоциативность: не выталкиваем оператор с тем же приоритетом
                    while (!ops.IsEmpty() && ops.Top() != '(' && IsHigherPrecedence(op, ops.Top())) {
                        postfix += ops.Pop();
                        postfix += ' ';
                    }
                    ops.Push(op);
                    last_lexem = OPERATOR;
                    i++;
                    continue;
                }
                if (last_lexem==OPBRACKET || last_lexem == OPERATOR || last_lexem==NONE)
                    throw invalid_argument(GetStrLexem(last_lexem) + " before OPERATOR at position " + to_string(i));
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

            throw invalid_argument("Unknown symbol: " + string(1, infix[i]));
        }

        if (last_lexem == OPERATOR || last_lexem == OPBRACKET)
            throw invalid_argument("The equation ends with " + GetStrLexem(last_lexem));
        // сбрасываем остаток операторов
        while (!ops.IsEmpty()) {
            if (ops.Top() == '(')
                throw invalid_argument("Inconsistent parentheses");
            postfix += ops.Pop();
            postfix += ' ';
        }
        if (vars.empty()) {
			readVariables = true;
        }
    }

double Postfix::Evaluate() {
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
                    if (st.IsEmpty()) throw invalid_argument("Ln error");
                    double a = st.Pop();
                    st.Push(std::log(a));
                    continue;
                }
                char v = postfix[i++];
                auto it = vars.find(token);
                if (it == vars.end())
                    throw invalid_argument(string("The variable is not defined: ") + v);
                st.Push(it->second);
                continue;
            }

            

            // оператор
            char op = postfix[i++];
            if (op != '~') {
                if (st.Size() < 2)
                    throw invalid_argument("Not enough operands");

                double b = st.Pop();
                double a = st.Pop();

                switch (op) {
                case '+': st.Push(a + b); break;
                case '-': st.Push(a - b); break;
                case '*': st.Push(a * b); break;
                case '/': st.Push(a / b); break;
                default: throw invalid_argument("Unknown operator");
                }
            }
            else {
                if(st.Size() <1)
                    throw invalid_argument("Not enough operands");
                double a = st.Pop();
                st.Push(-a);
            }
            
        }

        if (st.Size() != 1)
            throw invalid_argument("Calculation error");

        return st.Pop();
    }

void Postfix::ReadVariables(istream& in, ostream& out) {
        for (auto& var : vars) {
            out << "Enter value for variable " << var.first << ": ";
            in >> var.second;
			out << std::endl;
        }
		readVariables = true;
}
