#define ARGS_IMPLEMENTATION
#include "../args.h"

int main(int argc, char **argv)
{
  args_t args = args_new();
  args_default(args);
  int **a = args_arg(args, a, argarr(int), "Array?");
  args_parse(args, argc, argv);
  for (int i = 0; a[i]; ++i)
  {
    printf("%d ", *a[i]);
  }
}