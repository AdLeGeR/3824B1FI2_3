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
using std::invalid_argument;


TEST(PostfixBuild, SimpleExpression) {
    Postfix p("2+3");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 + ");
}

TEST(PostfixBuild, WithPrecedence) {
    Postfix p("2+3*4");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 4 * + ");
}

TEST(PostfixBuild, Parentheses) {
    Postfix p("(2+3)*4");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "2 3 + 4 * ");
}

TEST(PostfixBuild, Variables) {
    Postfix p("a+b*c");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "a b c * + ");
}

TEST(PostfixBuild, LnFunction) {
    Postfix p("ln(a)+3");
    p.ToPostfix();
    EXPECT_EQ(p.GetPostfix(), "a ln() 3 + ");
}

TEST(PostfixEval, SimpleMath) {
    Postfix p("2+3*4");
    p.ToPostfix();
    EXPECT_DOUBLE_EQ(p.Evaluate(), 14.0);
}

TEST(PostfixEval, Variables) {
    Postfix p("a+b");
    p.ToPostfix();
	istringstream in("2\n5\n");
	ostringstream _;
	p.ReadVariables(in, _);
    EXPECT_DOUBLE_EQ(p.Evaluate(), 7.0);
}

TEST(Postfix, SimpleExpressionsCalculateNoThrow)
{
    double a = 1.09, b = 2.34, c = 3.9;
    istringstream in1("1.09\n2.34\n");
    istringstream in2("3.9\n");
    ostringstream _;

    Postfix p1("a+b");
    p1.ToPostfix();
    p1.ReadVariables(in1, _); 
    in1.clear();
    in1.seekg(0);
    EXPECT_DOUBLE_EQ(p1.Evaluate(), a + b);

    Postfix p2("a-b");
    p2.ToPostfix();
    p2.ReadVariables(in1, _);
    in1.clear();
    in1.seekg(0);
    EXPECT_DOUBLE_EQ(p2.Evaluate(), a - b);

    Postfix p3("a*b");
    p3.ToPostfix();
    p3.ReadVariables(in1, _);
    in1.clear();
    in1.seekg(0);
    EXPECT_DOUBLE_EQ(p3.Evaluate(), a * b);

    Postfix p4("a/b");
    p4.ToPostfix();
    p4.ReadVariables(in1, _);
    in1.clear();
    in1.seekg(0);
    EXPECT_DOUBLE_EQ(p4.Evaluate(), a / b);

    Postfix p5("ln(c)");
    p5.ToPostfix();
    p5.ReadVariables(in2, _);
    in2.clear();
    in2.seekg(0);
    EXPECT_DOUBLE_EQ(p5.Evaluate(), std::log(c));

    Postfix p6("-d");
    p6.ToPostfix();
    p6.ReadVariables(in2, _);
    EXPECT_DOUBLE_EQ(p6.Evaluate(), -c);
}

TEST(PostfixEval, ParenthesesEval) {
    Postfix p("(2+3)*4");
    p.ToPostfix();
    EXPECT_DOUBLE_EQ(p.Evaluate(), 20.0);
}

TEST(PostfixEval, LnEval) {
    Postfix p("ln(x)");
    p.ToPostfix();
    istringstream in(to_string(std::exp(1.0)));
    ostringstream _;
    p.ReadVariables(in, _);
    EXPECT_NEAR(p.Evaluate(), 1.0, 1e-6);
}

TEST(PostfixErrors, MismatchedBrackets) {
    Postfix p("(2+3");
    EXPECT_THROW(p.ToPostfix(), invalid_argument);
}

TEST(PostfixErrors, TwoOperatorsInRow) {
    Postfix p("2++3");
    EXPECT_THROW(p.ToPostfix(), invalid_argument);
}

TEST(PostfixErrors, InvalidSymbol) {
    Postfix p("2&3");
    EXPECT_THROW(p.ToPostfix(), invalid_argument);
}

TEST(Postfix, ComplexExpressionsWithUnaryMinusWithNumbersNoThrow)
{
    Postfix p1("-(a+b/(3+c))");
    p1.ToPostfix();
    EXPECT_EQ(p1.GetPostfix(), "a b 3 c + / + ~ ");

    Postfix p2("-(a-b*(7.4-ln(c)))");
    p2.ToPostfix();
    EXPECT_EQ(p2.GetPostfix(), "a b 7.4 c ln() - * - ~ ");

    Postfix p3("-(a+v*b)");
    p3.ToPostfix();
    EXPECT_EQ(p3.GetPostfix(), "a v b * + ~ ");

    Postfix p4("-(a/b-0.4+n)");
    p4.ToPostfix();
    EXPECT_EQ(p4.GetPostfix(), "a b / 0.4 - n + ~ ");

    Postfix p5("-ln(c/3*f)");
    p5.ToPostfix();
    EXPECT_EQ(p5.GetPostfix(), "c 3 / f * ln() ~ ");

    Postfix p6("-(-d*k-9.12)");
    p6.ToPostfix();
    EXPECT_EQ(p6.GetPostfix(), "d ~ k * 9.12 - ~ ");

    //Выражение, содержащее все операции, операнды, числа:
    Postfix p7("-(a+3.5*ln(b-(-2.7)))/(c-(-(d/1.25)))+(-e*(f+0.4))");
    p7.ToPostfix();
    EXPECT_EQ(p7.GetPostfix(), "a 3.5 b 2.7 ~ - ln() * + ~ c d 1.25 / ~ - / e ~ f 0.4 + * + ");
}

