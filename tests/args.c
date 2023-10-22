#define ARGS_IMPLEMENTATION
#include "../args.h"

int main(int argc, char **argv)
{
  ARGS args = args_new();
  args_default(args);
  bool *a         = args_flag(args, a, "Some cool flag named a", true, false);
  bool *b         = args_flag(args, b, "A less cool flag named b");
  filepath *c     = args_arg(args, c, filepath, "A less cool flag named b", "hello");
  bool *something = args_flag(args, something, "A named flag");
  int *something2 = args_arg(args, something2, int, "A named int val", 10);
  int *test = args_pop(args, test, int, "This should be first popped off");
  int *test2 =
    args_pop(args, test2, int, "This won't be popped off and will fail");
  args_parse(args, argc, argv);
  args_help(stdout, args);
}
