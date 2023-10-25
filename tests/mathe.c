#define MATHE_PRECISION double
#define MATHE_IMPLEMENTATION
#include "../mathe.h"
#include "../test.h"

int main()
{
  test_group(mathe, {
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
  });
}