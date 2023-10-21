#include "../test.h"

int main()
{
  test_group(int, {
    test_group(add, {
      int a = 0;
      test_expr(a += 1, int, 1);
      test_true((a += 1) == 2);
      test_expr(a += 1, int, 3);
    });
    test_group(sub, {
      int a = 0;
      test_expr(a -= 1, int, -1);
      test_true((a -= 1) == -3);
      test_expr(a -= 1, int, -3);
    });
  });
}