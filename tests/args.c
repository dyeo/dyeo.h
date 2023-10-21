#include "../args.h"
#include "../test.h"

int main(int argc, char **argv)
{
  argdefaults();
  argflag(a, "Some cool flag named a");
  argflag(b, "A less cool flag named b");
  argflag(something, "A named flag");
  argvar(something2, int, "A named int val");
  argparse(argc, argv);
  poparg(test, int, "This should be first popped off");
  poparg(test2, int, "This won't be popped off and will fail");
  if (false)
  {
    poparg(test3, int, "This won't be popped off because it's excluded");
    poparg(test4, int, "This won't be popped off because it's excluded");
  }
  argpopend(argc, argv);
}