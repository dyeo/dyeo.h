#define CPU_DEBUG
#define CPU_IMPLEMENTATION
#include "../cpu.h"

#include <stdio.h>

#define MEM_SIZE 1024

int main()
{
  u64 memc = 0;
  u8 *mem  = asm_compf("../res/test2.asm", &memc);

  cpu c = cpu_new(memc, mem);
  cpu_run(c);
  free(c);
  free(mem);
  return 0;
}