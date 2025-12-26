#include "TPostfix.h"


void TPostfix::Parse() {
	TStack<char> brackets;
	std::string curr = "", prev = "";
	int i = 0;
	bool fist_unar_minus = false;
	while (i < infix.size()) {
		if (isspace(infix[i])) {
			++i;
			continue;
		}
		if (infix[i] == '(') {
			fist_unar_minus = false;
			if (prev != "cos" && prev != "~" && (isdigit(prev[0]) || isalpha(prev[0]))) {
				throw std::invalid_argument("Invalid input. Digit or operand befor opening parenthesis at position " + std::to_string(i));
			}
			else if (prev == "cos") {
				lexems.back() = "^";
			}
			if (!brackets.empty() && brackets.top() == ')') {
				throw std::invalid_argument("Invalid input. Wrong bracket sequence. Position: " + std::to_string(i + 1));
			}
			brackets.push(infix[i]);
			lexems.push_back("(");
			prev = "(";
			++i;
		}
		else if (infix[i] == ')') {
			fist_unar_minus = false;
			if (lexems.empty() || (!isdigit(prev[0]) && !isalpha(prev[0]))) {
				bool bad = true;
				int brackets = 1, ind = lexems.size() - 1;
				for (; ind >= 0; --ind) {
					if (brackets == 0 && lexems[ind] == "^") bad = false;
					if (lexems[ind] == ")") ++brackets;
					if (lexems[ind] == "(") --brackets;
				}
				if (bad) {
					throw std::invalid_argument("Invalid input. Expected digit or operand befor closing parenthesis at position " + std::to_string(i));
				}
			}
			if (brackets.empty()) {
				throw std::invalid_argument("Invalid input. Wrong bracket sequence. Position: " + std::to_string(i + 1));
			}
			brackets.pop();
			lexems.push_back(")");
			prev = ")";
			++i;
		}
		else if (isalpha(infix[i])) {
			fist_unar_minus = false;
			if (prev[0] == '^' || prev[0] == ')' || isalpha(prev[0]) || isdigit(prev[0])) {
				throw std::invalid_argument("Invalid input. Not expected symbols at position " + std::to_string(i + 1));
			}
			while (i < infix.size() && isalpha(infix[i])) {
				curr += infix[i++];
			}
			if(!(curr == "cos" && i < infix.size() && infix[i] == '(')) {
				bool was = false;
				for (const auto& op : operands) {
					if (curr == op.first) {
						was = true;
						break;
					}
				}
				if (!was) {
					operands.push_back({ curr, .0 });
				}
			}
			lexems.push_back(curr);
			prev.swap(curr);	
			curr.clear();
		}
		else if (isdigit(infix[i])) {
			fist_unar_minus = false;
			if (prev == "cos" || prev[0] == ')' || isalpha(prev[0]) || isdigit(prev[0])) {
				throw std::invalid_argument("Invalid input. Not expected symbols at position " + std::to_string(i + 1));
			}
			bool is_point = false;
			while (i < infix.size() && (isdigit(infix[i]) || infix[i] == '.')) {
				if (infix[i] == '.') {
					if (is_point) {
						throw std::invalid_argument("Invalid input. Multiple points in one number. Second point position: "
							+ std::to_string(i + 1));
					}
					is_point = true;
					curr += infix[i++];
				}
				else {
					curr += infix[i++];
				}
			}
			lexems.push_back(curr);
			prev.swap(curr);
			curr.clear();
		}
		else if (infix[i] == '-' || infix[i] == '+' || infix[i] == '/' || infix[i] == '*') {
			if (lexems.empty() || prev == "-" || prev == "+" || prev == "/" || prev == "*" || prev == "(" || prev == "~") {
				if (infix[i] == '-') {
					if (lexems.empty() || prev == "(") {
						fist_unar_minus = true;
						lexems.push_back("~");
						prev = "~";
						++i;
						continue;
					}
					if (!lexems.empty() && !fist_unar_minus) {
						fist_unar_minus = true;
						lexems.push_back("~");
						prev = "~";
						++i;
						continue;
					}
					else if (fist_unar_minus) {
						throw std::invalid_argument("Invalid input. Two unar minuses in row. Position: " + std::to_string(i + 1));
					}
				}
				else {
					throw std::invalid_argument("Invalid input. Invalid left operand. Position: " + std::to_string(i + 1));
				}
			}
			prev.clear();
			prev += infix[i];
			lexems.push_back(prev);
			++i;
		}
		else {
			throw std::invalid_argument("Invalid input. Unknown symbol. Position: " + std::to_string(i + 1));
		}
	}
	if (prev == "~" || prev == "-" || prev == "+" || prev == "/" || prev == "*") {
		throw std::invalid_argument("Invalid input. No right argument at " + std::to_string(i));
	}
	else if (!brackets.empty()) {
		throw std::invalid_argument("Invalid input. Wrong bracket sequence. Position " + std::to_string(i));
	}
}


