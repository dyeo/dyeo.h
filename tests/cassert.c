#include "../cassert.h"

int main(void)
{
  cassert(1, "test");
  cassert(0, "test");
  return 0;
}