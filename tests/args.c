#include "../args.h"
#include "../test.h"

int main(int argc, char **argv)
{
  argdefaults();
  argvar(test, int, "Test message");
  argreqd(test);
  argparse(argc, argv);
  printf("%d", test);
}