void TPostfix::ToPostfix() {
	Parse();
	TStack<char> ops;
	for (const std::string& item: lexems) {
		char curr = item[0];
		switch (curr) {
		case '(':
			ops.push(curr);
			break;

		case ')':
			curr = ops.top();
			ops.pop();
			if (ops.top() == '^') {
				postfix_lexems.push_back("^");
				postfix += '^';
				ops.pop();
				break;
			}
			while (curr != '(') {
				if ((!postfix.empty() && !postfix_lexems.back().empty()) || postfix.empty()) {
					postfix_lexems.push_back("");
				}
				postfix_lexems.back() += curr;
				postfix += curr;
				curr = ops.top();
				ops.pop();
			}
			break;
		case '+': case'-': case'*': case'/': case'~':case'^':
			if (!ops.empty()) {
				size_t item_priority = 0, stack_item_priority = 0;
				for (const auto& pair : priorities) {
					if (pair.first == curr) {
						item_priority = pair.second;
					}
					if (pair.first == ops.top()) {
						stack_item_priority = pair.second;
					}
				}
				if (priorities[item_priority] <= priorities[stack_item_priority]) {
					if (item_priority == stack_item_priority && item_priority == 3 && postfix_lexems.size() == 0) {
						ops.push(curr);
						break;
					}
					if ((!postfix.empty() && !postfix_lexems.back().empty()) || postfix.empty()) {
						postfix_lexems.push_back("");
					}
					postfix_lexems.back() += ops.top();
					postfix += ops.top();
					ops.pop();
				}
			}
			ops.push(curr);
			break;
		default:
			postfix_lexems.push_back(item);
			postfix += item;
		}
	}
	while (!ops.empty()) {	
		if ((!postfix.empty() && !postfix_lexems.back().empty()) || postfix.empty()) {
			postfix_lexems.push_back("");
		}
		postfix_lexems.back() += ops.top();
		postfix += ops.top();
		ops.pop();
	}
}

TPostfix::TPostfix(const std::string& src) :
	infix(src), postfix(""), priorities({ {'(', 0}, {')',0}, {'-' , 1}, {'+', 1},
	{'/', 2}, {'*', 2}, {'~', 3}, {'^', 3} }) // ^ == cos
{
	ToPostfix();
}

std::string TPostfix::GetInfix() const {
	return infix;
}
std::string TPostfix::GetPostfix() const {
	return postfix;
}

std::vector<std::string> TPostfix::GetOperands() const {
	std::vector<std::string> ops;
	for (const auto& s : operands) {
		ops.push_back(s.first);
	}
	return ops;
}

double TPostfix::Calculate(const std::vector<std::pair<std::string, double>>& values) {
	if (values.size() < operands.size()) throw std::invalid_argument("Not enough operands");
	std::vector<bool>found(operands.size());
	for (auto& val : values) {
		for (int i = 0; i < operands.size(); ++i) {
			if (operands[i].first == val.first) {
				found[i] = true;
				operands[i].second = val.second;
			}
		}
	}
	for (int i = 0; i < found.size(); ++i) {
		if (!found[i]) throw std::invalid_argument("Missing operand " + operands[i].first);
	}
	TStack<double> st;
	double left, right;
	for (std::string& lexem : postfix_lexems) {
		if (lexem == "+") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left + right);
		}
		else if (lexem == "-") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left - right);
		}
		else if (lexem == "*") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left * right);
		}
		else if (lexem == "/") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left / right);
		}
		else if (lexem == "~") {
			right = st.top();
			st.pop();
			st.push(-right);
		}
		else if (lexem == "^") {
			right = st.top();
			st.pop();
			st.push(double(cos(right)));
		}
		else if(isalpha(lexem[0]) || isdigit(lexem[0])) {
			bool found = false;
			for (auto& op : operands) {
				if (op.first == lexem) {
					st.push(op.second);
					found = true;
					break;
				}
			}
			if (!found) {
				st.push(std::stod(lexem));
			}
		}
	}
	return st.top();
}

double TPostfix::Calculate() {
	for (auto& op : operands) {
		std::cout << "Enter " << op.first << ": ";
		std::cin >> op.second;
	}
	TStack<double> st;
	double left, right;
	for (std::string& lexem : postfix_lexems) {
		if (lexem == "+") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left + right);
		}
		else if (lexem == "-") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left - right);
		}
		else if (lexem == "*") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left * right);
		}
		else if (lexem == "/") {
			right = st.top();
			st.pop();
			left = st.top();
			st.pop();
			st.push(left / right);
		}
		else if (lexem == "~") {
			right = st.top();
			st.pop();
			st.push(-right);
		}
		else if (lexem == "^") {
			right = st.top();
			st.pop();
			st.push(double(cos(right)));
		}
		else if (isalpha(lexem[0]) || isdigit(lexem[0])) {
			bool found = false;
			for (auto& op : operands) {
				if (op.first == lexem) {
					st.push(op.second);
					found = true;
					break;
				}
			}
			if (!found) {
				st.push(std::stod(lexem));
			}
		}
	}
	return st.top();
}