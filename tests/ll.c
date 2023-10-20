// if your crt is statically linked, #define LL_STATIC before including
#define LL_IMPLEMENTATION
#include "../ll.h"

#include <stdio.h>

#ifdef _WIN32
ll_lib(crt, msvcrt, {
  ll_val(test, int);
  ll_func(printf, int, const char *, ...);
  ll_func(fprintf, int, FILE *, const char *, ...);
});
ll_lib(asdf, dsf, {
  ll_func(asf, int, const char *, ...);
  ll_func(def, int, FILE *, const char *, ...);
});
#else
ll_lib(crt, libc, {
  ll_func(printf, void, const char *, ...);
  ll_func(fprintf, void, FILE *, const char *, ...);
});
#endif

int main(void) {
  ll_load_lib(crt);
  ll_load_func(crt, printf);
  ll_load_var(crt, test); // this should fail safely unless you have a lib
                          // called asdf in your PATH
  if (!ll_has_func(crt, printf)) {
  } else {
    crt.printf("Hello, world!\n");
  }
  ll_load_func(crt, fprintf);
  if (ll_has_func(crt, fprintf)) {
    crt.fprintf(stdout, "Hello, world!\n");
  }
  ll_close_lib(crt);
}