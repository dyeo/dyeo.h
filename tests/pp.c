#include "pp.h"

#define Y(I, X) STR(I)":"STR(X),
const char *s[] = {YEARGS(A,B,C,D,E,F)};
#undef Y

int main()
{
    return 0;
}