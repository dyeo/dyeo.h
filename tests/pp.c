#include "../pp.h"

#include "../c99.h"

#define Y(I, X) STR(I) ":" STR(X),
const char *s[] = {YEARGS(A, B, C, D, E, F)};
#undef Y

#define _fprintf_arg0 stdout
#define _fprintf_arg1 "no args\n"
#define fprintf(...) CALLFUNCD(fprintf, 2, __VA_ARGS__)

int main()
{
  fprintf();
  fprintf(stderr, "args: %s\n", "b");
  return 0;
}