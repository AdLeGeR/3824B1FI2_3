#include "pch.h"
#include <map>
#include <stdexcept>
#include <sstream>
#include "../Lab3/TStack.h"
#include "../Lab3/TPostfix.h"
#include "../Lab3/TPostfix.cpp"
using std::cout;
using std::endl;
using std::istringstream;
using std::ostringstream;
using std::exp;

TEST(PostfixBuild, SimpleExpression) {
    TPostfix p("2+3");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 + ");
}

TEST(PostfixBuild, WithPrecedence) {
    TPostfix p("2+3*4");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 4 * + ");
}

TEST(PostfixBuild, Parentheses) {
    TPostfix p("(2+3)*4");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 + 4 * ");
}

TEST(PostfixBuild, Variables) {
    TPostfix p("a+b*c");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "a b c * + ");
}

TEST(PostfixBuild, LnFunction) {
    TPostfix p("ln(a)+3");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "a ln() 3 + ");
}

TEST(PostfixEval, SimpleMath) {
    TPostfix p("2+3*4");
    p.ToPostfix();
    EXPECT_DOUBLE_EQ(p.Evaluate(), 14.0);
}

TEST(PostfixEval, Variables) {
    TPostfix p("a+b");
    p.ToPostfix();
	istringstream in("2\n5\n");
	ostringstream _;
	p.ReadVariables(in, _);
    EXPECT_DOUBLE_EQ(p.Evaluate(), 7.0);
}

TEST(PostfixEval, ParenthesesEval) {
    TPostfix p("(2+3)*4");
    p.ToPostfix();
    EXPECT_DOUBLE_EQ(p.Evaluate(), 20.0);
}

TEST(PostfixEval, LnEval) {
    TPostfix p("ln(x)");
    p.ToPostfix();
    istringstream in(to_string(std::exp(1.0)));
    ostringstream _;
    p.ReadVariables(in, _);
    EXPECT_NEAR(p.Evaluate(), 1.0, 1e-6);
}

TEST(PostfixErrors, MismatchedBrackets) {
    TPostfix p("(2+3");
    EXPECT_THROW(p.ToPostfix(), std::runtime_error);
}

TEST(PostfixErrors, TwoOperatorsInRow) {
    TPostfix p("2++3");
    EXPECT_THROW(p.ToPostfix(), std::runtime_error);
}

TEST(PostfixErrors, InvalidSymbol) {
    TPostfix p("2&3");
    EXPECT_THROW(p.ToPostfix(), std::runtime_error);
}

