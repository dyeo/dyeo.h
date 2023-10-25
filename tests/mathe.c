#define MATHE_PRECISION double
#define MATHE_IMPLEMENTATION
#include "../mathe.h"
#include "../test.h"

int main()
{
  test_group(mathe, {
    // Basic operations
    test_expr(mathe("pi"), double, 3.141592653589793);
    test_expr(mathe("e"), double, 2.718281828459045);
    test_expr(mathe("phi"), double, 1.618033988749895);
    test_expr(mathe("sqrt2"), double, 1.4142135623730951);
    test_expr(mathe("ln2"), double, 0.6931471805599453);
    test_expr(mathe("log2e"), double, 1.4426950408889634);
    test_expr(mathe("log10e"), double, 0.4342944819032518);
    test_expr(mathe("gamma"), double, 0.5772156649015329);

    // Basic operations
    test_expr(mathe("1 + 1"), double, 2.0);
    test_expr(mathe("1 + 1 * 2"), double, 3.0);
    test_expr(mathe("(2*5)+4"), double, 14.0);
    test_expr(mathe("5 - 3"), double, 2.0);
    test_expr(mathe("10 / 2"), double, 5.0);

    // Precedence
    test_expr(mathe("1 + 2 * 3"), double, 7.0);
    test_expr(mathe("1 * 2 + 3"), double, 5.0);
    test_expr(mathe("4 / 2 + 3"), double, 5.0);
    test_expr(mathe("4 + 2 * 3"), double, 10.0);

    // Paranthesis
    test_expr(mathe("(1 + 2) * 3"), double, 9.0);
    test_expr(mathe("1 + (2 * 3)"), double, 7.0);
    test_expr(mathe("(4 / 2) + 3"), double, 5.0);
    test_expr(mathe("(4 + 2) * 3"), double, 18.0);

    // Complex expressions
    test_expr(mathe("(3 + 4) * (5 - 2)"), double, 21.0);
    test_expr(mathe("3 + 4 * 2 / (1 - 5)"), double,
              1.0); // Be careful of division by zero!
    test_expr(mathe("3 + 4 * 2 / (1 - 5) ^ 2"), double, 3.5);

    // Negative numbers
    test_expr(mathe("-1 + 2"), double, 1.0);
    test_expr(mathe("1 + -2"), double, -1.0);
    test_expr(mathe("-1 * -1"), double, 1.0);
    test_expr(mathe("2 * -3"), double, -6.0);

    // Floating point
    test_expr(mathe("1.5 + 2.5"), double, 4.0);
    test_expr(mathe("10.0 / 3.0"), double,
              3.3333333333333335); // Precision might be tricky!
    test_expr(mathe("5.5 * 2"), double, 11.0);

    // Powers
    test_expr(mathe("2 ^ 3"), double, 8.0);
    test_expr(mathe("9 ^ 0.5"), double, 3.0);
    test_expr(mathe("(-1) ^ 2"), double, 1.0);

    // For sin(x)
    test_expr(mathe("sin(0)"), double, 0.0);
    test_expr(mathe("sin(pi/2)"), double, 1.0);
    test_expr(mathe("sin(pi)"), double, 0.0);
    test_expr(mathe("sin(3*pi/2)"), double, -1.0);
    test_expr(mathe("sin(2*pi)"), double, 0.0);

    // For cos(x)
    test_expr(mathe("cos(0)"), double, 1.0);
    test_expr(mathe("cos(pi/2)"), double, 0.0);
    test_expr(mathe("cos(pi)"), double, -1.0);
    test_expr(mathe("cos(3*pi/2)"), double, 0.0);
    test_expr(mathe("cos(2*pi)"), double, 1.0);

    // For tan(x)
    test_expr(mathe("tan(0)"), double, 0.0);
    test_expr(mathe("tan(pi/4)"), double, 1.0);
    test_expr(mathe("tan(pi/2)"), double, 16331239353195370);
    test_expr(mathe("tan(3*pi/4)"), double, -1.0);
    test_expr(mathe("tan(pi)"), double, 0.0);

    // For sqrt(x)
    test_expr(isnan(mathe("sqrt(-1)")), bool, true);
    test_expr(mathe("sqrt(0)"), double, 0.0);
    test_expr(mathe("sqrt(1)"), double, 1.0);
    test_expr(mathe("sqrt(4)"), double, 2.0);
    test_expr(mathe("sqrt(9)"), double, 3.0);
    test_expr(mathe("sqrt(16)"), double, 4.0);
  });
}