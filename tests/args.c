#include "../args.h"
#include "../test.h"

int main(int argc, char **argv)
{
  argdefaults();
  argvar(test, int, "Test message", 1);
  argreqd(test);
  argparse(argc, argv);

}