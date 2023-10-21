#include "../args.h"
#include "../test.h"

int main(int argc, char **argv)
{
  argdefaults();
  argflag(a, "Some cool flag named a");
  argflag(b, "A less cool flag named b");
  argflag(something, "A named flag");
  argvar(something2, int, "A named int val");
  parseargs(argc, argv);
  argpop(test, int, "This should be first popped off");
  argpop(test2, int, "This won't be popped off and will fail");
  if (false)
  {
    argpop(test3, int, "This won't be popped off because it's excluded");
    argpop(test4, int, "This won't be popped off because it's excluded");
  }
  popargs(argc, argv);
}