TEST(Postfix, IncorrectExpressionsAnyThrow)
{
    //Incorrect infix form: incorrect number input.
    EXPECT_THROW(Postfix p("ln2.3", true), invalid_argument);
    EXPECT_THROW(Postfix p(")5.87", true), invalid_argument);
    EXPECT_THROW(Postfix p("d1.3", true), invalid_argument);
    EXPECT_THROW(Postfix p("dgf0.34", true), invalid_argument);
    EXPECT_THROW(Postfix p("LO6.5", true), invalid_argument);
    EXPECT_THROW(Postfix p("ln3", true), invalid_argument);

    //Incorrect infix form: invalid number.
    EXPECT_THROW(Postfix p("5.4.6", true), invalid_argument);

    //Incorrect infix form: incorrect input of the operand (or the ln() function).
    EXPECT_THROW(Postfix p(")a", true), invalid_argument);
    EXPECT_THROW(Postfix p("6.4B", true), invalid_argument);

    //Incorrect infix form: a gap was found between the operands.
    EXPECT_THROW(Postfix p("a b", true), invalid_argument);
    EXPECT_THROW(Postfix p("av Kj", true), invalid_argument);

    //Incorrect infix form: incorrect entry of the opening bracket.
    EXPECT_THROW(Postfix p("a+5.3(b+c)", true), invalid_argument);
    EXPECT_THROW(Postfix p("a+d(b+c)", true), invalid_argument);
    EXPECT_NO_THROW(Postfix p("a+ln(b+c)", true));
    EXPECT_NO_THROW(Postfix p("a+(-(b+c))", true));
    EXPECT_THROW(Postfix p("(a+f)(b+c)", true), invalid_argument);

    //Incorrect infix form: incorrect entry of the closing bracket.
    EXPECT_THROW(Postfix p(")", true), invalid_argument);
    EXPECT_THROW(Postfix p("+)", true), invalid_argument);
    EXPECT_THROW(Postfix p("-)", true), invalid_argument);
    EXPECT_THROW(Postfix p("*)", true), invalid_argument);
    EXPECT_THROW(Postfix p("/)", true), invalid_argument);
    EXPECT_THROW(Postfix p("ln)", true), invalid_argument);

    //Incorrect infix form: incorrect number of opening and closing brackets.
    EXPECT_THROW(Postfix p("(a+b)-c)", true), invalid_argument);

    //Incorrect infix form: incorrect unary minus sign input.
    EXPECT_THROW(Postfix p("a+-b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a--b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a*-b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a/-b", true), invalid_argument);

    EXPECT_THROW(Postfix p("a+(--b)", true), invalid_argument);

    //Incorrect infix form: incorrect operation input.
    EXPECT_THROW(Postfix p("+a", true), invalid_argument);
    EXPECT_THROW(Postfix p("*a", true), invalid_argument);
    EXPECT_THROW(Postfix p("/a", true), invalid_argument);

    EXPECT_THROW(Postfix p("a++b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a-+b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a*+b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a/+b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a+(+b", true), invalid_argument);
    EXPECT_THROW(Postfix p("-+a", true), invalid_argument);
    EXPECT_THROW(Postfix p("(-+a)", true), invalid_argument);

    EXPECT_THROW(Postfix p("a+*b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a-*b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a**b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a/*b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a+(*b", true), invalid_argument);
    EXPECT_THROW(Postfix p("-*a", true), invalid_argument);
    EXPECT_THROW(Postfix p("(-*a)", true), invalid_argument);

    EXPECT_THROW(Postfix p("a+/b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a-/b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a*/b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a//b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a+(/b", true), invalid_argument);
    EXPECT_THROW(Postfix p("-/a", true), invalid_argument);
    EXPECT_THROW(Postfix p("(-/a)", true), invalid_argument);

    //Incorrect infix form: unknown symbol detected.
    EXPECT_THROW(Postfix p("a&b", true), invalid_argument);
    EXPECT_THROW(Postfix p("a+b^c", true), invalid_argument);
    EXPECT_THROW(Postfix p("a$b", true), invalid_argument);

    //Incorrect infix form: the infix form cannot end with ln, +, -, *, /, (. Or: incorrect number of opening and closing brackets.
    EXPECT_THROW(Postfix p("a+", true), invalid_argument);
    EXPECT_THROW(Postfix p("a-", true), invalid_argument);
    EXPECT_THROW(Postfix p("a*", true), invalid_argument);
    EXPECT_THROW(Postfix p("a/", true), invalid_argument);
    EXPECT_THROW(Postfix p("a+(", true), invalid_argument);

    EXPECT_THROW(Postfix p("(a+b-(c*d)", true), invalid_argument);
}