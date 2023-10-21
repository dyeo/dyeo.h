#include "../args.h"
#include "../test.h"

int main(int argc, char **argv)
{
  args_default();
  args_flag(a, "Some cool flag named a");
  args_flag(b, "A less cool flag named b");
  args_flag(something, "A named flag");
  args_var(something2, int, "A named int val");
  args_pop(test, int, "This should be first popped off");
  args_pop(test2, int, "This won't be popped off and will fail");
  if (false)
  {
    args_pop(test3, int, "This won't be popped off because it's excluded");
    args_pop(test4, int, "This won't be popped off because it's excluded");
  }
  args_parse(argc, argv);
  args_popend(argc, argv);
}