#include "pch.h"
#include <map>
#include <stdexcept>
#include "../Lab3/TStack.h"
#include "../Lab3/TPostfix.h"
#include "../Lab3/TPostfix.cpp"


TEST(PostfixBuild, SimpleExpression) {
    MyTPostfix p("2+3");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 + ");
}

TEST(PostfixBuild, WithPrecedence) {
    MyTPostfix p("2+3*4");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 4 * + ");
}

TEST(PostfixBuild, Parentheses) {
    MyTPostfix p("(2+3)*4");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 + 4 * ");
}

TEST(PostfixBuild, Variables) {
    MyTPostfix p("a+b*c");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "a b c * + ");
}

TEST(PostfixBuild, LnFunction) {
    MyTPostfix p("ln(a)+3");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "a ln 3 + ");
}

TEST(PostfixEval, SimpleMath) {
    MyTPostfix p("2+3*4");
    p.ToPostfix();
    std::map<char, double> vars;
    EXPECT_DOUBLE_EQ(p.Evaluate(vars), 14.0);
}

TEST(PostfixEval, Variables) {
    MyTPostfix p("a+b");
    p.ToPostfix();
    std::map<char, double> vars = { {'a', 2}, {'b', 5} };
    EXPECT_DOUBLE_EQ(p.Evaluate(vars), 7.0);
}

TEST(PostfixEval, ParenthesesEval) {
    MyTPostfix p("(2+3)*4");
    p.ToPostfix();
    std::map<char, double> vars;
    EXPECT_DOUBLE_EQ(p.Evaluate(vars), 20.0);
}

TEST(PostfixEval, LnEval) {
    MyTPostfix p("ln(x)");
    p.ToPostfix();
    std::map<char, double> vars = { {'x', std::exp(1.0)} };
    EXPECT_NEAR(p.Evaluate(vars), 1.0, 1e-9);
}

TEST(PostfixErrors, UnknownVariable) {
    MyTPostfix p("a+1");
    p.ToPostfix();
    std::map<char, double> vars; // a не определена
    EXPECT_THROW(p.Evaluate(vars), std::runtime_error);
}

TEST(PostfixErrors, MismatchedBrackets) {
    MyTPostfix p("(2+3");
    EXPECT_THROW(p.ToPostfix(), std::runtime_error);
}

TEST(PostfixErrors, TwoOperatorsInRow) {
    MyTPostfix p("2++3");
    EXPECT_THROW(p.ToPostfix(), std::runtime_error);
}

TEST(PostfixErrors, InvalidSymbol) {
    MyTPostfix p("2&3");
    EXPECT_THROW(p.ToPostfix(), std::runtime_error);
}

