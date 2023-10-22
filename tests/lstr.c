#include "../lstr.h"
#include "../test.h"

#include <stdio.h>

int main()
{
  test_group(lstr, {
    lstr_t a = lstr("Hello world!\n");
    test_true(a != NULL);
    test_expr(strcmp(lstrc(lstrsub(a, -5, 4)), "rld!"), int, 0);
  });
}