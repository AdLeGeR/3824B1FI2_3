#pragma once
#include <string>
#include <map>
#include <iostream>
#include "TStack.h"

using std::string;
using std::map;
using std::istream;
using std::ostream;
using std::cin;
using std::cout;

bool IsHigherPrecedence(char op1, char op2);

bool IsOperator(char c);

enum Lexems {
	NONE = 0,
    NUMBER = 1,
    OPERATOR,
	VARIABLE,
	OPBRACKET,
	CLBRACKET,
};

string GetStrLexem(int lexem);

class TPostfix {
    string infix;
    string postfix;
    map<string, double> vars;
	bool readVariables = false;

public:
    TPostfix(string infix_);

    const string& GetPostfix() const;

    void ToPostfix();

    double Evaluate();

	void ReadVariables(istream& in=cin, ostream& out = cout);
};