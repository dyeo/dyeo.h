// if your crt is statically linked, #define LL_STATIC before including
#define LL_IMPLEMENTATION
#include "../ll.h"
#include "../test.h"
#include <stdio.h>

#ifdef _WIN32
ll_lib(crt, msvcrt, {
  ll_val(asdf, int);
  ll_func(printf, int, const char *, ...);
  ll_func(fprintf, int, FILE *, const char *, ...);
});
#else
ll_lib(crt, libc, {
  ll_func(printf, void, const char *, ...);
  ll_func(fprintf, void, FILE *, const char *, ...);
});
#endif

int main(void) {
  test_group(ll, {
    test_true(ll_load_lib(crt));
    test_true(ll_load_func(crt, printf));
    crt.printf("Hello, world!\n");
    test_true(ll_load_func(crt, fprintf));
    crt.fprintf(stdout, "Hello, world!\n");
    test_expr(ll_load_var(crt, asdf), bool, false);
    test_true(ll_close_lib(crt));
  });
}