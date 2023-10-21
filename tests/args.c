#include "../args.h"
#include "../test.h"

int main(int argc, char **argv)
{
  argdefaults();
  argflag(a, "TEST");
  argflag(b, "TEST");
  argparse(argc, argv);
}